#include"stdafx.h"

typedef struct
{
	char id[20];
	char pw[20];
}User;

typedef struct
{
	int type;
	int FuncType;
	char Message[200];
	User user;
}Msg;
