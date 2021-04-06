/*
 * postfix.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10 // 스택의 크기값
#define MAX_EXPRESSION_SIZE 20 //식의 크기값

// stack 내에서 우선순위, lparen = 0 가장 낮음
typedef enum{
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   /* 계산 결과를 저장 */

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

	printf("[----- [이연규] [2018038038] -----]\n\n");
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

void postfixPush(char x) //후위변환을 위한 함수로 필요한 스택에 문자 x를 넣어준다.
{
    postfixStack[++postfixStackTop] = x; // +1되있는 스택top에 x의 값을 넣어준다.
}

char postfixPop() // 후위변환을 위해 스택에서 x를 뺀 것을 리턴한다.
{
    char x;
    if(postfixStackTop == -1) //스택이 비어있으면 빈칸을 반환한다.
        return '\0';
    else {
    	x = postfixStack[postfixStackTop--]; // 비어있지 않으면 x에 top에 있는 값을 넣고 top의 위치를 -1해준다.
    }
    return x;
}

void evalPush(int x) // 계산에 사용하는 스택에 정수 x를 넣어준다.
{
    evalStack[++evalStackTop] = x;
}

int evalPop() // 계산에 사용하는 스택에서 x를 뺀 것을 리턴한다.
{
    if(evalStackTop == -1) //스택이 비었다면 비정상값 리턴.
        return -1;
    else
        return evalStack[evalStackTop--]; // 안비어있으면 top의 정수를 리턴해주고 -1해준다.
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix() //중위변환식을 입력받는다.
{
    printf("Type the expression >>> ");
	fflush(stdout);
    scanf("%s",infixExp);
}

precedence getToken(char symbol) //저장되있는 하나의 문자가 피연산자인지, 연산자라면 우선순위를 배정한다.
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

precedence getPriority(char x) // 우선순위 반환
{
	return getToken(x);
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c) // 문자하나를 전달받아, postfixExp에 추가
{
	if (postfixExp == '\0') //비어있다면 첫문자 복사.
		strncpy(postfixExp, c, 1);
	else// 아니면 순서대로 이어서 복사.
		strncat(postfixExp, c, 1);
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp;
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */
	int i=0; //토큰값 임시변수.
	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0')
	{
		i=getToken(*exp); // 계산식에서 읽을 문자의 토큰을 저장한다.
		switch (i){
		case operand: // 피연산자라면 바로 저장한다.
			charCat(exp);
			break;
		case rparen: // )문자는 (문자가 나올때까지 pop해준다.
			while(getToken(postfixStack[postfixStackTop])!=lparen){
				x=postfixPop();
				charCat(&x);
			}
			postfixPop(); // (이 top이라면 (제거해준다.
			break;
		case lparen: // (문자라면 바로 넣어준다.
			postfixPush(*exp);
			break;
		default:
			while(postfixStackTop !=-1 && postfixStack[postfixStackTop]>=*exp)
			{ //스택이 비어있거나 top에있는 값의 우선순위가 *exp값보다 낮아질때까지 pop해준다.
				x=postfixPop();
				charCat(&x);
			}
			postfixPush(*exp);
			break;
		}
		exp++; // 다음 문자 받기.
	}
	while(postfixStackTop != -1) // 스택에 남아있는 모든값 pop해서 비우기.
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

	printf("postfixStack : "); //스택에 저장되있는 값 출력.
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

void reset() // 초기화시키는 함수.
{
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';

	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

void evaluation() //int evalStackTop = -1;	   /* evalStack용 top */
{ //후위변환 후 계산.

	char *exp=postfixExp; //문자를 하나씩 읽어줄 포인터 선언.
	int op1,op2; //연산에 활용할 변수 2개.
	precedence token= getToken(*exp); //우선순위설정을 위한 변수 선언.
	while(*exp != '\0'){ //계산식끝까지 반복.
		if(token == operand) //피연산자라면
			evalPush((int)*exp-48); //정수값을 스택에 넣어준다.
		else{
			op2=evalPop(); //스택 삭제 및 변수에 넣어주기.
			op1=evalPop(); // 스택 삭제 및 변수에 넣어주기.
			switch(token){ //계산 후 스택에 넣어주기.
			case plus: evalPush(op1+op2); break;
			case minus: evalPush(op1-op2); break;
			case divide: evalPush(op1/op2); break;
			case times: evalPush(op1*op2); break;
			}
		}
		exp++; // 다음 문자 받기.

		token=getToken(*exp); // 토큰값을 바꿔주기 위해서.
	}
	evalResult = evalPop(); //최종값 변수 대입.
}
