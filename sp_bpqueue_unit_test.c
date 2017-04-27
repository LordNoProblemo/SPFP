#include "../SPBPriorityQueue.h"
#include "unit_test_util.h"
#include <stdbool.h>
#include <stdlib.h>

bool testBPQCopy() {
	int indices[3] = { 1, 2, 3 };
	int values[3] = { 6, 5, 4 };
	SPBPQueue* q1 = spBPQueueCreate(3);
	ASSERT_TRUE(spBPQueueSize(q1) == 0);
	for (int i = 0; i < 3; i++)
		spBPQueueEnqueue(q1, indices[i], values[i]);
	ASSERT_TRUE(spBPQueueGetMaxSize(q1) == 3);
	ASSERT_TRUE(spBPQueueSize(q1) == 3);
	ASSERT_TRUE(spBPQueueMinValue(q1) == 4);
	ASSERT_TRUE(spBPQueueMaxValue(q1) == 6);
	SPBPQueue* q2 = spBPQueueCopy(q1);
	ASSERT_TRUE(spBPQueueGetMaxSize(q2) == 3);
	ASSERT_TRUE(spBPQueueSize(q2) == 3);
	ASSERT_TRUE(spBPQueueMinValue(q2) == 4);
	ASSERT_TRUE(spBPQueueMaxValue(q2) == 6);
	spBPQueueDequeue(q1);
	ASSERT_TRUE(spBPQueueGetMaxSize(q1) == 3);
	ASSERT_TRUE(spBPQueueSize(q1) == 2);
	ASSERT_TRUE(spBPQueueMinValue(q1) == 5);
	spBPQueueDestroy(q1);
	spBPQueueDestroy(q2);
	return true;
}

bool testBPQClear() {
	SPBPQueue* cleared = spBPQueueCreate(5);
	SPBPQueue* notCleared = spBPQueueCreate(5);
	ASSERT_TRUE(
			spBPQueueGetMaxSize(notCleared) == spBPQueueGetMaxSize(cleared));
	spBPQueueEnqueue(notCleared, 1, 42);
	spBPQueueEnqueue(notCleared, 3, 43);
	spBPQueueEnqueue(notCleared, 6, 42.5);
	spBPQueueEnqueue(notCleared, -10, 0);
	ASSERT_TRUE(spBPQueueSize(notCleared) == 4);
	ASSERT_TRUE(spBPQueueMaxValue(notCleared) == 43);
	ASSERT_TRUE(spBPQueueMinValue(notCleared) == 0);
	spBPQueueDequeue(notCleared);
	ASSERT_TRUE(spBPQueueMinValue(notCleared) == 42);
	spBPQueueClear(notCleared);
	ASSERT_TRUE(spBPQueueIsEmpty(notCleared));
	spBPQueueDestroy(cleared);
	spBPQueueDestroy(notCleared);
	return true;

}

bool testBPQPeek() {
	SPBPQueue* queue = spBPQueueCreate(5);
	spBPQueueEnqueue(queue, 1574, 254);
	spBPQueueEnqueue(queue, 2212, 3316);
	spBPQueueEnqueue(queue, 2056, 610);
	spBPQueueEnqueue(queue, 3339, 1114);
	BPQueueElement element;
	spBPQueuePeek(queue, &element);
	ASSERT_TRUE(element.index == 1574);
	ASSERT_TRUE(element.value == 254);
	spBPQueueDequeue(queue);
	spBPQueuePeek(queue, &element);
	ASSERT_TRUE(element.index == 2056);
	ASSERT_TRUE(element.value == 610);
	spBPQueuePeekLast(queue, &element);
	ASSERT_TRUE(element.index == 2212);
	ASSERT_TRUE(element.value == 3316);
	spBPQueuePeekLast(queue, &element);
	ASSERT_TRUE(element.index == 2212);
	ASSERT_TRUE(element.value == 3316);

	spBPQueueDestroy(queue);
	return true;
}

int main()
{
	RUN_TEST(testBPQCopy);
	RUN_TEST(testBPQClear);
	RUN_TEST(testBPQPeek);
	return 0;
}
