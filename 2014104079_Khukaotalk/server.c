#include"stdafx.h"
#include"Connect.h"
#include"serv_func.h"

int Sockfd;

void CloseServerSignal()
{
	close(Sockfd);
	printf("\nServer Closed!\n");
	exit(0);
}

void main()
{
	int newSockfd, cliAddrLen, n, checkuser, checkchoice;
	struct sockaddr_in cliAddr, servAddr;
	char ID[20];
	char PW[20];
	int pid;
	Msg msg;

	signal(SIGINT, CloseServerSignal); // ctrl + c signal
	
	if((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) // make TCP Socket
	{
		perror("socket");
		exit(1);
	}
	
	bzero((char*)&servAddr, sizeof(servAddr)); // write zero(init)
	servAddr.sin_family = PF_INET;//set TCP
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);//set 32bit addr
	servAddr.sin_port = htons(SERV_TCP_PORT);//set tcp number

	if(bind(Sockfd, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)  // bind name
	{
		perror("bind");
		exit(1);
	}

	listen(Sockfd ,5);
	printf("Server Run\n");
	cliAddrLen = sizeof(cliAddr);
	while(1) {
		newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
		if(newSockfd < 0){
			perror("accept");
			exit(1);
		}
		if((pid = fork()) == 0){ // child
			close(Sockfd);

			while(1){
				checkchoice = 0;
				if((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0)
				{
					perror("read");
					exit(1);
				}
				if(msg.type == MSG_LOGIN){
					printf("%s Login tried.\n", msg.user.id);
					/*Login Check*/
					checkuser = 0;
					strcpy(ID, msg.user.id);
					strcpy(PW, msg.user.pw);
					checkuser = LoginCheck(ID, PW);
					switch(checkuser){
					case -1 :
						printf("file read error!\n");
						msg.FuncType = SERVER_ERROR;
						break;
					case 0 :
						printf("ID not correct!\n");
						msg.FuncType = NO_ID;
						break;
					case 1 :
						printf("PW not correct!\n");
						msg.FuncType = NO_PW;
						break;
					case 2 :
						printf("%s Logined.\n", ID);
						msg.FuncType = SUCCESS;
						break;
					}
					msg.type = MSG_REPLY;
					if(write(newSockfd, (char*) &msg, sizeof(msg)) < 0)
					{
						perror("write");
						exit(0);
					}
				}
				else if(msg.type == MSG_Friend){
					char FID[30];
					printf("%s tried FriendMenu. \n", msg.user.id);
					/*Friend Check*/
					strcpy(ID, msg.user.id);
					strcpy(FID, msg.user.pw); // Friend ID
					checkchoice = msg.FuncType;
					if(checkchoice == 1){
						n = UserCheck(FID);
						if(n==0){ // not exist
							printf("Not exist User\n");
							msg.FuncType = REQ_FR_NOTEXIST;
						}
						else{ // exist
							AddFriendToUser(ID,FID);
							printf("%s Add Friend %s", ID, FID);
							msg.FuncType = REQ_FR_SUCCESS;
						}
					}
					msg.type = MSG_REPLY;
					if(write(newSockfd, (char*) &msg, sizeof(msg)) < 0)
					{
						perror("write");
						exit(0);
					}
				}
				else if(msg.type == MSG_ROOM){
					printf("%s tried RoomMenu. \n", msg.user.id);
					checkchoice = msg.FuncType;

					if(checkchoice == ADD_ROOM){ // ADD_ROOM
						AddRoom(msg.user.id, msg.Message);
					}
					else if(checkchoice == DEL_ROOM){// DEL_ROOM
						DeleteUserChatRoom(msg.user.id, msg.Message);
					}
					else if(checkchoice == SEND_ROOM){ // chat
						AddMessageToRoom(msg.user.id, msg.Message);
					}
					msg.type = MSG_REPLY;
					msg.FuncType = SUCCESS;
					if(write(newSockfd, (char*) &msg, sizeof(msg)) < 0)
					{
						perror("write");
						exit(0);
					}
				}
				else if(msg.type == MSG_LOGOUT){
					printf("%s Logout\n", ID);
					msg.type = MSG_REPLY;
					if(write(newSockfd, (char*) &msg, sizeof(msg)) < 0)
					{
						perror("write");
						exit(0);
					}
				}
				//exit(0);
			}
			exit(0);
		}
		else//parent
			close(newSockfd);
	}
}
