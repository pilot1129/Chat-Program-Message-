#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<signal.h>
#include<string.h>
#include<strings.h>
#include<dirent.h>
#include<time.h>
#include<stdio_ext.h>
#include<fcntl.h>
#include<dirent.h>
#include<error.h>

#define MY_ID 05
#define SHM_KEY (0x9000 + MY_ID)
#define SHM_MODE (SHM_R | SHM_W | IPC_CREATE)
#define EMPTY_SEM_KEY (0x5000 + MY_ID)
#define FULL_SEM_KEY (0x6000 + MY_ID)
#define MUTEX_SEM_KEY (0x7000 + MY_ID)
#define SERV_TCP_PORT (8000 + MY_ID)
#define SERV_HOST_ADDR "127.0.0.1"

//Request Type
#define MSG_REQUEST 1
#define MSG_REPLY 2
#define MSG_LOGIN 3
#define MSG_Friend 4
#define MSG_ROOM 5
#define MSG_LOGOUT 6

//Login Type
#define SERVER_ERROR 0
#define NO_ID 1
#define NO_PW 2
#define SUCCESS 3

//Friend Type
#define FR_Add 1

//AddFriend
#define REQ_FR_NOTEXIST 1
#define REQ_FR_SUCCESS 2

//RoomType
#define SEND_ROOM 1
#define ADD_ROOM 2
#define DEL_ROOM 3

#define clear() printf("\033[H\033[J")

