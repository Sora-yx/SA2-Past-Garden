#pragma once

VoidFunc(Chao_OOBLimit, 0x52B200);
DataPointer(float, flt_1657CEC, 0x1657CEC);
DataArray(NJS_VECTOR, TreePos, 0x1366B30, 30);

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