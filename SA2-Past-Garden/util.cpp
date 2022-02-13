#include "pch.h"

const char* ModelFormatStrings[]
{
	"collision",
	"chunk",
	"battle"
};

//Load Object File
ModelInfo* LoadMDL(const char* name, ModelFormat format) {
	std::string fullPath;

	if (format == ModelFormat_Chunk)
	{
		fullPath = "resource\\gd_PC\\Models\\";
	}


	if (format == ModelFormat_Basic)
	{
		fullPath = "resource\\gd_PC\\Collisions\\";
	}

	fullPath += name;

	switch (format)
	{
	case ModelFormat_Basic:
		fullPath += ".sa1mdl";
		break;
	case ModelFormat_Chunk:
		fullPath += ".sa2mdl";
		break;
	case ModelFormat_SA2B:
		fullPath += ".sa2bmdl";
		break;
	}

	const char* foo = fullPath.c_str();

	ModelInfo* temp = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath(foo));

	if (temp->getformat() == format)
	{
		PrintDebug("[Past Garden] Loaded %s model: %s.", ModelFormatStrings[(int)format - 1], name);
	}
	else {
		PrintDebug("[Past Garden] Failed loading %s model: %s.", ModelFormatStrings[(int)format - 1], name);
	}

	return temp;
}

void LoadAnimation(AnimationFile** info, const char* name, const HelperFunctions& helperFunctions)
{
	std::string fullPath = "system\\anims\\";
	fullPath = fullPath + name + ".saanim";

	AnimationFile* anm = new AnimationFile(helperFunctions.GetReplaceablePath(fullPath.c_str()));

	if (anm->getmodelcount() == 0)
	{
		delete anm;
		*info = nullptr;
	}
	else {
		*info = anm;
	}
};

AnimationFile* LoadAnim(const char* name)
{
	std::string fullPath = "resource\\gd_PC\\Anim\\";

	fullPath = fullPath + name + ".saanim";

	AnimationFile* file = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath(fullPath.c_str()));

	if (file->getmotion() != nullptr) {
		PrintDebug("[SA2 Past Garden] Loaded animation: %s.", name);
	}
	else {
		PrintDebug("[Past Garden] Failed loading animation: %s.", name);
	}

	return file;
}

void FreeMDL(ModelInfo* pointer)
{
	if (pointer) delete(pointer);
}

void FreeAnim(AnimationFile* pointer)
{
	if (pointer) delete pointer;
}

void FreeLandTableObj()
{
	if (LandManagerPtr)
	{
		DeleteObject_(LandManagerPtr);
		LandManagerPtr = nullptr;
	}
}

void FreeLandTableInfo(LandTableInfo** info)
{
	if (*info)
	{
		delete* info;
	}
}

static void FixLand(LandTable* land)
{
	for (int i = 0; i < land->COLCount; ++i) {
		COL* col = &land->COLList[i];

		col->field_14 = 0;
		col->Chunks = 0;


		if (col->Flags == 0x2)
		{
			col->Flags = 0x40000002;
		}
		else if (col->Flags & SurfaceFlag_Visible)
		{
			col->Flags = SurfaceFlag_Visible;
		}
		else if (col->Flags & SurfaceFlag_Water)
		{
			col->Flags = SurfaceFlag_Water;
		}
		else
		{
			col->Flags = 0x1;
		}
	}
}

void LoadLandTable(const char* path, LandTableInfo** land, const TexPackInfo* tex)
{
	LandTableInfo* land_ = new LandTableInfo(HelperFunctionsGlobal.GetReplaceablePath(path));

	if (land_ != nullptr)
	{

		LandTable* geo = land_->getlandtable();

		//FixLand(geo);

		geo->TextureList = tex->TexList;
		geo->TextureName = (char*)tex->TexName;
		LoadLandManager(geo);
	}
	else {
		PrintDebug("LOAD CUSTOM GARDEN FAILED");
	}

	*land = land_;
}

extern NJS_TEXLIST PAST01Obj_TEXLIST;

void __cdecl ObjectGeneric_DisplayV(ObjectMaster* obj)
{

	if (obj->field_4C == nullptr)
	{
		PrintDebug("Past Garden Mod: Error, an object couldn't draw properly.");
		return;
	}

	EntityData1* data = obj->Data1.Entity;

	njSetTexture(&PAST01Obj_TEXLIST);
	njPushMatrix(0);
	njTranslateV(0, &data->Position);

	njRotateY(0, (unsigned __int16)data->Rotation.y);

	DrawObject((NJS_OBJECT*)obj->field_4C);

	njPopMatrix(1u);
}