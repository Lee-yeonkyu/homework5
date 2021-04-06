/*
 * postfix.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10 // ������ ũ�Ⱚ
#define MAX_EXPRESSION_SIZE 20 //���� ũ�Ⱚ

// stack ������ �켱����, lparen = 0 ���� ����
typedef enum{
	lparen = 0,  /* ( ���� ��ȣ */
	rparen = 9,  /* ) ������ ��ȣ*/
	times = 7,   /* * ���� */
	divide = 6,  /* / ������ */
	plus = 5,    /* + ���� */
	minus = 4,   /* - ���� */
	operand = 1 /* �ǿ����� */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression�� �����ϴ� �迭 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix�� ����� ���ڿ��� �����ϴ� �迭 */
char postfixStack[MAX_STACK_SIZE];	/* postfix�� ��ȯ�� ���� �ʿ��� ���� */
int evalStack[MAX_STACK_SIZE];		/* ����� ���� �ʿ��� ���� */

int postfixStackTop = -1;  /* postfixStack�� top */
int evalStackTop = -1;	   /* evalStack�� top */

int evalResult = 0;	   /* ��� ����� ���� */

void postfixpush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

int main()
{
	char command;

	printf("[----- [�̿���] [2018038038] -----]\n\n");
	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		fflush(stdout);
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			getInfix();
			break;
		case 'p': case 'P':
			toPostfix();
			break;
		case 'e': case 'E':
			evaluation();
			break;
		case 'd': case 'D':
			debug();
			break;
		case 'r': case 'R':
			reset();
			break;
		case 'q': case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

void postfixPush(char x) //������ȯ�� ���� �Լ��� �ʿ��� ���ÿ� ���� x�� �־��ش�.
{
    postfixStack[++postfixStackTop] = x; // +1���ִ� ����top�� x�� ���� �־��ش�.
}

char postfixPop() // ������ȯ�� ���� ���ÿ��� x�� �� ���� �����Ѵ�.
{
    char x;
    if(postfixStackTop == -1) //������ ��������� ��ĭ�� ��ȯ�Ѵ�.
        return '\0';
    else {
    	x = postfixStack[postfixStackTop--]; // ������� ������ x�� top�� �ִ� ���� �ְ� top�� ��ġ�� -1���ش�.
    }
    return x;
}

void evalPush(int x) // ��꿡 ����ϴ� ���ÿ� ���� x�� �־��ش�.
{
    evalStack[++evalStackTop] = x;
}

int evalPop() // ��꿡 ����ϴ� ���ÿ��� x�� �� ���� �����Ѵ�.
{
    if(evalStackTop == -1) //������ ����ٸ� ������ ����.
        return -1;
    else
        return evalStack[evalStackTop--]; // �Ⱥ�������� top�� ������ �������ְ� -1���ش�.
}

/**
 * infix expression�� �Է¹޴´�.
 * infixExp���� �Էµ� ���� �����Ѵ�.
 */
void getInfix() //������ȯ���� �Է¹޴´�.
{
    printf("Type the expression >>> ");
	fflush(stdout);
    scanf("%s",infixExp);
}

precedence getToken(char symbol) //������ִ� �ϳ��� ���ڰ� �ǿ���������, �����ڶ�� �켱������ �����Ѵ�.
{
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}

precedence getPriority(char x) // �켱���� ��ȯ
{
	return getToken(x);
}

/**
 * �����ϳ��� ���޹޾�, postfixExp�� �߰�
 */
void charCat(char* c) // �����ϳ��� ���޹޾�, postfixExp�� �߰�
{
	if (postfixExp == '\0') //����ִٸ� ù���� ����.
		strncpy(postfixExp, c, 1);
	else// �ƴϸ� ������� �̾ ����.
		strncat(postfixExp, c, 1);
}

/**
 * infixExp�� ���ڸ� �ϳ��� �о�鼭 stack�� �̿��Ͽ� postfix�� �����Ѵ�.
 * ����� postfix�� postFixExp�� ����ȴ�.
 */
void toPostfix()
{
	/* infixExp�� ���� �ϳ����� �б����� ������ */
	char *exp = infixExp;
	char x; /* �����ϳ��� �ӽ÷� �����ϱ� ���� ���� */
	int i=0; //��ū�� �ӽú���.
	/* exp�� �������Ѱ��鼭, ���ڸ� �а� postfix�� ���� */
	while(*exp != '\0')
	{
		i=getToken(*exp); // ���Ŀ��� ���� ������ ��ū�� �����Ѵ�.
		switch (i){
		case operand: // �ǿ����ڶ�� �ٷ� �����Ѵ�.
			charCat(exp);
			break;
		case rparen: // )���ڴ� (���ڰ� ���ö����� pop���ش�.
			while(getToken(postfixStack[postfixStackTop])!=lparen){
				x=postfixPop();
				charCat(&x);
			}
			postfixPop(); // (�� top�̶�� (�������ش�.
			break;
		case lparen: // (���ڶ�� �ٷ� �־��ش�.
			postfixPush(*exp);
			break;
		default:
			while(postfixStackTop !=-1 && postfixStack[postfixStackTop]>=*exp)
			{ //������ ����ְų� top���ִ� ���� �켱������ *exp������ ������������ pop���ش�.
				x=postfixPop();
				charCat(&x);
			}
			postfixPush(*exp);
			break;
		}
		exp++; // ���� ���� �ޱ�.
	}
	while(postfixStackTop != -1) // ���ÿ� �����ִ� ��簪 pop�ؼ� ����.
	{
		x=postfixPop();
		charCat(&x);
	}
}
void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : "); //���ÿ� ������ִ� �� ���.
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

void reset() // �ʱ�ȭ��Ű�� �Լ�.
{
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';

	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

void evaluation() //int evalStackTop = -1;	   /* evalStack�� top */
{ //������ȯ �� ���.

	char *exp=postfixExp; //���ڸ� �ϳ��� �о��� ������ ����.
	int op1,op2; //���꿡 Ȱ���� ���� 2��.
	precedence token= getToken(*exp); //�켱���������� ���� ���� ����.
	while(*exp != '\0'){ //���ĳ����� �ݺ�.
		if(token == operand) //�ǿ����ڶ��
			evalPush((int)*exp-48); //�������� ���ÿ� �־��ش�.
		else{
			op2=evalPop(); //���� ���� �� ������ �־��ֱ�.
			op1=evalPop(); // ���� ���� �� ������ �־��ֱ�.
			switch(token){ //��� �� ���ÿ� �־��ֱ�.
			case plus: evalPush(op1+op2); break;
			case minus: evalPush(op1-op2); break;
			case divide: evalPush(op1/op2); break;
			case times: evalPush(op1*op2); break;
			}
		}
		exp++; // ���� ���� �ޱ�.

		token=getToken(*exp); // ��ū���� �ٲ��ֱ� ���ؼ�.
	}
	evalResult = evalPop(); //������ ���� ����.
}
