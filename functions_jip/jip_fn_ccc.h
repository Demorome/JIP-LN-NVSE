#pragma once

DEFINE_COMMAND_PLUGIN(CCCOnLoad, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(CCCSetFloat, , 0, 2, kParams_JIP_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(CCCSetString, , 0, 22, kParams_JIP_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(CCCSetTrait, , 0, 3, kParams_JIP_TwoInts_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(CCCGetDistance, , 1, 2, kParams_JIP_OneObjectRef_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(CCCInFaction, , 1, 2, kParams_JIP_OneFaction_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(CCCSetNCCS, , 1, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetEncumbranceRate, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(CCCLoadNCCS, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(CCCSavedForm, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(CCCLocationName, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(CCCGetReputation, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(CCCSayTo, , 1, 4, kParams_JIP_OneActor_OneTopic_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(CCCRunResultScripts, , 1, 1, kParams_JIP_OneTopic);
DEFINE_COMMAND_PLUGIN(CCCSetFollowState, , 1, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(RefToPosStr, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(MoveToPosStr, , 1, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(LockEquipment, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(CCCSetEquipped, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(CCCSMS, , 0, 1, kParams_OneOptionalObjectRef);
DEFINE_COMMAND_PLUGIN(CCCTaskPackageFlags, , 0, 4, kParams_JIP_OneAIPackage_ThreeInts);

UInt8 s_CCCModIdx = 0;
UnorderedMap<const char*, const char*> s_avatarPaths(0x100);
UnorderedMap<char*, const char*> s_avatarCommon(0x40);
bool s_UILoaded = false;
Tile::Value **s_UIelements = NULL;
UInt32 s_savedForms[10] = {};
TESFaction *s_taskFaction;
const char kLoadError[] = "ERROR: Failed to load JIP CC&C UI components.";

bool Cmd_CCCOnLoad_Execute(COMMAND_ARGS)
{
	if (s_CCCModIdx) return true;

	s_CCCModIdx = scriptObj->GetOverridingModIdx();
	UInt32 index = s_CCCModIdx << 24;
	TESGlobal *globVar = (TESGlobal*)LookupFormByRefID(index | 0x5000);
	if (!globVar || (globVar->data < 260))
	{
		globVar = (TESGlobal*)LookupFormByRefID(index | 0x8F6D);
		if (globVar) globVar->data = 1;
		*ShowMessageBox_pScriptRefID = scriptObj->refID;
		*ShowMessageBox_button = 0xFF;
		ShowMessageBox("Please be advised:\n\nJIP CC&C versions older than 2.60 will NOT work with JIP LN NVSE.\n\nYou MUST update JIP CC&C to the latest version.", MSGBOX_ARGS);
		return false;
	}

	char *dataPtr, *delim, *pathStr;
	LineIterator lineIter("Data\\Config\\ccc_avatars.ini", s_strArgBuffer);
	index = 0;
	while (!lineIter.End())
	{
		dataPtr = lineIter.Get();
		lineIter.Next();
		delim = GetNextToken(dataPtr, '=');
		pathStr = (char*)malloc(StrLen(delim) + 28);
		memcpy(StrCopy(StrLenCopy(pathStr, "jazzisparis\\ccc\\avatar_", 23), delim), ".dds", 5);
		if (index < 129) s_avatarPaths[dataPtr] = pathStr;
		else s_avatarCommon[dataPtr] = pathStr;
		index++;
	}

	for (DirectoryIterator iter("Data\\textures\\jazzisparis\\ccc\\avatar_*.dds"); !iter.End(); iter.Next())
	{
		if (!iter.IsFile()) continue;
		delim = const_cast<char*>(iter.Get());
		index = StrLen(delim);
		pathStr = (char*)malloc(index + 17);
		StrCopy(StrLenCopy(pathStr, "jazzisparis\\ccc\\", 16), delim);
		delim[index - 4] = 0;
		s_avatarPaths[delim + 7] = pathStr;
	}

	s_taskFaction = (TESFaction*)LookupFormByRefID((s_CCCModIdx << 24) + 0x2D501);

	index = g_dataHandler->GetModIndex("Companion Core.esm");
	if (index < 0xFF)
	{
		index <<= 24;
		s_savedForms[0] = index | 0x812;
		s_savedForms[1] = index | 0x810;
		s_savedForms[2] = index | 0x950D;
		s_savedForms[3] = index | 0x5B3B4;
		TESPackage *package = (TESPackage*)LookupFormByRefID(index | 0x80E);
		if (package && package->location) package->location->radius = 0;
	}

	index = g_dataHandler->GetModIndex("NosCo Companion System.esm");
	if (index < 0xFF)
	{
		index <<= 24;
		s_savedForms[4] = index | 0xAE0;
		s_savedForms[5] = index | 0xAE2;
		s_savedForms[6] = index | 0x86A7;
		s_savedForms[7] = index | 0xAE5;
		s_savedForms[8] = index | 0xADD;
	}

	index = g_dataHandler->GetModIndex("EZ_CompanionNVSE.esm");
	if (index < 0xFF) s_savedForms[9] = (index << 24) | 0xDC3;
	
	Tile *tile = g_HUDMainMenu->tile->GetComponentTile("JIPCCCHUDDisplay");
	if (!tile)
	{
		Console_Print(kLoadError);
		return true;
	}
	if (!s_UIelements) s_UIelements = (Tile::Value**)calloc(0x77, 4);
	UInt32 traitID06 = TraitNameToID("_Size"), traitID07 = TraitNameToID("_Anchor"), traitID08 = TraitNameToID("_Selected"),
		traitID09 = TraitNameToID("_HealthFill"), traitID10 = TraitNameToID("_Distance"), traitID11 = TraitNameToID("_Action"),
		traitID17 = TraitNameToID("_CmdType"), traitID20 = TraitNameToID("_Value");
	DListNode<Tile> *node0 = tile->children.Tail(), *node1, *node2;
	s_UIelements[0] = tile->GetValueName("_AltStyle");
	s_UIelements[1] = tile->GetValueName("_SelectMode");
	s_UIelements[2] = tile->GetValueName("_Adjust");
	tile = node0->data;
	s_UIelements[3] = tile->GetValueName("_TotalAvatars");
	s_UIelements[4] = tile->GetValueName("_NumHired");
	s_UIelements[5] = tile->GetValueName("_Layout");
	s_UIelements[6] = tile->GetValue(traitID06);
	s_UIelements[7] = tile->GetValue(traitID07);
	s_UIelements[8] = tile->GetValue(traitID08);
	s_UIelements[9] = tile->GetValue(kTileValue_visible);
	node1 = tile->children.Tail();
	for (index = 0; index < 10; index++)
	{
		tile = node1->data;
		node1 = node1->prev;
		s_UIelements[10 + index] = tile->GetValue(traitID09);
		s_UIelements[20 + index] = tile->GetValue(traitID10);
		s_UIelements[30 + index] = tile->GetValue(traitID11);
		s_UIelements[40 + index] = tile->GetNthChild(2)->data->GetValue(kTileValue_filename);
		s_UIelements[50 + index] = tile->GetNthChild(4)->data->GetValue(kTileValue_string);
	}
	node0 = node0->prev;
	tile = node0->data;
	s_UIelements[60] = tile->GetValue(traitID06);
	s_UIelements[61] = tile->GetValue(traitID08);
	s_UIelements[62] = tile->GetValue(traitID07);
	s_UIelements[63] = tile->GetValueName("_IsWaiting");
	s_UIelements[64] = tile->GetValueName("_IsFollowingLong");
	s_UIelements[65] = tile->GetValueName("_IsUsingMelee");
	s_UIelements[66] = tile->GetValueName("_IsAggressive");
	s_UIelements[67] = tile->GetValueName("_LabelsON");
	s_UIelements[68] = tile->GetValue(kTileValue_x);
	node1 = tile->GetNthChild(3);
	for (index = 69; index < 74; index++)
	{
		s_UIelements[index] = node1->data->GetValue(traitID17);
		node1 = node1->prev;
	}
	s_UIelements[74] = node1->data->GetValue(kTileValue_string);
	node0 = node0->prev;
	tile = node0->data;
	s_UIelements[75] = tile->GetValueName("_Menu");
	s_UIelements[76] = tile->GetValue(traitID08);
	s_UIelements[77] = tile->GetValueName("_HasHome");
	node1 = tile->GetNthChild(3);
	node2 = node1->data->children.Tail();
	for (index = 78; index < 81; index++)
	{
		tile = node2->data;
		node2 = node2->prev;
		s_UIelements[index] = tile->GetValue(traitID20);
	}
	node2 = node2->Regress(3);
	s_UIelements[81] = node2->data->GetValue(kTileValue_string);
	node1 = node1->Regress(2);
	node2 = node1->data->children.Tail();
	s_UIelements[82] = node2->data->GetValue(kTileValue_string);
	node2 = node2->prev;
	s_UIelements[83] = node2->data->GetValue(kTileValue_string);
	node2 = node2->prev;
	for (index = 84; index < 110; index++)
	{
		s_UIelements[index] = node2->data->GetValue(traitID20);
		node2 = node2->prev;
	}
	s_UIelements[110] = node2->data->children.Tail()->data->GetValue(kTileValue_string);
	node0 = node0->prev;
	tile = node0->data;
	s_UIelements[111] = tile->GetValueName("_Targeting");
	s_UIelements[112] = tile->GetValue(kTileValue_string);
	node0 = node0->prev;
	s_UIelements[113] = node0->data->children.Tail()->data->GetValue(kTileValue_string);
	tile = tile->parent->parent;

	char compPath[] = "SneakMeter\\y\0QuestWrap\\QuestReminder\\x\0XPWrap\\x\0EH_Wrap\\EnemyHealth\\y\0QuestReminder\\x\0XPMeter\\x\0Info\\y\0EnemyHealth\\y\0InfoWrap\\Info\\y";
	s_UIelements[114] = tile->GetComponentValue(compPath);
	if (tile->GetValueName("_DUIF3")->num)
	{
		s_UIelements[115] = tile->GetComponentValue(compPath + 13);
		s_UIelements[116] = tile->GetComponentValue(compPath + 39);
		s_UIelements[118] = tile->GetComponentValue(compPath + 48);
	}
	else
	{
		s_UIelements[115] = tile->GetComponentValue(compPath + 70);
		s_UIelements[116] = tile->GetComponentValue(compPath + 86);
		s_UIelements[117] = tile->GetComponentValue(compPath + 96);
		s_UIelements[118] = tile->GetComponentValue(compPath + 103);
	}
	if (!s_UIelements[117]) s_UIelements[117] = tile->GetComponentValue(compPath + 117);

	for (index = 0; index < 119; index++)
	{
		if (!s_UIelements[index])
		{
			Console_Print(kLoadError);
			return true;
		}
	}
	s_UILoaded = true;

	return true;
}

bool Cmd_CCCSetFloat_Execute(COMMAND_ARGS)
{
	UInt32 trait;
	float value;
	if (s_UILoaded && ExtractArgs(EXTRACT_ARGS, &trait, &value))
		s_UIelements[trait]->SetFloat(value);
	return true;
}

bool Cmd_CCCSetString_Execute(COMMAND_ARGS)
{
	UInt32 trait;
	if (s_UILoaded && ExtractFormatStringArgs(1, s_strArgBuffer, EXTRACT_ARGS_EX, kCommandInfo_CCCSetString.numParams, &trait))
		s_UIelements[trait]->SetString(s_strArgBuffer);
	return true;
}

const char kAvatarMale[] = "jazzisparis\\ccc\\avatar_male.dds", kAvatarFemale[] = "jazzisparis\\ccc\\avatar_female.dds";
UnorderedMap<UInt32, const char*> s_pathForID(0x20);

bool Cmd_CCCSetTrait_Execute(COMMAND_ARGS)
{
	SInt32 trait, child, value = 0;
	if (!s_UILoaded || !ExtractArgs(EXTRACT_ARGS, &trait, &child, &value)) return true;
	if (trait == 40)
	{
		const char *pathStr = kAvatarMale;
		if (thisObj && thisObj->IsActor())
		{
			TESActorBase *actorBase = (TESActorBase*)thisObj->baseForm;
			const char *fullName = actorBase->fullName.name.m_data, **findID;
			if (!fullName || !*fullName) return true;
			if (s_pathForID.Insert(actorBase->refID, &findID))
			{
				const char *findName = s_avatarPaths.Get(fullName);
				if (!findName)
				{
					if IS_TYPE(actorBase, TESCreature)
					{
						for (auto iter = s_avatarCommon.Begin(); iter; ++iter)
						{
							if (SubStrCI(fullName, iter.Key()))
							{
								pathStr = *iter;
								break;
							}
						}
					}
					else if (actorBase->baseData.flags & 1) pathStr = kAvatarFemale;
				}
				else pathStr = findName;
				*findID = pathStr;
			}
			else pathStr = *findID;
			s_UIelements[50 + child]->SetString(fullName);
		}
		s_UIelements[40 + child]->SetString(pathStr);
	}
	else if (!trait)
	{
		if (!thisObj->IsActor()) return true;
		Actor *actor = (Actor*)thisObj;
		if (!actor->GetKnockedState())
		{
			s_UIelements[10 + child]->SetFloat(64 * actor->avOwner.GetActorValue(kAVCode_Health) / actor->avOwner.GetBaseActorValue(kAVCode_Health));
			s_UIelements[20 + child]->SetFloat(g_thePlayer->GetDistance(actor) > 8192);
		}
		else s_UIelements[10 + child]->SetFloat(-1);
		value = actor->extraDataList.GetExtraFactionRank(s_taskFaction);
		if (value < 7)
		{
			PackageInfo *packageInfo = actor->GetPackageInfo();
			if (!packageInfo) return true;
			switch (packageInfo->package->type)
			{
			case 1:
			case 2:
			case 6:
			case 7:
				if (packageInfo->targetRef != g_thePlayer) return true;
				value = 1;
				break;
			case 5:
			case 9:
			case 13:
			case 14:
				value = 2;
				break;
			case 18:
				value = 5 + packageInfo->package->GetFleeCombat();
				break;
			case 10:
			case 22:
				value = 6;
				break;
			case 12:
				value = 13;
				break;
			default:
				return true;
			}
		}
		s_UIelements[30 + child]->SetFloat(value);
	}
	else s_UIelements[trait + child]->SetFloat(value);
	return true;
}

bool Cmd_CCCGetDistance_Execute(COMMAND_ARGS)
{
	TESObjectREFR *objRef;
	UInt32 axis = 7;
	if (ExtractArgs(EXTRACT_ARGS, &objRef, &axis))
		*result = GetAxisDistance(thisObj, objRef, axis);
	else *result = 0;
	return true;
}

bool Cmd_CCCInFaction_Execute(COMMAND_ARGS)
{
	TESFaction *faction;
	UInt32 inBase = 0;
	if (ExtractArgs(EXTRACT_ARGS, &faction, &inBase) && thisObj->IsActor() && 
		((inBase && (((TESActorBase*)thisObj->baseForm)->baseData.GetFactionRank(faction) >= 0)) || 
		(thisObj->extraDataList.GetExtraFactionRank(faction) >= 0))) *result = 1;
	else *result = 0;
	return true;
}

UnorderedSet<TESActorBase*> s_NCCSActors;

void SetNCCS(TESActorBase *actorBase, bool doSet)
{
	Script *script = actorBase->scriptable.script;
	if (script)
	{
		SetScriptDisabled(script, doSet);
		SetScriptEventDisabled(actorBase, 0, true, doSet);
	}
}

bool Cmd_CCCSetNCCS_Execute(COMMAND_ARGS)
{
	UInt32 doSet = 1;
	if (ExtractArgs(EXTRACT_ARGS, &doSet) && thisObj->IsActor())
	{
		TESActorBase *actorBase = (TESActorBase*)thisObj->baseForm;
		if (doSet)
		{
			if (!s_NCCSActors.Insert(actorBase)) return true;
		}
		else if (!s_NCCSActors.Erase(actorBase)) return true;
		SetNCCS(actorBase, doSet != 0);
	}
	return true;
}

bool Cmd_GetEncumbranceRate_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (thisObj->IsActor())
	{
		ExtraContainerChanges *xChanges = GetExtraType(&thisObj->extraDataList, ContainerChanges);
		if (xChanges && xChanges->data)
			*result = 100 * xChanges->data->GetInventoryWeight() / GetMax(((Actor*)thisObj)->avOwner.GetActorValue(kAVCode_CarryWeight), 1.0F);
	}
	return true;
}

bool Cmd_CCCLoadNCCS_Execute(COMMAND_ARGS)
{
	if (!s_NCCSActors.Empty())
	{
		for (auto nccs = s_NCCSActors.Begin(); nccs; ++nccs)
			SetNCCS(*nccs, false);
		s_NCCSActors.Clear();
	}
	return true;
}

bool Cmd_CCCSavedForm_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 idx;
	if (ExtractArgs(EXTRACT_ARGS, &idx)) REFR_RES = s_savedForms[idx];
	return true;
}

typedef UnorderedMap<UInt32, TESObjectREFR*> MapMarkersGrid;
UnorderedMap<TESWorldSpace*, MapMarkersGrid> s_worldspaceMap;

bool Cmd_CCCLocationName_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell = thisObj->GetParentCell();
	if (!cell)
	{
		AssignString(PASS_COMMAND_ARGS, "at an unknown location");
		return true;
	}
	TESWorldSpace *currentWspc = cell->worldSpace;
	if (!currentWspc)
	{
		memcpy(StrLenCopy(s_strValBuffer, "in ", 3), cell->fullName.name.m_data, cell->fullName.name.m_dataLen + 1);
		AssignString(PASS_COMMAND_ARGS, s_strValBuffer);
		return true;
	}
	TESObjectREFR *mkRefr;
	ExtraMapMarker *xMarker;
	Coordinate coord;
	MapMarkersGrid *markersGrid;
	if (s_worldspaceMap.Insert(currentWspc, &markersGrid))
	{
		if (currentWspc->cell && (currentWspc->texture.ddsPath.m_dataLen || currentWspc->parent))
		{
			ListNode<TESObjectREFR> *objIter = currentWspc->cell->objectList.Head();
			do
			{
				mkRefr = objIter->data;
				if (!mkRefr || (mkRefr->baseForm->refID != 0x10)) continue;
				xMarker = GetExtraType(&mkRefr->extraDataList, MapMarker);
				if (!xMarker || !xMarker->data || !xMarker->data->fullName.name.m_dataLen) continue;
				coord.x = lfloor(mkRefr->posX / 4096);
				coord.y = lfloor(mkRefr->posY / 4096);
				(*markersGrid)[coord.xy] = mkRefr;
			}
			while (objIter = objIter->next);
		}
	}
	if (!markersGrid->Empty())
	{
		mkRefr = NULL;
		SInt16 gridX = coord.x = lfloor(thisObj->posX / 4096), gridY = coord.y = lfloor(thisObj->posY / 4096);
		char modX, modY, count;
		TESObjectREFR *findMarker;
		float distMin = 40000, distTmp;
		for (char radius = 0; radius <= 8; radius++)
		{
			coord.x = gridX - radius;
			coord.y = gridY - radius;
			modX = 0;
			modY = 1;
			count = 1;
			do
			{
				findMarker = markersGrid->Get(coord.xy);
				if (findMarker)
				{
					distTmp = Vector3Distance(thisObj->PosVector(), findMarker->PosVector());
					if (distMin > distTmp)
					{
						mkRefr = findMarker;
						distMin = distTmp;
					}
				}
				if (!radius) break;
				if (modX) coord.x += modX;
				else coord.y += modY;
				if (count % (radius * 2)) continue;
				modX = modY;
				modY = -!modY;
			}
			while (++count <= (radius * 8));
			if (mkRefr) break;
		}
		if (mkRefr)
		{
			xMarker = GetExtraType(&mkRefr->extraDataList, MapMarker);
			if (xMarker && xMarker->data)
			{
				if (distMin > 4096)
				{
					distTmp = (thisObj->posX - mkRefr->posX) / distMin;
					distMin = thisObj->posY - mkRefr->posY;
					if ((distTmp >= 0.97F) && (distTmp <= 1)) memcpy(s_strValBuffer, "East of ", 9);
					else if ((distTmp >= -1) && (distTmp <= -0.97F)) memcpy(s_strValBuffer, "West of ", 9);
					else if ((distTmp >= -0.26F) && (distTmp <= 0.26F)) StrCopy(s_strValBuffer, (distMin > 0) ? "North of " : "South of ");
					else if (distTmp > 0) StrCopy(s_strValBuffer, (distMin > 0) ? "NE of " : "SE of ");
					else StrCopy(s_strValBuffer, (distMin > 0) ? "NW of " : "SW of ");
				}
				else memcpy(s_strValBuffer, "at ", 4);
				StrCat(s_strValBuffer, xMarker->data->fullName.name.m_data);
				AssignString(PASS_COMMAND_ARGS, s_strValBuffer);
				return true;
			}
		}
	}
	memcpy(StrLenCopy(s_strValBuffer, "at ", 3), currentWspc->fullName.name.m_data, currentWspc->fullName.name.m_dataLen + 1);
	AssignString(PASS_COMMAND_ARGS, s_strValBuffer);
	return true;
}

bool Cmd_CCCGetReputation_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (!thisObj->IsActor()) return true;
	ListNode<FactionListData> *baseFacIt = ((TESActorBase*)thisObj->baseForm)->baseData.factionList.Head();
	FactionListData *facData;
	do
	{
		if (!(facData = baseFacIt->data)) continue;
		if (facData->faction->reputation)
		{
			REFR_RES = facData->faction->reputation->refID;
			return true;
		}
	}
	while (baseFacIt = baseFacIt->next);
	ExtraFactionChanges *xChanges = GetExtraType(&thisObj->extraDataList, FactionChanges);
	if (!xChanges || !xChanges->data) return true;
	ListNode<FactionListData> *refFacIt = xChanges->data->Head();
	FactionListData *fclData;
	do
	{
		if (!(fclData = refFacIt->data)) continue;
		if (fclData->faction->reputation)
		{
			REFR_RES = fclData->faction->reputation->refID;
			return true;
		}
	}
	while (refFacIt = refFacIt->next);
	return true;
}

bool Cmd_CCCSayTo_Execute(COMMAND_ARGS)
{
	if (!thisObj->IsActor()) return true;
	SayTo(PASS_COMMAND_ARGS);
	ExtraSayToTopicInfo *xSayTo = GetExtraType(&thisObj->extraDataList, SayToTopicInfo);
	if (xSayTo)
	{
		if (xSayTo->info) xSayTo->info->RunResultScript(true, (Actor*)thisObj);
		RemoveExtraData(&thisObj->extraDataList, xSayTo, true);
	}
	return true;
}

bool Cmd_CCCRunResultScripts_Execute(COMMAND_ARGS)
{
	TESTopic *topic;
	if (ExtractArgs(EXTRACT_ARGS, &topic) && thisObj->IsActor())
	{
		Actor *actor = (Actor*)thisObj;
		bool eval;
		TESTopicInfo *topicInfo = GetTopicInfo(topic, &eval, actor, g_thePlayer, 1, NULL, NULL);
		if (topicInfo)
		{
			topicInfo->RunResultScript(false, actor);
			topicInfo->RunResultScript(true, actor);
		}
	}
	return true;
}

bool Cmd_CCCSetFollowState_Execute(COMMAND_ARGS)
{
	SInt32 state = -1;
	if (!ExtractArgs(EXTRACT_ARGS, &state) || !thisObj->IsActor()) return true;
	Actor *actor = (Actor*)thisObj;
	if (state < 0)
	{
		if (!(actor->jipActorFlags1 & kHookActorFlag1_PCTeleportAI)) return true;
		actor->jipActorFlags1 &= ~kHookActorFlag1_PCTeleportAI;
		HOOK_MOD(TeleportWithPC, false);
	}
	else
	{
		UInt8 flag = state ? kHookActorFlag1_PCTeleportFollow : kHookActorFlag1_PCTeleportWait;
		if (actor->jipActorFlags1 & flag) return true;
		if (actor->jipActorFlags1 & kHookActorFlag1_PCTeleportAI) actor->jipActorFlags1 ^= kHookActorFlag1_PCTeleportAI;
		else
		{
			actor->jipActorFlags1 |= flag;
			HOOK_MOD(TeleportWithPC, true);
		}
	}
	return true;
}

bool Cmd_RefToPosStr_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell = thisObj->GetParentCell();
	if (cell)
	{
		char *pos = s_strValBuffer;
		pos = UIntToHex(pos, cell->worldSpace ? cell->worldSpace->refID : cell->refID);
		*pos++ = ' ';
		pos = IntToStr(pos, lfloor(thisObj->posX));
		*pos++ = ' ';
		pos = IntToStr(pos, lfloor(thisObj->posY));
		*pos++ = ' ';
		IntToStr(pos, lceil(thisObj->posZ));
	}
	else s_strValBuffer[0] = 0;
	AssignString(PASS_COMMAND_ARGS, s_strValBuffer);
	return true;
}

bool Cmd_MoveToPosStr_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (!ExtractArgs(EXTRACT_ARGS, &s_strArgBuffer)) return true;
	char *pos = s_strArgBuffer, *delim = GetNextToken(pos, ' ');
	UInt32 refID = HexToUInt(pos);
	if (!refID || !*delim || !ResolveRefID(refID, &refID)) return true;
	TESForm *form = LookupFormByRefID(refID);
	if (!form) return true;
	NiVector3 posVector;
	pos = delim;
	delim = GetNextToken(pos, ' ');
	posVector.x = StrToInt(pos);
	if (!*delim) return true;
	pos = delim;
	delim = GetNextToken(pos, ' ');
	posVector.y = StrToInt(pos);
	if (!*delim) return true;
	posVector.z = StrToInt(delim);
	if (thisObj->MoveToCell(form, posVector)) *result = 1;
	return true;
}

bool Cmd_LockEquipment_Execute(COMMAND_ARGS)
{
	UInt32 lockEqp;
	if (ExtractArgs(EXTRACT_ARGS, &lockEqp) && IS_TYPE(thisObj, Character) && 
		(!lockEqp != !(((Character*)thisObj)->jipActorFlags1 & kHookActorFlag1_LockedEquipment)))
	{
		((Character*)thisObj)->jipActorFlags1 ^= kHookActorFlag1_LockedEquipment;
		bool doLock = lockEqp != 0;
		HOOK_MOD(EquipItem, doLock);
		HOOK_MOD(ReEquipAll, doLock);
		HOOK_MOD(WeaponSwitchSelect, doLock);
		HOOK_MOD(WeaponSwitchUnequip, doLock);
		HOOK_MOD(GetPreferedWeapon, doLock);
	}
	return true;
}

bool Cmd_CCCSetEquipped_Execute(COMMAND_ARGS)
{
	*result = 0;
	Character *character;
	TESCombatStyle *cmbStyle;
	TESObjectWEAP *weapon;
	ContChangesEntry *entry;
	TESForm *item;
	ExtraDataList *xData = NULL;
	UInt8 restrictions;
	if (thisObj)
	{
		if NOT_TYPE(thisObj, Character) return true;
		character = (Character*)thisObj;
		if (!character->GetEquippedWeapon() && character->GetInventoryItems(kFormType_TESObjectWEAP))
		{
			cmbStyle = character->GetCombatStyle();
			restrictions = cmbStyle ? cmbStyle->weaponRestrictions : 0;
			for (auto wpnIter = s_inventoryItemsMap.Begin(); wpnIter; ++wpnIter)
			{
				weapon = (TESObjectWEAP*)wpnIter.Key();
				if (weapon->IsPlayable() || (restrictions && ((restrictions == 1) != (weapon->eWeaponType < 3))))
					continue;
				entry = wpnIter().entry;
				if (entry && entry->extendData) xData = entry->extendData->GetFirstItem();
				character->EquipItem(weapon, 1, xData, 1, 0, 0);
				break;
			}
		}
		if (!character->validBip01Names || !character->GetInventoryItems(kFormType_TESObjectARMO))
			return true;
		UInt32 usedSlots = 0;
		ValidBip01Names::Data *slotData = character->validBip01Names->slotData;
		for (UInt8 count = 20; count; count--, slotData++)
		{
			item = slotData->item;
			if (!item || NOT_TYPE(item, TESObjectARMO))
				continue;
			s_inventoryItemsMap.Erase(item);
			usedSlots |= ((TESObjectARMO*)item)->bipedModel.partMask;
		}
		TESBipedModelForm *biped;
		for (auto armIter = s_inventoryItemsMap.Begin(); armIter; ++armIter)
		{
			item = armIter.Key();
			biped = &((TESObjectARMO*)item)->bipedModel;
			if (biped->IsPlayable() || (usedSlots & biped->partMask))
				continue;
			entry = armIter().entry;
			xData = (entry && entry->extendData) ? entry->extendData->GetFirstItem() : NULL;
			character->EquipItem(item, 1, xData, 1, 0, 0);
			usedSlots |= biped->partMask;
		}
		character->ModActorValue(kAVCode_DamageResist, 0, 0);
		character->ModActorValue(kAVCode_DamageThreshold, 0, 0);
		return true;
	}
	ContChangesEntry *menuEntry = *g_containerMenuSelection;
	if (!menuEntry || !*g_containerMenu || !(*g_containerMenu)->rightItems.selected || (GetActiveTileID() != 20) || FindMainLoopCallback(RefreshContainerMenu))
		return true;
	item = menuEntry->type;
	character = (Character*)(*g_containerMenu)->containerRef;
	if (!item || NOT_TYPE(character, Character))
		return true;
	ExtraContainerChanges::EntryDataList *entryList = character->GetContainerChangesList();
	if (!entryList) return true;
	if IS_TYPE(item, AlchemyItem)
	{
		character->EquipItem(item);
		RefreshContainerMenu();
		return true;
	}
	if (NOT_TYPE(item, TESObjectARMO) && NOT_TYPE(item, TESObjectWEAP))
		return true;
	if (menuEntry->extendData) xData = menuEntry->extendData->GetFirstItem();
	bool equipped = xData && xData->HasType(kExtraData_Worn);
	if IS_TYPE(item, TESObjectWEAP)
	{
		if (xData && xData->HasType(kExtraData_CannotWear))
		{
			entry = entryList->FindForItem(item);
			xData = (entry && entry->extendData) ? entry->extendData->RemoveByType(xData, kExtraData_CannotWear) : NULL;
		}
		if (!equipped)
		{
			weapon = (TESObjectWEAP*)item;
			UInt32 wpnType = weapon->eWeaponType;
			if ((wpnType == 11) || (wpnType == 12) || (weapon->weaponFlags2 & 1))
			{
				*result = -1;
				return true;
			}
			cmbStyle = character->GetCombatStyle();
			restrictions = cmbStyle ? cmbStyle->weaponRestrictions : 0;
			if (restrictions && ((restrictions == 1) != (wpnType < 3)))
			{
				*result = restrictions;
				return true;
			}
			wpnType = (wpnType > 9) ? menuEntry->countDelta : 1;
			entry = character->baseProcess->GetWeaponInfo();
			if (entry && entry->type)
			{
				entry = entryList->FindForItem(entry->type);
				if (entry) entry->RemoveCannotWear();
			}
			character->EquipItem(item, wpnType, xData, 1, 0, 0);
		}
		else character->UnequipItem(item, 1, xData, 1, 0, 0);
	}
	else if (equipped)
	{
		if (xData->HasType(kExtraData_CannotWear))
		{
			entry = entryList->FindForItem(item);
			xData = (entry && entry->extendData) ? entry->extendData->RemoveByType(xData, kExtraData_CannotWear) : NULL;
		}
		character->UnequipItem(item, 1, xData, 1, 0, 0);
	}
	else
	{
		if (entryList)
		{
			ListNode<ContChangesEntry> *entryIter = entryList->Head();
			do
			{
				entry = entryIter->data;
				if (entry && IS_TYPE(entry->type, TESObjectARMO))
					entry->RemoveCannotWear();
			}
			while (entryIter = entryIter->next);
		}
		character->EquipItem(item, 1, xData, 1, 0, 0);
	}
	MainLoopAddCallbackEx(RefreshContainerMenu, NULL, 1, 15);
	return true;
}

bool Cmd_CCCSMS_Execute(COMMAND_ARGS)
{
	TESObjectREFR *objRefr = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &objRefr) || !objRefr) return true;
	ListNode<BSTempEffect> *effIter = g_processManager->tempEffects.Head();
	MagicShaderHitEffect *mshEff;
	do
	{
		mshEff = (MagicShaderHitEffect*)effIter->data;
		if (mshEff && IS_TYPE(mshEff, MagicShaderHitEffect) && (mshEff->target == objRefr) && (mshEff->effectShader->modIndex == s_CCCModIdx))
			mshEff->flags = 1;
	}
	while (effIter = effIter->next);
	return true;
}

bool Cmd_CCCTaskPackageFlags_Execute(COMMAND_ARGS)
{
	TESPackage *package;
	UInt32 taskType, var1, var2;
	if (ExtractArgs(EXTRACT_ARGS, &package, &taskType, &var1, &var2))
	{
		if (taskType)
		{
			if (var1)
			{
				package->packageFlags |= (kPackageFlag_NoIdleAnims | kPackageFlag_NoCombatAlert | kPackageFlag_AlwaysWalk);
				package->packageFlags &= ~(kPackageFlag_AlwaysRun | kPackageFlag_WeaponsDrawn | kPackageFlag_ContinueDuringCombat);
			}
			else
			{
				package->packageFlags |= (kPackageFlag_AlwaysRun | kPackageFlag_NoIdleAnims | kPackageFlag_NoCombatAlert);
				package->packageFlags &= ~(kPackageFlag_WeaponsDrawn | kPackageFlag_ContinueDuringCombat | kPackageFlag_AlwaysWalk);
			}
		}
		else if (var1)
		{
			package->packageFlags |= (kPackageFlag_AlwaysRun | kPackageFlag_WeaponsDrawn | kPackageFlag_ContinueDuringCombat);
			package->packageFlags &= ~(kPackageFlag_NoIdleAnims | kPackageFlag_NoCombatAlert | kPackageFlag_AlwaysWalk);
		}
		else
		{
			package->packageFlags |= (kPackageFlag_AlwaysRun | kPackageFlag_WeaponsDrawn);
			package->packageFlags &= ~(kPackageFlag_NoIdleAnims | kPackageFlag_ContinueDuringCombat | kPackageFlag_NoCombatAlert | kPackageFlag_AlwaysWalk);
		}
		if (var2) package->packageFlags |= kPackageFlag_AlwaysSneak;
		else package->packageFlags &= ~kPackageFlag_AlwaysSneak;
	}
	return true;
}