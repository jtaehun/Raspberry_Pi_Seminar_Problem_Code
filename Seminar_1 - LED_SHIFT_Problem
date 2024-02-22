#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>

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


unsigned int LED[8] = {LED1, LED2, LED3, LED4, LED5, LED6, LED7, LED8}; // 사용할 WiringPi 핀 번호
unsigned int LED_Problem4[8] = {LED8, LED7, LED6, LED5, LED4, LED3, LED2, LED1}; // 4번 문제의 WiringPi 핀 번호 (2진수 표현 ex) 00000101 반대 배열)
unsigned int SW[4] = {SW1, SW2, SW3, SW4}; // 사용할 WiringPi 핀 번호

int SW_1 = HIGH; // 버튼1 초기 상태 설정
int pro_SW_1 = HIGH; // 이전 버튼1 초기 상태 설정

int SW_2 = HIGH; // 버튼2 상태 변수 초기 상태 설정
int pro_SW_2 = HIGH; // 이전 버튼2 초기 상태 설정

int SW_3 = HIGH; // 버튼3 초기 상태 설정
int pro_SW_3 = HIGH; // 이전 버튼3 초기 상태 설정

int SW_4 = HIGH; // 버튼3 초기 상태 설정
int pro_SW_4 = HIGH; // 이전 버튼3 초기 상태 설정

int Toggle1 = 0; // 버튼 토글 변수 (0: LED 이동, 1: LED 정지)
int Toggle3 = 0; // 버튼 토글 변수 (0: LED 이동, 1: LED 정지)
int Toggle4 = 0; // 버튼 토글 변수 (0: LED ON, 1: LED OFF)

unsigned int delayTime = 200; // 초기 딜레이 시간 설정 (0.2초)

void setup(void) { // 초기 상태 setup
    for (int i = 0; i < 8; i++) {
        pinMode(LED[i], OUTPUT); 
    }

    for (int i = 0; i < 4; i++) {
        pinMode(SW[i], INPUT); 
    }
}

void Button_Control1(void) {
    SW_1 = digitalRead(SW1); // 버튼1 상태 읽기

    if (SW_1 == LOW && pro_SW_1 == HIGH) {
         Toggle1 ^= 1; // 버튼을 눌렸을 때마다 토글 변경(0,1) (XOR 연산 : 같으면 0, 다르면 1)
         // 처음상태에서 버튼을 누르면 토글이 1이 되서 XOR 연산을 통해 0으로 변경됨
         // 다음에는 서로 다른 값이 되서 XOR 연산을 통해 1으로 변경됨
    }

    pro_SW_1 = SW_1; // 현재 버튼 상태를 이전 버튼 상태에 저장 (LED가 이동하면서 상태 변화를 파악)
}

void Button_Control3(void) {
    SW_3 = digitalRead(SW3); // 버튼3 상태 읽기

    if (SW_3 == LOW && pro_SW_3 == HIGH) {
        Toggle3 ^= 1; // 버튼을 눌렸을 때마다 토글 변경(0,1) (XOR 연산 : 같으면 0, 다르면 1)
         // 처음상태에서 버튼을 누르면 토글이 1이 되서 XOR 연산을 통해 0으로 변경됨
         // 다음에는 서로 다른 값이 되서 XOR 연산을 통해 1으로 변경됨
    }

    pro_SW_3 = SW_3; // 현재 버튼 상태를 이전 버튼 상태에 저장 (LED가 이동하면서 상태 변화를 파악)
}

void Button_Control4(void) {
    SW_4 = digitalRead(SW4); // 버튼1 상태 읽기

    if (SW_4 == LOW && pro_SW_4 == HIGH) {
        Toggle4 ^= 1; // 버튼을 눌렸을 때마다 토글 변경(0,1) (XOR 연산 : 같으면 0, 다르면 1)
         // 처음상태에서 버튼을 누르면 토글이 1이 되서 XOR 연산을 통해 0으로 변경됨
         // 다음에는 서로 다른 값이 되서 XOR 연산을 통해 1으로 변경됨
    }

    pro_SW_4 = SW_4; // 현재 버튼 상태를 이전 버튼 상태에 저장 (LED가 이동하면서 상태 변화를 파악)
}

int LED_SHIFT = 0; // LED 상태 변수

// LED 오른쪽 이동
void LEDShift(void) {
    digitalWrite(LED[LED_SHIFT], HIGH); // 현재 LED 켜기
    delay(delayTime); // 딜레이 시간만큼 대기
    digitalWrite(LED[LED_SHIFT], LOW); // 현재 LED 끄기
    LED_SHIFT = (LED_SHIFT + 1) % 8; // 다음 LED 상태로 업데이트
}

// LED 왼쪽 이동
void LEDShiftLeft(void) {
    digitalWrite(LED[LED_SHIFT], HIGH); // 현재 LED 켜기
    delay(delayTime); // 딜레이 시간만큼 대기
    digitalWrite(LED[LED_SHIFT], LOW); // 현재 LED 끄기
    LED_SHIFT = (LED_SHIFT + 7) % 8; // 이전 LED 상태로 업데이트 (왼쪽으로 이동)
}

// LED 현재 위치에서 정지
void LEDStop(void) {
    digitalWrite(LED[LED_SHIFT], HIGH); // 현재 LED 켜기
    // 정지 상태에서 대기
}

// LED 끄기
void LED_OFF() {
for (int i = 0; i < 8; i++) {
     digitalWrite(LED_Problem4[i], LOW); // 전체 LED 끄기
    }
}

// 2진수 방식으로 LED ON
void Binary(int value) {
    for (int i = 0; i < 8; ++i) {
        if (value & (1 << i)) { // (1 << i) 가 i번째 비트가 1이고 나머지 비트가 0인 값을 나타냄 (2^i)
        // value 2진수 값의 각각의 비트자릿수가 1인지 0인지 판단

            digitalWrite(LED_Problem4[i], HIGH); // 1이면 HIGH
        } else {
            digitalWrite(LED_Problem4[i], LOW); // 0이면 LOW
        }
    }
}

// 문제 4번 풀이 코드
void problem4() {

    // 0 - 빈 공간 / 1 - 집 / 2 - 치킨집
    int city[5][5] = {
        {0, 0, 1, 0, 0},
        {0, 0, 2, 0, 1},
        {0, 1, 2, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 2}
    };

    int Ones = 0; // 1의 값이 있는 위치의 개수를 0으로 초기화
    int Ones_Place[5][2]; // 1의 값이 있는 위치의 좌표를 저장하는 배열

    int Twos = 0; // 2의 값이 있는 위치의 개수를 0으로 초기화
    int Twos_Place[5][2]; // 2의 값이 있는 위치의 좌표를 저장하는 배열

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (city[i][j] == 1) { // 위치에 값이 1인 경우
                Ones_Place[Ones][0] = i; // 1의 위치의 행 좌표를 Ones_Place에 저장
                Ones_Place[Ones][1] = j; // 1의 위치의 열 좌표를 Ones_Place에 저장
                Ones++; // 1의 값이 있는 위치 개수 증가

            } else if (city[i][j] == 2) { // 위치에 값이 2인 경우
                Twos_Place[Twos][0] = i; // 2의 위치의 행 좌표를 Ones_Place에 저장
                Twos_Place[Twos][1] = j; // 2의 위치의 열 좌표를 Ones_Place에 저장
                Twos++; // 2의 값이 있는 위치 개수 증가
            }
        }
    }

    int sum_Distances = 0; // 거리의 합을 0으로 초기화

    for (int i = 0; i < Ones; ++i) {
        int max_Distance = 8; // 최대 거리 초기화 (최대 거리 : 8 (0,0) , (4,4))
        for (int j = 0; j < Twos; ++j) {
            int distance = abs(Ones_Place[i][0] - Twos_Place[j][0]) + abs(Ones_Place[i][1] - Twos_Place[j][1]);
            // 거리 계산 abs(row_1 - row_2) + abs(col_1 - col_2)
            if (distance < max_Distance) { // 현재 거리가 최대 거리보다 작은 경우
                max_Distance = distance;  
               // 최대 거리 갱신 (각 1의 위치마다 해당하는 2의 위치들과 가장 짦은 거리의 값을 max_Distance에 저장)
            }
        }
        sum_Distances += max_Distance; // 갱신한 거리를 모두 합함
    }

        Binary(sum_Distances); // 2진수로 변환 후 출력
    }



void Button1(void) {
    Button_Control1(); // 버튼 상태 제어

    if (Toggle1 == 0) {
        LEDShift(); // LED 이동
    } else {
        LEDStop(); // LED 정지
    }
}

void Button2(void) {
    SW_2 = digitalRead(SW2); // 버튼1 상태 읽기

    if (SW_2 == LOW && pro_SW_2 == HIGH) {
        delayTime += 200; // 딜레이 시간을 0.2초씩 증가시킴
        if (delayTime > 600) {
            delayTime = 200; // 0.6초 다음으로는 0.2초로 초기화
        }
    }

    pro_SW_2 = SW_2; // 이전 버튼2 상태 업데이트
}

void Button3(void) {
    Button_Control3(); // 버튼 상태 제어

    if (Toggle3 == 0) {
        LEDShiftLeft(); // LED 왼쪽으로 이동
    } else {
        LEDStop(); // LED 정지
    }
}

void Button4(void) {
    Button_Control4(); // 버튼 상태 제어

    if (Toggle4 == 0) {
        problem4(); // 문제4 구현 - 정답 : 00000101 (5)
    } else {
        LED_OFF(); // LED OFF
    }
}

int main(void) {
    if (wiringPiSetup() < 0) { // wiringPi 라이브러리 초기화
        return -1; // 초기화 과정에서 오류가 나오면 -1로 반환
    }

    setup();

    while (1) {

        Button1(); // 버튼1 상태 제어
        Button2(); // 버튼2 상태 제어
        Button3(); // 버튼3 상태 제어
        Button4(); // 버튼4 상태 제어
    }

    return 0;
}
