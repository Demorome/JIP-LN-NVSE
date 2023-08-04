#pragma once

DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetSize, AuxVarSize, 0, 2, kParams_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetType, AuxVarType, 0, 3, kParams_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetFloat, AuxVarGetFlt, 0, 3, kParams_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetRef, AuxVarGetRef, 0, 3, kParams_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetString, AuxVarGetStr, 0, 3, kParams_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetAsArray, AuxVarGetAsArr, 0, 2, kParams_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetAll, AuxVarGetAll, 0, 2, kParams_OneInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableSetFloat, AuxVarSetFlt, 0, 4, kParams_OneString_OneDouble_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableSetRef, AuxVarSetRef, 0, 4, kParams_OneString_OneForm_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableSetString, AuxVarSetStr, 0, 4, kParams_TwoStrings_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableSetFromArray, AuxVarSetFromArr, 0, 3, kParams_OneString_OneInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableErase, AuxVarErase, 0, 3, kParams_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableEraseAll, AuxVarEraseAll, 0, 2, kParams_OneInt_OneOptionalForm);
DEFINE_CMD_COND_ONLY(AuxVarGetFltCond, 1, 2, kParams_OneQuest_OneInt);

#define AUX_VAR_CS	ScopedPrimitiveCS cs(&s_auxVarCS);

AuxVarValsArr* __fastcall AVGetArray(const AuxVarInfo &varInfo, bool addArr = false)
{
	if (addArr) 
		return &varInfo.ModsMap()[varInfo.modIndex][varInfo.ownerID][varInfo.varName];
	if (AuxVarOwnersMap *ownersMap = varInfo.ModsMap().GetPtr(varInfo.modIndex))
		if (AuxVarVarsMap *varsMap = ownersMap->GetPtr(varInfo.ownerID))
			return varsMap->GetPtr(varInfo.varName);
	return nullptr;
}

AuxVariableValue* __fastcall AVGetValue(const AuxVarInfo &varInfo, SInt32 idx, bool addVal = false)
{
	if (AuxVarValsArr *valsArr = AVGetArray(varInfo, addVal && (idx <= 0)); valsArr && (idx <= (SInt32)valsArr->Size()))
	{
		if (!addVal)
		{
			if (valsArr->Empty())
				return nullptr;
			if (idx < 0)
				idx = valsArr->Size() - 1;
		}
		else if ((idx < 0) || (idx == valsArr->Size()))
			return valsArr->Append();
		return &(*valsArr)[idx];
	}
	return nullptr;
}

void __fastcall MarkVarModified(TESObjectREFR *thisObj)
{
	s_dataChangedFlags |= kChangedFlag_AuxVars;
	if (thisObj)
		thisObj->MarkModified(0);
}

bool Cmd_AuxiliaryVariableGetSize_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &form))
		if (AuxVarInfo varInfo(form, thisObj, scriptObj, varName); varInfo.ownerID)
		{
			AUX_VAR_CS
			if (AuxVarValsArr *valsArr = AVGetArray(varInfo))
				*result = (int)valsArr->Size();
		}
	return true;
}

bool Cmd_AuxiliaryVariableGetType_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	SInt32 idx = 0;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &idx, &form) && (idx >= 0))
		if (AuxVarInfo varInfo(form, thisObj, scriptObj, varName); varInfo.ownerID)
		{
			AUX_VAR_CS
			if (AuxVariableValue *value = AVGetValue(varInfo, idx))
				*result = value->GetType();
		}
	return true;
}

bool Cmd_AuxiliaryVariableGetFloat_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	SInt32 idx = 0;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &idx, &form) && (idx >= 0))
		if (AuxVarInfo varInfo(form, thisObj, scriptObj, varName); varInfo.ownerID)
		{
			AUX_VAR_CS
			if (AuxVariableValue *value = AVGetValue(varInfo, idx))
				*result = value->GetFlt();
		}
	return true;
}

bool Cmd_AuxiliaryVariableGetRef_Execute(COMMAND_ARGS)
{
	REFR_RES = 0;
	char varName[0x50];
	SInt32 idx = 0;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &idx, &form) && (idx >= 0))
		if (AuxVarInfo varInfo(form, thisObj, scriptObj, varName); varInfo.ownerID)
		{
			AUX_VAR_CS
			if (AuxVariableValue *value = AVGetValue(varInfo, idx))
				REFR_RES = value->GetRef();
		}
	return true;
}

bool Cmd_AuxiliaryVariableGetString_Execute(COMMAND_ARGS)
{
	const char *resStr = nullptr;
	char varName[0x50];
	SInt32 idx = 0;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &idx, &form) && (idx >= 0))
		if (AuxVarInfo varInfo(form, thisObj, scriptObj, varName); varInfo.ownerID)
		{
			AUX_VAR_CS
			if (AuxVariableValue *value = AVGetValue(varInfo, idx))
				resStr = value->GetStr();
		}
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_AuxiliaryVariableGetAsArray_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &form))
		if (AuxVarInfo varInfo(form, thisObj, scriptObj, varName); varInfo.ownerID)
		{
			AUX_VAR_CS
			if (AuxVarValsArr *valsArr = AVGetArray(varInfo))
			{
				TempElements *tmpElements = GetTempElements();
				for (auto value = valsArr->Begin(); value; ++value)
					tmpElements->Append(value().GetAsElement());
				if (!tmpElements->Empty())
					*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
			}
		}
	return true;
}

bool Cmd_AuxiliaryVariableGetAll_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 type;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &type, &form))
		if (AuxVarInfo varInfo(form, thisObj, scriptObj, type); varInfo.ownerID)
		{
			AUX_VAR_CS
			if (AuxVarOwnersMap *findMod = varInfo.ModsMap().GetPtr(varInfo.modIndex))
				if (AuxVarVarsMap *findOwner = findMod->GetPtr(varInfo.ownerID))
				{
					NVSEArrayVar *varsMap = CreateStringMap(nullptr, nullptr, 0, scriptObj);
					TempElements *tmpElements = GetTempElements();
					for (auto varIter = findOwner->Begin(); varIter; ++varIter)
					{
						for (auto value = varIter().Begin(); value; ++value)
							tmpElements->Append(value().GetAsElement());
						SetElement(varsMap, ArrayElementL(varIter.Key()), ArrayElementL(CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj)));
						tmpElements->Clear();
					}
					*result = (int)varsMap;
				}
		}
	return true;
}

bool Cmd_AuxiliaryVariableSetFloat_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	double fltVal;
	SInt32 idx = 0;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &fltVal, &idx, &form))
		if (AuxVarInfo varInfo(form, thisObj, scriptObj, varName); varInfo.ownerID)
		{
			AUX_VAR_CS
			if (AuxVariableValue *value = AVGetValue(varInfo, idx, true))
			{
				*value = fltVal;
				if (varInfo.isPerm)
					MarkVarModified(thisObj);
				*result = 1;
			}
		}
	return true;
}

bool Cmd_AuxiliaryVariableSetRef_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	SInt32 idx = 0;
	TESForm *refVal, *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &refVal, &idx, &form))
		if (AuxVarInfo varInfo(form, thisObj, scriptObj, varName); varInfo.ownerID)
		{
			AUX_VAR_CS
			if (AuxVariableValue *value = AVGetValue(varInfo, idx, true))
			{
				*value = refVal;
				if (varInfo.isPerm)
					MarkVarModified(thisObj);
				*result = 1;
			}
		}
	return true;
}

bool Cmd_AuxiliaryVariableSetString_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50], *buffer = GetStrArgBuffer();
	SInt32 idx = 0;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, buffer, &idx, &form))
		if (AuxVarInfo varInfo(form, thisObj, scriptObj, varName); varInfo.ownerID)
		{
			AUX_VAR_CS
			if (AuxVariableValue *value = AVGetValue(varInfo, idx, true))
			{
				*value = buffer;
				if (varInfo.isPerm)
					MarkVarModified(thisObj);
				*result = 1;
			}
		}
	return true;
}

bool Cmd_AuxiliaryVariableSetFromArray_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	UInt32 arrID;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &arrID, &form) && arrID)
		if (NVSEArrayVar *srcArr = LookupArrayByID(arrID))
			if (ArrayData arrData(srcArr, true); arrData.size)
				if (AuxVarInfo varInfo(form, thisObj, scriptObj, varName); varInfo.ownerID)
				{
					AUX_VAR_CS
					AuxVarValsArr *valsArr = AVGetArray(varInfo, true);
					if (!valsArr->Empty())
						valsArr->Clear();
					for (UInt32 idx = 0; idx < arrData.size; idx++)
						valsArr->Append(arrData.vals[idx]);
					if (varInfo.isPerm)
						MarkVarModified(thisObj);
					*result = (int)arrData.size;
				}
	return true;
}

bool Cmd_AuxiliaryVariableErase_Execute(COMMAND_ARGS)
{
	*result = -1;
	char varName[0x50];
	SInt32 idx = -1;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &idx, &form))
		if (AuxVarInfo varInfo(form, thisObj, scriptObj, varName); varInfo.ownerID)
		{
			AUX_VAR_CS
			if (auto findMod = varInfo.ModsMap().Find(varInfo.modIndex))
				if (auto findOwner = findMod().Find(varInfo.ownerID))
					if (auto findVar = findOwner().Find(varInfo.varName))
					{
						if (idx >= 0)
						{
							if (!findVar().RemoveNth(idx))
								return true;
							*result = (int)findVar().Size();
							if (findVar().Empty()) findVar.Remove();
						}
						else
						{
							findVar.Remove();
							*result = 0;
						}
						if (findOwner().Empty())
						{
							findOwner.Remove();
							if (findMod().Empty()) findMod.Remove();
						}
						if (varInfo.isPerm)
							s_dataChangedFlags |= kChangedFlag_AuxVars;
					}
		}
	return true;
}

bool Cmd_AuxiliaryVariableEraseAll_Execute(COMMAND_ARGS)
{
	UInt32 type;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &type, &form))
		if (AuxVarInfo varInfo(form, thisObj, scriptObj, type); varInfo.ownerID)
		{
			AUX_VAR_CS
			if (auto findMod = varInfo.ModsMap().Find(varInfo.modIndex))
				if (auto findOwner = findMod().Find(varInfo.ownerID))
				{
					findOwner.Remove();
					if (findMod().Empty()) findMod.Remove();
					if (varInfo.isPerm)
						s_dataChangedFlags |= kChangedFlag_AuxVars;
				}
		}
	return true;
}

bool Cmd_AuxVarGetFltCond_Eval(COMMAND_ARGS_EVAL)
{
	*result = 0;
	if (TESQuest *quest = (TESQuest*)arg1; quest->scriptEventList)
		if (ScriptVar *scriptVar = quest->scriptEventList->GetVariable((UInt32)arg2))
			if (const char *varName = GetStringVar((int)scriptVar->data.num); varName && *varName)
			{
				AuxVarInfo varInfo(nullptr, thisObj, quest->scriptable.script, (char*)varName);
				AUX_VAR_CS
				if (AuxVariableValue *value = AVGetValue(varInfo, 0))
					*result = value->GetFlt();
			}
	return true;
}