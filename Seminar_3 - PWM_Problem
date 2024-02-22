#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <errno.h>
#include <string.h>
#include <softPwm.h>

 // LED 제어 핀 (Hardware PWM0 핀 : GPIO 18)
#define LED 1

// BUTTON 제어 핀
#define SW1 6
#define SW2 5
#define SW3 4
#define SW4 0

// DC 모터 제어 핀
#define DC_A 2				
#define DC_B 3

// DC 모터 최대 RPM 값
#define MAX_RPM 350 

// 문제 4번
#define MAX_books 1000 // 책의 최대 개수
#define Books_len 50 // 책의 최대 제목 길이


int fd = 0; // 파일 디스크립터
char data = 0; // 데이터 버퍼

int inputMode1 = 0; // Button1의 입출력 모드 변수
int inputMode2 = 0; // Button2의 입출력 모드 변수
int inputMode4 = 0; // Button4의 입출력 모드 변수

int inputValue = 0; // Button2의 시리얼 통신으로 입력 받은 값

int SW_3 = HIGH; // Button3 초기 상태 설정
int pro_SW_3 = HIGH; // 이전 Button3 초기 상태 설정
int Toggle3 = 0; // Button3 토글

// 문제 4번
char inputString[100] = {0}; // 입력 받은 문자열을 저장할 배열
int inputIndex = 0; // 입력된 문자열의 인덱스


// LED 밝기 제어 함수
void fadeInOut() {

        for (int brightness = 0; brightness <= 1023; brightness++) {
            pwmWrite(LED, brightness); // LED의 밝기를 0부터 1023까지 점점 증가
            delay(2000); // 2초 동안 점점 밝기가 밝아짐
        }

        for (int brightness = 1023; brightness >= 0; brightness--) {
            pwmWrite(LED, brightness); // LED의 밝기를 1023부터 0까지 점점 감소
            delay(2000); // 2초 동안 점점 밝기가 낮아짐
        }
}

// Problem1
void Button1(int buttonpin) {

    pinMode(LED, PWM_OUTPUT); // LED_PIN을 PWM 출력으로 설정
    pinMode(SW1, INPUT);

    pwmSetMode(PWM_MODE_MS); // PWM 모드를 mark-space 모드로 설정(LED 제어에 적합)
    pwmSetClock(192);        // PWM 클럭을 192로 설정 - 19.2MHz / 192 = 100kHz(일반적인 모터에 대해 효율적인 범위)
    pwmSetRange(1024);       // PWM 범위를 1024로 설정 (0 ~ 1023)

    if (digitalRead(buttonpin) == LOW) { // 버튼 제어
        delay(200); // 디바운싱

        inputMode1 = !inputMode1;
        // 버튼이 눌리면 inputMode1 == 1 다시 누르면 inputMode1 == 0 번갈아가며 전환 

        if (inputMode1 == 1) {
            fadeInOut(); // inputMode1 == 1 일때, LED 밝기 제어 함수 호출
            } 
        
        else {
            pwmWrite(LED, 0); // inputMode1 == 0 일때, LED 밝기를 0으로 제어
            }
    }
}

// DC 모터 제어 함수
// 정방향 회전
void dc_motor(int rpm) {

    softPwmCreate(DC_A, 0, 1023);
    softPwmCreate(DC_B, 0, 1023);

    int pwm_value = (int)(((float)rpm / MAX_RPM) * 1023); // 입력한 rpm 값을 0부터 1023 사이의 PWM 값으로 반환
    if (pwm_value < 0) pwm_value = 0; // PWM 값이 0 미만인 경우, PWM 값을 0으로 처리
    if (pwm_value > 1023) pwm_value = 1023; // PWM 값이 최대값을 넘는 경우, PWM 값을 최대로 처리

    // DC_A 핀을 ON / DC_B 핀을 OFF로 설정 : 모터를 정방향으로 회전
    softPwmWrite(DC_A, pwm_value);
    softPwmWrite(DC_B, 0);
}

// 역방향 회전
void dc_motor_r(int rpm) {

    softPwmCreate(DC_A, 0, 1023);
    softPwmCreate(DC_B, 0, 1023);

    int pwm_value = (int)(((float)rpm / MAX_RPM) * 1023); // 입력한 rpm 값을 0부터 1023 사이의 PWM 값으로 반환
    if (pwm_value < 0) pwm_value = 0; // PWM 값이 0 미만인 경우, PWM 값을 0으로 처리
    if (pwm_value > 1023) pwm_value = 1023; // PWM 값이 최대값을 넘는 경우, PWM 값을 최대로 처리

    // DC_A 핀을 OFF / DC_B 핀을 ON로 설정 : 모터를 역방향으로 회전
    softPwmWrite(DC_A, 0);
    softPwmWrite(DC_B, pwm_value);
}

// 현재 rpm 값을 화면에 시리얼 통신
void Display_rpm(int rpm) {
    char buffer[50];
    sprintf(buffer, "\nRPM: %d\n", rpm); // rpm값을 버퍼에 저장
    serialPuts(fd, buffer); // 시리얼 포트를 통해 출력
    delay(1500); // 1.5초 간격으로 시리얼 출력
}

// Problem2
void Button2(int buttonPin, int serialFd) {
    
    if (digitalRead(buttonPin) == LOW) { // 버튼 제어
        delay(200); // 디바운싱

        inputMode2 = !inputMode2;
        // 버튼이 눌리면 inputMode2 == 1 다시 누르면 inputMode2 == 0 번갈아가며 전환 

        if (inputMode2 == 1) {
            printf("\n문제_2 RPM 값을 입력하세요 : \n"); // 입력 대기 모드
            } 
        
        else {
            while (serialDataAvail(serialFd) > 0) { // 시리얼 포트로부터 데이터가 들어올 때까지 반복
                data = serialGetchar(serialFd); // 데이터를 한 바이트씩 읽음
                printf("%c", data); // 읽은 데이터를 화면에 출력

                if (data >= '0' && data <= '9') { // 읽은 데이터가 숫자인 경우
                    int num = data - '0'; // 문자를 숫자로 변환
                    inputValue = inputValue * 10 + num;
                }
            }
        }
       
        if (inputValue >= 0 && inputValue <= 350) { // 입력 받은 rpm의 범위가 0 ~ 350 인 경우
        
           dc_motor(inputValue); // dc_moter 함수를 호출해서 모터 회전 제어
           Display_rpm(inputValue); // Display_rpm 함수를 호출해서 현재 rpm 값을 시리얼 통신으로 출력

           inputValue = 0; // 모터 제어 후 다시 inputValue = 0으로 초기화
        }
    }
}

// Button3 제어
void Button_Control3(void) {
    SW_3 = digitalRead(SW3); // 버튼3 상태 읽기

     if (digitalRead(SW3) == LOW) {
        delay(200); // 디바운싱
        Toggle3 ^= 1; // 버튼을 눌렸을 때마다 토글 변경(0,1) (XOR 연산 : 같으면 0, 다르면 1)
    }

    pro_SW_3 = SW_3; // 현재 버튼 상태를 이전 버튼 상태에 저장 (LED가 이동하면서 상태 변화를 파악)
}

// Problem3
void Button3(int rqm) {
    Button_Control3(); // Button3 제어 함수

    if (Toggle3 == 1) {
        dc_motor_r(rqm); // 모터 역방향 회전
    } else {
        dc_motor(rqm); // 모터 정방향 회전
    }
}

// Problem4
void Button4(int buttonPin, int serialFd) {
    
    if (digitalRead(buttonPin) == LOW) { // 버튼 제어
        delay(200); // 디바운싱

        inputMode4 = !inputMode4;
        // 버튼이 눌리면 inputMode4 == 1 다시 누르면 inputMode4 == 0 번갈아가며 전환

    if (inputMode4 == 1) { // 입력 대기 모드
        
        printf("\n문제_4 오늘 하루 팔린 책을 입력하세요(공백 문자로 나눠서 책을 입력) : \n");
        inputIndex = 0; // 입력된 문자열의 인덱스 초기화

    } else {

        while (serialDataAvail(fd) > 0) { // 시리얼 포트로부터 데이터가 들어올 때까지 반복
        data = serialGetchar(fd); // 데이터를 한 바이트씩 읽음
        printf("%c", data); // 읽은 데이터를 화면에 출력 
        fflush(stdout); // 출력 버퍼 비우기
        
        
        if (inputIndex < sizeof(inputString) ) { // 입력된 문자열의 인덱스가 inputString 배열의 크기 미만일 떄,
            inputString[inputIndex] = data;  // 시리얼 통신으로 입력된 데이터를 inputString 배열에 저장
            inputIndex++; // 데이터의 저장 인덱스를 증가
        }
    }

    // Problem4_Soving
    char books[MAX_books][Books_len]; // 책들을 저장할 배열 : 최대 1000개의 책, 책 제목의 길이는 최대 50
    int book_count[MAX_books] = {0}; // 팔린 책의 개수를 저장할 배열
    int all_book = 0; // 책의 개수

    char* token = strtok((char*)inputString, " "); // strtok 함수 : 입력받은 책들을 공백 문자를 기준으로 분리
    while (token != NULL) { // 더 이상 분리할 책들이 없으면 NULL 반환

        int found = 0; // found를 0으로 초기화, 현재 책이 books 배열에 존재하는지 여부

        for (int i = 0; i < all_book; i++) {  // 모든 책들을 루프
            if (strcmp(token, books[i]) == 0) { // 입력한 책과 books 배열 안에 있는 책을 비교해서 같은 책이면
                book_count[i]++; // 책의 판매수를 증가
                found = 1; // 현재 책이 books 배열에 존재
                break; // 루프 종료
            }
        }

        if (!found) { // found가 0인 경우, 현재 책이 books 배열에 존재하지 않고 중복되는 책이 없는 경우
            strcpy(books[all_book], token); // 현재 책을 books 배열에 복사
            book_count[all_book] = 1; // 해당 책의 판매수를 1로 초기화
            all_book++; // 전체 책의 개수를 증가
        }

        token = strtok(NULL, " "); // 다음 입력한 책을 찾기 위해 strtok 함수를 다시 호출
    }

    // 가장 많이 팔린 책 구하기
    int maxIndex = 0; // 가장 많이 팔린 책의 인덱스

    for (int i = 1; i < all_book; i++) { // 모든 책들을 루프
        if (book_count[i] > book_count[maxIndex]) { // 가장 많이 팔린 책의 인덱스가 갱신되면
            maxIndex = i; // i를 maxIndex로 갱신
        }
    }
    // 입력된 문자열이 존재하면 출력
    printf("\n가장 많이 팔린 책의 제목 : %s\n", books[maxIndex]);
    }
    }
}

int main(void) {
    if (wiringPiSetup() < 0) {
        fprintf(stderr, "wiringPi 오류 %s\n", strerror(errno));
        return 1;
    }

    if ((fd = serialOpen("/dev/ttyAMA1", 9600)) < 0) {
        fprintf(stderr, "serial 오류 %s\n", strerror(errno));
        return 1;
    }

    // 입력 버퍼를 비웁니다.
    serialFlush(fd);

    while (1) {
        Button1(SW1);
        Button2(SW2, fd);
        Button3(inputValue);
        Button4(SW4,fd);
    }
    
 return 0;
}
