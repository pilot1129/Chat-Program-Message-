#include"stdafx.h"

void First_UI(char* id, char* pw);
int Second_UI(char* id, char* pw);
int Friend_UI(char* id, char* pw);
void View_Friend_UI(char* id);
void Add_Friend_UI(char* id, char* fid);
int Chat_UI(char* id, char* pw);
int AddChat_UI(char* id, char*** FID, char* RoomName);
void DeleteChat_UI(char* RoomName);
int ViewRoomName(char* id, char* RoomName);
int Chat(char* id, char* RoomName, char* UsrMsg);
