/*
 * Author: Dreyton Scott
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/*
 * Global Variables
 */
#define PHILOSOPHERS 5
pthread_t philos[PHILOSOPHERS];
pthread_mutex_t forks[PHILOSOPHERS];
int food = 25;

/*
 * methods
 */
void *philosopher(void *arg);
void take(int p, int f, char *fh);
void release(int p, int f, char *fh);
void eat(int);
void think(int);

/*
 * Philosopher thread
 */
void *philosopher(void *arg){
  int left_fork, right_fork, philoso;
  philoso = (int)arg;
  right_fork = philoso;
  left_fork = philoso + 1;
  if (left_fork == PHILOSOPHERS){
    left_fork = 0;
  }
  while(food>0){
    if (philoso == 1){
      think(philoso);
    }
    take(philoso,right_fork,"Right Fork");
    take(philoso,left_fork,"Left Fork");
    eat(philoso);
    printf("\nPhilosopher %d Done Eating", philoso);
    release(philoso,right_fork,"Right Fork");
    release(philoso,left_fork,"Left Fork");    
    food--;
  }
  return NULL;
}

/*
 * called when philosopher takes a fork. Locks the fork so
 * it may not be used by other threads
 */
void take(int philosopher, int fork, char *fork_hand){
  pthread_mutex_lock(&forks[fork]);
  printf("\nPhilosopher %d has taken the %s:%d", philosopher,fork_hand,fork); 
}

/* 
 * called when philosopher places a fork down. Unlocks the fork
 * for use again.
 */
void release(int philosopher, int fork, char *fork_hand){
  pthread_mutex_unlock(&forks[fork]);
  printf("\nPhilosopher %d has released the %s:%d", philosopher,fork_hand,fork);   
}

/*
 * called when thread begins eating
 */
void eat(int philosopher){
  printf("\nPhilosopher %d has entered and begun eating.", philosopher);
  sleep(1);
}

/*
 * called when thread is thinking
 */
void think(int philosopher){
  printf("\nPhilosopher %d has begun thinking.", philosopher);
  sleep(2);  
}

/*
 * where main thread starts
 */
int main(){
  int i;
  for (i=0;i<PHILOSOPHERS;i++){
    pthread_mutex_init(&forks[i], NULL);
  }  
  for (i=0;i<PHILOSOPHERS;i++){
    pthread_create(&philos[i], NULL, philosopher, (void*)i);
  }
  for (i=0;i<PHILOSOPHERS;i++){
    pthread_join(philos[i], NULL);
  }
  exit(0);
}
