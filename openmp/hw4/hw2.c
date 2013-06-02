#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<math.h>
#include<time.h>

int main(){
  
  const double Gravity = 6.67428 * pow(10.0, -11);
  srand(time(NULL));
  clock_t begin, end;
  double** pos;
  double** forces;
  double** vel;
  double* masses;
  double* force_qk;
  double delta_t = .05;
  double dist_cubed = 0.0;
  double dist = 0.0;
  double x_diff = 0.0;
  double y_diff = 0.0;
  double time_spent;
  int q = 0; //counter for q particles
  int k = 0; //counter for k particles
  int x = 0; //the books says to intialize x and y to 0 and 1
  int y = 1;
  int num_particles;
  int i = 0;
  int j = 0;

  printf("enter the number of particles: ");
  fflush(stdout);
  scanf("%d/n", &num_particles);
  

  //create array for masses[] and force_qk[]
  masses = (double*)malloc(num_particles*sizeof(double));
  force_qk = (double*)malloc(num_particles*sizeof(double));

  //create 2d array for pos[][], vel[][], forces[][]
  pos = (double**) malloc(num_particles * sizeof(double*));
  forces = (double**) malloc(num_particles * sizeof(double*));
  vel = (double**) malloc(num_particles * sizeof(double*));
  for(i = 0; i < num_particles; i++){
    pos[i] = (double*) malloc(num_particles * sizeof(double));
    forces[i] = (double*) malloc(num_particles * sizeof(double));
    vel[i] = (double*) malloc(num_particles * sizeof(double));
  }

  //initialize velocities  and forces to 0, intialize positions randomly 
  for(q=0; q<num_particles; q++){
    for(k=0; k < num_particles; k++){
      forces[q][k] = 0;
      vel[q][k] = 0;
      pos[q][k] = rand();
    }
  }
  //initalizes the masses[] randomly and force_qk[] to 0
  for(q=0; q<num_particles; q++){
    masses[q] = rand();
    force_qk[q] = 0;
  }
  begin = clock();
  //for each  time step
  //#pragma omp parallel for //<-------------
  for(i = 0; i<1000; i++){
    delta_t = delta_t + 0.05; 
    //Go from 500 to 2000 particles in steps of 100 particles
    //creates automatic barrier in first for loop that prevents the 
    //second loop to start if the first loop hasnt updated the correct 
    //variables "book", this version of openmp doesn't do that...
    //#pragma omp parallel for private(x_diff,y_diff,dist,dist_cubed,k,q)//<------------
    for(q=500; q < num_particles; q++) {
      //force_qk[x] = force_qk[y] = 0.0;
      for(k=q+1; k < num_particles; k++) {
	x_diff = pos[q][x] - pos[k][x];
	y_diff = pos[q][y] - pos[k][y];
	dist = sqrt(x_diff*x_diff + y_diff * y_diff);
	dist_cubed = dist*dist*dist;
	force_qk[x] = Gravity*masses[q]*masses[k]/dist_cubed * x_diff;
	force_qk[y] = Gravity*masses[q]*masses[k]/dist_cubed * y_diff;
	
	//compute forces for each particle..reduced n-body solver
	//#pragma omp critical //<-----------
	//	{
	  forces[q][x] += force_qk[x];
	  forces[q][y] += force_qk[y];
	  forces[k][x] -= force_qk[x];//this part I need to do critical directive
	  forces[k][y] -= force_qk[y]; 
	  //	}
      }
    }
    
    //compute position and velocity of each particle
    //#pragma omp parallel for private(q)//<------------might need to make delta_t private
    for(q=500; q < num_particles; q++){
      pos[q][x] += delta_t*vel[q][x];
      pos[q][y] += delta_t*vel[q][y];
      vel[q][x] += delta_t/masses[q]*forces[q][x];
      vel[q][y] += delta_t/masses[q]*forces[q][y];
    }
  }

  end = clock();
  time_spent = (double)(end - begin);
  printf("time: %lf\n", time_spent);

  return 0;
}
