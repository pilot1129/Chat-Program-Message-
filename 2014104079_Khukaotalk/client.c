#include"stdafx.h"
//#include"Connect.h"
#include"UI.h"
#include"client_func.h"

void main()
{
	int	sockfd, n;
	struct sockaddr_in	servAddr;
	char ID[20];
	char PW[20];
	char FRID[30];
	char FriendList[10][40];
	char RoomName[30];
	int select;
	int Num;
	char* DeleteUList;
	char UserMsg[255];
	int pid;

	/* Connect Server */
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){//create TCP
		perror("socket");
		exit(1);
	}
	bzero((char *)&servAddr, sizeof(servAddr)); // init zero
	servAddr.sin_family = PF_INET; // set TCP
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); // set ip
	servAddr.sin_port = htons(SERV_TCP_PORT); // set TCP

	if (connect(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
		perror("connect");
		exit(1);
	}
	
	/* Login */
	while(1){
		First_UI(ID, PW);
		n = Login(ID, PW, sockfd);
		if(n ==1)
			break;
	}

	/*Login After Menu*/
	while(1){
		int Second = 0;
		int Third = 0;
		Second = Second_UI(ID, PW);
		switch(Second){
			case 1 : // Friend Menu
				clear();
				while(1){
					Third = Friend_UI(ID, PW);
					switch(Third){
						case 1 : // view friend
							View_Friend_UI(ID);
							break;
						case 2 : // add friend
							Add_Friend_UI(ID,FRID);//FRID input
							select = AddFriend(ID,sockfd,FRID);
							break;
						case 3 : // back to After Menu
							break;
					}
					if(Third == 3)
						break;
				}
				break;
			case 2 :
				clear();
				while(1){
					Third = Chat_UI(ID, PW);
					switch(Third){
						case 1 :
							//view chatroom
							select = ViewRoomName(ID, RoomName);
							if(select ==0)
								break;
							while(1){
								Num = Chat(ID, RoomName,UserMsg); // Must make real-time synchronization[Can't implementation]
								if(Num == 0)
									break;
								SendMessage(ID, RoomName, sockfd, UserMsg);
							}
							break;
						case 2 : // Add ChatRoom
							select = AddChat_UI(ID, &FriendList, RoomName);
							printf("%s\n",RoomName);
							MakeChatRoom(ID,&FriendList,sockfd,RoomName,select);
							break;
						case 3 : // exit chatroom
							DeleteChat_UI(RoomName);
							ExitChatRoom(ID,sockfd,RoomName);
							break;
						case 4 : // back to After Menu
							break;
					}
					if(Third == 4)
						break;
				}
			case 3 :
				printf("Thank you for use KHUKAOTALK\n");
				break;
		}
		if(Second ==3)
			break;
	}
	close(sockfd);
}
