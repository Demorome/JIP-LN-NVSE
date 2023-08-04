#pragma once

DEFINE_COMMAND_PLUGIN(GetWorldspaceFlag, 0, 2, kParams_OneWorldspace_OneInt);
DEFINE_COMMAND_PLUGIN(SetWorldspaceFlag, 0, 3, kParams_OneWorldspace_TwoInts);
DEFINE_COMMAND_PLUGIN(GetWorldspaceClimate, 0, 1, kParams_OneWorldspace);
DEFINE_COMMAND_PLUGIN(SetWorldspaceClimate, 0, 2, kParams_OneWorldspace_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetWorldspaceNoiseTexture, 0, 1, kParams_OneWorldspace);
DEFINE_COMMAND_PLUGIN(SetWorldspaceNoiseTexture, 0, 2, kParams_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(GetWorldspaceWaterType, 0, 1, kParams_OneWorldspace);
DEFINE_COMMAND_PLUGIN(SetWorldspaceWaterType, 0, 2, kParams_OneWorldspace_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetWorldspaceImagespace, 0, 1, kParams_OneWorldspace);
DEFINE_COMMAND_PLUGIN(SetWorldspaceImagespace, 0, 2, kParams_OneWorldspace_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetWorldspacePersistentCell, 0, 1, kParams_OneWorldspace);

bool Cmd_GetWorldspaceFlag_Execute(COMMAND_ARGS)
{
	TESWorldSpace *wspc;
	UInt32 flagID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &wspc, &flagID) && (flagID <= 7))
		*result = (wspc->worldFlags & (1 << flagID)) ? 1 : 0;
	else *result = 0;
	return true;
}

bool Cmd_SetWorldspaceFlag_Execute(COMMAND_ARGS)
{
	TESWorldSpace *wspc;
	UInt32 flagID, val;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &wspc, &flagID, &val) && (flagID <= 7))
	{
		flagID = 1 << flagID;
		if (val) wspc->worldFlags |= flagID;
		else wspc->worldFlags &= ~flagID;
	}
	return true;
}

bool Cmd_GetWorldspaceClimate_Execute(COMMAND_ARGS)
{
	REFR_RES = 0;
	TESWorldSpace *wldSpc;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &wldSpc))
	{
		while (wldSpc->parent && (wldSpc->parentFlags & 0x10))
			wldSpc = wldSpc->parent;
		TESClimate *climate = wldSpc->climate;
		if (climate) REFR_RES = climate->refID;
		DoConsolePrint(climate);
	}
	return true;
}

bool Cmd_SetWorldspaceClimate_Execute(COMMAND_ARGS)
{
	TESWorldSpace *wldSpc;
	TESClimate *climate = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &wldSpc, &climate) && (!climate || IS_ID(climate, TESClimate)))
	{
		if (wldSpc->parent && (wldSpc->parentFlags & 16))
			wldSpc->parentFlags -= 16;
		wldSpc->climate = climate;
		if (Sky *currSky = Sky::Get(); currSky && g_thePlayer->parentCell)
			if (TESWorldSpace *pcWspc = g_thePlayer->parentCell->worldSpace)
			{
				while (pcWspc->parent && (pcWspc->parentFlags & 16))
					pcWspc = pcWspc->parent;
				if (pcWspc == wldSpc)
					currSky->currClimate = climate;
			}
	}
	return true;
}

bool Cmd_GetWorldspaceNoiseTexture_Execute(COMMAND_ARGS)
{
	const char *resStr;
	TESWorldSpace *wspc;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &wspc))
		resStr = wspc->waterNoiseTexture.ddsPath.m_data;
	else resStr = NULL;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetWorldspaceNoiseTexture_Execute(COMMAND_ARGS)
{
	TESWorldSpace *wspc;
	char path[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &wspc, &path) && IS_ID(wspc, TESWorldSpace))
		wspc->waterNoiseTexture.ddsPath.Set(path);
	return true;
}

bool Cmd_GetWorldspaceWaterType_Execute(COMMAND_ARGS)
{
	REFR_RES = 0;
	TESWorldSpace *wldSpc;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &wldSpc) && wldSpc->waterFormFirst)
		REFR_RES = wldSpc->waterFormFirst->refID;
	return true;
}

bool Cmd_SetWorldspaceWaterType_Execute(COMMAND_ARGS)
{
	TESWorldSpace *wldSpc;
	TESWaterForm *water = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &wldSpc, &water) && (!water || IS_ID(water, TESWaterForm)))
		wldSpc->waterFormFirst = water;
	return true;
}

bool Cmd_GetWorldspaceImagespace_Execute(COMMAND_ARGS)
{
	REFR_RES = 0;
	TESWorldSpace *wldSpc;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &wldSpc) && wldSpc->imageSpace)
		REFR_RES = wldSpc->imageSpace->refID;
	return true;
}

bool Cmd_SetWorldspaceImagespace_Execute(COMMAND_ARGS)
{
	TESWorldSpace *wldSpc;
	TESImageSpace *imgSpc = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &wldSpc, &imgSpc) && (!imgSpc || IS_ID(imgSpc, TESImageSpace)))
		wldSpc->imageSpace = imgSpc;
	return true;
}

bool Cmd_GetWorldspacePersistentCell_Execute(COMMAND_ARGS)
{
	REFR_RES = 0;
	TESWorldSpace *wldSpc;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &wldSpc) && wldSpc->cell)
		REFR_RES = wldSpc->cell->refID;
	return true;
}