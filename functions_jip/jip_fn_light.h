#pragma once

DEFINE_COMMAND_PLUGIN(GetLightTraitNumeric, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetLightTraitNumeric, , 0, 3, kParams_JIP_OneForm_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetLightFlag, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetLightFlag, , 0, 3, kParams_JIP_OneForm_TwoInts);

bool Cmd_GetLightTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectLIGH *light;
	UInt32 traitID;
	if (!ExtractArgs(EXTRACT_ARGS, &light, &traitID) || NOT_TYPE(light, TESObjectLIGH)) return true;
	switch (traitID)
	{
	case 0:
		*result = (int)light->radius;
		break;
	case 1:
		*result = light->red;
		break;
	case 2:
		*result = light->green;
		break;
	case 3:
		*result = light->blue;
		break;
	case 4:
		*result = light->falloffExp;
		break;
	case 5:
		*result = light->FOV;
		break;
	case 6:
		*result = light->fadeValue;
	}
	return true;
}

bool Cmd_SetLightTraitNumeric_Execute(COMMAND_ARGS)
{
	TESObjectLIGH *light;
	UInt32 traitID;
	float val;
	if (!ExtractArgs(EXTRACT_ARGS, &light, &traitID, &val) || NOT_TYPE(light, TESObjectLIGH)) return true;
	UInt32 intVal = (int)val;
	switch (traitID)
	{
	case 0:
		light->radius = intVal;
		break;
	case 1:
		light->red = intVal;
		break;
	case 2:
		light->green = intVal;
		break;
	case 3:
		light->blue = intVal;
		break;
	case 4:
		light->falloffExp = val;
		break;
	case 5:
		light->FOV = val;
		break;
	case 6:
		light->fadeValue = val;
	}
	return true;
}

bool Cmd_GetLightFlag_Execute(COMMAND_ARGS)
{
	TESObjectLIGH *light;
	UInt32 flagID;
	if (ExtractArgs(EXTRACT_ARGS, &light, &flagID) && IS_TYPE(light, TESObjectLIGH) && (flagID <= 10))
		*result = (light->lightFlags & (1 << flagID)) ? 1 : 0;
	else *result = 0;
	return true;
}

bool Cmd_SetLightFlag_Execute(COMMAND_ARGS)
{
	TESObjectLIGH *light;
	UInt32 flagID, val;
	if (ExtractArgs(EXTRACT_ARGS, &light, &flagID, &val) && IS_TYPE(light, TESObjectLIGH) && (flagID <= 10))
		light->SetFlag(1 << flagID, val != 0);
	return true;
}