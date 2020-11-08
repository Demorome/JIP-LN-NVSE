#pragma once

DEFINE_COMMAND_PLUGIN(GetBodyPartDataSkeleton, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetBodyPartDataSkeleton, , 0, 2, kParams_JIP_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(GetBodyPartDataRagdoll, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetBodyPartDataRagdoll, , 0, 2, kParams_JIP_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetBodyPartDataHasPart, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetBodyPartTraitNumeric, , 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetBodyPartTraitNumeric, , 0, 4, kParams_JIP_OneForm_TwoInts_OneFloat);
DEFINE_COMMAND_PLUGIN(GetBodyPartTraitForm, , 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetBodyPartTraitForm, , 0, 4, kParams_JIP_OneForm_TwoInts_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetBodyPartName, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetBodyPartName, , 0, 3, kParams_JIP_OneForm_OneInt_OneString);
DEFINE_COMMAND_PLUGIN(GetBodyPartReplacementModel, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetBodyPartReplacementModel, , 0, 3, kParams_JIP_OneForm_OneInt_OneString);
DEFINE_COMMAND_PLUGIN(GetBodyPartFlag, , 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetBodyPartFlag, , 0, 4, kParams_JIP_OneForm_ThreeInts);

bool Cmd_GetBodyPartDataSkeleton_Execute(COMMAND_ARGS)
{
	const char *resStr;
	BGSBodyPartData *bpData;
	if (ExtractArgs(EXTRACT_ARGS, &bpData) && IS_TYPE(bpData, BGSBodyPartData))
		resStr = bpData->model.GetModelPath();
	else resStr = NULL;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetBodyPartDataSkeleton_Execute(COMMAND_ARGS)
{
	BGSBodyPartData *bpData;
	if (ExtractArgs(EXTRACT_ARGS, &bpData, &s_strArgBuffer) && IS_TYPE(bpData, BGSBodyPartData))
		bpData->model.SetModelPath(s_strArgBuffer);
	return true;
}

bool Cmd_GetBodyPartDataRagdoll_Execute(COMMAND_ARGS)
{
	*result = 0;
	BGSBodyPartData *bpData;
	if (ExtractArgs(EXTRACT_ARGS, &bpData) && IS_TYPE(bpData, BGSBodyPartData) && bpData->ragDoll)
		REFR_RES = bpData->ragDoll->refID;
	return true;
}

bool Cmd_SetBodyPartDataRagdoll_Execute(COMMAND_ARGS)
{
	BGSBodyPartData *bpData;
	BGSRagdoll *ragDoll = NULL;
	if (ExtractArgs(EXTRACT_ARGS, &bpData, &ragDoll) && IS_TYPE(bpData, BGSBodyPartData) && (!ragDoll || IS_TYPE(ragDoll, BGSRagdoll)))
		bpData->ragDoll = ragDoll;
	return true;
}

bool Cmd_GetBodyPartDataHasPart_Execute(COMMAND_ARGS)
{
	BGSBodyPartData *bpData;
	UInt32 partID;
	if (ExtractArgs(EXTRACT_ARGS, &bpData, &partID) && IS_TYPE(bpData, BGSBodyPartData) &&
		(partID <= 14) && bpData->bodyParts[partID]) *result = 1;
	else *result = 0;
	return true;
}

bool Cmd_GetBodyPartTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	BGSBodyPartData *bpData;
	UInt32 partID, traitID;
	if (!ExtractArgs(EXTRACT_ARGS, &bpData, &partID, &traitID) || NOT_TYPE(bpData, BGSBodyPartData) || (partID > 14) || (traitID > 18)) return true;
	BGSBodyPart *bodyPart = bpData->bodyParts[partID];
	if (!bodyPart) return true;
	switch (traitID)
	{
	case 0:
		*result = bodyPart->actorValue;
		break;
	case 1:
		*result = bodyPart->damageMult;
		break;
	case 2:
		*result = bodyPart->healthPercent;
		break;
	case 3:
		*result = bodyPart->toHitChance;
		break;
	case 4:
		*result = bodyPart->trackingMaxAngle;
		break;
	case 5:
		*result = bodyPart->sevrDebrisCount;
		break;
	case 6:
		*result = bodyPart->sevrDebrisScale;
		break;
	case 7:
		*result = bodyPart->sevrDecalCount;
		break;
	case 8:
		*result = bodyPart->explChance;
		break;
	case 9:
		*result = bodyPart->limbRepScale;
		break;
	case 10:
		*result = bodyPart->explDebrisCount;
		break;
	case 11:
		*result = bodyPart->explDebrisScale;
		break;
	case 12:
		*result = bodyPart->explDecalCount;
		break;
	case 13:
	case 14:
	case 15:
		*result = bodyPart->goreEffTranslate[traitID - 13];
		break;
	default:
		*result = bodyPart->goreEffRotation[traitID - 16] / kDblPId180;
	}
	return true;
}

bool Cmd_SetBodyPartTraitNumeric_Execute(COMMAND_ARGS)
{
	BGSBodyPartData *bpData;
	UInt32 partID, traitID;
	float val;
	if (!ExtractArgs(EXTRACT_ARGS, &bpData, &partID, &traitID, &val) || NOT_TYPE(bpData, BGSBodyPartData) || (partID > 14) || (traitID > 18) || (val < 0))
		return true;
	BGSBodyPart *bodyPart = bpData->bodyParts[partID];
	if (!bodyPart) return true;
	switch (traitID)
	{
	case 0:
		bodyPart->actorValue = val;
		break;
	case 1:
		bodyPart->damageMult = val;
		break;
	case 2:
		bodyPart->healthPercent = val;
		break;
	case 3:
		bodyPart->toHitChance = val;
		break;
	case 4:
		bodyPart->trackingMaxAngle = val;
		break;
	case 5:
		bodyPart->sevrDebrisCount = val;
		break;
	case 6:
		bodyPart->sevrDebrisScale = val;
		break;
	case 7:
		bodyPart->sevrDecalCount = val;
		break;
	case 8:
		bodyPart->explChance = val;
		break;
	case 9:
		bodyPart->limbRepScale = val;
		break;
	case 10:
		bodyPart->explDebrisCount = val;
		break;
	case 11:
		bodyPart->explDebrisScale = val;
		break;
	case 12:
		bodyPart->explDecalCount = val;
		break;
	case 13:
	case 14:
	case 15:
		bodyPart->goreEffTranslate[traitID - 13] = val;
		break;
	default:
		bodyPart->goreEffRotation[traitID - 16] = val * kDblPId180;
	}
	return true;
}

bool Cmd_GetBodyPartTraitForm_Execute(COMMAND_ARGS)
{
	*result = 0;
	BGSBodyPartData *bpData;
	UInt32 partID, traitID;
	if (!ExtractArgs(EXTRACT_ARGS, &bpData, &partID, &traitID) || NOT_TYPE(bpData, BGSBodyPartData) || (partID > 14) || (traitID > 5)) return true;
	BGSBodyPart *bodyPart = bpData->bodyParts[partID];
	if (!bodyPart) return true;
	switch (traitID)
	{
	case 0:
		if (bodyPart->sevrExplosion) REFR_RES = bodyPart->sevrExplosion->refID;
		break;
	case 1:
		if (bodyPart->explExplosion) REFR_RES = bodyPart->explExplosion->refID;
		break;
	case 2:
		if (bodyPart->sevrDebris) REFR_RES = bodyPart->sevrDebris->refID;
		break;
	case 3:
		if (bodyPart->explDebris) REFR_RES = bodyPart->explDebris->refID;
		break;
	case 4:
		if (bodyPart->sevrImpactDS) REFR_RES = bodyPart->sevrImpactDS->refID;
		break;
	default:
		if (bodyPart->explImpactDS) REFR_RES = bodyPart->explImpactDS->refID;
	}
	return true;
}

bool Cmd_SetBodyPartTraitForm_Execute(COMMAND_ARGS)
{
	BGSBodyPartData *bpData;
	TESForm *object = NULL;
	UInt32 partID, traitID;
	if (!ExtractArgs(EXTRACT_ARGS, &bpData, &partID, &traitID, &object) || NOT_TYPE(bpData, BGSBodyPartData) || (partID > 14) || (traitID > 5)) return true;
	BGSBodyPart *bodyPart = bpData->bodyParts[partID];
	if (!bodyPart) return true;
	switch (traitID)
	{
	case 0:
		bodyPart->sevrExplosion = (object && IS_TYPE(object, BGSExplosion)) ? (BGSExplosion*)object : NULL;
		break;
	case 1:
		bodyPart->explExplosion = (object && IS_TYPE(object, BGSExplosion)) ? (BGSExplosion*)object : NULL;
		break;
	case 2:
		bodyPart->sevrDebris = (object && IS_TYPE(object, BGSDebris)) ? (BGSDebris*)object : NULL;
		break;
	case 3:
		bodyPart->explDebris = (object && IS_TYPE(object, BGSDebris)) ? (BGSDebris*)object : NULL;
		break;
	case 4:
		bodyPart->sevrImpactDS = (object && IS_TYPE(object, BGSImpactDataSet)) ? (BGSImpactDataSet*)object : NULL;
		break;
	default:
		bodyPart->explImpactDS = (object && IS_TYPE(object, BGSImpactDataSet)) ? (BGSImpactDataSet*)object : NULL;
	}
	return true;
}

bool Cmd_GetBodyPartName_Execute(COMMAND_ARGS)
{
	const char *resStr = NULL;
	BGSBodyPartData *bpData;
	UInt32 partID;
	if (ExtractArgs(EXTRACT_ARGS, &bpData, &partID) && IS_TYPE(bpData, BGSBodyPartData) && (partID <= 14))
	{
		BGSBodyPart *bodyPart = bpData->bodyParts[partID];
		if (bodyPart && bodyPart->partName.m_dataLen)
			resStr = bodyPart->partName.m_data;
	}
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetBodyPartName_Execute(COMMAND_ARGS)
{
	BGSBodyPartData *bpData;
	UInt32 partID;
	if (!ExtractArgs(EXTRACT_ARGS, &bpData, &partID, &s_strArgBuffer) || NOT_TYPE(bpData, BGSBodyPartData) || (partID > 14)) return true;
	BGSBodyPart *bodyPart = bpData->bodyParts[partID];
	if (bodyPart) bodyPart->partName.Set(s_strArgBuffer);
	return true;
}

bool Cmd_GetBodyPartReplacementModel_Execute(COMMAND_ARGS)
{
	const char *resStr = NULL;
	BGSBodyPartData *bpData;
	UInt32 partID;
	if (ExtractArgs(EXTRACT_ARGS, &bpData, &partID) && IS_TYPE(bpData, BGSBodyPartData) && (partID <= 14))
	{
		BGSBodyPart *bodyPart = bpData->bodyParts[partID];
		if (bodyPart) resStr = bodyPart->limbReplacement.GetModelPath();
	}
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetBodyPartReplacementModel_Execute(COMMAND_ARGS)
{
	BGSBodyPartData *bpData;
	UInt32 partID;
	if (!ExtractArgs(EXTRACT_ARGS, &bpData, &partID, &s_strArgBuffer) || NOT_TYPE(bpData, BGSBodyPartData) || (partID > 14)) return true;
	BGSBodyPart *bodyPart = bpData->bodyParts[partID];
	if (bodyPart) bodyPart->limbReplacement.SetModelPath(s_strArgBuffer);
	return true;
}

bool Cmd_GetBodyPartFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	BGSBodyPartData *bpData;
	UInt32 partID, flagID;
	if (!ExtractArgs(EXTRACT_ARGS, &bpData, &partID, &flagID) || NOT_TYPE(bpData, BGSBodyPartData) || (partID > 14) || (flagID > 6)) return true;
	BGSBodyPart *bodyPart = bpData->bodyParts[partID];
	if (bodyPart) *result = (bodyPart->flags & (1 << flagID)) ? 1 : 0;
	return true;
}

bool Cmd_SetBodyPartFlag_Execute(COMMAND_ARGS)
{
	BGSBodyPartData *bpData;
	UInt32 partID, flagID, val;
	if (!ExtractArgs(EXTRACT_ARGS, &bpData, &partID, &flagID, &val) || NOT_TYPE(bpData, BGSBodyPartData) || (partID > 14) || (flagID > 6)) return true;
	BGSBodyPart *bodyPart = bpData->bodyParts[partID];
	if (bodyPart) bodyPart->SetFlag(1 << flagID, val != 0);
	return true;
}