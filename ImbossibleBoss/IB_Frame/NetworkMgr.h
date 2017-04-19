#pragma once

#include "SingleTon.h"
//protocol

#define SERVER_PORT 9595

#define MAX_CLIENT 100

#define THREAD_NUM 8

#define BUF_SIZE 1024
#define WM_SOCKET WM_USER + 1


#define MAX_BUFFER_SIZE 1024
#define MAX_PACKET_SIZE 1024
#define MAX_STR_SIZE 100
#define PACKET_SIZE_SIZE 2
#define PACKET_TYPE_SIZE 2


#define SC_ID 1
#define SC_NOTICE 2
#define SC_LOBBY_CLIENT_LIST 3
#define SC_PUT_PLAYER 4
#define SC_REMOVE_PLAYER 5
#define SC_PLAYER_POS 6

#define CS_PLAYER_POS 1

//////////////////////////////////////////////////////
//
//   Struct
//
//////////////////////////////////////////////////////
enum EVENTTYPE {
	E_RECV, E_SEND
};

struct Player {
	float PosX, PosY, PosZ;
	float RotY;
	char AnimNumber;
};

struct WSAOVERLAPPED_EX {
	WSAOVERLAPPED Overlap;
	WSABUF Wsabuf;
	unsigned char IocpBuffer[MAX_BUFFER_SIZE];
	EVENTTYPE eEventType;
};

struct ClientInfo {
	int Id;
	int iRoomNumber;
	bool bConnected;
	SOCKET Sock;
	WSAOVERLAPPED_EX RecvOverEx;
	mutex myLock;

	unsigned char PacketBuffer[MAX_PACKET_SIZE];
	int iPrevRecvSize;
	int iCurrPacketSize;

	Player tPlayer;
};

struct ClientInLobby {
	int id;
	int iRoomNumber;
};



//////////////////////////////////////////////////////////////////
//
//  Protocol
//
//////////////////////////////////////////////////////////////////

#pragma pack(push, 1)
struct sc_packet_id {
	WORD Size;
	WORD Type;
	INT Id;
};

struct sc_packet_notice {
	WORD Size;
	WORD Type;
	UCHAR Message[MAX_STR_SIZE];
};

struct sc_packet_lobby_client_list {
	WORD Size;
	WORD Type;
	INT ClientId[MAX_CLIENT];
	INT ClientRoomNumber[MAX_CLIENT];
};
struct sc_packet_put_player {
	WORD Size;
	WORD Type;
	INT Id;
	INT x;
	INT y;
};

struct sc_packet_remove_player {
	WORD Size;
	WORD Type;
	INT Id;
};

struct sc_packet_player_position {
	WORD Size;
	WORD Type;

	INT Id;

	DWORD PosX;
	DWORD PosY;
	DWORD PosZ;

	DWORD RotY;

	BYTE AnimNumber;
};

struct cs_packet_player_position {
	WORD Size;
	WORD Type;

	DWORD PosX;
	DWORD PosY;
	DWORD PosZ;

	DWORD RotY;

	BYTE AnimNumber;
};
#pragma pack(pop)
//protocol
unsigned char Notice[MAX_STR_SIZE] = "sssss";

class CNetworkManager : public CSingleTonBase<CNetworkManager>{

public:
	bool Begin();
	bool End();

	bool Connect(string ip);

	bool OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void ProcessPacket(char* ptr);

	void SendPacket(void* pData, int iDataSize);

	void ReadPacket(SOCKET sock);
private:
	SOCKET      ClientSocket;
	WSABUF      send_wsabuf;
	char      send_buffer[BUF_SIZE];
	WSABUF      recv_wsabuf;
	char      recv_buffer[BUF_SIZE];
	char      packet_buffer[BUF_SIZE];
	DWORD      in_packet_size = 0;
	int         saved_packet_size = 0;
	int         g_myid;

public:
	CNetworkManager();
	virtual ~CNetworkManager();

};