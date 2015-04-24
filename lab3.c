
/*****************************************************************************\
* Laboratory Exercises COMP 3510                                              *
* Author: Saad Biaz                                                           *
* Date  : April 1, 2014                                                   *
\*****************************************************************************/

/*****************************************************************************\
*                             Global system headers                           *
\*****************************************************************************/


#include "common.h"

/*****************************************************************************\
*                             Global data types                               *
\*****************************************************************************/

/*****************************************************************************\
*                             Global definitions                              *
\*****************************************************************************/
#define MAX_QUEUE_SIZE 32 /* Max queue size */
#define Q 2 /* Queue depth for each device */

/*****************************************************************************\
*                            Global data structures                           *
\*****************************************************************************/
typedef struct EventQueue {
  Identifier head;
  Identifier tail;
  Event contents[Q];
} EventQueue;

const struct EventQueue BLANK_QUEUE = { .head = 0, .tail = -1 };

EventQueue * enqueue(EventQueue *eq, Event e) {
  if(eq->tail == -1) {
    eq->tail = eq->head;
    eq->contents[eq->head] = e;
  }
  else if(eq->tail + 1 == eq->head
  ||(eq->tail + 1 >= Q && eq->head == 0)) {
    // we're full, so print that and drop the event
    printf("Event queue is full. (Head: %d, tail: %d)\n", eq->head, eq->tail);
  }
  else {
    int newTail = eq->tail + 1;
    if(newTail >= Q) newTail = 0;
    eq->contents[newTail] = e;
    eq->tail = newTail;
  }
  return eq;
}

int isEmpty(EventQueue *eq) {
  if(eq->tail == -1) return TRUE;
  return FALSE;
}

EventQueue * dequeue(EventQueue *eq) {
  if(isEmpty(eq) == FALSE) {

    eq->contents[eq->head].EventID = -1;

    if(eq->head == eq->tail) {
      // Now it's empty, so mess it all up
      eq->tail = -1;
    }
    else if (eq->head + 1 == Q) {
      eq->head = 0;
    }
    else {
      eq->head++;
    }
  }
  else {
    printf("Attempted to dequeue from an empty queue.");
  }
  return eq;
}

Event * peek(EventQueue *eq) {
  // If they're different (so the queue has contents),
  // or they're the same, but nonnegative (so the queue has one element),
  if(isEmpty(eq) == FALSE) {
    // return the front element.
    return &eq->contents[eq->head];
  }
  // Otherwise, return a bogus event.
  return NULL;
}



/*****************************************************************************\
*                                  Global data                                *
\*****************************************************************************/
EventQueue queue [MAX_NUMBER_DEVICES];
void QueueInitialization() {
  int x;
  for(x = 0; x <= MAX_NUMBER_DEVICES; x++) {
    queue[x] = BLANK_QUEUE;
  }
}


/*****************************************************************************\
*                               Function prototypes                           *
\*****************************************************************************/

void Control(void);
void InterruptRoutineHandlerDevice(void);
void BookKeeping();


/*****************************************************************************\
* function: main()                                                            *
* usage:    Create an artificial environment for embedded systems. The parent *
*           process is the "control" process while children process will gene-*
*           generate events on devices                                        *
*******************************************************************************
* Inputs: ANSI flat C command line parameters                                 *
* Output: None                                                                *
*                                                                             *
* INITIALIZE PROGRAM ENVIRONMENT                                              *
* START CONTROL ROUTINE                                                       *
\*****************************************************************************/

int main (int argc, char **argv) {

   if (Initialization(argc,argv)){
     Control();
   }
} /* end of main function */

/***********************************************************************\
 * Input : none                                                          *
 * Output: None                                                          *
 * Function: Monitor Devices and process events (written by students)    *
 \***********************************************************************/
void Control(void){

  Identifier did = 0; // Device ID
  QueueInitialization();
  while (1) {
    if(!isEmpty(&queue[did])) {
      Server(peek(&queue[did]));
      queue[did] = *dequeue(&queue[did]);
      did = 0;
      continue;
    }
    did += 1;
    if(did >= MAX_NUMBER_DEVICES) did = 0;
  }
}


/***********************************************************************\
* Input : None                                                          *
* Output: None                                                          *
* Function: This routine is run whenever an event occurs on a device    *
*           The id of the device is encoded in the variable flag        *
\***********************************************************************/
void InterruptRoutineHandlerDevice(void){
  printf("An event occured at %f  Flags = %d \n", Now(), Flags);
	// Put Here the most urgent steps that cannot wait
  Status flags = Flags; // lower-case is our temp clone
  Identifier did = 0; // Device ID

  Flags = 0; // Allow Flags to start counting events again immediately

  while(flags) {
    if(flags & 1) {
      queue[did] = *enqueue(&queue[did], BufferLastEvent[did]);
    }
    did++;
    flags >>= 1;
  }
}


/***********************************************************************\
* Input : None                                                          *
* Output: None                                                          *
* Function: This must print out the number of Events buffered not yet   *
*           not yet processed (Server() function not yet called)        *
\***********************************************************************/
void BookKeeping(void){
  // For EACH device, print out the following metrics :
  // 1) the percentage of missed events, 2) the average response time, and
  // 3) the average turnaround time.
  // Print the overall averages of the three metrics 1-3 above
}
