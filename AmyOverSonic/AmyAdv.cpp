#include "pch.h"
#include "satextlist.h"

#define ReplacePVM(a, b) helperFunctions.ReplaceFile("system\\" a ".PVM", "system\\" b ".PVM");
NJS_MATERIAL* TemporaryMaterialArray[] = { nullptr };

void Tornado_init(const HelperFunctions& helperFunctions);

DataPointer(NJS_OBJECT, SonicPointingHand_Object, 0x2DD8708);

//Light Dash Aura for DX, colors in Alpha, Red, Green, Blue
void __cdecl SetLSDColor()
{
	SetMaterialAndSpriteColor_Float(0.8f, 0.7f, 0.1f, 0.65f);
}

//Light Dash Aura for DC conversion
void __cdecl Sonic_DisplayLightDashModel_mod(EntityData1* data1, EntityData2* data2_pp, CharObj2* data2)
{
	int v3; // eax
	__int16 v4; // t1
	double v5; // st7
	float v6; // ST28_4
	double v7; // st7
	NJS_ACTION v8; // [esp+4h] [ebp-18h]
	NJS_ARGB a1; // [esp+Ch] [ebp-10h]

	if (!MissedFrames)
	{
		v3 = (unsigned __int16)data2->AnimationThing.Index;
		v8.object = SONIC_OBJECTS[54];
		if (data2->AnimationThing.State == 2)
		{
			v4 = data2->AnimationThing.LastIndex;
			v8.motion = data2->AnimationThing.action->motion;
		}
		else
		{
			v8.motion = data2->AnimationThing.AnimData[v3].Animation->motion;
		}
		v5 = (double)(LevelFrameCount & 0x7F);
		if (v5 >= 64.0)
		{
			v5 = 128.0 - v5;
		}
		v6 = v5 * 0.015625;
		njPushMatrixEx();
		njControl3D(NJD_CONTROL_3D_CONSTANT_MATERIAL | NJD_CONTROL_3D_ENABLE_ALPHA | NJD_CONTROL_3D_CONSTANT_ATTR);
		njColorBlendingMode(0, NJD_COLOR_BLENDING_SRCALPHA);
		njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_ONE);
		v7 = v6 * 0.1;
		a1.r = 0.7;
		a1.a = 0.8;
		a1.g = 0.1 - v7;
		a1.b = 0.65 - v7;
		SetMaterialAndSpriteColor(&a1);
		njAction_Queue_407FC0(&v8, data2->AnimationThing.Frame, 0);
		njScale(0, 1.05, 1.05, 1.05);
		njAction_Queue_407FC0(&v8, data2->AnimationThing.Frame, 0);
		njScale(0, 1.05, 1.05, 1.05);
		njAction_Queue_407FC0(&v8, data2->AnimationThing.Frame, 0);
		njColorBlendingMode(0, NJD_COLOR_BLENDING_SRCALPHA);
		njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_INVSRCALPHA);
		njPopMatrixEx();
	}
}

void PointingFinger_Init(const HelperFunctions& helperFunctions) {
	ModelInfo* mdl = new ModelInfo(helperFunctions.GetReplaceablePath("system\\AMY_POINTINGHAND.sa1mdl"));

	if (mdl->getformat() == ModelFormat_Basic) {
		SonicPointingHand_Object = *mdl->getmodel();
	}
}

static ModelInfo* AmyPlaneMdl = nullptr;

DataPointer(NJS_OBJECT, Tornado1_Object, 0x28B1DA0);
DataPointer(NJS_OBJECT, Tornado2Before_Object, 0x27EB198);
DataPointer(NJS_OBJECT, Tornado2Change_Object, 0x280C158);

bool CheckIfSonicPlane(NJS_MODEL_SADX* model) {
	return AmyPlaneMdl->getlabel(model) != "";
}

void __cdecl TornadoCallBack(NJS_MODEL_SADX* model, int blend, int idk) {
	NonStaticFunctionPointer(void, sub_407FC0, (NJS_MODEL_SADX*, int), 0x407FC0);

	// If the currently drawn model is part of AMY_PLANE, we use the character's texlist instead
	if (CheckIfSonicPlane(model)) {
		NJS_TEXLIST* tex_orig = CurrentTexList;

		njSetTexture(&SONIC_TEXLIST);
		sub_407FC0(model, blend);
		njSetTexture(tex_orig);
	}
	else {
		sub_407FC0(model, blend);
	}
}

void njAction_Queue_Tornado(NJS_ACTION* action, float frame, QueuedModelFlagsB flags) {
	DisplayAnimationFrame(action, frame, flags, 0.0, TornadoCallBack);
}

void Tornado_init(const HelperFunctions& helperFunctions) {
	ModelInfo* mdl = new ModelInfo(helperFunctions.GetReplaceablePath("system\\AMY_PLANE.sa1mdl"));

	if (mdl->getformat() == ModelFormat_Basic) {
		AmyPlaneMdl = mdl;

		// We only replace Sonic's part of the plane
		Tornado1_Object.child->sibling->sibling->sibling->child = AmyPlaneMdl->getmodel();
		Tornado2Before_Object.child->sibling->sibling->child = AmyPlaneMdl->getmodel();
		Tornado2Change_Object.child->sibling->sibling->sibling->sibling->child = AmyPlaneMdl->getmodel();

		// Hack to use our own texture for Amy's part of the plane, allowing compatibility with dc mods.
		WriteCall((void*)0x62753A, njAction_Queue_Tornado);
	}
	else {
		PrintDebug("[Amy over Sonic] Cannot open \"AMY_PLANE.sa1mdl\".\n");
	}
}

uint16_t Amy_UpperArmIndices_DC[] = {
	0, 2,
	4, 6,
	5, 7,
	1, 3,
};

uint16_t Amy_LowerArmIndices_DC[] = {
	4, 14,
	5, 15,
	1, 11,
	0, 10,
};

uint16_t Amy_KneeIndices_DC[] = {
	0, 2,
	4, 6,
	5, 7,
	1, 3,
};

uint16_t Amy_LegIndices_DC[] = {
	0, 2,
	1, 3,
	4, 6,
	5, 7,
};

uint16_t Amy_ShoeIndices_DC[] = {
	2, 3,
	3, 2,
	0, 1,
	1, 0,
};

uint16_t Amy_HandIndices_DC[] = {
	0, 13,
	1, 1,
	5, 3,
	4, 12,
};

void InitSonicWeldInfo_DC();
Trampoline InitSonicWeldInfo_t((int)InitSonicWeldInfo, (int)InitSonicWeldInfo + 0x5, InitSonicWeldInfo_DC);
void InitSonicWeldInfo_DC() {
	VoidFunc(original, InitSonicWeldInfo_t.Target());
	original(); // For Metal Sonic compatibility with other mods

	// Hand fix (The Mod Loader's chrmodels replacement system does that wrong)
	SONIC_OBJECTS[4] = SONIC_OBJECTS[0]->child->child->sibling->sibling->sibling->sibling->sibling->sibling->child->child->child->child->sibling->sibling;
	SONIC_OBJECTS[5] = SONIC_OBJECTS[0]->child->child->sibling->sibling->sibling->sibling->sibling->sibling->child->child->child->child->sibling->sibling->child->sibling;
	SONIC_OBJECTS[63]->sibling = SONIC_OBJECTS[4];

	// Shoe fix
	SONIC_OBJECTS[15] = SONIC_OBJECTS[0]->child->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->child->child->child->child->sibling->sibling;
	SONIC_OBJECTS[16] = SONIC_OBJECTS[0]->child->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->child->child->child->child->sibling->sibling->child;
	SONIC_OBJECTS[20] = SONIC_OBJECTS[0]->child->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->child->child->child->child->sibling->sibling;
	SONIC_OBJECTS[21] = SONIC_OBJECTS[0]->child->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->child->child->child->child->sibling->sibling->child;

	// Sonic

	SonicWeldInfo[0].BaseModel = SONIC_OBJECTS[0];
	SonicWeldInfo[0].ModelA = SONIC_OBJECTS[1];
	SonicWeldInfo[0].ModelB = SONIC_OBJECTS[2];
	SonicWeldInfo[0].anonymous_5 = 0;
	SonicWeldInfo[0].VertexBuffer = 0;
	SonicWeldInfo[0].VertexPairCount = (uint8_t)(LengthOfArray(Amy_UpperArmIndices_DC) / 2);
	SonicWeldInfo[0].WeldType = 2;
	SonicWeldInfo[0].VertIndexes = Amy_UpperArmIndices_DC;

	SonicWeldInfo[1].BaseModel = SONIC_OBJECTS[0];
	SonicWeldInfo[1].ModelA = SONIC_OBJECTS[2];
	SonicWeldInfo[1].ModelB = SONIC_OBJECTS[3];
	SonicWeldInfo[1].VertexPairCount = (uint8_t)(LengthOfArray(Amy_LowerArmIndices_DC) / 2);
	SonicWeldInfo[1].WeldType = 2;
	SonicWeldInfo[1].anonymous_5 = 0;
	SonicWeldInfo[1].VertexBuffer = 0;
	SonicWeldInfo[1].VertIndexes = Amy_LowerArmIndices_DC;

	SonicWeldInfo[2].BaseModel = SONIC_OBJECTS[0];
	SonicWeldInfo[2].ModelA = SONIC_OBJECTS[7];
	SonicWeldInfo[2].ModelB = SONIC_OBJECTS[8];
	SonicWeldInfo[2].VertexPairCount = (uint8_t)(LengthOfArray(Amy_UpperArmIndices_DC) / 2);
	SonicWeldInfo[2].WeldType = 2;
	SonicWeldInfo[2].anonymous_5 = 0;
	SonicWeldInfo[2].VertexBuffer = 0;
	SonicWeldInfo[2].VertIndexes = Amy_UpperArmIndices_DC;

	SonicWeldInfo[3].BaseModel = SONIC_OBJECTS[0];
	SonicWeldInfo[3].ModelA = SONIC_OBJECTS[8];
	SonicWeldInfo[3].ModelB = SONIC_OBJECTS[9];
	SonicWeldInfo[3].VertIndexes = Amy_LowerArmIndices_DC;
	SonicWeldInfo[3].VertexPairCount = (uint8_t)(LengthOfArray(Amy_LowerArmIndices_DC) / 2);
	SonicWeldInfo[3].WeldType = 2;
	SonicWeldInfo[3].anonymous_5 = 0;
	SonicWeldInfo[3].VertexBuffer = 0;

	SonicWeldInfo[4].BaseModel = SONIC_OBJECTS[0];
	SonicWeldInfo[4].ModelA = SONIC_OBJECTS[12];
	SonicWeldInfo[4].ModelB = SONIC_OBJECTS[13];
	SonicWeldInfo[4].VertexPairCount = (uint8_t)(LengthOfArray(Amy_KneeIndices_DC) / 2);
	SonicWeldInfo[4].WeldType = 2;
	SonicWeldInfo[4].anonymous_5 = 0;
	SonicWeldInfo[4].VertexBuffer = 0;
	SonicWeldInfo[4].VertIndexes = Amy_KneeIndices_DC;

	SonicWeldInfo[5].BaseModel = SONIC_OBJECTS[0];
	SonicWeldInfo[5].ModelA = SONIC_OBJECTS[13];
	SonicWeldInfo[5].ModelB = SONIC_OBJECTS[14];
	SonicWeldInfo[5].VertexPairCount = (uint8_t)(LengthOfArray(Amy_LegIndices_DC) / 2);
	SonicWeldInfo[5].WeldType = 2;
	SonicWeldInfo[5].anonymous_5 = 0;
	SonicWeldInfo[5].VertexBuffer = 0;
	SonicWeldInfo[5].VertIndexes = Amy_LegIndices_DC;

	SonicWeldInfo[6].BaseModel = SONIC_OBJECTS[0];
	SonicWeldInfo[6].ModelA = SONIC_OBJECTS[17];
	SonicWeldInfo[6].VertIndexes = Amy_KneeIndices_DC;
	SonicWeldInfo[6].ModelB = SONIC_OBJECTS[18];
	SonicWeldInfo[6].VertexPairCount = (uint8_t)(LengthOfArray(Amy_KneeIndices_DC) / 2);
	SonicWeldInfo[6].WeldType = 2;
	SonicWeldInfo[6].anonymous_5 = 0;
	SonicWeldInfo[6].VertexBuffer = 0;

	SonicWeldInfo[7].BaseModel = SONIC_OBJECTS[0];
	SonicWeldInfo[7].ModelA = SONIC_OBJECTS[18];
	SonicWeldInfo[7].ModelB = SONIC_OBJECTS[19];
	SonicWeldInfo[7].VertexPairCount = (uint8_t)(LengthOfArray(Amy_LegIndices_DC) / 2);
	SonicWeldInfo[7].WeldType = 2;
	SonicWeldInfo[7].anonymous_5 = 0;
	SonicWeldInfo[7].VertexBuffer = 0;
	SonicWeldInfo[7].VertIndexes = Amy_LegIndices_DC;

	SonicWeldInfo[8].BaseModel = SONIC_OBJECTS[0];
	SonicWeldInfo[8].ModelA = SONIC_OBJECTS[15];
	SonicWeldInfo[8].ModelB = SONIC_OBJECTS[16];
	SonicWeldInfo[8].VertexPairCount = (uint8_t)(LengthOfArray(Amy_ShoeIndices_DC) / 2);
	SonicWeldInfo[8].WeldType = 2;
	SonicWeldInfo[8].anonymous_5 = 0;
	SonicWeldInfo[8].VertexBuffer = 0;
	SonicWeldInfo[8].VertIndexes = Amy_ShoeIndices_DC;

	SonicWeldInfo[9].BaseModel = SONIC_OBJECTS[0];
	SonicWeldInfo[9].ModelA = SONIC_OBJECTS[20];
	SonicWeldInfo[9].VertIndexes = Amy_ShoeIndices_DC;
	SonicWeldInfo[9].ModelB = SONIC_OBJECTS[21];
	SonicWeldInfo[9].VertexPairCount = (uint8_t)(LengthOfArray(Amy_ShoeIndices_DC) / 2);
	SonicWeldInfo[9].WeldType = 2;
	SonicWeldInfo[9].anonymous_5 = 0;
	SonicWeldInfo[9].VertexBuffer = 0;

	SonicWeldInfo[10].BaseModel = SONIC_OBJECTS[0];
	SonicWeldInfo[10].ModelA = SONIC_OBJECTS[10];
	SonicWeldInfo[10].ModelB = SONIC_OBJECTS[11];
	SonicWeldInfo[10].VertexPairCount = (uint8_t)(LengthOfArray(Amy_HandIndices_DC) / 2);
	SonicWeldInfo[10].WeldType = 2;
	SonicWeldInfo[10].anonymous_5 = 0;
	SonicWeldInfo[10].VertexBuffer = 0;
	SonicWeldInfo[10].VertIndexes = Amy_HandIndices_DC;

	SonicWeldInfo[11].BaseModel = SONIC_OBJECTS[0];
	SonicWeldInfo[11].ModelA = SONIC_OBJECTS[4];
	SonicWeldInfo[11].ModelB = SONIC_OBJECTS[5];
	SonicWeldInfo[11].VertexPairCount = (uint8_t)(LengthOfArray(Amy_HandIndices_DC) / 2);
	SonicWeldInfo[11].WeldType = 2;
	SonicWeldInfo[11].anonymous_5 = 0;
	SonicWeldInfo[11].VertexBuffer = 0;
	SonicWeldInfo[11].VertIndexes = Amy_HandIndices_DC;

	SonicWeldInfo[12].BaseModel = SONIC_OBJECTS[0];
	SonicWeldInfo[12].ModelA = SONIC_OBJECTS[58];
	SonicWeldInfo[12].ModelB = SONIC_OBJECTS[59];
	SonicWeldInfo[12].VertexPairCount = (uint8_t)(LengthOfArray(Amy_ShoeIndices_DC) / 2);
	SonicWeldInfo[12].WeldType = 2;
	SonicWeldInfo[12].anonymous_5 = 0;
	SonicWeldInfo[12].VertexBuffer = 0;
	SonicWeldInfo[12].VertIndexes = Amy_ShoeIndices_DC;

	SonicWeldInfo[13].BaseModel = SONIC_OBJECTS[0];
	SonicWeldInfo[13].ModelA = SONIC_OBJECTS[60];
	SonicWeldInfo[13].ModelB = SONIC_OBJECTS[61];
	SonicWeldInfo[13].VertexPairCount = (uint8_t)(LengthOfArray(Amy_ShoeIndices_DC) / 2);
	SonicWeldInfo[13].WeldType = 2;
	SonicWeldInfo[13].anonymous_5 = 0;
	SonicWeldInfo[13].VertexBuffer = 0;
	SonicWeldInfo[13].VertIndexes = Amy_ShoeIndices_DC;

	SonicWeldInfo[14].BaseModel = SONIC_OBJECTS[0];
	SonicWeldInfo[14].ModelA = SONIC_OBJECTS[2];
	SonicWeldInfo[14].ModelB = SONIC_OBJECTS[63];
	SonicWeldInfo[14].VertexPairCount = (uint8_t)(LengthOfArray(Amy_LowerArmIndices_DC) / 2);
	SonicWeldInfo[14].WeldType = 2;
	SonicWeldInfo[14].anonymous_5 = 0;
	SonicWeldInfo[14].VertexBuffer = 0;
	SonicWeldInfo[14].VertIndexes = Amy_LowerArmIndices_DC;

	// Super Sonic

	SonicWeldInfo[22].BaseModel = SONIC_OBJECTS[22];
	SonicWeldInfo[22].ModelA = SONIC_OBJECTS[23];
	SonicWeldInfo[22].ModelB = SONIC_OBJECTS[24];
	SonicWeldInfo[22].VertexPairCount = (uint8_t)(LengthOfArray(Amy_UpperArmIndices_DC) / 2);
	SonicWeldInfo[22].WeldType = 2;
	SonicWeldInfo[22].anonymous_5 = 0;
	SonicWeldInfo[22].VertexBuffer = 0;
	SonicWeldInfo[22].VertIndexes = Amy_UpperArmIndices_DC;

	SonicWeldInfo[23].BaseModel = SONIC_OBJECTS[22];
	SonicWeldInfo[23].ModelA = SONIC_OBJECTS[24];
	SonicWeldInfo[23].ModelB = SONIC_OBJECTS[25];
	SonicWeldInfo[23].VertexPairCount = (uint8_t)(LengthOfArray(Amy_LowerArmIndices_DC) / 2);
	SonicWeldInfo[23].WeldType = 2;
	SonicWeldInfo[23].anonymous_5 = 0;
	SonicWeldInfo[23].VertexBuffer = 0;
	SonicWeldInfo[23].VertIndexes = Amy_LowerArmIndices_DC;

	SonicWeldInfo[24].BaseModel = SONIC_OBJECTS[22];
	SonicWeldInfo[24].ModelA = SONIC_OBJECTS[28];
	SonicWeldInfo[24].ModelB = SONIC_OBJECTS[29];
	SonicWeldInfo[24].VertexPairCount = (uint8_t)(LengthOfArray(Amy_UpperArmIndices_DC) / 2);
	SonicWeldInfo[24].WeldType = 2;
	SonicWeldInfo[24].anonymous_5 = 0;
	SonicWeldInfo[24].VertexBuffer = 0;
	SonicWeldInfo[24].VertIndexes = Amy_UpperArmIndices_DC;

	SonicWeldInfo[25].BaseModel = SONIC_OBJECTS[22];
	SonicWeldInfo[25].ModelA = SONIC_OBJECTS[29];
	SonicWeldInfo[25].ModelB = SONIC_OBJECTS[30];
	SonicWeldInfo[25].VertexPairCount = (uint8_t)(LengthOfArray(Amy_LowerArmIndices_DC) / 2);
	SonicWeldInfo[25].WeldType = 2;
	SonicWeldInfo[25].anonymous_5 = 0;
	SonicWeldInfo[25].VertexBuffer = 0;
	SonicWeldInfo[25].VertIndexes = Amy_LowerArmIndices_DC;

	SonicWeldInfo[26].BaseModel = SONIC_OBJECTS[22];
	SonicWeldInfo[26].ModelA = SONIC_OBJECTS[33];
	SonicWeldInfo[26].ModelB = SONIC_OBJECTS[34];
	SonicWeldInfo[26].VertexPairCount = (uint8_t)(LengthOfArray(Amy_KneeIndices_DC) / 2);
	SonicWeldInfo[26].WeldType = 2;
	SonicWeldInfo[26].anonymous_5 = 0;
	SonicWeldInfo[26].VertexBuffer = 0;
	SonicWeldInfo[26].VertIndexes = Amy_KneeIndices_DC;

	SonicWeldInfo[27].BaseModel = SONIC_OBJECTS[22];
	SonicWeldInfo[27].ModelA = SONIC_OBJECTS[34];
	SonicWeldInfo[27].ModelB = SONIC_OBJECTS[35];
	SonicWeldInfo[27].VertexPairCount = (uint8_t)(LengthOfArray(Amy_LegIndices_DC) / 2);
	SonicWeldInfo[27].WeldType = 2;
	SonicWeldInfo[27].anonymous_5 = 0;
	SonicWeldInfo[27].VertexBuffer = 0;
	SonicWeldInfo[27].VertIndexes = Amy_LegIndices_DC;

	SonicWeldInfo[28].BaseModel = SONIC_OBJECTS[22];
	SonicWeldInfo[28].ModelA = SONIC_OBJECTS[38];
	SonicWeldInfo[28].VertIndexes = Amy_KneeIndices_DC;
	SonicWeldInfo[28].ModelB = SONIC_OBJECTS[39];
	SonicWeldInfo[28].VertexPairCount = (uint8_t)(LengthOfArray(Amy_KneeIndices_DC) / 2);
	SonicWeldInfo[28].WeldType = 2;
	SonicWeldInfo[28].anonymous_5 = 0;
	SonicWeldInfo[28].VertexBuffer = 0;

	SonicWeldInfo[29].BaseModel = SONIC_OBJECTS[22];
	SonicWeldInfo[29].ModelA = SONIC_OBJECTS[39];
	SonicWeldInfo[29].ModelB = SONIC_OBJECTS[40];
	SonicWeldInfo[29].VertexPairCount = (uint8_t)(LengthOfArray(Amy_LegIndices_DC) / 2);
	SonicWeldInfo[29].WeldType = 2;
	SonicWeldInfo[29].anonymous_5 = 0;
	SonicWeldInfo[29].VertexBuffer = 0;
	SonicWeldInfo[29].VertIndexes = Amy_LegIndices_DC;

	SonicWeldInfo[30].BaseModel = SONIC_OBJECTS[22];
	SonicWeldInfo[30].ModelA = SONIC_OBJECTS[36];
	SonicWeldInfo[30].ModelB = SONIC_OBJECTS[37];
	SonicWeldInfo[30].VertexPairCount = (uint8_t)(LengthOfArray(Amy_ShoeIndices_DC) / 2);
	SonicWeldInfo[30].WeldType = 2;
	SonicWeldInfo[30].anonymous_5 = 0;
	SonicWeldInfo[30].VertexBuffer = 0;
	SonicWeldInfo[30].VertIndexes = Amy_ShoeIndices_DC;

	SonicWeldInfo[31].BaseModel = SONIC_OBJECTS[22];
	SonicWeldInfo[31].ModelA = SONIC_OBJECTS[41];
	SonicWeldInfo[31].ModelB = SONIC_OBJECTS[42];
	SonicWeldInfo[31].VertexPairCount = (uint8_t)(LengthOfArray(Amy_ShoeIndices_DC) / 2);
	SonicWeldInfo[31].WeldType = 2;
	SonicWeldInfo[31].anonymous_5 = 0;
	SonicWeldInfo[31].VertexBuffer = 0;
	SonicWeldInfo[31].VertIndexes = Amy_ShoeIndices_DC;

	SonicWeldInfo[32].BaseModel = SONIC_OBJECTS[22];
	SonicWeldInfo[32].ModelA = SONIC_OBJECTS[31];
	SonicWeldInfo[32].ModelB = SONIC_OBJECTS[32];
	SonicWeldInfo[32].VertexPairCount = (uint8_t)(LengthOfArray(Amy_HandIndices_DC) / 2);
	SonicWeldInfo[32].WeldType = 2;
	SonicWeldInfo[32].anonymous_5 = 0;
	SonicWeldInfo[32].VertexBuffer = 0;
	SonicWeldInfo[32].VertIndexes = Amy_HandIndices_DC;

	SonicWeldInfo[33].BaseModel = SONIC_OBJECTS[22];
	SonicWeldInfo[33].ModelA = SONIC_OBJECTS[26];
	SonicWeldInfo[33].anonymous_5 = 0;
	SonicWeldInfo[33].VertexBuffer = 0;
	SonicWeldInfo[33].VertIndexes = Amy_HandIndices_DC;
	SonicWeldInfo[33].ModelB = SONIC_OBJECTS[27];
	SonicWeldInfo[33].VertexPairCount = (uint8_t)(LengthOfArray(Amy_HandIndices_DC) / 2);
	SonicWeldInfo[33].WeldType = 2;

	SonicWeldInfo[34].BaseModel = SONIC_OBJECTS[22];
	SonicWeldInfo[34].ModelA = SONIC_OBJECTS[31];
	SonicWeldInfo[34].ModelB = SONIC_OBJECTS[32];
	SonicWeldInfo[34].anonymous_5 = 0;
	SonicWeldInfo[34].VertexBuffer = 0;
	SonicWeldInfo[34].VertexPairCount = (uint8_t)(LengthOfArray(Amy_HandIndices_DC) / 2);
	SonicWeldInfo[34].VertIndexes = Amy_HandIndices_DC;
	SonicWeldInfo[34].WeldType = 2;

	SonicWeldInfo[35].BaseModel = SONIC_OBJECTS[22];
	SonicWeldInfo[35].ModelA = SONIC_OBJECTS[26];
	SonicWeldInfo[35].anonymous_5 = 0;
	SonicWeldInfo[35].VertexBuffer = 0;
	SonicWeldInfo[35].VertIndexes = Amy_HandIndices_DC;
	SonicWeldInfo[35].VertexPairCount = (uint8_t)(LengthOfArray(Amy_HandIndices_DC) / 2);
	SonicWeldInfo[35].ModelB = SONIC_OBJECTS[27];
	SonicWeldInfo[35].WeldType = 2;

	SonicWeldInfo[36].BaseModel = 0;
	SonicWeldInfo[36].ModelA = 0;
	SonicWeldInfo[36].ModelB = 0;
	SonicWeldInfo[36].VertexPairCount = 0;
	SonicWeldInfo[36].VertexBuffer = 0;
	SonicWeldInfo[36].VertIndexes = 0;
}

void Init_Sonic()
{
	HMODULE hm = GetModuleHandle(L"CHRMODELS_orig");
	NJS_OBJECT** SONIC_OBJECTS = (NJS_OBJECT**)GetProcAddress(hm, "___SONIC_OBJECTS");
}

extern "C"
{
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		HMODULE Hyper = GetModuleHandle(L"sadx-hyper-sonic");
		Init_Sonic();
		// Replace the light speed dash aura color
		WriteCall((void*)0x4A1705, SetLSDColor);
		WriteJump((void*)0x4A1630, Sonic_DisplayLightDashModel_mod);

		// Replace the pointing finger model that's used in cutscenes
		PointingFinger_Init(helperFunctions);

		// Replace Sonic on the Tornado
		Tornado_init(helperFunctions);

		//To make Super form work
		WriteData((NJS_TEXLIST**)0x55E65C, SSAura01);
		WriteData((NJS_TEXLIST**)0x55E751, SSAura01);
		WriteData((NJS_TEXLIST**)0x55E712, SSAura02);
		WriteData((NJS_TEXLIST**)0x55E7CD, SSWaterThing);
		WriteData((NJS_TEXLIST**)0x55F2B3, SSHomingTex1);
		WriteData((NJS_TEXLIST**)0x55F1D1, SSHomingTex1);
		WriteData((NJS_TEXLIST**)0x55F1DC, SSHomingTex2);
		WriteData((NJS_TEXLIST**)0x55F2BE, SSHomingTex2);
		WriteData((NJS_TEXLIST**)0x55F677, SSHomingTex2);
		WriteData((NJS_TEXLIST**)0x55F669, SSHomingTex3);
		SUPERSONIC_TEXLIST = SS_PVM;

		//Replace textures
		ReplacePVM("sonic", "AmySpeed");
		ReplacePVM("supersonic", "SuperAmy");
		ReplacePVM("hypersonic", "HyperAmy");
	}
	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}