#include <stdio.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <string.h>  


unsigned int fd;


void I2C_Seting(unsigned char Channel)
{
	//wiringPiI2CWriteReg8(fd, device 주소, bit data)

	wiringPiI2CWriteReg8 (fd, 0x00, 0x01); // 0x00 주소
	// RR(1) : Register 초기화

	wiringPiI2CWriteReg8 (fd, 0x00, 0x96); // 0x00 주소
	// AVDDS(1) : 내부 전압 ON / CS(1) : 클락 동기화 / PUA(1) - 아날로그 회로 전원 ON / PUD(1) - 디지털 회로 전원 ON

	wiringPiI2CWriteReg8 (fd, 0x01, 0x18);	// 0x01 주소
	// VLDO[5:3](011) : 3.6V

	wiringPiI2CWriteReg8(fd, 0x02, (Channel << 7)); // 0x02 주소
	// Channel 0 or 1
	
}

int ADC()
{
	int adc_data=0;
	unsigned int data_H=0, data_M=0, data_L=0; // 각각 8bit씩 총 24bit
	int start = 0;
	
	do
	{
		start = wiringPiI2CReadReg8(fd,0x00); // 0x00의 레지스터 Read
	}
	while( start & 0x20 == 0x00 );  // CR(1) (ADC DATA is ready / 0010 0000) 조건이 아닐시 do-while문 반복

	data_H = wiringPiI2CReadReg8(fd, 0x12);     // 23:16 상위 비트 read
	data_M = wiringPiI2CReadReg8(fd, 0x13);     // 15:8 중간 비트 read
	data_L = wiringPiI2CReadReg8(fd, 0x14);     // 7:0 하위 비트 read

	adc_data = (data_H << 16)|(data_M << 8)|(data_L);  // data_H(8bit) + data_M(8bit) + data_L(8bit) = 24bit
	return adc_data;
}
int main()
{
	unsigned int adcvalue=0;
	
	if(wiringPiSetup() < 0) // WiringPi 라이브러리 초기화
	{
		fprintf (stdout, "Unable to start wiringPi : %s\n",strerror(errno)); // 오류 검사
		return 1;
	}
	
	if ((fd = wiringPiI2CSetup(0x2a))<0) //I2C 확인 0x2a = 슬레이브 주소
	{
		fprintf (stdout, "Unable to initialize I2C : %s\n",strerror(errno)); // 오류 검사
		return 1; 
	}
	
	I2C_Seting(0); // Channal : 0
	
	
	while(1)
	{
		adcvalue = ADC();
		printf("sensor = %d \n" ,adcvalue);
		delay(200);
	}
}
