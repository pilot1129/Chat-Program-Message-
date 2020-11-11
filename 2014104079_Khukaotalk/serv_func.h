#include"stdafx.h"

int LoginCheck(char* id, char* pw)
{
	FILE *fd = NULL;
	int i, j;
	int readn = 0;
	char *buf;//buf = saving line, ptr = saving strstr
	char Fstr[255]; // read file buffer
	int Find;//Find = id find

	fd = fopen("/home/lsp05/Project/UserList.txt", "r");
	if(fd == NULL)
	{
		perror("File Read Error! \n");
		fclose(fd);
		return -1;
	}
	/* login check */
	Find = 0;
	i = 0;
	while(!feof(fd)){
		buf = fgets(Fstr, sizeof(Fstr), fd);
		if(Find == 0 && (i%2 == 0)){ // ID Find Func.
			*strchr(Fstr,'\n') = '\0';
			if(strcmp(Fstr,id) == 0){ // if id find
				Find = 1;
			}
		}
		else if(Find != 0){ //ID Correct, Find Pw Func.
			*strchr(Fstr,'\n') = '\0';
			if(strcmp(Fstr,pw) == 0){
				fclose(fd);
				return 2; // id, pw correct
			}
			else{//pw not correct
				fclose(fd);
				return 1;
			}
		}
		i++;
	}
	fclose(fd);
	return 0;
} // 2= correct, 1= Uncorrect PW, 0=no id, -1 = error

int UserCheck(char* FID)
{
	FILE *fd = NULL;
	int readn = 0;
	char *buf;//buf = saving line, ptr = saving strstr
	char Fstr[255]; // read file buffer

	fd = fopen("/home/lsp05/Project/UserList.txt", "r");
	if(fd == NULL)
	{
		perror("File Read Error! \n");
		fclose(fd);
		return -1;
	}
	/* login check */
	while(!feof(fd)){
		buf = fgets(Fstr, sizeof(Fstr), fd);
		*strchr(Fstr, '\n') = '\0';
		if(strcmp(FID,Fstr) == 0){ // find User ID
			fclose(fd);
			return 1;
		}
	}
	fclose(fd); // not find
	return 0;
}

void AddFriendToUser(char* ID, char* FID)
{
	FILE *fd = NULL;
	char *buf;
	char Fstr[255];
	char openstring[300] = "/home/lsp05/Project/";
	strcat(openstring, ID);
	strcat(openstring,"/FriendList.txt");
	fd = fopen(openstring, "a"); // add Friend
	if(fd == NULL){
		perror("File Read Error!!\n");
		fclose(fd);
		exit(1);
	}
	fprintf(fd,"%s\n",FID);
	fclose(fd);
}

void AddRoom(char* ID, char* FIDS)
{
	int i = 0;
	char RoomName[20];
	char UserName[30];
	int UserCount = 0;
	/*find RoomName*/
	while(1){
		if(FIDS[i] == ' ')
			break;
		else{
			RoomName[i] = FIDS[i];
			i++;
		}
	}
	i++;
	RoomName[i] = '\0';
	/*find UserCount*/
	UserCount = FIDS[i] - '0';
	i++; i++;

	/*find Users */
	char Users[UserCount][20];//userList(include ID)
	int j = 1;
	int k = 0;

	strcpy(Users[0],ID); // error?
	
	while(1){
		k = 0;
		while(1){
			if(FIDS[i] == ' '){
				i++;
				break;
			}
			else{
				UserName[k] = FIDS[i];
				k++;
				i++;
			}
		}
		strcpy(Users[j], UserName);
		j++;
			
		if(j == UserCount)
			break;
	}

	printf("Make Clear!\n");

	for(i = 0; i<UserCount; i++){//write every User
		FILE *fp;
		char openstring[300] = "/home/lsp05/Project/";
		strcat(openstring, Users[i]);
		strcat(openstring, "/ChatRoom/");
		strcat(openstring, RoomName);
		strcat(openstring, ".txt");
		fp = fopen(openstring, "wt");
		if(fp == NULL){
			perror("File Make Error!!\n");
			fclose(fp);
			exit(1);
		}
		fprintf(fp, "%d\n", UserCount);
		for(j = 0; j<UserCount; j++)
			fprintf(fp, "%s\n", Users[j]);
		fprintf(fp,"================\n");

		fclose(fp);
	}
}

void DeleteUserChatRoom(char* id, char* List){
	char RoomName[20];
	char UserName[30];
	int UserCount = 0;
	int i = 0;
	int j = 0;
	/* find UserCount(Not include Login User) */
	UserCount = List[i] - '0';
	
	/* find RoomName */
	while(1){
		if(List[i] == ' ')
			break;
		i++;
	}
	i++;
	while(1){
		if(List[i] == ' ')
			break;
		else{
			RoomName[j] = List[i];
			i++;
			j++;
		}
	}
	i++;
	
	printf("%s\n",RoomName);

	/*find Users*/
	char Users[UserCount][20]; // UserList
	j = 0;
	int k = 0;

	while(1){
		k = 0;
		while(1){
			if(List[i] == ' '){
				i++;
				break;
			}
			else{
				UserName[k] = List[i];
				k++;
				i++;
			}
		}
		strcpy(Users[j], UserName);
		j++;

		if(j == UserCount)
			break;
	}

	printf("Find Remove Users!\n");
	printf("%s\n", Users[0]);
	printf("%s\n", Users[1]);// user2

	for(i = 0; i<UserCount; i++){
		int readn = 0;
		char *buf, *ptr;
		char Fstr[255];
		char temp[30];
		char strtemp[30];

		/* read pointer */
		FILE *rp;
		char openstring[300] = "/home/lsp05/Project/";
		strcat(openstring, Users[i]);
		strcat(openstring, "/ChatRoom/");
		strcat(openstring, RoomName);
		strcat(openstring, ".txt");
		rp = fopen(openstring, "rt+");
		if(rp == NULL){
			perror("File Read Error!!\n");
			fclose(rp);
			exit(1);
		}

		/*write prointer*/
		FILE *wp;
		char Newopenstring[300] = "/home/lsp05/Project/";
		strcat(Newopenstring, Users[i]);
		strcat(Newopenstring,  "/ChatRoom/");
		strcat(Newopenstring, RoomName);
		strcat(Newopenstring, "_temp.txt");
		wp = fopen(Newopenstring, "wt");
		if(wp == NULL){
			perror("File Make Error!!\n");
			fclose(wp);
			exit(1);
		}
		j = 0;
		while(!feof(rp)){
			buf = fgets(Fstr, sizeof(Fstr), rp);
			ptr = strstr(Fstr, "\n");
			if(ptr != NULL)
				*strchr(Fstr, '\n') = '\0';
			if(strcmp(strtemp,Fstr)==0)
				break;
			strcpy(strtemp,Fstr);
			
			if(j==0){ // ChatRoom User Num
				readn = atoi(Fstr);
				readn--;
				sprintf(temp, "%d", readn);
				fprintf(wp,"%s\n", temp);
				j++;
			}
			else{
				if(strcmp(Fstr, id) != 0){ // Not UserID
					fprintf(wp,"%s\n",Fstr);
					j++;
				}
			}
		}
	fclose(rp);
	strcpy(Fstr,id);
	strcat(Fstr," is out of ChatRoom\n");
	fprintf(wp,Fstr);
	fclose(wp);
	int Rst = rename(Newopenstring, openstring);
	if(Rst == 0)
		printf("%s Change Name Clear!\n",Users[i]);
	else
		printf("%s Change Name Fail!\n", Users[i]);
	}
}

void AddMessageToRoom(char* id, char* UMsg){
	FILE *fd = NULL;
	char *buf, *ptr;
	char Fstr[255];
	char RoomName[20];
	char UserName[30];
	char SendMsg[255];
	char openstring[255];
	int UserCount = 0;
	int i = 0;
	int j = 0;

	/*find UserCount */
	UserCount = UMsg[i] - '0';
	i++; i++;
	
	/*find RoomName */
	while(1){
		if(UMsg[i] == ' ')
			break;
		else{
			RoomName[j] = UMsg[i];
			i++;
			j++;
		}
	}
	i++;

	/*find User*/
	char Users[UserCount][20];
	j = 0;
	int k = 0;

	while(1){
		k=0;
		while(1){
			if(UMsg[i] == ' '){
				i++;
				break;
			}
			else{
				UserName[k] = UMsg[i];
				k++;
				i++;
			}
		}
		strcpy(Users[j], UserName);
		j++;
		if(j == UserCount)
			break;
	}
	/*find Message*/
	k = 0;
	while(1){
		if(UMsg[i] == '=')
			break;
		SendMsg[k] = UMsg[i];
		k++;
		i++;
	}
	SendMsg[k+1] = '\0';
	
	for(i = 0; i<UserCount; i++){
		int readn = 0;
		strcpy(openstring,"/home/lsp05/Project/");
		strcat(openstring, Users[i]);
		strcat(openstring, "/ChatRoom/");
		strcat(openstring, RoomName);
		strcat(openstring, ".txt");
		fd = fopen(openstring, "a");
		if(fd == NULL){
			perror("File Open Error!\n");
			fclose(fd);
			exit(1);
		}
		fprintf(fd,"%s\n",SendMsg);
		fclose(fd);
	}
}
