#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <time.h>

int main(){

  const double Gravity = 6.67428 * pow(10.0, -11);
  srand(time(NULL));
  clock_t begin, end;
  double** pos;
  double** forces;
  double** vel;
  double* masses;
  double* force_qk;
  double delta_t;
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
  omp_lock_t* lock; //used to lock the particles to find forces of q and k

  printf("enter the number of particles: ");
  fflush(stdout);
  scanf("%d/n", &num_particles);
  omp_set_num_threads(8);
  begin = clock();

  //create array for masses[] and force_qk[] and for lock[]
  masses = (double*)malloc(num_particles*sizeof(double));
  force_qk = (double*)malloc(num_particles*sizeof(double));
  lock = (omp_lock_t*)malloc(num_particles*sizeof(omp_lock_t));
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
  }

  k = 0;
  //for each time step 
#pragma omp parallel for schedule(dynamic,1)//<-------------
  for(i = 0; i<1000; i++){ 
    delta_t = delta_t + 0.05;
    //initialize forces to 0
    force_qk[x] = force_qk[y] = 0.0;
    //Go from 500 to 2000 particles in steps of 100 particles
#pragma omp parallel for schedule(dynamic,1) private(x_diff,y_diff,dist,dist_cubed,k,q)//<-------
    for(q=500; q < num_particles; q++) {
      force_qk[x] = force_qk[y] = 0.0;
      for(k=q+1; k < num_particles; k++) {
	x_diff = pos[q][x] - pos[k][x];
	y_diff = pos[q][y] - pos[k][y];
	dist = sqrt((x_diff * x_diff) + (y_diff * y_diff));
	dist_cubed = dist*dist*dist;
	force_qk[x] = Gravity*masses[q]*masses[k]/dist_cubed * x_diff;
	force_qk[y] = Gravity*masses[q]*masses[k]/dist_cubed * y_diff;

	//compute forces for each particle(reduced n-body solver)
	omp_init_lock(&lock[q]);//each q will get its own lock <-----------
	omp_init_lock(&lock[k]);//each k will get its own lock <-----------
      	omp_set_lock(&lock[q]);//<--------------
	forces[q][x] += force_qk[x];
	forces[q][y] += force_qk[y];
	omp_unset_lock(&lock[q]);//<-------------
	
	omp_set_lock(&lock[k]);//<-------------
	forces[k][x] -= force_qk[x];//this part I need to do critical directive
	forces[k][y] -= force_qk[y]; 
	omp_unset_lock(&lock[k]);//<-------------
      }
    }
    
    //compute position and velocity of each particle
#pragma omp parallel for private(q) //<------------
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
