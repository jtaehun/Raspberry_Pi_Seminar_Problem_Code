#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>

#define CS_MCP3208 10        // SS/CS
#define SPI_CHANNEL 0 
#define SPI_SPEED 1000000  // 1MHz


int MCP3208_ADC(unsigned char Ch)
{
  // buff 구성 0000 011(D2) + (D1)(D0)00 0000 + xxxx xxxx
  unsigned char buff[3];
  
  int adcValue = 0; 

  buff[0] = 0x06 | ((Ch & 0x07) >> 2); // 0000 011(D2) : 시작 비트, SGL / DIFF, D2 (채널 설정)
  buff[1] = ((Ch & 0x07) << 6); // (D1)(D0)00 0000 : D1, D0 (채널 설정)
  buff[2] = 0x00; // xxxx xxxx : Don't Care

  digitalWrite(CS_MCP3208, LOW);  // CS 핀 활성화, 통신 시작 : LOW

  wiringPiSPIDataRW(SPI_CHANNEL, buff, 3); // SPI 통신 수행 buff의 3byte 읽고 쓰기

  buff[1] = 0x0F & buff[1]; // 0x0F와 AND연산 - 하위 4비트

  adcValue = ( buff[1] << 8) | buff[2]; // 하위 4비트만 남은 buff[1]을 8비트 왼쪽 시프트하여 상위 8비트로 이동
  // 하위 8비트의 데이터 buff[2]와 buff[1] 를 OR 연산하여 12비트의 아날로그 값을 나타냄

  digitalWrite(CS_MCP3208, HIGH);  // CS 핀 비활성화, 통신 종료 : HIGH

  return adcValue;
}


int main (void)
{
  int Ch = 0;
  int adcValue = 0;

  if(wiringPiSetup() == -1) // WiringPi 라이브러리 초기화
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror(errno)); // 오류 검사
    return 1 ;
  }

  if(wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1) // SPI 인터페이스 설정
  {
    fprintf (stdout, "wiringPiSPISetup Failed: %s\n", strerror(errno)); // 오류 검사
    return 1 ;
  }

  pinMode(CS_MCP3208, OUTPUT);

  while(1)
  {
    adcValue = MCP3208_ADC(Ch);
    printf("adcValue = %u\n", adcValue);

    delay(100);
  }

  return 0;
}
