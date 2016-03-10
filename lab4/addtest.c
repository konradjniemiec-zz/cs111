#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <getopt.h>

int opt_yield;
long long counter;
long long iterations;
pthread_mutex_t mutex;
volatile int spin_lock;
void add(long long *pointer, long long value) {
  long long sum = *pointer + value;
  if (opt_yield)
    pthread_yield();
  *pointer = sum;
}

void add_mutex(long long *pointer, long long value) {
  
  long long sum;
  pthread_mutex_lock(&mutex);
  sum = *pointer + value;
  if (opt_yield)
    pthread_yield();
  *pointer = sum;
  pthread_mutex_unlock(&mutex);
}

void add_spin (long long *pointer, long long value) {
  long long sum;
  while (__sync_lock_test_and_set(&spin_lock,1));
  sum = *pointer + value;
  if (opt_yield)
    pthread_yield();
  *pointer = sum;
  __sync_lock_release(&spin_lock);
}

void add_atomic(long long *pointer, long long value) {
  long long prev = *pointer;
  while(!__sync_bool_compare_and_swap(pointer,prev,prev+value)) {
    if (opt_yield)
      pthread_yield();
    prev = *pointer;
  }
}

void* start_routine(void *func);
int main(int argc, char **argv) {
  iterations = 1;
  long long threads = 1;
  opt_yield = 0;
  spin_lock = 0;
  pthread_mutex_init(&mutex,NULL);
  static struct option long_opts[] = {
    {"iter",required_argument,NULL,'i'},
    {"threads",required_argument,NULL,'t'},
    {"yield",required_argument,NULL,'y'},
    {"sync",required_argument,NULL,'s'},
    {0,0,0,0}
  };
  void (*function_to_call) (long long *,long long) = add;
  while (1) {
    int x = getopt_long(argc,argv,"",long_opts,NULL);
    if (x==-1) break;
    switch (x) {
    case 's': {
      if (optarg) {
	char c;
	sscanf(optarg,"%c",&c);
	switch (c) {
	case 'm':
	  function_to_call = add_mutex;
	  break;
	
	case 's':
	  function_to_call = add_spin;
	  break;
	
	case 'c':
	  function_to_call = add_atomic;
	  break;
	}
      }
      else {
	fprintf(stderr,"No argument provided for sync option\n");
      }
      break;
    }
    case 'i': {
      if (optarg) {
	sscanf(optarg,"%ld",&iterations);
      }
      else {
	fprintf(stderr,"No argument provided for iter option\n");
      }
      break;
    }
    case 't': {
      if (optarg) {
	sscanf(optarg,"%ld",&threads);
      }
      else {
	fprintf(stderr,"No argument provided for thread option\n");
      }
      break;
    }
    case 'y': {
      if (optarg) {
	sscanf(optarg,"%d",&opt_yield);
      }
      else {
	fprintf(stderr,"No argument provided for yield option\n");
      }
      break;
    }
    case '?': break;
    }
    
  }
  counter = 0;
  pthread_t * pthreads = malloc(sizeof(pthread_t) * threads);
  int i = 0;
  struct timespec time[2];
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&time[0]);
  for (i = 0; i < threads; i++)
    pthread_create( &pthreads[i], NULL, start_routine, function_to_call);
  
  for (i = 0; i < threads; i++)
    pthread_join(pthreads[i],NULL);
  
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&time[1]);
  long long operations = threads * iterations * 2;
  long long length = time[1].tv_nsec - time[0].tv_nsec;
  fprintf(stdout,"%d threads x %d iterations x 2 = %ld operations\n",threads, iterations, operations);
  fprintf(stdout,"Elapsed Time: %ld ns Nanoseconds per operation: %ld ns\n",length, length/operations);
  pthread_mutex_destroy(&mutex);
  if (counter != 0) {
    fprintf(stderr,"ERROR, the final count is: %ld\n",counter);
    
    exit(1);
  }
  
  exit(0);
}

void *  start_routine(void *func) {
  void (*function_to_call) (long long *,long long) = func;
  int i = 0;
  for (i = 0; i < iterations; i++)
    function_to_call(&counter,1);
  for (i = 0; i < iterations; i++)
    function_to_call(&counter,-1);
}
