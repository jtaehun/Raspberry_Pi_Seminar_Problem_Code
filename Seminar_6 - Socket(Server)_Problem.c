#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUFF_SIZE 1000

int server_socket;  // 서버 디스크립터
int client_socket;  // 클라이언트 디스크립터

int client_add_size; // 클라이언트 주소 크기

// 주소 정보 저장 구조체
struct sockaddr_in server_addr;
struct sockaddr_in client_addr;


char buff_receive[BUFF_SIZE];
char buff_send[BUFF_SIZE];
	
void *Keyboard_Input() // 사용자로부터 키보드 입력을 받고 클라이언트에 메시지를 보내는 스레드 함수
{
	while(1)
	{
		if(getchar() == '\n')
		{
			printf("server : ");
			fgets(buff_send, sizeof(buff_send), stdin); // 입력한 텍스트를 읽어서 버퍼에 저장
			write(client_socket, buff_send, strlen(buff_send)); // 클라이언트 소켓으로 버퍼에 저장된 메세지 전송
		}
	}
}

int main(void)
{
	pthread_t p_thread[1];  // 스레드 식별자
    int thread_id;
    int status;
	
	thread_id = pthread_create(&p_thread[0], NULL, Keyboard_Input, NULL); // 스레드 생성
    if(thread_id < 0)
	{
		perror("thread_error\n");
		exit(0);
	}
	
	// memset : 특정 메모리 영역을 특정 값으로 초기화하는데 사용
	memset(&server_addr, 0, sizeof(server_addr)); // 'server_addr' 구조체를 0으로 초기화

	server_addr.sin_family = AF_INET;                         // sin_family : 주소 패밀리 설정, IPv4 인터넷 프로토콜
	server_addr.sin_port = htons(4000);                       // sin_port : 포트 번호 설정, 4000
	server_addr.sin_addr.s_addr = inet_addr("192.168.0.102"); // sin_addr : IP 주소, 32bit IPv4주소

		 
	// 소켓 생성(create)
	server_socket = socket(PF_INET, SOCK_STREAM, 0);    // TCP/IP 기반 소켓 SOCK_STREAM 사용  *UDP/IP에서는 SOCK_DGRAM을 사용
	if(server_socket == -1) 
	{
		printf("socket_error\n"); 
		exit(1);
	}

	// 바인딩(bind)
	if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) // 서버 소켓에 주소와 포트 번호 정보를 할당
	{
		printf("bind_error\n");
		exit(1);
	}
	
	// 대기열 설정(listen)
	if(listen(server_socket, 5) == -1) // 서버 소켓을 수신 대기 상태로 설정 *대기큐 : 5(최대 크기) - 동시에 처리할 수 있는 연결 요청의 수
	{
		printf("listen error\n");
		exit(1);
	}
	
	// 클라이언트 연결(accept)
	client_add_size = sizeof(client_addr); // client 주소 크기 대입
	client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_add_size); // 클라이언트의 연결 요청을 수락하고 통신을 위한 클라이언트 소켓 생성
	if(client_socket == -1) 
	{
		printf("클라이언트 연결 실패\n");
		exit(1);
	}
	else
	{
		printf("클라이언트 연결\n");
	}
	
	while(1)
	{
		read(client_socket, buff_receive, BUFF_SIZE); // 클라이언트 소켓에서 데이터를 읽고 버퍼에 저장
		printf("client : %s\n", buff_receive); // 읽은 데이터 출력
	}
	
	return 0;
}
