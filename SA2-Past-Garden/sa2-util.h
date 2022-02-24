#pragma once

struct __declspec(align(4)) WaypointThing__
{
	int x;
	int z;
};

struct WaypointThing_
{
	int index;
	WaypointThing__ array[512]; //sa2 size different to sadx
};


VoidFunc(Chao_OOBLimit, 0x52B200);
DataPointer(float, flt_1657CEC, 0x1657CEC);
DataArray(NJS_VECTOR, TreePos, 0x1366B30, 30);
ObjectFunc(C_CLIMB, 0x55AB20); //used to make Chao Climb!
DataArray(WaypointThing_, WaypointThings, 0x1A15938, 16);

//void __usercall PlaySound4(EntityData1 *a1@<ebx>, int a2@<edi>, NJS_VECTOR *pos@<esi>, char a4, char a5, __int16 a6)
static const void* const Play3DSound2_EntityAndPosPtr = (void*)0x437590;
static inline char Play3DSound_EntityAndPos2(EntityData1* a1, int id, NJS_VECTOR* pos, char a4, char a5, __int16 a6)
{
	__asm
	{
		push [a6]
		push [a5]
		push [a4]
		mov esi, [pos]
		mov edi, [id]
		mov ebx, [a1]
		call Play3DSound2_EntityAndPosPtr
		add esp, 16
	}
}


VoidFunc(LoadSplashTextures, 0x6EDE40);

#pragma pack(push, 1)
struct ChaoData2
{
	char gap0[4];
	float float4;
	char gap8[212];
	float WaterHeight;
};
#pragma pack(pop)

FunctionPointer(void, GetCharacterSurfaceInfo, (NJS_VECTOR* position, CharSurfaceInfo* surface), 0x494DF0);

//void __usercall RunChaoBehaviour@<eax>(ObjectMaster* obj@<eax>, void* func, int idk)
static const void* const RunChaoBehaviourPtr = (void*)0x53D890;
static inline void RunChaoBehaviour(ObjectMaster* obj, void* func, int idk)
{
	__asm
	{
		push[idk]
		push[func]
		mov eax, [obj]
		call RunChaoBehaviourPtr
		add esp, 8
	}
}

VoidFunc(ALCam_CameraManager, 0x57D4C0);
VoidFunc(AL_MinimalCreate, 0x549490);
VoidFunc(ALW_Control, 0x530B80);
VoidFunc(Load_ChaoTree, 0x52EB70);
FunctionPointer(ObjectMaster*, sub_531E10, (), 0x531E10);
VoidFunc(sub_52F2A0, 0x52F2A0);
FunctionPointer(void, LoadWaterManager, (ObjectMaster* waterPtr), 0x56B040);


DataPointer(float, flt_1DD94A0, 0x1DD94A0);
DataPointer(float, flt_1DD94A4, 0x1DD94A4);
DataPointer(float, flt_1DD94A8, 0x1DD94A8);
DataPointer(float, flt_C5D234, 0xC5D234);
DataPointer(float, flt_9EBA14, 0x9EBA14);
DataPointer(char, byte_1DE4660, 0x1DE4660);
DataPointer(char, byte_1DE4400, 0x1DE4400);
DataArray(char, byte_1DE4664, 0x1DE4664, 2);
DataArray(char, byte_1DE4665, 0x1DE4665, 27);
DataArray(float, aFffSs, 0x01313CE8, 8);
DataArray(float, unk_1313D08, 0x1313D08, 9);

//void __usercall LoadChaoTexlist(unsigned __int16 a1@<ax>, char* a2@<ebx>, NJS_TEXLIST* texlist)
static const void* const LoadChaoTexlistPtr = (void*)0x530280;
static inline void LoadChaoTexlist(unsigned __int16 a1, char* a2, NJS_TEXLIST* texlist)
{
	__asm
	{

		push[texlist]
		mov ebx, [a2]
		mov ax, [a1]
		call LoadChaoTexlistPtr
		add esp, 4
	}
}

FunctionPointer(ObjectMaster*, DrawSubtitles, (int a1, const char* message, int displayTime, int language), 0x6B6E20);
VoidFunc(CollisionLoop, 0x486190);

static const void* const FreeColPtr = (void*)0x486430;
static inline void FreeEntityCollision(ObjectMaster* a1)
{
	__asm
	{
		mov ebx, [a1]
		call FreeColPtr
	}
}

FunctionPointer(void, njReleaseTexture, (NJS_TEXLIST* a1), 0x77F9F0);
DataPointer(ObjectMaster*, PointerToNormalCamera, 0x01AED2E8);