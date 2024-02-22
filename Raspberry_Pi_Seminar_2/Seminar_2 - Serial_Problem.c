#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <errno.h>

#define LED1 29
#define LED2 28
#define LED3 27
#define LED4 26
#define LED5 25
#define LED6 24
#define LED7 23
#define LED8 22

#define SW1 6
#define SW2 5
#define SW3 4
#define SW4 1

unsigned int LED[8] = {LED8, LED7, LED6, LED5, LED4, LED3, LED2, LED1}; // WiringPi 핀 번호 (반대 배열)
unsigned int SW[4] = {SW1, SW2, SW3, SW4}; // 사용할 WiringPi 핀 번호

// 시리얼 통신
int fd = 0; // 파일 디스크립터
char data = 0; // 데이터 버퍼

int inputMode2 = 0; // Button2의 입력 모드 변수
int inputMode3 = 0; // Button3의 입력 모드 변수
int inputMode4 = 0; // Button4의 입력 모드 변수


int buttonPressCount = 0; // 버튼을 누르는 횟수 변수

// Problem 1
void Button1(int SW_1, int serialFd) {

    if (digitalRead(SW_1) == LOW) { 
        delay(200); // 디바운싱 : 20ms

        buttonPressCount++; // 버튼 누름 횟수 증가

        if (buttonPressCount == 1) {
            serialPuts(serialFd, "Hello!\n"); // 시리얼 통신을 통해 버튼 횟수가 1일 때 "Hello" 문자열 전송
        } else if (buttonPressCount == 2) {
            serialPuts(serialFd, "My name is\n"); // 시리얼 통신을 통해 버튼 횟수가 2일 때 "My name is" 문자열 전송
        } else if (buttonPressCount == 3) {
            serialPuts(serialFd, "Ronald Coleman.\n"); // 시리얼 통신을 통해 버튼 횟수가 1일 때 "Ronald Coleman" 문자열 전송
            
            buttonPressCount = 0; // 버튼 누름 횟수 초기화
        }
        
    }
}

// 2진수 방식으로 LED ON
void Binary(int value) {
    for (int i = 0; i < 8; ++i) {
        int LED_ON = (value & (1 << i));
        digitalWrite(LED[i], LED_ON);
         // (1 << i) 가 i번째 비트가 1이고 나머지 비트가 0인 값을 나타냄 (2^i)
         // value 값의 각각의 비트자릿수가 1인지 0인지 판단
        }
    }


int inputValue = 0; // 시리얼 통신으로 입력 받은 값

// Problem 2
void Button2(int SW_2, int serialFd) {

    if (digitalRead(SW_2) == LOW) { // 버튼 제어
        delay(200); // 디바운싱 : 20ms

        inputMode2 = !inputMode2; // 버튼이 눌리면 입력 대기 모드(inputMode2 == 1) 와 데이터 출력 모드(inputMode2 == 0) 를 번갈아가며 전환

        if (inputMode2 == 1) {
            printf("\n문제_2 입력 대기 모드\n"); // 입력 대기 모드
            } 
        
        else {
            printf("\n문제_2 데이터 출력 모드\n"); // 데이터 출력 모드

            while (serialDataAvail(serialFd) > 0) { // 시리얼 포트로부터 데이터가 들어올 때까지 반복
                data = serialGetchar(serialFd); // 데이터를 한 바이트씩 읽음
                printf("%c", data); // 읽은 데이터를 화면에 출력

                if (data >= '0' && data <= '9') { // 읽은 데이터가 숫자인 경우
                    int num = data - '0'; // 문자를 숫자로 변환
                    inputValue = inputValue * 10 + num; // 한자릿수 이상의 정수를 입력받을 수 있게 입력받음
                }
            }
        }
       
        if (inputValue >= 0 && inputValue <= 255) { // 입력 받은 숫자의 범위가 0 ~ 255 (LED 8개 제어) 인 경우
            Binary(inputValue); // 2진수 변환 후 LED 점등
            inputValue = 0; // 입력값 초기화
        }
    }
}

char inputString[100] = {0}; // 입력 받은 문자열을 저장할 배열
int inputIndex = 0; // 입력된 문자열의 인덱스를 나타내는 변수

// Problem 3
void Button3(int SW_3, int serialFd) {

    if (digitalRead(SW_3) == LOW) { // 버튼 제어
        delay(200); // 디바운싱 : 20ms
            
        inputMode3 = !inputMode3; // 버튼이 눌리면 입력 대기 모드(inputMode2 == 1) 와 데이터 출력 모드(inputMode2 == 0) 를 번갈아가며 전환

        if (inputMode3 == 1) {
            printf("\n문제_3 입력 대기 모드\n"); // 입력 대기 모드
            }
            
        else {
            printf("\n문제_3 데이터 출력 모드\n"); // 데이터 출력 모드

            while (serialDataAvail(serialFd) > 0) { // 시리얼 통신으로부터 데이터가 들어올 때까지 반복
                data = serialGetchar(serialFd); // 데이터를 한 바이트씩 읽음
                printf("%c", data); // 읽은 데이터를 화면에 출력

                if (inputIndex < sizeof(inputString)) { // 입력된 문자열의 인덱스가 inputString 배열의 크기 미만일 때,
                    inputString[inputIndex] = data; // 시리얼 통신으로 입력된 데이터를 inputString 배열에 저장
                    inputIndex++; // 데이터의 저장 위치를 증가
                }
            }
                if (strstr(inputString, "weight")) {  // strstr : inputString 문자열에서 "weight" 문자열을 검색
                    
                    printf("weight\n"); // 입력된 문자열에 "weight"가 포함되어 있으면 출력
                }
            }
        }
}


// 문제 4 풀이법
int problem_4_solving(int N) {
    int cards[10]; // 최대 카드 개수 10

    int front = 0; // 제일 위에 있는 카드
    int rear = N; // 제일 아래에 있는 카드

    // 카드 배열 초기화
    for (int i = 0; i < N; i++) {
        cards[i] = i + 1;
    }

    while (rear - front > 1) { // front 와 rear 사이가 1 즉, 카드가 1장 남을때까지 반복

        front++; // 제일 위에 있는 카드를 버림

        cards[rear] = cards[front]; // 버린 카드 다음에 있는 카드를 제일 아래로 옮김

        front++; // 그 다음 제일 위에 있는 카드를 버림

        rear++; // 제일 아래로 옮길 카드의 자리 생성
    }

    return cards[front]; // while 루프 종료 시 남아있는 카드를 return
}

// Problem 4
void Button4(int SW_4, int serialFd) {
if (digitalRead(SW_4) == LOW) { // 버튼 제어
        delay(200); // 디바운싱 : 20ms
            
        inputMode4 = !inputMode4; // 버튼이 눌리면 입력 대기 모드(inputMode2 == 1) 와 데이터 출력 모드(inputMode2 == 0) 를 번갈아가며 전환

        if (inputMode4 == 1) {
            printf("\n문제_4 입력 대기 모드\n"); // 입력 대기 모드
            }
            
        else {
            printf("\n문제_4 데이터 출력 모드\n"); // 데이터 출력 모드

            while (serialDataAvail(serialFd) > 0) { // 시리얼 통신으로부터 데이터가 들어올 때까지 반복
                data = serialGetchar(serialFd); // 데이터를 한 바이트씩 읽음
                printf("%c", data); // 읽은 데이터를 화면에 출력

                int N = data - '0'; // 입력 받은 데이터를 정수로 변환

                int result = problem_4_solving(N); // 카드의 갯수를 입력받아 정답을 problem_4_solving 함수를 통해 구함
                printf("\n마지막에 남는 카드의 번호: %d\n", result);
            }
        }

    }
}

int main(void) {
    if (wiringPiSetup() < 0) {
        fprintf(stderr, "wiringPi 오류 %s\n", strerror(errno)); // wiringPi 라이브러리 초기화
        return -1;
    }

    if ((fd = serialOpen("/dev/ttyAMA1", 9600)) < 0) {
        fprintf(stderr, "serial 오류 %s\n", strerror(errno));  // 시리얼 통신
        return -1;
    }

    serialFlush(fd); // 시리얼 통신의 입력 버퍼 비우기

    while (1) {
        Button1(SW1, fd); // Problem1
        Button2(SW2, fd); // Problem2
        Button3(SW3, fd); // Problem3
        Button4(SW4, fd); // Problem4
    }
    return 0;
}
