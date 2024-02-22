#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <wiringPi.h>
#include <softPwm.h>
#include <wiringPiSPI.h>
#include <wiringSerial.h>
#include <pthread.h> // POSIX 스레드 라이브러리

#define SPI_CHANNEL 0 // SPI 채널
#define SPI_SPEED 1000000 // SPI 속도 (1MHz)
#define CS_MCP3208 8 // CS, SS

#define DC_A 3 // DC_Moter, GPIO 22
#define DC_B 2 // DC_Moter, GPIO 27 


int Adc_Value = 0; // ADC
int Sensor_Value = 0; // 온도 센서

int fd = 0;
char data = 0;


// SPI A/D
int MCP3208_ADC(unsigned char Ch)
{
    // buff 구성 0000 011(D2) + (D1)(D0)00 0000 + xxxx xxxx
  unsigned char buff[3];

  buff[0] = 0x06 | ((Ch & 0x07) >> 2); // 0000 011(D2) : 시작 비트, SGL / DIFF, D2 (채널 설정)
  buff[1] = ((Ch & 0x07) << 6); // (D1)(D0)00 0000 : D1, D0 (채널 설정)
  buff[2] = 0x00; // xxxx xxxx : Don't Care

  digitalWrite(CS_MCP3208, LOW);  // CS 핀 활성화, 통신 시작 : LOW

  wiringPiSPIDataRW(SPI_CHANNEL, buff, 3); // SPI 통신 수행 buff의 3byte 읽고 쓰기

  buff[1] = 0x0F & buff[1]; // 0x0F와 AND연산 - 하위 4비트

  Adc_Value = ( buff[1] << 8) | buff[2]; // 하위 4비트만 남은 buff[1]을 8비트 왼쪽 시프트하여 상위 8비트로 이동
  // 하위 8비트의 데이터 buff[2]와 buff[1] 를 OR 연산하여 12비트의 아날로그 값을 나타냄

  digitalWrite(CS_MCP3208, HIGH);  // CS 핀 비활성화, 통신 종료 : HIGH

  return Adc_Value;
}

// DC_MOTOR 제어
void dc_motor(int pwm_value)
{
    if (pwm_value < 0)
        pwm_value = 0; // PWM 최솟값
    if (pwm_value > 1023)
        pwm_value = 1023; // PWM 최댓값

    softPwmWrite(DC_A, pwm_value); // 정방향 회전
    softPwmWrite(DC_B, 0);
}

// ADC (온도 센서)
double Temp_Value(int adc_value)
{
    double temperature = ((double)adc_value / 1023.0) * 100.0; // 온도의 ADC 값을 0에서 100 사이의 온도로 변환 (아날로그 신호 : 0 ~ 1023)
    return temperature;
}

// ADC 스레드 함수
void *ADC()
{
    while (1)
    {
        int AdcValue = MCP3208_ADC(0); // Ch 0의 아날로그 값 읽음
        Sensor_Value = AdcValue; // 읽은 값을 Sensor_Value에 저장

        double temperature = Temp_Value(Sensor_Value); // 아날로그 값을 온도로 변환
        int motorSpeed = (int)(temperature * 10); // 온도에 따라 모터 속도 계산
        dc_motor(motorSpeed); // 모터 제어

        delay(1000);
    }
    return NULL;
}

// Serial 스레드 함수
void *Serial()
{
    while (1)
    {
        if (serialDataAvail(fd) > 0)
        {
            data = serialGetchar(fd); // 시리얼 포트로부터 문자를 읽어와 'data' 변수에 저장
            printf("%c", data); // 화면 출력
        }
        else
            NULL;

        if (data == '1') // 'data' 변수가 '1'
        {
            printf("Temp_sensor = %d\n", Sensor_Value); // 온도 센서값 출력
            fflush(stdout); // 버퍼 비움
            delay(300);
        }

        else
        {
            dc_motor(0); // 'data' 변수가 '1'이 아닌 경우 모터 정지
            printf("1을 눌러 실행시키세요\n"); // 다음 안내 메세지 출력
            fflush(stdout); // 버퍼 비움
            delay(300);
        }
    }
}


int main(void)
{
    if (wiringPiSetup() == -1) // WiringPi 라이브러리 초기화
    {
        fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
        return 1;
    }

    if (wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1) // SPI 인터페이스 설정
    {
        fprintf(stdout, "wiringPiSPISetup Failed: %s\n", strerror(errno));
        return 1;
    }

    if ((fd = serialOpen("/dev/ttyAMA1", 9600)) < 0) // 시리얼 포트 열기
    {
        fprintf(stderr, "unable to open serial device: %s\n", strerror(errno));
        return -1;
    }

    pthread_t p_thread[2]; // 스레드 식별자 저장 배열 (2개)
    int thread_id = 0; // 스레드 생성 결과를 저장할 변수
    int status;

    pinMode(CS_MCP3208, OUTPUT); // CS,SS 출력

    pinMode(DC_A, OUTPUT); // DC_A 모터 출력 
    pinMode(DC_B, OUTPUT); // DC_B 모터 출력
    softPwmCreate(DC_A, 0, 1023); // DC_A 모터 PWM 범위 0 ~ 1023 설정 
    softPwmCreate(DC_B, 0, 1023); // DC_B 모터 PWM 범위 0 ~ 1023 설정

    thread_id = pthread_create(&p_thread[0], NULL, ADC, NULL); // 첫 번째 스레드 생성 (ADC)
    if (thread_id < 0)
    {
        perror("thread create error");
        exit(1);
    }

    thread_id = pthread_create(&p_thread[1], NULL, Serial, NULL); // 두 번째 스레드 생성 (Serial)
    if (thread_id < 0)
    {
        perror("thread create error");
        exit(1);
    }

    // 스레드 종료 대기
    pthread_join(p_thread[0], (void **)&status);
    pthread_join(p_thread[1], (void **)&status);

    return 0;
}
