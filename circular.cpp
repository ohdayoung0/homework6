/**
 * circularQ.c
 *
 * School of Computer Science,
 * Chungbuk National University
 */
#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h> // <stdio.h> 라이브러리를 포함
#include <stdlib.h> // <stdlib.h> 라이브러리를 포함

#define MAX_QUEUE_SIZE 4 // 큐의 최대 크기를 4로 정의

typedef char element; // 큐 요소의 자료형을 char로 정의
typedef struct {
    element queue[MAX_QUEUE_SIZE]; // 요소를 저장하는 쿠 배열 선언
    int front, rear; // 큐의  front와 rear을 나타내는 변수 선언
} QueueType; // QueueType 구조체 정의


QueueType* createQueue(); // 큐를 생성하는 함수 선언
int freeQueue(QueueType* cQ); // 큐를 해제하는 함수 선언
int isEmpty(QueueType* cQ); // 큐가 비어있는지 확인하는 함수 선언
int isFull(QueueType* cQ); // 큐가 가득 찼는지 확인하는 함수 선언
void enQueue(QueueType* cQ, element item); // 큐에 요소를 추가하는 함수 선언
void deQueue(QueueType* cQ, element* item); // 큐에서 요소를 삭제하고 삭제한 요소를 반환하는 함수 선언
void printQ(QueueType* cQ); // 큐의 상태를 출력하는 함수 선언
void debugQ(QueueType* cQ); // 큐의 디버그 정보를 출력하는 함수 선언
element getElement(); // 요소를 입력받는 함수 선언

int main(void) // main 함수 시작
{
    printf("-----[오다영] [2023041090]-----\n"); // 이름 학번 출력
    QueueType* cQ = createQueue(); // 큐 생성
    element data; // 큐에 삽입할 요소를 저장하는 변수 data 선언
    char command; // 명령어 저장하는 변수 command 선언
     
    do {
        printf("\n-----------------------------------------------------\n");
        printf("                     Circular Q                   \n");
        printf("------------------------------------------------------\n");
        printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
        printf("------------------------------------------------------\n");

        printf("Command = "); // Command = 출력
        scanf(" %c", &command); // Command 입력받음

        switch (command) {
        case 'i': case 'I':
            data = getElement(); // 요소 입력 받음
            enQueue(cQ, data); // 큐에 요소 추가
            break;
        case 'd': case 'D':
            deQueue(cQ, &data); // 큐에서 요소 삭제, 삭제한 요소를 data에 저장
            break;
        case 'p': case 'P':
            printQ(cQ); // 큐의 상태 출력
            break;
        case 'b': case 'B':
            debugQ(cQ); // 큐 디버그 정보 출력
            break;
        case 'q': case 'Q':
            freeQueue(cQ); // 큐 해제
            break;
        default:
            printf("\n       >>>>>   Concentration!!   <<<<<     \n");
            break;
        }

    } while (command != 'q' && command != 'Q'); // q나 p입력할 때까지 반복


    return 1; // 종료되면 1을 반환함
}

QueueType* createQueue() // 큐 생성 함수 선언
{
    QueueType* cQ; // QueueType 구조체 포인터 cQ를 선언
    cQ = (QueueType*)malloc(sizeof(QueueType)); // 큐를 동적 할당하여 메모리에 생성
    cQ->front = 0; // front 초기화
    cQ->rear = 0; // rear 초기화
    return cQ; // 생성한 큐 반환
}

int freeQueue(QueueType* cQ) // 큐 해제 함수 선언
{
    if (cQ == NULL) return 0; // cQ가 NULL값이면 0을 반환
    free(cQ); // 동적 할당된 큐 메모리 해제
    return 1; // 1반환
} 

element getElement() // 요소 입력받는 함수 선언
{
    element item; // 입력받을 요소 저장할 변수 item 선언
    printf("Input element = ");
    if (scanf(" %c", &item) != 1) { // 입력이 잘못된 경우
        printf("Invalid input\n"); // 유효하지 않은 입력 메시지 출력
        item = getElement(); // 다시 입력받음
    }
    return item; // 입력 받은 요소 반환
}


/* complete the function */
int isEmpty(QueueType* cQ) // 큐가 비어있는지 여부를 확인하는 함수
{
    return (cQ->front == cQ->rear); // front와 rear가 같으면 큐가 비어있음
}

/* complete the function */
int isFull(QueueType* cQ) // 큐가 가득 찼는지 여부를 확인하는 함수
{
    return ((cQ->rear + 1) % MAX_QUEUE_SIZE == cQ->front); // rear 다음이 front이면 큐가 가득 찬 상태
}


/* complete the function */
void enQueue(QueueType* cQ, element item) // 큐에 요소를 추가하는 함수
{
    if (isFull(cQ)) { // 큐가 가득 찼는지 확인
        printf("Queue is full\n"); // 큐가 가득 차면 오버플로우 메시지 출력
        return; // 함수 종료
    }
    cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; // rear를 한 칸 이동
    cQ->queue[cQ->rear] = item; // rear가 위치한 곳에 요소 추가
}

/* complete the function */
void deQueue(QueueType* cQ, element* item) // 큐에서 요소를 삭제하고 삭제한 요소를 반환하는 함수
{
    if (isEmpty(cQ)) { // 큐가 비어있는지 확인
        printf("Queue is empty\n"); // 큐가 비어있으면 언더플로우 메시지 출력
        return; // 함수 종료
    }
    cQ->front = (cQ->front + 1) % MAX_QUEUE_SIZE; // front를 한 칸 이동
    *item = cQ->queue[cQ->front]; // front가 위치한 곳의 요소를 삭제하고 반환
}


void printQ(QueueType* cQ) // 큐 상태 출력 함수
{
    int i, first, last; // 정수형 변수 선언

    first = (cQ->front + 1) % MAX_QUEUE_SIZE; // 큐의 첫 번째 요소 위치 계산
    last = (cQ->rear + 1) % MAX_QUEUE_SIZE; // 큐의 마지막 요소 위치 계산

    printf("Circular Queue : [");

    i = first;
    while (i != last) {
        printf("%3c", cQ->queue[i]); // 요소 출력
        i = (i + 1) % MAX_QUEUE_SIZE; // 다음 요소 위치로 이동

    }
    printf(" ]\n");
}


void debugQ(QueueType* cQ) // 큐의 디버그 정보 출력하는 함수 선언
{

    printf("\n---DEBUG\n");
    for (int i = 0; i < MAX_QUEUE_SIZE; i++)
    {
        if (i == cQ->front) {
            printf("  [%d] = front\n", i); // front 위치 출력
            continue;
        }
        printf("  [%d] = %c\n", i, cQ->queue[i]); // 요소와 해당 위치 출력

    }
    printf("front = %d, rear = %d\n", cQ->front, cQ->rear); // front와 rear 출력
}