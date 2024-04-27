#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h> 
#include <string.h>

#define MAX_STACK_SIZE 100
#define MAX_EXPRESSION_LENGTH 100

typedef int element;
typedef struct Stacktype {

	element* data;
	int capacity;
	int top;
} StackType;

// init 
void init(StackType* sptr, int ofs) {
	sptr->data = (element*)malloc(sizeof(StackType) * ofs);
	sptr->top = -1;
	sptr->capacity = ofs;
}

// is_full
int is_full(StackType* sptr) {

	if (sptr->top == sptr->capacity - 1) {
		sptr->capacity = sptr->capacity * 2;
		sptr->data = (element*)realloc(sptr->data, sptr->capacity * sizeof(element));

	}
	return 0;
}

// is_empty
int is_empty(StackType* sptr) {
	return (sptr->top == -1);
}

// push
void push(StackType* sptr, element item) {

	if (is_full(sptr)) {
		fprintf(stderr, "Stack is full\n");
		return;
	}
	else {
		sptr->top = sptr->top + 1;
		sptr->data[sptr->top] = item;
	}
}

//pop
element pop(StackType* sptr) {
	element r;
	if (is_empty(sptr)) {
		fprintf(stderr, "stack is empty\n");
		return -1;
	}
	else {

		return (sptr->data[(sptr->top)--]);
	}
}

//peek
element peek(StackType* sptr) {

	element r;
	if (is_empty(sptr)) {
		fprintf(stderr, "stack is empty\n");
		return -1;
	}
	else {

		return (sptr->data[sptr->top]);
	}
}


//계산
int eval(char* expression) {
	int len;
	StackType s;

	init(&s, 20);
	len = strlen(expression);

	for (int i = 0; expression[i] != '\0'; i++) {

		char ch = expression[i];
		int value;
		int op1, op2;


		if ((ch == '+') || (ch == '-')
			|| (ch == '*') || (ch == '/')) {
			op1 = pop(&s);
			op2 = pop(&s);

			switch (ch) {
			case '+': push(&s, op2 + op1); break;
			case '-':push(&s, op2 - op1); break;
			case '*':push(&s, op2 * op1); break;
			case '/':push(&s, op2 / op1); break;
			default:;
			}
		}
		else if ((ch >= '0') && (ch <= '9')) {
			value = ch - '0';
			push(&s, value);
		}

	}
	return (pop(&s));
}

//연산자 우선순위
int prec(char op) {
	switch (op) {
	case'(': case')': return 0;
	case'+': case'-': return 1;
	case'*': case'/': return 2;
	}
	return -1;
}

//중위 >> 후위 변환
void infix_to_postfix(char infixExpression[], char postfixExpression[]) {

	char top_op;
	int len = strlen(infixExpression);
	StackType s;

	init(&s, MAX_STACK_SIZE);
	int postfix_index = 0;

	for (int i = 0; i < len; i++) {

		switch (infixExpression[i]) {
		case '+':
		case '-':
		case '*':
		case '/':
			while (!is_empty(&s) && prec(infixExpression[i]) <= prec((char)peek(&s))) {
				postfixExpression[postfix_index++] = pop(&s);
			}
			push(&s, (int)infixExpression[i]);
			break;
		case '(':
			push(&s, (int)infixExpression[i]);
			break;
		case')':
			while (!is_empty(&s) && peek(&s) != '(') {
				postfixExpression[postfix_index++] = pop(&s);
			}
			pop(&s); // 여는 괄호는 후위식에 추가x
			break;
		default:
			postfixExpression[postfix_index++] = infixExpression[i];
			break;
		}
	}
	while (!is_empty(&s)) {
		postfixExpression[postfix_index++] = pop(&s);
	}
	postfixExpression[postfix_index] = '\0';
}

int main() {
	char infixExpression[MAX_EXPRESSION_LENGTH];
	char postfixExpression[MAX_EXPRESSION_LENGTH];
	int result;

	while (1) {
		printf("다음과 같은 메뉴로 동작하는 프로그램 입니다.:\n");
		printf("1. 중위식을 입력 받음\n");
		printf("2. 중위식을 후위식으로 변환\n");
		printf("3. 후위식을 계산\n");
		printf("4. 종료\n");
		printf("메뉴를 선택하세요: ");

		int choice;
		scanf("%d", &choice);

		switch (choice) {
		case 1:
			// 중위식 입력 받기
			printf("중위식을 입력하세요: ");
			getchar();
			fgets(infixExpression, sizeof(infixExpression), stdin);
			printf("입력된 중위식 : %s\n", infixExpression);
			break;
		case 2:
			// 중위식 후위식 변환하기
			infix_to_postfix(infixExpression, postfixExpression);
			printf("%s\n", postfixExpression);
			break;
		case 3:
			//후휘식 계산하기
			result = eval(postfixExpression);
			printf("%d\n", result);
			break;
		case 4:
			//종료
			printf("프로그램을 종료합니다.\n");
			exit(0);

		default:
			printf("잘못된 메뉴 선택입니다. 다시 선택하세요.\n");
			break;

		}

	}
	return 0;
}