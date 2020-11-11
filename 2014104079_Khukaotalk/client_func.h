#include"stdafx.h"
#include"Connect.h"

Msg msg;
//memset(&msg, 0, sizeof(Msg));

/* Login Request */
int Login(char* ID, char* PW, int sockfd){
	msg.type = MSG_LOGIN;
	strcpy(msg.user.id, ID);
	strcpy(msg.user.pw, PW);
	if(write(sockfd, (char*)&msg, sizeof(msg)) < 0){
		perror("write");
		exit(1);
	}
	if((read(sockfd, (char*)&msg, sizeof(msg)))<0){
		perror("read");
		exit(1);
	}
	switch(msg.FuncType){
		case 1:
			printf("Uncorrect ID!\n");
			break;
		case 2:
			printf("Uncorrect PW!\n");
			break;
		case 3:
			printf("Login Success!\n");
			break;
	}
	if(msg.FuncType==3)
		return 1;
	else
		return 0;
}

/*friend Add */
int AddFriend(char* ID, int sockfd, char* FID){
	FILE *fd = NULL;
	int i;
	int readn = 0;
	char *buf, *ptr;
	char Fstr[255];
	char temp[255];
	int Find = 0;
	/* check Friend */
	char openstring[300] = "/home/lsp05/Project/";
	strcat(openstring,ID);
	strcat(openstring,"/FriendList.txt");
	fd = fopen(openstring, "r");
	if(fd == NULL){
		perror("File Read Error!\n");
		fclose(fd);
		exit(1);
	}
	while(!feof(fd)){
		buf = fgets(Fstr, sizeof(Fstr), fd);
		ptr = strstr(Fstr,"\n");
		if(ptr != NULL)
			*strchr(Fstr, '\n') = '\0';
		if(strcmp(FID,Fstr) == 0){
			Find = 1;
			break;
		}
	}

	if(Find == 1){
		printf("Already Exist Friend!\n");
		return 0;
	} // Already Exist
	
	if(strcmp(ID,FID) == 0){
		printf("This ID is User's ID!\n");
		return 0;
	} // my id

	//Connect Server
	msg.type = MSG_Friend;
	msg.FuncType = FR_Add;
	strcpy(msg.user.id, ID);
	strcpy(msg.user.pw, FID); // FriendID

	if(write(sockfd, (char*)&msg,sizeof(msg)) < 0){
		perror("write");
		exit(1);
	}
	if((read(sockfd, (char*)&msg, sizeof(msg))) < 0){
		perror("read");
		exit(1);
	}
	switch(msg.FuncType){
		case 1: // Not exist user
			printf("Not Exist User!\n");
			return 0;
		case 2: // success
			printf("Add Friend Success!\n");
			return 1;
	}
}

void MakeChatRoom(char *ID, char*** FID, int sockfd, char* ChatName, int Fsize){
	FILE *fd = NULL;
	int i;
	int readn = 0;
	char *buf, *ptr;
	char Fstr[255];
	char temp[255];
	int Find = 0;
	/* check Friend */
	char openstring[300] = "/home/lsp05/Project/";
	strcat(openstring,ID);
	strcat(openstring,"/FriendList.txt");
	fd = fopen(openstring, "r");
	if(fd == NULL){
		perror("File Read Error!\n");
		fclose(fd);
		exit(1);
	}
	while(!feof(fd)){
		buf = fgets(Fstr, sizeof(Fstr), fd);
		ptr = strstr(Fstr,"\n");
		if(ptr != NULL)
			*strchr(Fstr, '\n') = '\0';
		if(strcmp(temp,Fstr)==0)
			break;
		strcpy(temp,Fstr);
		for(i = 0; i<Fsize; i++){
			if(strcmp(&FID[i],Fstr) == 0){
				Find++;
				break;
			}
		}
	}
	fclose(fd);

	if(Find == Fsize){ // correct FriendName
		/* check RoomName */
		strcpy(openstring,"/home/lsp05/Project/");
		strcat(openstring,ID);
		strcat(openstring,"/ChatRoom/");
		strcat(openstring, ChatName);
		strcat(openstring, ".txt");

		int nResult = access(openstring, 0);
		if(nResult == 0){
			printf("Already Exist Room!\n");
			return 0;
		}
		printf("Request has been sent\n");

		//MakeChatRoom
		char str[10];
		msg.type = MSG_ROOM;
		msg.FuncType = ADD_ROOM;
		strcpy(msg.user.id,ID);
		strcpy(msg.Message, ChatName);
		strcat(msg.Message, " ");		
		Fsize = Fsize+1;
		sprintf(str, "%d", Fsize);
		strcat(msg.Message, str);		
		strcat(msg.Message, " ");		
		strcat(msg.Message,&FID[0]);
		strcat(msg.Message, " ");

		for(i = 1; i<Fsize; i++){
			strcat(msg.Message,&FID[i]);
			strcat(msg.Message, " ");
		}

		if(write(sockfd, (char*)&msg,sizeof(msg)) < 0){
			perror("write");
			exit(1);
		} // Make Request

		if((read(sockfd, (char*)&msg, sizeof(msg))) < 0){
			perror("read");
			exit(1);
		}
		if(msg.FuncType == SUCCESS)
			printf("Make Success!\n");
	}
	else // uncorrect FriendName
		printf("Uncorrect FriendName!\n");
}

void ExitChatRoom(char *ID, int sockfd, char* ChatRoom){
	FILE *fd = NULL;
	char *buf, *ptr;
	char Fstr[255];
	char temp[255];
	char openstring[300] = "/home/lsp05/Project/";
	int i, rmParam;
	int UserCount;
	char MoveString[300];

	strcat(openstring, ID);
	strcat(openstring, "/ChatRoom/");
	strcat(openstring, ChatRoom);
	strcat(openstring, ".txt");
	
	
	//find Chat Room
	int nResult = access(openstring, 0);
	if(nResult != 0){
		printf("Not Exist Chat Room!\n");
		exit(1);
	}
	printf("Find Chat Room!\n");

	// Read List
	fd = fopen(openstring, "r");
	if(fd == NULL){
		perror("File Read Error!\n");
		fclose(fd);
		exit(1);
	}
	i = 0;
	while(!feof(fd)){
		buf = fgets(Fstr, sizeof(Fstr), fd);
		ptr = strstr(Fstr, "\n");

		if(ptr != NULL)
			*strchr(Fstr, '\n') = '\0';
		
		if(strcmp(temp,Fstr) == 0)
			break;

		strcpy(temp, Fstr);

		if(i == 0){
			UserCount = atoi(Fstr);
			UserCount--;
			sprintf(MoveString,"%d",UserCount);
			strcat(MoveString, " ");
			i++;

			strcat(MoveString, ChatRoom);
			strcat(MoveString, " ");
		}
		else if(strcmp(Fstr,ID) == 0) // Fstr == UID
			;
		else{
			strcat(MoveString,Fstr);
			strcat(MoveString," ");
		}
	}
	fclose(fd);

	//Delete User's Chat Room
	rmParam = remove(openstring);
	if(rmParam == 0)
		printf("Remove Success!\n");
	else
		perror("Remove Error!\n");

	//Send to server
	msg.type = MSG_ROOM;
	msg.FuncType = DEL_ROOM;
	strcpy(msg.user.id, ID);
	strcpy(msg.Message,MoveString);

	if((write(sockfd, (char*)&msg, sizeof(msg))) < 0){
		perror("write");
		exit(1);
	}

	if((read(sockfd, (char*)&msg, sizeof(msg))) < 0){
		perror("read");
		exit(1);
	}
	if(msg.FuncType == SUCCESS)
		printf("Quit Success!\n");
	else
		printf("Quit Error!\n");
}

void SendMessage(char* id, char* RoomName, int sockfd, char* Message){
	char MoveString[255];
	FILE *fd = NULL;
	char *buf, *ptr;
	char Fstr[255];
	char temp[255];
	char openstring[300] = "/home/lsp05/Project/";
	int UserCount;

	strcat(openstring, id);
	strcat(openstring, "/ChatRoom/");
	strcat(openstring, RoomName);
	strcat(openstring, ".txt");

	// Read List
	fd = fopen(openstring, "r");
	if(fd == NULL){
		perror("File Read Error!\n");
		fclose(fd);
		exit(1);
	}
	int i = 0;
	while(!feof(fd)){
		buf = fgets(Fstr, sizeof(Fstr), fd);
		ptr = strstr(Fstr, "\n");

		if(ptr != NULL)
			*strchr(Fstr, '\n') = '\0';
		
		if(strcmp(temp,Fstr) == 0)
			break;

		strcpy(temp, Fstr);

		if(i == 0){
			UserCount = atoi(Fstr);
			UserCount--;
			sprintf(MoveString,"%d",UserCount);
			strcat(MoveString, " ");
			i++;

			strcat(MoveString, RoomName);
			strcat(MoveString, " ");
		}
		else if(strcmp(Fstr,id) == 0) // Fstr == UID
			;
		else{
			ptr = strstr(Fstr, "=");
			if(ptr != NULL)
				break;
			strcat(MoveString,Fstr);
			strcat(MoveString," ");
		}
	}
	fclose(fd);

	strcat(MoveString, id);
	strcat(MoveString, " : ");
	strcat(MoveString, Message);
	strcat(MoveString, "=");
	msg.type = MSG_ROOM;
	msg.FuncType = SEND_ROOM;
	strcpy(msg.user.id, id);
	strcpy(msg.Message, MoveString);
	
	if((write(sockfd, (char*)&msg, sizeof(msg))) < 0){
		perror("write");
		exit(1);
	}
	if((read(sockfd, (char*)&msg, sizeof(msg))) < 0){
		perror("read");
		exit(1);
	}
}
