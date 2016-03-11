#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include <string.h>
#include "SortedList.h"

int opt_yield;
long long iterations;
int num_lists = 1;
SortedList_t * list;

pthread_mutex_t *  mutex;
volatile int * spin_lock;

int is_mutex = 0;
int is_spin = 0;

int size_of_string = 10;

char * random_string(void) {
  int length = rand()%size_of_string + 1;
  char * string = malloc(sizeof(char) * (length + 1));
  int i;
  for (i = 0; i < length; i++)
    string[i] = '0' + rand()%72;
  string[length] = '0';
  return string;
}
void *  start_routine(void *start_pos);

void SortedList_insert(SortedList_t *list,SortedListElement_t *element) {
  SortedListElement_t *p = list; // Initialized at list head
  SortedListElement_t *n = list->next; // The first element

  
  
  while(n != list)
    {
      if (strcmp(element->key, n->key) <= 0)
	break;

      p = n;
      n = n->next;
    }
  if (opt_yield & INSERT_YIELD)
    pthread_yield();
  element->prev = p;
  element->next = n;
  p->next = element;
  n->prev = element;
}

int SortedList_delete(SortedListElement_t *element) {
  SortedListElement_t *p = element->prev; // Initialized at list head
  SortedListElement_t *n = element->next; // The first element
  //check
  if (n->prev != element) return -1;
  if (p->next != element) return -1;
  //delete yield
  if (opt_yield & DELETE_YIELD)
    pthread_yield();
  p->next = n;
  n->prev = p;
  element->next = NULL;
  element->prev = NULL;
}


int SortedList_length(SortedList_t *lists) {
  int count = 0;
  int final_count = 0;
  int i;
  for (i = 0; i < num_lists; i++) {
    count =0;
    SortedList_t *list = &lists[i];
    SortedListElement_t *p = list; // Initialized at list head
    SortedListElement_t *n = list->next; // The first element
  
    while(n != list)
      {
	count++;
	p = n;
	n = n->next;
      }
    final_count+=count;
  }
  return final_count;
}


SortedListElement_t * SortedList_lookup(SortedList_t *list, const char *key) {

  if (list == list->next)
    return NULL;
  SortedList_t *head = list;
  list = list->next;
  while (list != head) {
    if (strcmp(list->key, key) == 0) {
      //yield
      if (opt_yield & SEARCH_YIELD)
	pthread_yield();
      return list;
    }
    list = list->next;
  }

  return NULL;
}

int main(int argc, char** argv) {
  iterations = 1;
  long long threads = 1;
  opt_yield = 0;


  static struct option long_opts[] = {
    {"iter",required_argument,NULL,'i'},
    {"threads",required_argument,NULL,'t'},
    {"yield",required_argument,NULL,'y'},
    {"lists",required_argument,NULL,'l'},
    {"sync",required_argument,NULL,'s'},
    {0,0,0,0}
  };
  while (1) {
    int x = getopt_long(argc,argv,"",long_opts,NULL);
    if (x==-1) break;
    switch (x) {
    case 'l': {
      if (optarg) {
	int x=0;
	sscanf(optarg,"%d",&x);
	if (x > 0)
	  num_lists = x;
      }
      else {
	fprintf(stderr,"No argument provided for iter option\n");
      }
      break;
    }
    case 's': {
      if (optarg) {
	char c;
	sscanf(optarg,"%c",&c);
	switch (c) {
	case 'm':
	  is_mutex = 1;
	  break;	
	case 's':
	  is_spin = 1;
	  break;

	}
      }
      else {
	fprintf(stderr,"No argument provided for iter option\n");
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
	if (strcmp(optarg,"is")==0) {
	  opt_yield &= (INSERT_YIELD);
	  opt_yield &= (SEARCH_YIELD);
	} else if (strcmp(optarg,"ds") == 0) {
	  opt_yield &= (DELETE_YIELD);
	  opt_yield &= (SEARCH_YIELD);
	} else if (strcmp(optarg,"i") == 0) {
	  opt_yield &= (INSERT_YIELD);

	} else if (strcmp(optarg,"d") == 0) {
	  opt_yield &= (DELETE_YIELD);
	}
      }
      else {
	fprintf(stderr,"No argument provided for yield option\n");
      }
      break;
    }
    case '?': break;
    }
    
  }
  pthread_t * pthreads = malloc(sizeof(pthread_t) * threads);
  int i = 0;
  list = malloc(sizeof(SortedList_t) * num_lists);
  spin_lock = malloc(sizeof(int) * num_lists);
  mutex = malloc(sizeof(pthread_mutex_t) * num_lists);
  for (i = 0; i < num_lists; i++) {
    list[i].key = NULL;
    list[i].next = &list[i];
    list[i].prev= &list[i];
    spin_lock[i]=0;
    pthread_mutex_init(&mutex[i],NULL);
  }
  SortedListElement_t * elements = malloc(threads*iterations*sizeof(SortedListElement_t));
  for (i = 0; i < threads*iterations; i++) {
    elements[i].key = random_string();
  }
					  
  struct timespec time[2];
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&time[0]);
  for (i = 0; i < threads; i++)
    pthread_create( &pthreads[i], NULL, start_routine, elements + i * iterations);
  
  for (i = 0; i < threads; i++)
    pthread_join(pthreads[i],NULL);
  
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&time[1]);
  long long operations = threads * 3 * iterations;
  long long length = time[1].tv_nsec - time[0].tv_nsec;
  fprintf(stdout,"%d threads x %d iterations x (ins + lookup/del)  = %lld operations\n",threads, iterations,operations);
  fprintf(stdout,"Elapsed Time: %ld ns Nanoseconds per operation: %lld ns\n",length, length/operations);


  if (SortedList_length(list) != 0) {
    fprintf(stderr,"ERROR, the final count is: %d\n",SortedList_length(list));
    for (i = 0; i < iterations*threads; i++){
      free((char *)elements[i].key);
    }
    free(elements);
    free(list);
    free(mutex);
	 free((void*)spin_lock);
    exit(1);
  }
  for (i = 0; i < iterations*threads; i++){
    free((char *)elements[i].key);
  }
  free(elements);
  free(list);
  free(mutex);
  free((void*)spin_lock);
  exit(0);
}
int hash_func(const char * key) { return strlen(key); }
void *  start_routine(void *start_pos) {
  SortedListElement_t * elements = start_pos;
  int i = 0;
  for (i = 0; i < iterations; i++) {
    int which_list = hash_func(elements[i].key) % num_lists;
    
    if (is_mutex) {
      pthread_mutex_lock(&mutex[which_list]);
    }else if (is_spin) {
      while (__sync_lock_test_and_set(&spin_lock[which_list],1));
    }
    SortedList_insert(&list[which_list],&elements[i]);
    if (is_mutex) {
      pthread_mutex_unlock(&mutex[which_list]);
    } else if (is_spin) {
      __sync_lock_release(&spin_lock[which_list]);
    }
  }
  for (i = 0; i < num_lists; i++) {
  if (is_mutex) {
      pthread_mutex_lock(&mutex[i]);
    }else if (is_spin) {
      while (__sync_lock_test_and_set(&spin_lock[i],1));
    }
  }
  SortedList_length(list);
  for (i = 0; i < num_lists; i++) {
    if (is_mutex) {
      pthread_mutex_unlock(&mutex[i]);
    } else if (is_spin) {
      __sync_lock_release(&spin_lock[i]);
    }
  }
  

  for (i = 0; i < iterations; i++) {
    int which_list = hash_func(elements[i].key) % num_lists;

    if (is_mutex) {
      pthread_mutex_lock(&mutex[which_list]);
    }else if (is_spin) {
      while (__sync_lock_test_and_set(&spin_lock[which_list],1));
    }
    SortedListElement_t * element = SortedList_lookup(&list[which_list],elements[i].key);
    if (element != NULL) {
      if(SortedList_delete(element) == -1) fprintf(stderr,"ERROR IN DELETING ELEMENT %d %s\n",i,element->key);
    }
    else
      fprintf(stderr,"Could not find entry element %d %s\n",i,elements[i].key);
    if (is_mutex) {
      pthread_mutex_unlock(&mutex[which_list]);
    } else if (is_spin) {
      __sync_lock_release(&spin_lock[which_list]);
    }
  }
}
