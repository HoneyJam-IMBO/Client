#pragma once

#include "SingleTon.h"
//protocol

#define BUF_SIZE 1024

#define SERVER_PORT 9595

#define MAX_CLIENT 100

#define THREAD_NUM 8

#define WM_SOCKET WM_USER + 1


#define MAX_BUFFER_SIZE         1024
#define MAX_PACKET_SIZE         1024
#define MAX_STR_SIZE         100
#define PACKET_SIZE_SIZE      2
#define PACKET_TYPE_SIZE      2
#define MAX_CLIENT_IN_ROOM		4

#define SC_ID                  1
#define SC_NOTICE               2
#define SC_LOBBY_CLIENT_LIST      3
#define SC_ROOM_INFO            13
#define SC_CREATE_ROOM_SUCCESS      11
#define SC_CREATE_ROOM_FAIL         12
#define SC_JOIN_ROOM_SUCCESS      4
#define SC_JOIN_ROOM_FAIL         5
#define SC_CLIENT_IN_ROOM         6
#define SC_ALL_READY_COMPLETE      7
#define SC_GAME_START            8
#define SC_REMOVE_PLAYER         9
#define SC_PLAYER_POS            10

#define CS_CREATE_ROOM            6
#define CS_JOIN_ROOM            1
#define CS_OUT_ROOM               2
#define CS_CHARACTER_READY_CHANGE   3
#define CS_LOADING_COMPLETE         4
#define CS_PLAYER_POS            5

//////////////////////////////////////////////////////
//
//   Struct
//
//////////////////////////////////////////////////////
enum EVENTTYPE {
	E_RECV, E_SEND
};

struct ClientInLobby {
	int id;
	int iRoomNumber;
};

struct ClientInRoom {
	char Character;
	bool isReady;
};

struct Player {
	float PosX{ 0.f }, PosY{ 0.f }, PosZ{ 0.f };
	float RotY{ 0.f };
	char AnimNumber{ -1 };
};

struct WSAOVERLAPPED_EX {
	WSAOVERLAPPED Overlap;
	WSABUF Wsabuf;
	unsigned char IocpBuffer[MAX_BUFFER_SIZE];
	EVENTTYPE eEventType;
};

class ClientInfo {
public:
	int Id{ -1 };
	int iRoomNumber{ -1 };
	bool bConnected{ false };
	SOCKET Sock;
	WSAOVERLAPPED_EX RecvOverEx;
	mutex myLock;

	unsigned char PacketBuffer[MAX_PACKET_SIZE];
	int iPrevRecvSize;
	int iCurrPacketSize;

	bool bLoadingComplete;
	ClientInRoom tInRoom;
	Player tPlayer;
};


static int num_other_player = 0;
static int other_id[3] = {-1, -1, -1};

class RoomInfo {
public:
	int iRoomNumber;
	char ClientCount;
	bool isStart;
	int ClientsIDInRoom[MAX_CLIENT_IN_ROOM];
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

struct sc_packet_join_room_result {
	WORD Size;
	WORD Type;
};

struct sc_packet_room_info {
	WORD Size;
	WORD Type;
	INT RoomNumber;
	INT ClientCount;
	BOOL isStart;
	INT ClientsId[MAX_CLIENT_IN_ROOM];
};

struct sc_packet_client_info_in_room {
	WORD Size;
	WORD Type;
	INT Id;
	int Character;
	BOOL isReady;
};

struct sc_packet_all_ready {
	WORD Size;
	WORD Type;
};

struct sc_packet_all_loading_complete {
	WORD Size;
	WORD Type;
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

	float PosX;
	float PosY;
	float PosZ;

	float RotY;

	UINT AnimNumber;
};
////////////////////////////////////////////////////////
// CS
////////////////////////////////////////////////////////
struct cs_packet_create_room {
	WORD Size;
	WORD Type;
};
struct cs_packet_join_room {
	WORD Size;
	WORD Type;
	INT RoomNumber;
};

struct cs_packet_out_room {
	WORD Size;
	WORD Type;
	INT RoomNumber;
};

struct cs_packet_client_info_in_room {
	WORD Size;
	WORD Type;
	int Character;
	BOOL isReady;
};

struct cs_packet_loading_complete {
	WORD Size;
	WORD Type;
};

struct cs_packet_player_position {
	WORD Size;
	WORD Type;

	float PosX;
	float PosY;
	float PosZ;

	float RotY;

	UINT AnimNumber;
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
	char		send_buffer[BUF_SIZE];
	WSABUF      recv_wsabuf;
	char		recv_buffer[BUF_SIZE];
	char		packet_buffer[BUF_SIZE];
	DWORD		in_packet_size = 0;
	int         saved_packet_size = 0;
	int         g_myid;

public:
	CNetworkManager();
	virtual ~CNetworkManager();

};