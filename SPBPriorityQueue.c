#include<stdlib.h>

#include "SPBPriorityQueue.h"

//Define the struct of queue
struct sp_bp_queue_t{
	int capacity; // The capacity of the queue
	int size; //Amount of elements in queue
	//Elements indexes are MinIndex<=i<MinIndex+size<=capacity
	int MinIndex;//The index of the element with lowest value in the elements array, -1<=> queue is empty
	BPQueueElement* elements; //The array of Elements
};

SPBPQueue* spBPQueueCreate(int maxSize)
{
	SPBPQueue* ret = (SPBPQueue*)malloc(sizeof(SPBPQueue));
	if(ret == NULL)
		return NULL;
	ret->MinIndex = -1;//Queue is empty
	ret->size = 0;//Queue is empty
	ret->capacity = maxSize;
	ret->elements = (BPQueueElement*)malloc(sizeof(BPQueueElement)*maxSize);//Initialize memory for the array with size maxSize
	if(ret->elements == NULL)
	{
		spBPQueueDestroy(ret);
		return NULL;
	}
	return ret;
}

/*
 * Creates new element of queue
 * @param index - the index of new element
 * @param val - the value of new element
 * @return
 * The new element
 */
BPQueueElement createElement(int index, double val)
{
	BPQueueElement ret;
	ret.index = index;
	ret.value = val;
	return ret;
}

/**
 * Returns the index of element with max value in queue in the array
 * @param source - the queue
 * @assert source!=NULL and source isn't empty
 */
int MaxIndex(SPBPQueue* source)
{
	return source->MinIndex + source->size-1;
}

SPBPQueue* spBPQueueCopy(SPBPQueue* source)
{
	SPBPQueue* ret = spBPQueueCreate(source->capacity);//Initialize memory for the elements
	ret->MinIndex = source->MinIndex;
	ret->size = source->size;
	// Updates the elements of ret to be the same as those of source
	int i = 0;

	for(; i < ret->size ; i++)
	{
		ret->elements[i+ret->MinIndex] = createElement(source->elements[i+ret->MinIndex].index,source->elements[i+ret->MinIndex].value);
	}
	return ret;
}

int spBPQueueSize(SPBPQueue* source)
{
	return source->size;
}

int spBPQueueGetMaxSize(SPBPQueue* source)
{
	return source->capacity;
}

bool spBPQueueIsEmpty(SPBPQueue* source)
{
	return source->size == 0;
}

bool spBPQueueIsFull(SPBPQueue* source)
{
	return source->size == source->capacity;
}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue* source)
{
	if(source == NULL)
		return SP_BPQUEUE_INVALID_ARGUMENT;
	if(spBPQueueIsEmpty(source))
		return SP_BPQUEUE_EMPTY;
    source->elements[source->MinIndex].index = 0;
    source->elements[source->MinIndex].value = 0;
	source->size--;
	if(source->size==0)
		source->MinIndex = -1;
	else
		source->MinIndex++;
	return SP_BPQUEUE_SUCCESS;

}

double spBPQueueMinValue(SPBPQueue* source)
{
	return source->elements[source->MinIndex].value;
}

double spBPQueueMaxValue(SPBPQueue* source)
{
	return source->elements[MaxIndex(source)].value;
}

SP_BPQUEUE_MSG spBPQueuePeekLast(SPBPQueue* source, BPQueueElement* res)
{
	if(source == NULL)
		return SP_BPQUEUE_INVALID_ARGUMENT;
	if(spBPQueueIsEmpty(source))
		return SP_BPQUEUE_EMPTY;
	*res = createElement(source->elements[MaxIndex(source)].index,source->elements[MaxIndex(source)].value);
	return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueuePeek(SPBPQueue* source, BPQueueElement* res)
{
	if(source == NULL)
		return SP_BPQUEUE_INVALID_ARGUMENT;
	if(spBPQueueIsEmpty(source))
		return SP_BPQUEUE_EMPTY;
	*res = createElement(source->elements[source->MinIndex].index,source->elements[source->MinIndex].value);
	return SP_BPQUEUE_SUCCESS;
}

void spBPQueueClear(SPBPQueue* source)
{
	int i = 0;
	for(; i < source->capacity ; i++)
	{
		source->elements[i].index = 0;
		source->elements[i].value = 0;
	}
	source->size = 0;
	source->MinIndex = -1;
}

void spBPQueueDestroy(SPBPQueue* source)
{
	if(source == NULL)
		return;
	if(source->elements != NULL)
		free(source->elements);
	free(source);
}

/*
 * Returns the place in array of the new element according to value
 * @param array - the array of elements
 * @assert array is not empty and array!=null
 * @param start - the index from which we start searching
 * @param end - the index on which we end searching
 * @assert start<=end
 * @param value - the value of new element
 * @return
 * The index i for which is the place of the new element
 */
int findPlace(BPQueueElement* array, int start, int end, double value,int idx)
{
	if(value < array[start].value)
		return start-1;
	if(value > array[end].value)
		return end + 1;
	int i = start;
	while(i < end+1 && array[i].value<value)
        i++;
    while(i < end + 1 && array[i].value == value && array[i].index < idx)
        i++;
	return i;
}


SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue* source, int index, double value)
{
	if(source == NULL)
			return SP_BPQUEUE_INVALID_ARGUMENT;

	if(spBPQueueIsEmpty(source))
	{
		source->MinIndex = 0;
		source->size = 1;
		source->elements[0] = createElement(index,value);
		return SP_BPQUEUE_SUCCESS;
	}
	int insertIndex = findPlace(source->elements,source->MinIndex,MaxIndex(source),value, index);
	if(insertIndex > MaxIndex(source))
	{
		if(spBPQueueIsFull(source))
			return SP_BPQUEUE_FULL;

		if(MaxIndex(source) == source->capacity - 1)
		{
			for(int i = source->MinIndex ; i <= source->capacity-1 ; i++)
			{
				if(i == 0)
					return SP_BPQUEUE_OUT_OF_MEMORY;
				source->elements[i - 1] = source->elements[i];
			}
			source->MinIndex--;
			source->elements[source->capacity - 1] = createElement(index,value);
		}
		else
		{
			source->elements[insertIndex] = createElement(index,value);
		}
		source->size++;
		return SP_BPQUEUE_SUCCESS;
	}

	if(insertIndex < source->MinIndex)
	{
		if(source->MinIndex > 0)
		{
			source->MinIndex--;
			source->elements[insertIndex] = createElement(index,value);
			source->size++;
		}
		else
		{
			if(spBPQueueIsFull(source))
			{
				source->size--;
			}
			for(int i = MaxIndex(source) ; i >= 0 ; i--)
			{
				if(i >= source->capacity)
					return SP_BPQUEUE_OUT_OF_MEMORY;
				source->elements[i + 1] = source->elements[i];
			}
			source->elements[0] = createElement(index,value);
			source->size++;
		}
		return SP_BPQUEUE_SUCCESS;
	}
	if(spBPQueueIsFull(source))
	{
		source->size--;
	}
	if(MaxIndex(source) < source->capacity - 1)
	{
		for(int i = MaxIndex(source) ; i >= insertIndex ; i--)
		{
			if(i >= source->capacity)
				return SP_BPQUEUE_OUT_OF_MEMORY;
			source->elements[i + 1] = source->elements[i];
		}
	}
	else
	{
		for(int i = source->MinIndex ; i <= insertIndex ; i++)
		{
			if(i==0)
				return SP_BPQUEUE_OUT_OF_MEMORY;
			source->elements[i - 1] = source->elements[i];
		}
		source->MinIndex--;
	}
	source->size++;
	source->elements[insertIndex] = createElement(index,value);
	return SP_BPQUEUE_SUCCESS;
}



