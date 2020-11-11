#include"UI.h"

void First_UI(char* id, char* pw){
	printf("=========================KHUKAOTALK=========================\n\n");
	printf("ID : ");
	scanf("%s", id);
	printf("PW : ");
	 __fpurge(stdin);
	 scanf(" %s", pw);
	 clear();
}

int Second_UI(char* id, char* pw){
	int request;
	printf("============================MENU============================\n\n");
	printf("1. FriendList\n");
	printf("2. ChatRoomList\n");
	printf("3. LogOut\n");
	printf("Choose Menu : ");
	scanf(" %d", &request);
	clear();

	return request;
}

int Friend_UI(char* id, char* pw){
	int request;
	printf("=========================FriendMenu=========================\n\n");
	printf("1. View Friend List\n");
	printf("2. Add Friend\n");
	printf("3. Back To Main Menu\n");
	printf("Choose Menu : ");
	scanf(" %d", &request);
	clear();
	  
	return request;
	}

void View_Friend_UI(char* id){
	FILE *fd = NULL;
	int i;
	int readn = 0;
	char *buf;
	char Fstr[255];
	char temp[255];
	char openstring[300] = "/home/lsp05/Project/";
	strcat(openstring,id);
	strcat(openstring,"/FriendList.txt");
	fd = fopen(openstring,"r");
	if(fd == NULL)
	{
		perror("File Read Error! \n");
		fclose(fd);
		exit(1);
	}
	printf("=========================FriendList=========================\n\n");
	while(!feof(fd)){
		buf = fgets(Fstr, sizeof(Fstr), fd);
		if(strcmp(temp,Fstr) == 0)
			break;
		strcpy(temp,Fstr);
		puts(Fstr);
	}
	fclose(fd);
	printf("======================\n");
	printf("Please Put 1 to Back Menu : \n");
	scanf(" %d", &i);
	clear();
	if(i == 1)
		return;
}
	
void Add_Friend_UI(char* id, char* fid){
	printf("==========================AddMenu===========================\n\n");
	printf("Please Enter the ID of the friend you want to add.\n");
	printf(" ID : ");
	scanf(" %s", fid);
	clear();
}

int Chat_UI(char* id, char* pw){
	int request;
	printf("==========================ChatMenu==========================\n\n");
	printf("1. View ChatRoom List\n");
	printf("2. Add ChatRoom \n");
	printf("3. Exit ChatRoom\n");
	printf("4. Back To Main Menu\n");
	printf("Choose Menu : ");
	scanf(" %d", &request);
	clear();
	   
	return request;
}

int AddChat_UI(char* id, char*** FID, char* RoomName){
	int i = 0;
	char InID[40];
	printf("==========================ChatMenu==========================\n\n");
	printf("Please Enter the ID of the friend you want to add to Chatroom\n");
	printf("+ Input One by One and Please enter 0 to finish typing\n");
	printf("[FriendID] \n");
	while(1)
	{
		printf("%d. : ", i+1);
		getchar();
		scanf("%s",InID);
		if(strcmp(InID,"0") == 0){ //end signal
			break;
		}
		else{
			//FID[i] = InID;
			strcpy(&FID[i], InID);
			i++;
		}
	}
	if(i == 0){
		clear();
		return i;
	}
	else{
		printf("Enter the RoomName : ");
		scanf(" %s",RoomName);
		clear();
		return i;
	}
}
void DeleteChat_UI(char* RoomName){
	printf("==========================ChatMenu==========================\n\n");
	printf("Please Enter the name of the chat room to exit\n");
	printf("ChatRoomName : ");
	scanf("%s",RoomName);
	clear();
}

int ViewRoomName(char* id, char* RoomName){
	DIR *dir;
	struct dirent *ent;
	char openstring[300] = "/home/lsp05/Project/";
	int Rnum = 0;
	int i = 1;
	int j = 0;
	int FileNum = 0;
	char RName[30];
	char TempName[30];
	strcat(openstring, id);
	strcat(openstring, "/ChatRoom");
	dir = opendir(openstring);
	printf("========================ChatRoomList========================\n\n");
	if(dir != NULL){
		while((ent = readdir(dir)) != NULL){
			FileNum++;
			if(strcmp(ent->d_name,".") == 0)//first
				continue;
			else if(strcmp(ent->d_name,"..")==0)//second
				continue;
			else{
				j = 0;
				strcpy(TempName, ent->d_name);
				while(1){
					if(TempName[j] == '.')
						break;
					RName[j] = TempName[j];
					j++;
				}
				RName[j] = '\0';
				printf("%d. %s\n", i, RName);
				i++;
			}
		}
		closedir(dir);
	}
	else{
		printf("Not Exist ChatRoom!\n");
		getchar();
		return 0;
	}

	printf("Please enter the number of the chat room you want to enter!\n");
	printf("If you want to exit, please enter '0'\n");
	printf("ListNumber : ");
	scanf("%d",&Rnum);

	if(Rnum == 0)
		return 0;
	else{
		i = 1;
		dir = opendir(openstring);
		if(dir != NULL){
			while((ent = readdir(dir)) != NULL){
				if((Rnum+2) == i){
					strcpy(TempName, ent->d_name);
					j = 0;
					while(1){
						if(TempName[j] == '.')
							break;
						RName[j] = TempName[j];
						j++;
					}
					RName[j] = '\0';
					strcpy(RoomName,RName);
				}
				i++;
			}
		}
	}
	return 1;	
}


int Chat(char* id, char* RoomName, char* UsrMsg){ // DEBUG PLZ
	FILE *fp;
	char openstring[300] = "/home/lsp05/Project/";
	char *buf, *ptr;
	char temp[255];
	char Fstr[255];
	char Message[255];
	int write = 0;//note start
	clear();
	printf("==========================%s==========================\n\n",RoomName);
	printf("[system : Welcome to Chatting Room.]\n");
	strcat(openstring, id);
	strcat(openstring, "/ChatRoom/");
	strcat(openstring, RoomName);
	strcat(openstring, ".txt");
	fp = fopen(openstring, "rt");
	if(fp == NULL){
		perror("File Make Error!!\n");
		fclose(fp);
		exit(1);
	}
	while(!feof(fp)){
		buf = fgets(Fstr, sizeof(Fstr), fp);
		ptr = strstr(Fstr, "\n");
		if(ptr != NULL)
			*strchr(Fstr, "\n");
		if(strcmp(temp,Fstr) == 0)
			break;
		strcpy(temp,Fstr);

		ptr = strstr(Fstr, "="); // find startline
		if(ptr != NULL){
			write = 1;
			continue;
		}
		if(write == 1)
			printf("%s\n",Fstr); // print Messages
	}
	fclose(fp);
	printf("===============================================\n");
	printf("[If you want exit the Chatroom, input '0']\n");
	printf("Message : ");
	scanf("%s", UsrMsg);
	
	if(strcmp(UsrMsg,"0")==0)
		return 0;
	//write My file
	fp = fopen(openstring, "a");
	fprintf(fp, "%s > %s\n",id,UsrMsg);
    fclose(fp);
	return 1;
}
