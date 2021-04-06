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

QueueType *createQueue()// �� ť�� ������ִ� �Լ�.
{
	QueueType *cQ;
	cQ = (QueueType *)malloc(sizeof(QueueType)); // �����Ҵ�
	cQ->front = 0;
	cQ->rear = 0;
	return cQ;
}

int freeQueue(QueueType *cQ) // ť�� �Ҵ��� �޸��� �ʱ�ȭ ���ִ� �Լ�.
{
    if(cQ == NULL) return 1;
    free(cQ);
    return 1;
}

element getElement() //ť�� �� ���� �Է¹޴� �Լ�.
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
	if(cQ->front == cQ->rear){ //front���� rear���� ���ٸ� ���.
		printf("Circular Queue is empty!\n");
			return 1;}
		else
			return -1;

}

/* complete the function */
int isFull(QueueType *cQ) //  ť ���� ���� ��á���� Ȯ���ϴ� �Լ�.
{
   if((cQ->rear+1)% MAX_QUEUE_SIZE == cQ->front){ // (rear+1)%queue size���� front���� ���ٸ� ���
   		printf("Circular Queue is Full!\n");
   			return 1;}
   		else
   			return -1;
}


/* complete the function */
void enQueue(QueueType *cQ, element item) // ť�� push�� �����ϴ� �Լ�.
{
	if(isFull(cQ)!=1){ // ť�� �� �̻� �߰� �� �� ������ �Լ��� ������.
		cQ->rear = (cQ->rear +1) %MAX_QUEUE_SIZE-1;
		cQ->queue[++cQ->rear]=item; //queue���� rear�� ����ĭ�� item�� ���� �ִ´�.
	}
}

/* complete the function */
void deQueue(QueueType *cQ, element *item) // ť�� pop�� �����ϴ� �Լ�.
{
  	if(isEmpty(cQ)!=1)	{ // ť�� ������� �Լ��� ������.
		cQ->front = (cQ->front+1)%MAX_QUEUE_SIZE-1;
		cQ->queue[++cQ->front]=item; // queue���� front�� ��ġ�� �Ű��ش�
	}
}


void printQ(QueueType *cQ) //ť�� ����ϴ� �Լ�
{
	int i, first, last;

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; //ť�� ���ۺκ�.
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE; // ť�� ���κ�.

	printf("Circular Queue : [");

	i = first;
	while(i != last){ // i���� ���κк��� ũ�� �����.
		printf("%3c", cQ->queue[i]);
		i = (i+1)%MAX_QUEUE_SIZE;

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ) //ť������ front��, rear��, �Լ����� ������ ����ϴ� �Լ�.
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++) //ť�� ũ�⸸ŭ �ݺ�.
	{
		if(i == cQ->front) { //front�� ��ġ�� ���.
			printf("  [%d] = front\n", i);
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); // front�� rear�� ��ġ ���.
}

