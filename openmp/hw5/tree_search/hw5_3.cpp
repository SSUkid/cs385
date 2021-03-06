//CORY BEKKER
#include<iostream>
#include<vector>
#include<omp.h>
#include<queue>
#include"tour.h"
using namespace std;

bool terminated(vector<Tour>thread_stack, vector<Tour>new_stack, queue<int>thread_ranks, int threads_in_cond_wait, int num_threads,omp_lock_t lock);

int awakened_thread = -1;
int work_remains = 1;

int main(int argc, char* argv[]){
  
  if(argc != 2){
    cout<<"needs command line arg: ./program.out [number of threads]\n";
    system("PAUSE");
    return 0;
    }
    
    int num_threads;
    num_threads = strtol(argv[1], NULL, 10);
  
    omp_set_num_threads(num_threads); //Sets the thread count to the users input
  
    Tour hometown;
    Tour tour;
    Tour best_tour(1000000);
    vector<Tour>stack; //The stack is the tree and its nodes!
    vector<Tour>thread_stack; //a stack for a specific thread
    vector<Tour>new_stack;//used to help split stacks 
    unsigned int city = 0;
    int threads_in_cond_wait = 0;//------
    queue<int>thread_ranks;//------
    omp_lock_t lock; 
	
    
    stack.push_back(hometown); //Tour that visits only the hometown
    while(!stack.empty()) {
      if(num_threads <= stack.size()) break; //if there are enough tours for each thread
      tour.set_tour(stack.back());
      stack.pop_back();
      //if the current tour is the best, then change it to best_tour
      if(tour.get_num_cities() == num_cities) { 
	if(tour.better_than(best_tour))
	  best_tour.set_tour(tour);
      }
      else {
	for(city = 1; city < num_cities; city++){ //cycles through each city
	  if(tour.add_city(city, best_tour)){	//adds a city to the current tour if it isnt bigger than the best tour
	    stack.push_back(tour);//adds the tour to the stack aka adds a new leaf node to the tree
	    tour.remove_last();//removes added leaf node so that way we can go back up to the tree and try the next leaf node
	  }
	}
      }
    }
    
//split the tours in the stack across threads by splitting up the stacks different tours
#pragma omp parallel for private(thread_stack,tour,city)
    for(int i = 0; i < stack.size(); i++){
      thread_stack.push_back(stack[i]);///each thread gets its own stack
      while(terminated(thread_stack,new_stack,thread_ranks,threads_in_cond_wait,num_threads,lock)!=1){
	tour.set_tour(thread_stack.back());
	thread_stack.pop_back();
	//if the current tour is the best, then change it to best_tour
	if(tour.get_num_cities() == num_cities) { 
	  //best_tour is a shared variable and all threads must be able to update it, but only one at a time
#pragma omp critical 
	  {
	    if(tour.better_than(best_tour))
	      best_tour.set_tour(tour);
	  } 
	}
	else {
	  for(city = 1; city < num_cities; city++){ 
	    if(tour.add_city(city, best_tour)){	
	      thread_stack.push_back(tour);
	      tour.remove_last();
	    }
	  }
	}
      }
    }
    best_tour.print_best();
    return 0;
}

bool terminated(vector<Tour>thread_stack, vector<Tour>new_stack, queue<int>thread_ranks, int threads_in_cond_wait, int num_threads, omp_lock_t lock){
  
  int got_lock = 0;
  omp_lock_t lock1; 
  omp_init_lock(&lock1);
  omp_init_lock(&lock);
  
  if(thread_stack.size() >= 2 && new_stack.size() == 0 && threads_in_cond_wait > 0){
    omp_set_lock(&lock);
    if (threads_in_cond_wait > 0 && new_stack.size() == 0) { 
      for(int i = 1; i < thread_stack.size(); i= i+2){ //splitting the stack
	new_stack.push_back(thread_stack[i]);//put half of the stack onto a new stack	
	thread_stack.erase(thread_stack.begin() + i);//erase the ones that were put on the new stack
      }
      //pthreads_cond_signal
      got_lock = omp_test_lock(&lock);
      if (got_lock != 0) { 
	if (thread_ranks.size() > 0 && new_stack.size() == 0) { 
	  for(int i = 1; i < thread_stack.size(); i= i+2){ //I do not know why we are splitting the stack again
	    new_stack.push_back(thread_stack[i]);	
	    thread_stack.erase(thread_stack.begin() + i);
	  }
	  awakened_thread = thread_ranks.front(); //dequeue a thread
	  thread_ranks.pop();
	}
	omp_unset_lock(&lock);
      }//end pthreads_cond_signal
    }
    omp_unset_lock(&lock);
    return 0;  //return false and don't quit
  }
  else if(thread_stack.size() != 0){ 
    return 0; //return false and don't quit
  }
  else { //my stack is empty
    omp_set_lock(&lock);
    if(threads_in_cond_wait == num_threads-1){ //last thread running
      threads_in_cond_wait++;
      work_remains = 0; //assign false to work remains
      omp_unset_lock(&lock);
      return 1; //return true and quit
    }
    else{//other threads still working, wait for work
      threads_in_cond_wait++;
      thread_ranks.push(omp_get_thread_num()); //adds the thread that is missing work to the queue
      // omp_set_lock(&lock1);
      //#pragma omp critical
	// {
      while(awakened_thread != omp_get_thread_num() && work_remains)cout<<omp_get_thread_num;
	// }
      // omp_unset_lock(&lock1);
      //we have been awakened
      if(threads_in_cond_wait < num_threads){ //we got work
	for(int i = 0; i < new_stack.size(); i++){
	  thread_stack.push_back(new_stack[i]); //copy new stack to threads stack
	}
	new_stack.clear(); //set stack to null
	threads_in_cond_wait--;
	omp_unset_lock(&lock);
	return 0;
      }
      else { //all threads done
	omp_unset_lock(&lock);
	return 1; //return true and quit
      }
    }
  }
}
