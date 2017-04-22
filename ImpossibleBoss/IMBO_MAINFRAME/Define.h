#pragma once

#define NEAR_PLANE 3.f
#define FAR_PLANE 3000.f

//#define WINSIZEX 1024
//#define WINSIZEY 768
#define WINSIZEX 1400
#define WINSIZEY 1050


// vs
#define VS_CB_CAMERA			0
#define VS_CB_MODEL				1
#define VS_CB_TEXTURE_MATRIX	2
#define VS_GLOBAL_BUFFER_SLOT	10
// ds
#define DS_CB_CAMERA			0		
// cs
// gs
#define GS_CB_CAMERA			0
#define GS_CB_AABB				1
// ps
#define PS_CB_MATERIAL			3
#define PS_CB_SHADOW			4


//TEXTURE
//ds
#define DS_SLOT_HEIGHTMAP		0
//ps
#define PS_TEXTURE 0x00
#define PS_SLOT_TERRAIN_BASE	0
#define PS_SLOT_TERRAIN_DETAIL	1
#define PS_SLOT_NORMALMAP		5
#define PS_SLOT_SKYBOX 13
//TEXTURE

//SAMPLER
#define PS_TEXTURE_SAMPLER		0
#define PS_NORMALMAP_SAMPLER	5
//SAMPLER

//DEFERRED_LIGHT
#define PS_UNPACKING_SLOT		0x00
#define PS_CAMERA_DATA_SLOT		0x01
#define PS_OBJECT_BUFFER_SLOT	0x02

#define DS_OBJECT_BUFFER_SLOT	0x00

#define MAX(a,b)(a>b? a : b)
#define MIN(a,b)(a<b? a : b)

#define MAX_SPACE_NUM  64

//texture, constant buffer bind flag
#define BIND_VS 0b000001   // 0x0001 //0001
#define BIND_HS 0b000010   // 0x0002 //0010
#define BIND_DS 0b000100   // 0x0004 //0100
#define BIND_GS 0b001000   // 0x0008 //1000
#define BIND_PS 0b010000   // 0x0010 //...
#define BIND_CS 0b100000   // 0x0020 //..

//aabb
#define BOUNDINGBOX_NUM 10000
//coordinatesystem
#define COORD_NUM 5000

#define RENDER_TARGET_NUMBER 3


#define MSG_BOX(_message)	MessageBox(NULL, TEXT(_message), TEXT("System Message"), MB_OK)