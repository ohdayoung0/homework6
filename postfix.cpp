/**
 * postfix.c
 *
 * School of Computer Science,
 * Chungbuk National University
 */

#include<stdio.h> // <stdio.h> 라이브러리를 포함
#include<stdlib.h> // <stdlib.h> 라이브러리를 포함
#include<string.h> // <string.h> 라이브러리를 포함

#define MAX_STACK_SIZE 10 // stack의 크기를 10으로 정의
#define MAX_EXPRESSION_SIZE 20 // expression의 크기를 20으로 정의

 /* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum {
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

int main() //main 함수 시작
{
	printf("-----[오다영] [2023041090]-----\n"); // 이름 학번 출력
	char command;

	do {
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch (command) {
		case 'i': case 'I':
			getInfix(); // Infix 표현식을 입력받는 함수 호출
			break;
		case 'p': case 'P':
			toPostfix(); // Infix 표현식을 Postfix 표현식으로 변환하는 함수 호출
			break;
		case 'e': case 'E':
			evaluation(); // Postfix 표현식을 평가하는 함수 호출
			break;
		case 'd': case 'D':
			debug(); // 디버그 정보 출력 함수 호출
			break;
		case 'r': case 'R':
			reset(); // 모든 변수와 스택을 초기화하는 함수 호출
			break;
		case 'q': case 'Q':
			break; // 프로그램 종료
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	} while (command != 'q' && command != 'Q'); // 'q'나 'Q'가 입력되기 전까지 반복

	return 1; // 프로그램이 종류되면 1을 반환
}

void postfixPush(char x) // postfixStack에 문자를 push하는 함수.
{
	postfixStack[++postfixStackTop] = x; // postfixStack 배열의 top을 증가시키고 x를 저장
}

char postfixPop() // postfixStack에서 문자를 pop하는 함수 
{
	char x;
	if (postfixStackTop == -1)
		return '\0'; // 스택이 비어있으면 널 문자 반환
	else {
		x = postfixStack[postfixStackTop--]; // top에 있는 값을 꺼내고 top을 감소
	}
	return x; // 꺼낸 값 반환
}

void evalPush(int x) // evalStack에 정수를 push하는 함수
{
	evalStack[++evalStackTop] = x; // evalStack 배열의 top을 증가시키고 x를 저장
}

int evalPop() // evalStack에서 정수를 pop하는 함수
{
	if (evalStackTop == -1)
		return -1; // 스택이 비어있으면 -1 반환
	else
		return evalStack[evalStackTop--]; // top에 있는 값을 꺼내고 top을 감소
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
	printf("Type the expression >>> ");
	scanf("%s", infixExp); // 사용자로부터 infix 표현식 입력받아 infixExp 배열에 저장
}

precedence getToken(char symbol)
{ // 주어진 문자(symbol)에 해당하는 연산자 또는 피연산자의 우선순위를 반환하는 함수

	switch (symbol) {
	case '(': return lparen; // '('인 경우 lparen 우선순위 반환
	case ')': return rparen; // ')'인 경우 rparen 우선순위 반환
	case '+': return plus; // '+'인 경우 plus 우선순위 반환
	case '-': return minus; // '-'인 경우 minus 우선순위 반환
	case '/': return divide; // '/'인 경우 divide 우선순위 반환
	case '*': return times; // '*'인 경우 times 우선순위 반환
	default: return operand; // 그 외의 경우 operand 우선순위 반환
	}
}

precedence getPriority(char x) // 주어진 문자(x)에 해당하는 연산자의 우선순위를 반환하는 함수
{
	return getToken(x); // getToken 함수 호출하여 주어진 문자에 해당하는 연산자의 우선순위 반환
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == '\0') // postfixExp 배열이 비어있는 경우
		strncpy(postfixExp, c, 1); // c 문자를 postfixExp 배열에 복사
	else
		strncat(postfixExp, c, 1); // c 문자를 postfixExp 배열에 이어붙임
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	char* exp = infixExp; // infixExp의 문자를 읽기 위한 포인터
	char x; // 문자 하나를 임시로 저장하기 위한 변수

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while (*exp != '\0')
	{
		x = *exp; // 현재 문자를 읽음

		/* 피연산자인 경우 */
		if (getToken(x) == operand) {
			charCat(&x); // postfixExp에 피연산자 추가
		}
		/* 연산자인 경우 */
		else {
			precedence tokenPriority = getToken(x); // 현재 연산자의 우선순위

			/* 스택이 비어있거나 현재 연산자의 우선순위가 스택의 top보다 높은 경우 */
			if (postfixStackTop == -1 || tokenPriority > getPriority(postfixStack[postfixStackTop])) {
				postfixPush(x); // 현재 연산자를 스택에 push
			}
			/* 스택의 top에 있는 연산자의 우선순위가 더 높거나 같은 경우 */
			else {
				/* 현재 연산자의 우선순위가 스택의 top에 있는 연산자의 우선순위보다 낮을 때까지 스택에서 pop하고 postfixExp에 추가 */
				while (postfixStackTop != -1 && tokenPriority <= getPriority(postfixStack[postfixStackTop])) {
					charCat(&postfixStack[postfixStackTop]); // postfixExp에 스택의 top에 있는 연산자 추가
					postfixPop(); // 스택에서 pop
				}
				postfixPush(x); // 현재 연산자를 스택에 push
			}
		}
		exp++; // 다음 문자로 이동
	}

	/* 스택에 남아있는 모든 연산자를 postfixExp에 추가 */
	while (postfixStackTop != -1) {
		charCat(&postfixStack[postfixStackTop]); // postfixExp에 스택의 top에 있는 연산자 추가
		postfixPop(); // 스택에서 pop
	}
}

void debug() // 프로그램 디버그 정보를 출력하는 함수
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for (int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]); // postfixStack 배열의 내용 출력

	printf("\n");

}

void reset() // 프로그램을 초기화하는 함수
{
	infixExp[0] = '\0'; // infixExp 배열 초기화
	postfixExp[0] = '\0'; // postfixExp 배열 초기화

	for (int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0'; // postfixStack 배열 초기화

	postfixStackTop = -1; // postfixStackTop 초기화
	evalStackTop = -1; // evalStackTop 초기화
	evalResult = 0; // evalResult 초기화
}

void evaluation() 
//postfixExp와 evalStack을 이용하여 postfix 표현식을 평가하는 함수
{
	char token; // 현재 토큰을 저장하는 변수
	int operand1, operand2; // 피연산자를 저장하는 변수

	char* exp = postfixExp; // postfixExp를 가리키는 포인터

	while (*exp != '\0') // exp를 증가시켜가면서, 문자를 읽고 평가 진행
	{
		token = *exp; // 현재 문자를 읽음

		if (getToken(token) == operand) { //피연산자인 경우
			evalPush(token - '0'); // ASCII 코드를 이용하여 문자를 정수로 변환하여 evalStack에 push
		}
		else { // 연산자인 경우
			operand2 = evalPop(); // 먼저 pop된 피연산자
			operand1 = evalPop(); // 그 다음 pop된 피연산자

			switch (token) { // 연산자에 따라 연산을 수행하여 결과를 evalStack에 push
			case '+':
				evalPush(operand1 + operand2);
				break;
			case '-':
				evalPush(operand1 - operand2);
				break;
			case '*':
				evalPush(operand1 * operand2);
				break;
			case '/':
				evalPush(operand1 / operand2);
				break;
			}
		}
		exp++; // 다음 문자로 이동
	}

	evalResult = evalPop(); // 최종 계산 결과를 evalResult에 저장
}
