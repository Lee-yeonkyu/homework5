/*
 * circularQ.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4

typedef char element;
typedef struct {
	element queue[MAX_QUEUE_SIZE];
	int front, rear;
}QueueType;


QueueType *createQueue();
int freeQueue(QueueType *cQ);
int isEmpty(QueueType *cQ);
int isFull(QueueType *cQ);
void enQueue(QueueType *cQ, element item);
void deQueue(QueueType *cQ, element* item);
void printQ(QueueType *cQ);
void debugQ(QueueType *cQ);
element getElement();

int main(void)
{
	QueueType *cQ = createQueue();
	element data;
	char command;

	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		fflush(stdout);
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			data = getElement();
			enQueue(cQ, data);
			break;
		case 'd': case 'D':
			deQueue(cQ, &data);
			break;
		case 'p': case 'P':
			printQ(cQ);
			break;
		case 'b': case 'B':
			debugQ(cQ);
			break;
		case 'q': case 'Q':
   	        freeQueue(cQ);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');


	return 1;
}

QueueType *createQueue()// 빈 큐를 만들어주는 함수.
{
	QueueType *cQ;
	cQ = (QueueType *)malloc(sizeof(QueueType)); // 동적할당
	cQ->front = 0;
	cQ->rear = 0;
	return cQ;
}

int freeQueue(QueueType *cQ) // 큐에 할당한 메모리을 초기화 해주는 함수.
{
    if(cQ == NULL) return 1;
    free(cQ);
    return 1;
}

element getElement() //큐에 들어갈 값을 입력받는 함수.
{
	element item;
	printf("Input element = ");
	fflush(stdout);
	scanf(" %c", &item);
	return item;
}


/* complete the function */
int isEmpty(QueueType *cQ)
{
	if(cQ->front == cQ->rear){ //front값과 rear값이 같다면 출력.
		printf("Circular Queue is empty!\n");
			return 1;}
		else
			return -1;

}

/* complete the function */
int isFull(QueueType *cQ) //  큐 안의 값이 꽉찼는지 확인하는 함수.
{
   if((cQ->rear+1)% MAX_QUEUE_SIZE == cQ->front){ // (rear+1)%queue size값과 front값이 같다면 출력
   		printf("Circular Queue is Full!\n");
   			return 1;}
   		else
   			return -1;
}


/* complete the function */
void enQueue(QueueType *cQ, element item) // 큐에 push를 실행하는 함수.
{
	if(isFull(cQ)!=1){ // 큐에 더 이상 추가 할 수 없을때 함수를 나간다.
		cQ->rear = (cQ->rear +1) %MAX_QUEUE_SIZE-1;
		cQ->queue[++cQ->rear]=item; //queue에서 rear의 다음칸에 item의 값을 넣는다.
	}
}

/* complete the function */
void deQueue(QueueType *cQ, element *item) // 큐에 pop를 실행하는 함수.
{
  	if(isEmpty(cQ)!=1)	{ // 큐가 비었을때 함수를 나간다.
		cQ->front = (cQ->front+1)%MAX_QUEUE_SIZE-1;
		cQ->queue[++cQ->front]=item; // queue에서 front의 위치를 옮겨준다
	}
}


void printQ(QueueType *cQ) //큐를 출력하는 함수
{
	int i, first, last;

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; //큐의 시작부분.
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE; // 큐의 끝부분.

	printf("Circular Queue : [");

	i = first;
	while(i != last){ // i값이 끝부분보다 크면 멈춘다.
		printf("%3c", cQ->queue[i]);
		i = (i+1)%MAX_QUEUE_SIZE;

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ) //큐에서의 front값, rear값, 함수안의 내용을 출력하는 함수.
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++) //큐의 크기만큼 반복.
	{
		if(i == cQ->front) { //front의 위치에 출력.
			printf("  [%d] = front\n", i);
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); // front와 rear의 위치 출력.
}

