#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include <stdbool.h>

/**
 * SP Bounded Priority Queue summary
 *
 * TODO Complete documentation
 */


/** type used to define Bounded priority queue **/
typedef struct sp_bp_queue_t SPBPQueue;

typedef struct sp_bpq_element_t {
	int index;
	double value;
} BPQueueElement;

/** type for error reporting **/
typedef enum sp_bp_queue_msg_t {
	SP_BPQUEUE_OUT_OF_MEMORY,
	SP_BPQUEUE_FULL,
	SP_BPQUEUE_EMPTY,
	SP_BPQUEUE_INVALID_ARGUMENT,
	SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;

/**
 * Creates new empty queue with capacity maxSize
 * @param maxSize - capacity of the new queue
 * @assert maxSize>0
 * @return
 *  New empty queue with capacity=maxSize
 */
SPBPQueue* spBPQueueCreate(int maxSize);

/**
 * Creates a copy of the queue source
 * @param source - the queue to be copied
 * @assert source!=NULL
 * @return
 * A copy of source
 */
SPBPQueue* spBPQueueCopy(SPBPQueue* source);

/**
 * Frees all memory associated with queue
 * @param source - the queue
 * @assert source !=null
 */
void spBPQueueDestroy(SPBPQueue* source);

/**
 * Removes all elements in queue
 * @param source - the queue
 * @assert source!=NULL
 *
 */
void spBPQueueClear(SPBPQueue* source);

/**
 * Returns the amount of elements in queue source
 * @param source - the queue
 * @assert source!=null
 * @return
 * Amount of elements is source
 */
int spBPQueueSize(SPBPQueue* source);

/**
 * Returns the max capacity of queue source
 * @param source - the queue
 * @assert source!=null
 * @return
 * Max capacity of source
 */
int spBPQueueGetMaxSize(SPBPQueue* source);

/**
 * Insert new element to queue if queue isn't full
 * or if queue is full but the new value is less the the max value in queue
 * @param source - the queue
 * @assert source != NULL
 * @param index - the index of new element
 * @param value - the value of new element
 * @return
 * SP_BPQUEUE_INVALID_ARGUMENT if source is NULL,
 * SP_BPQUEUE_FULL if source is full and cannot add new element
 * SP_BPQUEUE_SUCCESS else
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue* source, int index, double value);

/**
 * Removes the element with lowest value in source
 * @param source - the queue
 * @return
 *
 * SP_BPQUEUE_INVALID_ARGUMENT if source == null, SP_BPQUEUE_EMPTY if queue is Empty, SP_BPQUEUE_SUCCESS else
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue* source);

/**
 * Returns a copy of element with minimum value in queue
 * @param source - the queue
 * @param res - the element which will hold the copy of element with min value
 * @return
 * SP_BPQUEUE_EMPTY if source is empty,SP_BPQUEUE_INVALID_ARGUMENT if source==NULL, SP_BPQUEUE_SUCCESS else
 */
SP_BPQUEUE_MSG spBPQueuePeek(SPBPQueue* source, BPQueueElement* res);

/**
 * Returns a copy of element with maximum value in queue
 * @param source - the queue
 * @param res - the element which will hold the copy of element with max value
 * @return
 * SP_BPQUEUE_EMPTY if source is empty,SP_BPQUEUE_INVALID_ARGUMENT if source==NULL, SP_BPQUEUE_SUCCESS else
 */
SP_BPQUEUE_MSG spBPQueuePeekLast(SPBPQueue* source, BPQueueElement* res);

/** Returns the minimum value in queue
 * @param source - queue
 * @assert source != NULL and source is not empty
 * @return
 * The minimum value in queue sorce
 */
double spBPQueueMinValue(SPBPQueue* source);

/** Returns the maximum value in queue
 * @param source - queue
 * @assert source != NULL and source is not empty
 * @return
 * The maximum value in queue source
 */
double spBPQueueMaxValue(SPBPQueue* source);

/**
 * Checks if queue source is empty
 * @param source - the queue
 * @assert source!=null
 * @return
 * true iff source->size==0
 */
bool spBPQueueIsEmpty(SPBPQueue* source);

/**
 *Checks if queue source is Full
 * @param source - the queue
 * @assert source!=null
 * @return
 * true iff source->size==source->capacity
 */
bool spBPQueueIsFull(SPBPQueue* source);

#endif
