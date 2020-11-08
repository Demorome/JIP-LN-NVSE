#pragma once

DEFINE_COMMAND_PLUGIN(IsComponentLoaded, , 0, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(InjectUIXML, , 0, 2, kParams_TwoStrings);
DEFINE_COMMAND_PLUGIN(InjectUIComponent, , 0, 22, kParams_JIP_OneString_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetCursorPos, , 0, 1, kParams_Axis);
DEFINE_COMMAND_PLUGIN(GetUIString, , 0, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(GetActiveMenuMode, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetActiveUIComponentName, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetActiveUIComponentFullName, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetActiveUIComponentID, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetMenuTargetRef, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetMenuItemFilter, , 0, 2, kParams_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(ClickMenuButton, , 0, 2, kParams_JIP_OneString_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetSelectedItemRef, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetBarterItems, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetBarterGoldAlt, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetRecipeMenuSelection, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetRecipeMenuCategory, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(UnlockRecipeMenuQuantity, , 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetRecipeMenuNumSubcategories, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(MessageExAlt, , 0, 22, kParams_JIP_OneFloat_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetFontFile, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetFontFile, , 0, 2, kParams_JIP_OneInt_OneString);
DEFINE_COMMAND_PLUGIN(ShowTextInputMenu, , 0, 24, kParams_JIP_OneForm_TwoFloats_OneFormatString);
DEFINE_COMMAND_PLUGIN(SetTextInputExtendedProps, , 0, 5, kParams_JIP_TwoFloats_ThreeOptionalInts);
DEFINE_COMMAND_PLUGIN(SetTextInputString, , 0, 21, kParams_FormatString);
DEFINE_COMMAND_PLUGIN(SetMessageDisabled, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetMessageDisabled, , 0, 1, kParams_JIP_OneMessage);
DEFINE_COMMAND_PLUGIN(GetMessageFlags, , 0, 1, kParams_JIP_OneMessage);
DEFINE_COMMAND_PLUGIN(SetMessageFlags, , 0, 2, kParams_JIP_OneMessage_OneInt);
DEFINE_COMMAND_PLUGIN(SetMessageDisplayTime, , 0, 2, kParams_JIP_OneMessage_OneInt);
DEFINE_COMMAND_PLUGIN(SetOnMenuClickEventHandler, , 0, 3, kParams_JIP_OneForm_OneInt_OneString);
DEFINE_COMMAND_PLUGIN(SetOnMenuOpenEventHandler, , 0, 3, kParams_JIP_OneForm_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetOnMenuCloseEventHandler, , 0, 3, kParams_JIP_OneForm_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetOnMouseoverChangeEventHandler, , 0, 3, kParams_JIP_OneForm_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(RefreshItemsList, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetBarterPriceMult, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetBarterPriceMult, , 0, 2, kParams_JIP_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(SetTerminalUIModel, , 0, 2, kParams_JIP_OneForm_OneOptionalString);
DEFINE_COMMAND_PLUGIN(ShowQuantityMenu, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(MessageBoxExAlt, , 0, 22, kParams_JIP_OneForm_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetVATSTargets, , 0, 0, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(IsInCharGen, , , 0, NULL);
DEFINE_COMMAND_PLUGIN(ToggleCraftingMessages, , 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetCursorPos, , 0, 2, kParams_TwoFloats);
DEFINE_COMMAND_PLUGIN(UnloadUIComponent, , 0, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(ClearMessageQueue, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetSystemColor, , 0, 4, kParams_JIP_FourInts);
DEFINE_COMMAND_PLUGIN(SuppressQuestMessages, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(EnableImprovedRecipeMenu, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(ClickMenuTile, , 0, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(TogglePipBoyLight, , 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(InitItemFilter, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetItemFilter, , 0, 1, kParams_JIP_OneOptionalString)
DEFINE_COMMAND_PLUGIN(ToggleHUDCursor, , 0, 1, kParams_OneOptionalInt)
DEFINE_COMMAND_PLUGIN(AddTileFromTemplate, , 0, 21, kParams_FormatString);
DEFINE_COMMAND_PLUGIN(SetUIFloatGradual, , 0, 5, kParams_JIP_OneString_ThreeOptionalFloats_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(CloseActiveMenu, , 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(ShowLevelUpMenuEx, , 0, 1, kParams_OneInt);

TileMenu* __fastcall GetMenuTile(const char *componentPath)
{
	UInt32 menuID = s_menuNameToID.Get(componentPath);
	return menuID ? g_tileMenuArray[menuID - kMenuType_Min] : NULL;
}

Tile* __fastcall GetTargetComponent(const char *componentPath, Tile::Value **value = NULL)
{
	char *slashPos = SlashPos(componentPath);
	if (!slashPos)
		return GetMenuTile(componentPath);
	*slashPos = 0;
	Tile *component = GetMenuTile(componentPath);
	if (!component)
		return NULL;
	const char *trait = NULL;
	component = component->GetComponent(slashPos + 1, &trait);
	if (!component)
		return NULL;
	if (trait)
	{
		if (!value || !(*value = component->GetValueName(trait)))
			return NULL;
	}
	else if (value)
		return NULL;
	return component;
}

Menu* __fastcall GetMenuByType(UInt32 menuID)
{
	menuID -= kMenuType_Min;
	if (menuID > 83) return NULL;
	TileMenu *tileMenu = g_tileMenuArray[menuID];
	return tileMenu ? tileMenu->menu : NULL;
}

bool Cmd_IsComponentLoaded_Execute(COMMAND_ARGS)
{
	if (ExtractArgs(EXTRACT_ARGS, &s_strArgBuffer) && GetTargetComponent(s_strArgBuffer))
		*result = 1;
	else *result = 0;
	return true;
}

bool Cmd_InjectUIXML_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (ExtractArgs(EXTRACT_ARGS, &s_strArgBuffer, &s_strValBuffer))
	{
		Tile *component = GetTargetComponent(s_strArgBuffer);
		if (component) *result = component->ReadXML(s_strValBuffer) ? 1 : 0;
	}
	return true;
}

const char kComponentTempXML[] = "jip_temp.xml";

bool Cmd_InjectUIComponent_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (ExtractFormatStringArgs(1, s_strValBuffer, EXTRACT_ARGS_EX, kCommandInfo_InjectUIComponent.numParams, &s_strArgBuffer))
	{
		Tile *component = GetTargetComponent(s_strArgBuffer);
		if (component)
		{
			FileStream tempFile;
			if (tempFile.Create(kComponentTempXML))
			{
				tempFile.WriteStr(s_strValBuffer);
				tempFile.Close();
				*result = component->ReadXML(kComponentTempXML) ? 1 : 0;
			}
		}
	}
	return true;
}

bool Cmd_GetCursorPos_Execute(COMMAND_ARGS)
{
	char axis;
	if (ExtractArgs(EXTRACT_ARGS, &axis))
		*result = (axis == 'X') ? g_interfaceManager->cursorX : g_interfaceManager->cursorY;
	else *result = 0;
	return true;
}

bool Cmd_GetUIString_Execute(COMMAND_ARGS)
{
	const char *resStr = NULL;
	if (ExtractArgs(EXTRACT_ARGS, &s_strArgBuffer))
	{
		Tile::Value *value = NULL;
		if (GetTargetComponent(s_strArgBuffer, &value) && value)
			resStr = value->str;
	}
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_GetActiveMenuMode_Execute(COMMAND_ARGS)
{
	*result = (int)g_interfaceManager->GetTopVisibleMenuID();
	return true;
}

bool Cmd_GetActiveUIComponentName_Execute(COMMAND_ARGS)
{
	const char *tileName = NULL;
	Tile *activeTile = g_interfaceManager->GetActiveTile();
	if (activeTile)
	{
		activeTile->GetComponentFullName(s_strValBuffer);
		tileName = SlashPosR(s_strValBuffer);
		if (tileName) tileName++;
	}
	AssignString(PASS_COMMAND_ARGS, tileName);
	return true;
}

bool Cmd_GetActiveUIComponentFullName_Execute(COMMAND_ARGS)
{
	s_strValBuffer[0] = 0;
	Tile *activeTile = g_interfaceManager->GetActiveTile();
	if (activeTile) activeTile->GetComponentFullName(s_strValBuffer);
	AssignString(PASS_COMMAND_ARGS, s_strValBuffer);
	return true;
}

SInt32 GetActiveTileID()
{
	Tile *activeTile = g_interfaceManager->GetActiveTile();
	if (activeTile)
	{
		Tile::Value *val = activeTile->GetValue(kTileValue_id);
		if (val) return val->num;
	}
	return -1;
}

bool Cmd_GetActiveUIComponentID_Execute(COMMAND_ARGS)
{
	*result = GetActiveTileID();
	return true;
}

bool Cmd_GetMenuTargetRef_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 menuID;
	if (!ExtractArgs(EXTRACT_ARGS, &menuID) || (menuID > kMenuType_Max)) return true;
	TileMenu *tileMenu = g_tileMenuArray[menuID - kMenuType_Min];
	Menu *menu = tileMenu ? tileMenu->menu : NULL;
	if (!menu) return true;
	TESForm *menuRef = NULL;
	switch (menuID)
	{
		case kMenuType_Container:
			menuRef = ((ContainerMenu*)menu)->containerRef;
			break;
		case kMenuType_Dialog:
		{
			menuRef = ((DialogMenu*)menu)->partnerRef;
			if (menuRef && IS_TYPE(((TESObjectREFR*)menuRef)->baseForm, BGSTalkingActivator))
			{
				ExtraTalkingActor *xTalker = GetExtraType(&((TESObjectREFR*)menuRef)->extraDataList, TalkingActor);
				if (xTalker) menuRef = xTalker->actor;
			}
			break;
		}
		case kMenuType_LockPick:
			menuRef = ((LockPickMenu*)menu)->targetRef;
			break;
		case kMenuType_Quantity:
		{
			if (g_menuVisibility[kMenuType_Inventory]) menuRef = CreateRefForStack(g_thePlayer, *g_inventoryMenuSelection);
			else if (*g_containerMenu)
			{
				ContainerMenu *cntMenu = *g_containerMenu;
				menuRef = CreateRefForStack((cntMenu->currentItems == &cntMenu->leftItems) ? g_thePlayer : cntMenu->containerRef, *g_containerMenuSelection);
			}
			else if (*g_barterMenu)
			{
				BarterMenu *brtMenu = *g_barterMenu;
				menuRef = CreateRefForStack((brtMenu->currentItems == &brtMenu->leftItems) ? g_thePlayer : brtMenu->merchantRef->GetMerchantContainer(), *g_barterMenuSelection);
			}
			break;
		}
		case kMenuType_Map:
		{
			MapMenu *mapMenu = (MapMenu*)menu;
			if (mapMenu->mapMarker)
			{
				static UInt32 valueID = 0;
				if (!valueID) valueID = TraitNameToID("_MarkerIndex");
				Tile::Value *markerIdx = mapMenu->mapMarker->GetValue(valueID);
				if (markerIdx) menuRef = mapMenu->mapMarkerList.GetNthItem(markerIdx->num);
			}
			break;
		}
		case kMenuType_Repair:
			menuRef = CreateRefForStack(g_thePlayer, *g_repairMenuTarget);
			break;
		case kMenuType_Barter:
			menuRef = ((BarterMenu*)menu)->merchantRef;
			break;
		case kMenuType_Hacking:
			menuRef = GetHackingMenuRef((HackingMenu*)menu);
			break;
		case kMenuType_VATS:
			menuRef = *g_VATSTargetRef;
			break;
		case kMenuType_Computers:
			menuRef = ((ComputersMenu*)menu)->targetRef;
			break;
		case kMenuType_ItemMod:
			menuRef = CreateRefForStack(g_thePlayer, *g_modMenuTarget);
			break;
		case kMenuType_CompanionWheel:
			menuRef = ((CompanionWheelMenu*)menu)->companionRef;
			break;
		case kMenuType_Recipe:
			menuRef = ((RecipeMenu*)menu)->sourceRef;
			break;
		case kMenuType_Caravan:
			menuRef = ((CaravanMenu*)menu)->opponentRef;
			break;
		case kMenuType_Trait:
		{
			TraitMenu *traitMenu = (TraitMenu*)menu;
			if (traitMenu->perkListBox.selected)
				menuRef = traitMenu->perkListBox.GetSelected();
		}
	}		
	if (menuRef) REFR_RES = menuRef->refID;
	return true;
}

bool Cmd_GetMenuItemFilter_Execute(COMMAND_ARGS)
{
	*result = -1;
	UInt32 menuID, useRef = 0;
	if (!ExtractArgs(EXTRACT_ARGS, &menuID, &useRef)) return true;
	switch (menuID)
	{
		case kMenuType_Inventory:
			if (*g_inventoryMenu) *result = (int)(*g_inventoryMenu)->filter + 1;
			break;
		case kMenuType_Container:
			if (*g_containerMenu) *result = useRef ? (int)(*g_containerMenu)->rightFilter : (int)(*g_containerMenu)->leftFilter;
			break;
		case kMenuType_Barter:
			if (*g_barterMenu) *result = useRef ? (int)(*g_barterMenu)->rightFilter : (int)(*g_barterMenu)->leftFilter;
	}
	return true;
}

bool Cmd_ClickMenuButton_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 times = 1;
	if (!ExtractArgs(EXTRACT_ARGS, &s_strArgBuffer, &times) || !GetMenuMode() || !times) return true;
	Tile *component = NULL;
	Menu *parentMenu = NULL;
	SInt32 tileID = -1;
	char *hashPos = FindChr(s_strArgBuffer, '#');
	if (hashPos)
	{
		tileID = StrToInt(hashPos + 1);
		if (tileID >= 0)
		{
			*hashPos = 0;
			TileMenu *tileMenu = GetMenuTile(s_strArgBuffer);
			if (tileMenu) parentMenu = tileMenu->menu;
		}
	}
	else
	{
		component = GetTargetComponent(s_strArgBuffer);
		if (!component) return true;
		parentMenu = component->GetParentMenu();
		Tile::Value *tileVal = component->GetValue(kTileValue_id);
		if (tileVal) tileID = tileVal->num;
	}
	if (parentMenu)
	{
		while (times--)
			parentMenu->HandleClick(tileID, component);
		*result = 1;
	}
	return true;
}

bool Cmd_GetSelectedItemRef_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *itemRef = NULL;
	switch (g_interfaceManager->GetTopVisibleMenuID())
	{
		case kMenuType_Inventory:
		{
			InventoryMenu *invMenu = *g_inventoryMenu;
			if (invMenu->itemList.selected)
				itemRef = CreateRefForStack(g_thePlayer, *g_inventoryMenuSelection);
			break;
		}
		case kMenuType_Stats:
		{
			StatsMenu *statsMenu = *g_statsMenu;
			if (statsMenu->perkRankList.selected)
				itemRef = statsMenu->perkRankList.GetSelected()->perk;
			break;
		}
		case kMenuType_Container:
		{
			ContainerMenu *cntMenu = *g_containerMenu;
			if (cntMenu->leftItems.selected || cntMenu->rightItems.selected)
				itemRef = CreateRefForStack(cntMenu->leftItems.selected ? g_thePlayer : cntMenu->containerRef, *g_containerMenuSelection);
			break;
		}
		case kMenuType_Map:
		{
			MapMenu *mapMenu = *g_mapMenu;
			if (mapMenu->questList.selected)
				itemRef = mapMenu->questList.GetSelected();
			else if (mapMenu->noteList.selected)
				itemRef = mapMenu->noteList.GetSelected();
			else if (mapMenu->challengeList.selected)
				itemRef = mapMenu->challengeList.GetSelected();
			break;
		}
		case kMenuType_Repair:
		{
			RepairMenu *rprMenu = *g_repairMenu;
			if (rprMenu->repairItems.selected)
				itemRef = CreateRefForStack(g_thePlayer, rprMenu->repairItems.GetSelected());
			break;
		}
		case kMenuType_Barter:
		{
			BarterMenu *brtMenu = *g_barterMenu;
			if (brtMenu->leftItems.selected || brtMenu->rightItems.selected)
				itemRef = CreateRefForStack(brtMenu->leftItems.selected ? g_thePlayer : brtMenu->merchantRef->GetMerchantContainer(), *g_barterMenuSelection);
			break;
		}
		case kMenuType_RepairServices:
		{
			RepairServicesMenu *rpsMenu = *g_repairServicesMenu;
			if (rpsMenu->itemList.selected)
				itemRef = CreateRefForStack(g_thePlayer, rpsMenu->itemList.GetSelected());
			break;
		}
		case kMenuType_ItemMod:
		{
			ItemModMenu *modMenu = *g_itemModMenu;
			if (modMenu->itemModList.selected)
				itemRef = CreateRefForStack(g_thePlayer, modMenu->itemModList.GetSelected());
		}
	}
	if (itemRef) REFR_RES = itemRef->refID;
	return true;
}

bool Cmd_GetBarterItems_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 sold;
	if (!ExtractArgs(EXTRACT_ARGS, &sold) || !*g_barterMenu || !(*g_barterMenu)->merchantRef) return true;
	BarterMenu *brtMenu = *g_barterMenu;
	TESObjectREFR *target = sold ? brtMenu->merchantRef->GetMerchantContainer() : g_thePlayer, *itemRef;
	s_tempElements.Clear();
	ListNode<ContChangesEntry> *iter = sold ? brtMenu->rightBarter.Head() : brtMenu->leftBarter.Head();
	do
	{
		if (itemRef = CreateRefForStack(target, iter->data))
			s_tempElements.Append(itemRef);
	}
	while (iter = iter->next);
	if (!s_tempElements.Empty())
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	return true;
}

bool Cmd_GetBarterGoldAlt_Execute(COMMAND_ARGS)
{
	*result = *g_barterMenu ? (int)(*g_barterMenu)->merchantGold : 0;
	return true;
}

bool Cmd_GetRecipeMenuSelection_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (*g_recipeMenu && *g_recipeMenuSelection) REFR_RES = (*g_recipeMenuSelection)->refID;
	return true;
}

bool Cmd_GetRecipeMenuCategory_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (*g_recipeMenu) REFR_RES = (*g_recipeMenu)->category->refID;
	return true;
}

bool Cmd_UnlockRecipeMenuQuantity_Execute(COMMAND_ARGS)
{
	UInt32 limit = 0x7FFFFFFF;
	if (ExtractArgs(EXTRACT_ARGS, &limit)) SafeWrite32(0x727975, limit);
	return true;
}

bool Cmd_GetRecipeMenuNumSubcategories_Execute(COMMAND_ARGS)
{
	*result = (int)g_recipeMenuCategories->size;
	return true;
}

const UInt32 kMsgIconsPathAddr[] = {0x10208A0, 0x10208E0, 0x1025CDC, 0x1030E78, 0x103A830, 0x1049638, 0x104BFE8};

bool Cmd_MessageExAlt_Execute(COMMAND_ARGS)
{
	float displayTime;
	if (!ExtractFormatStringArgs(1, s_strArgBuffer, EXTRACT_ARGS_EX, kCommandInfo_MessageExAlt.numParams, &displayTime))
		return true;
	char *msgPtr = GetNextToken(s_strArgBuffer, '|');
	msgPtr[0x203] = 0;
	if (*msgPtr)
	{
		if ((s_strArgBuffer[0] == '#') && (s_strArgBuffer[1] >= '0') && (s_strArgBuffer[1] <= '6') && !s_strArgBuffer[2])
			QueueUIMessage(msgPtr, 0, (const char*)kMsgIconsPathAddr[s_strArgBuffer[1] - '0'], NULL, displayTime, 0);
		else QueueUIMessage(msgPtr, 0, s_strArgBuffer, NULL, displayTime, 0);
	}
	else QueueUIMessage(s_strArgBuffer, 0, NULL, NULL, displayTime, 0);
	return true;
}

bool Cmd_GetFontFile_Execute(COMMAND_ARGS)
{
	const char *resStr = NULL;
	UInt32 fontID;
	if (ExtractArgs(EXTRACT_ARGS, &fontID) && fontID && (fontID <= 89) && (fontID != 9))
	{
		FontInfo *fontInfo = g_fontManager->fontInfos[fontID - 1];
		if (fontInfo) resStr = fontInfo->filePath;
	}
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetFontFile_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 fontID;
	if (!ExtractArgs(EXTRACT_ARGS, &fontID, s_dataPath) || !fontID || (fontID > 89) || (fontID == 9) || !*s_dataPath) return true;
	FontInfo *fontInfo = s_fontInfosMap.Get(s_dataPath);
	if (!fontInfo)
	{
		if (!FileExists(s_dataPathFull)) return true;
		fontInfo = (FontInfo*)GameHeapAlloc(sizeof(FontInfo));
		InitFontInfo(fontInfo, fontID, s_dataPath, true);
		if (!fontInfo->bufferData)
		{
			GameHeapFree(fontInfo);
			return true;
		}
		s_fontInfosMap[s_dataPath] = fontInfo;
	}
	g_fontManager->fontInfos[fontID - 1] = fontInfo;
	*result = 1;
	return true;
}

__declspec(naked) TextEditMenu* __stdcall ShowTextEditMenu(float width, float height, Script *callback)
{
	static const char altXMLPath[] = "Data\\NVSE\\plugins\\textinput\\texteditmenu.xml";
	__asm
	{
		push	esi
		push	edi
		mov		eax, 0x703DA0
		call	eax
		mov		esi, g_interfaceManager
		push	offset altXMLPath
		mov		ecx, [esi+0x9C]
		mov		eax, 0xA01B00
		call	eax
		mov		edi, [eax+0x3C]
		test	edi, edi
		jz		done
		mov		[edi+4], eax
		mov		dword ptr [edi+0x14], 0x41B
		push	0x41B
		mov		ecx, esi
		mov		eax, 0x714D90
		call	eax
		cmp		[esi+0xD0], edi
		jz		isActive
		mov		ecx, [esi+0xCC]
		test	ecx, ecx
		jz		resetActive
		push	1
		push	0
		push	kTileValue_mouseover
		mov		eax, kAddr_TileSetFloat
		call	eax
	resetActive:
		mov		dword ptr [esi+0xCC], 0
		mov		dword ptr [esi+0xD0], 0
	isActive:
		mov		ds:[0x11DAEC4], edi
		mov		eax, 0xA1DFB0
		call	eax
		push	1
		push	ecx
		fstp	dword ptr [esp]
		push	kTileValue_depth
		mov		ecx, [edi+4]
		mov		eax, kAddr_TileSetFloat
		call	eax
		mov		eax, [esp+0xC]
		push	1
		push	eax
		push	kTileValue_user0
		mov		ecx, [edi+4]
		mov		eax, kAddr_TileSetFloat
		call	eax
		mov		eax, [esp+0x10]
		push	1
		push	eax
		push	kTileValue_user1
		mov		ecx, [edi+4]
		mov		eax, kAddr_TileSetFloat
		call	eax
		mov		esi, offset s_strArgBuffer
		mov		[esi+0x50], 0
		cmp		[esi], 0
		jnz		hasTitle
		push	1
		push	0
		push	kTileValue_visible
		mov		ecx, [edi+0x30]
		mov		eax, kAddr_TileSetFloat
		call	eax
	hasTitle:
		push	1
		push	esi
		push	kTileValue_string
		mov		ecx, [edi+0x30]
		mov		eax, kAddr_TileSetString
		call	eax
		push	kTileValue_font
		mov		ecx, [edi+0x28]
		mov		eax, kAddr_TileGetFloat
		call	eax
		fistp	dword ptr [edi+0x4C]
		mov		ecx, [edi+0x4C]
		mov		esi, offset s_fontHeightDatas
		lea		esi, [esi+ecx*8]
		push	1
		push	dword ptr [esi]
		push	kTileValue_user0
		mov		ecx, [edi+0x28]
		mov		eax, kAddr_TileSetFloat
		call	eax
		push	1
		push	dword ptr [esi+4]
		push	kTileValue_user1
		mov		ecx, [edi+0x28]
		mov		eax, kAddr_TileSetFloat
		call	eax
		push	0x400
		lea		ecx, [edi+0x34]
		call	String::Init
		push	0x400
		lea		ecx, [edi+0x3C]
		call	String::Init
		mov		ecx, [edi+0x3C]
		mov		word ptr [ecx], '|'
		inc		word ptr [edi+0x40]
		push	1
		push	dword ptr ds:[0x11D38BC]
		push	kTileValue_string
		mov		ecx, [edi+0x2C]
		mov		eax, kAddr_TileSetString
		call	eax
		mov		dword ptr [edi+0x44], 0
		mov		dword ptr [edi+0x48], 0x7FFF0001
		mov		eax, [edi+0x28]
		mov		eax, [eax+0x28]
		mov		[edi+0x4C], eax
		mov		dword ptr [edi+0x54], 0x00000100
		mov		eax, [esp+0x14]
		mov		[edi+0x58], eax
		mov		ecx, edi
		call	TextInputRefreshHook
		push	0
		mov		ecx, edi
		mov		eax, 0xA1DC20
		call	eax
		push	1
		mov		ecx, offset s_hookInfos+kHook_TextInputClose*0x10
		call	HookInfo::Set
		push	TextInputRefreshHook
		push	0x1070060
		call	SafeWrite32
		push	TextInputKeyPressHook
		push	0x1070064
		call	SafeWrite32
	done:
		mov		eax, edi
		pop		edi
		pop		esi
		retn	0xC
	}
}

bool Cmd_ShowTextInputMenu_Execute(COMMAND_ARGS)
{
	Script *callback;
	float width, height;
	if (!*g_textEditMenu && ExtractFormatStringArgs(3, s_strArgBuffer, EXTRACT_ARGS_EX, kCommandInfo_ShowTextInputMenu.numParams,
		&callback, &width, &height) && IS_TYPE(callback, Script) && ShowTextEditMenu(width, height, callback))
		*result = 1;
	else *result = 0;
	return true;
}

bool Cmd_SetTextInputExtendedProps_Execute(COMMAND_ARGS)
{
	float posX, posY;
	UInt32 minLength = 1, maxLength = 0, miscFlags = 0;
	TextEditMenu *textMenu = *g_textEditMenu;
	if (textMenu && HOOK_INSTALLED(TextInputClose) && ExtractArgs(EXTRACT_ARGS, &posX, &posY, &minLength, &maxLength, &miscFlags))
	{
		if (posX > 0)
			textMenu->tile->SetFloat(kTileValue_user2, posX);
		if (posY > 0)
			textMenu->tile->SetFloat(kTileValue_user3, posY);
		textMenu->minLength = minLength;
		if (maxLength)
			textMenu->maxLength = maxLength & 0x7FFF;
		else
			textMenu->maxLength = 0x7FFF;
		textMenu->miscFlags = miscFlags & 3;
	}
	return true;
}

bool Cmd_SetTextInputString_Execute(COMMAND_ARGS)
{
	TextEditMenu *textMenu = *g_textEditMenu;
	if (textMenu && HOOK_INSTALLED(TextInputClose) && ExtractFormatStringArgs(0, s_strArgBuffer, EXTRACT_ARGS_EX, kCommandInfo_SetTextInputString.numParams))
	{
		textMenu->currentText.Set(s_strArgBuffer);
		textMenu->cursorIndex = 0;
		textMenu->HandleKeyboardInput(0x80000006);
	}
	return true;
}

bool Cmd_SetMessageDisabled_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 disable;
	if (!ExtractArgs(EXTRACT_ARGS, &form, &disable)) return true;
	ListNode<TESForm> *iter;
	if IS_TYPE(form, BGSListForm)
		iter = ((BGSListForm*)form)->list.Head();
	else
	{
		ListNode<TESForm> tempList(form);
		iter = &tempList;
	}
	do
	{
		if (!(form = iter->data) || NOT_TYPE(form, BGSMessage)) continue;
		form->SetJIPFlag(kHookFormFlag6_MessageDisabled, disable != 0);
	}
	while (iter = iter->next);
	HOOK_SET(ShowMessage, true);
	HOOK_SET(ShowTutorial, true);
	return true;
}

bool Cmd_GetMessageDisabled_Execute(COMMAND_ARGS)
{
	BGSMessage *message;
	if (ExtractArgs(EXTRACT_ARGS, &message) && (message->jipFormFlags6 & kHookFormFlag6_MessageDisabled)) *result = 1;
	else *result = 0;
	return true;
}

bool Cmd_GetMessageFlags_Execute(COMMAND_ARGS)
{
	BGSMessage *message;
	if (ExtractArgs(EXTRACT_ARGS, &message)) *result = message->msgFlags;
	else *result = 0;
	return true;
}

bool Cmd_SetMessageFlags_Execute(COMMAND_ARGS)
{
	BGSMessage *message;
	UInt32 flags;
	if (ExtractArgs(EXTRACT_ARGS, &message, &flags) && (flags <= 3))
		message->msgFlags = flags;
	return true;
}

bool Cmd_SetMessageDisplayTime_Execute(COMMAND_ARGS)
{
	BGSMessage *message;
	UInt32 displayTime;
	if (ExtractArgs(EXTRACT_ARGS, &message, &displayTime))
		message->displayTime = displayTime;
	return true;
}

bool Cmd_SetOnMenuClickEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	if (!ExtractArgs(EXTRACT_ARGS, &script, &addEvnt, &s_strArgBuffer) || NOT_TYPE(script, Script)) return true;
	char *slashPos = SlashPos(s_strArgBuffer), *hashPos = NULL;
	if (slashPos) *slashPos = 0;
	else
	{
		hashPos = FindChr(s_strArgBuffer, '#');
		if (hashPos) *hashPos = 0;
	}
	UInt32 menuID = s_menuNameToID.Get(s_strArgBuffer);
	if (!menuID) return true;
	MenuClickEvent &clickEvent = s_menuClickEventMap[kMenuIDJumpTable[menuID - kMenuType_Min]];
	if (slashPos) *slashPos = '/';
	else if (hashPos)
	{
		int tileID = StrToInt(hashPos + 1);
		if (tileID < 0) return true;
		if (addEvnt)
		{
			if (clickEvent.Empty())
				clickEvent.SetHook(true);
			clickEvent.idsMap[tileID].Insert(script);
		}
		else if (!clickEvent.idsMap.Empty())
		{
			auto findID = clickEvent.idsMap.Find(tileID);
			if (findID)
			{
				findID().Erase(script);
				if (findID().Empty()) findID.Remove();
				if (clickEvent.Empty())
					clickEvent.SetHook(false);
			}
		}
		return true;
	}
	FixPath(s_strArgBuffer);
	if (addEvnt)
	{
		bool match = false;
		if (!clickEvent.filtersMap.Empty())
		{
			for (auto filterb = clickEvent.filtersMap.FindOpDir(s_strArgBuffer, false); filterb; --filterb)
			{
				if (!StrBeginsCS(s_strArgBuffer, filterb.Key())) break;
				if (filterb().HasKey(script))
					return true;
			}
			char cmpr;
			for (auto filterf = clickEvent.filtersMap.FindOpDir(s_strArgBuffer, true); filterf; ++filterf)
			{
				cmpr = StrBeginsCS(filterf.Key(), s_strArgBuffer);
				if (!cmpr) break;
				if (cmpr == 2)
				{
					filterf().Insert(script);
					match = true;
				}
				else if (filterf().Erase(script) && filterf().Empty())
					filterf.Remove(clickEvent.filtersMap);
			}
		}
		if (!match)
		{
			if (clickEvent.Empty())
				clickEvent.SetHook(true);
			clickEvent.filtersMap[s_strArgBuffer].Insert(script);
		}
	}
	else if (!clickEvent.filtersMap.Empty())
	{
		for (auto filterf = clickEvent.filtersMap.FindOpDir(s_strArgBuffer, true); filterf; ++filterf)
		{
			if (!StrBeginsCS(filterf.Key(), s_strArgBuffer)) break;
			filterf().Erase(script);
			if (filterf().Empty()) filterf.Remove(clickEvent.filtersMap);
		}
		if (clickEvent.Empty())
			clickEvent.SetHook(false);
	}
	return true;
}

void SetOnMenuStateEvent(Script *script, bool doAdd, char idx, UInt8 evtType)
{
	MenuStateCallbacks *callbacks = s_menuStateEventMap[idx];
	if (!callbacks)
	{
		if (!doAdd) return;
		callbacks = (MenuStateCallbacks*)malloc(sizeof(MenuStateCallbacks));
		new (callbacks) MenuStateCallbacks();
		s_menuStateEventMap[idx] = callbacks;
	}
	EventCallbackScripts *scripts = (evtType == 2) ? &callbacks->onMouseover : (evtType ? &callbacks->onClose : &callbacks->onOpen);
	if (doAdd)
	{
		if (scripts->HasKey(script)) return;
		if (evtType == 2)
		{
			if (scripts->Empty())
				HOOK_MOD(MenuHandleMouseover, true);
		}
		else if (callbacks->onOpen.Empty() && callbacks->onClose.Empty())
		{
			HOOK_MOD(MenuStateOpen, true);
			HOOK_MOD(MenuStateClose, true);
		}
		scripts->Insert(script);
	}
	else
	{
		if (!scripts->Erase(script)) return;
		if (evtType == 2)
		{
			if (scripts->Empty())
				HOOK_MOD(MenuHandleMouseover, false);
		}
		else if (callbacks->onOpen.Empty() && callbacks->onClose.Empty())
		{
			HOOK_MOD(MenuStateOpen, false);
			HOOK_MOD(MenuStateClose, false);
		}
	}
}

bool SetOnMenuStateEventHandler_Execute(COMMAND_ARGS, UInt8 evtType)
{
	Script *script;
	UInt32 addEvnt, menuID = 0;
	if (ExtractArgs(EXTRACT_ARGS, &script, &addEvnt, &menuID) && (menuID <= kMenuType_Max) && IS_TYPE(script, Script))
	{
		char idx;
		bool doAdd = (addEvnt != 0);
		if (menuID)
		{
			idx = kMenuIDJumpTable[menuID - kMenuType_Min];
			if (idx != -1) SetOnMenuStateEvent(script, doAdd, idx, evtType);
		}
		else
		{
			for (idx = 0; idx < 35; idx++)
				SetOnMenuStateEvent(script, doAdd, idx, evtType);
		}
	}
	return true;
}

bool Cmd_SetOnMenuOpenEventHandler_Execute(COMMAND_ARGS)
{
	return SetOnMenuStateEventHandler_Execute(PASS_COMMAND_ARGS, 0);
}

bool Cmd_SetOnMenuCloseEventHandler_Execute(COMMAND_ARGS)
{
	return SetOnMenuStateEventHandler_Execute(PASS_COMMAND_ARGS, 1);
}

bool Cmd_SetOnMouseoverChangeEventHandler_Execute(COMMAND_ARGS)
{
	return SetOnMenuStateEventHandler_Execute(PASS_COMMAND_ARGS, 2);
}

__declspec(naked) void __fastcall RefreshRecipeMenu(RecipeMenu *menu)
{
	_asm
	{
		push	esi
		push	edi
		mov		esi, ecx
		add		ecx, 0x6C
		call	ListBox<TESRecipe>::Clear
		mov		ecx, esi
		push	dword ptr [ecx+0x64]
		push	0
		mov		eax, 0x727680
		call	eax
		add		esi, 0x6C
		mov		ecx, esi
		mov		edi, [ecx]
		push	0
		push	0
		call	dword ptr [edi+0x14]
		push	eax
		mov		ecx, esi
		call	dword ptr [edi]
		mov		ecx, esi
		call	dword ptr [edi+0x10]
		mov		ecx, esi
		mov		eax, 0x7312E0
		call	eax
		mov		eax, 0x727637
		push	dword ptr [eax]
		mov		ecx, esi
		mov		eax, 0x729FE0
		call	eax
		push	1
		mov		ecx, esi
		mov		eax, 0x72A660
		call	eax
		pop		edi
		pop		esi
		retn
	}
}

bool Cmd_RefreshItemsList_Execute(COMMAND_ARGS)
{
	if (*g_recipeMenu)
		RefreshRecipeMenu(*g_recipeMenu);
	else RefreshItemListBox();
	return true;
}

bool Cmd_GetBarterPriceMult_Execute(COMMAND_ARGS)
{
	UInt32 sellMult;
	if (ExtractArgs(EXTRACT_ARGS, &sellMult) && *g_barterMenu)
		*result = sellMult ? (*g_barterMenu)->sellValueMult : (*g_barterMenu)->buyValueMult;
	else *result = 0;
	return true;
}

bool Cmd_SetBarterPriceMult_Execute(COMMAND_ARGS)
{
	UInt32 sellMult;
	float valueMult;
	if (ExtractArgs(EXTRACT_ARGS, &sellMult, &valueMult) && *g_barterMenu)
	{
		if (sellMult) (*g_barterMenu)->sellValueMult = valueMult;
		else (*g_barterMenu)->buyValueMult = valueMult;
		RefreshItemListBox();
	}
	return true;
}

void DoPurgePath(char *path)
{
	if (!path) return;
	if (*g_terminalModelPtr == path)
	{
		PurgeTerminalModel();
		*g_terminalModelPtr = g_terminalModelDefault;
	}
	free(path);
}

bool Cmd_SetTerminalUIModel_Execute(COMMAND_ARGS)
{
	TESForm *form;
	s_strArgBuffer[0] = 0;
	if (!ExtractArgs(EXTRACT_ARGS, &form, &s_strArgBuffer)) return true;
	bool bRemove = !s_strArgBuffer[0];
	ListNode<TESForm> *lstIter;
	if IS_TYPE(form, BGSListForm)
		lstIter = ((BGSListForm*)form)->list.Head();
	else
	{
		ListNode<TESForm> tempList(form);
		lstIter = &tempList;
	}
	BGSTerminal *terminal;
	char **pathPtr;
	do
	{
		terminal = (BGSTerminal*)lstIter->data;
		if (!terminal || NOT_TYPE(terminal, BGSTerminal)) continue;
		if (bRemove) DoPurgePath(s_terminalAltModelsMap.GetErase(terminal));
		else
		{
			if (!s_terminalAltModelsMap.Insert(terminal, &pathPtr)) DoPurgePath(*pathPtr);
			*pathPtr = CopyString(s_strArgBuffer);
		}
	}
	while (lstIter = lstIter->next);
	HOOK_SET(SetTerminalModel, !s_terminalAltModelsMap.Empty());
	return true;
}

Script *s_quantityMenuScript = NULL;

__declspec(naked) void QuantityMenuCallback(int count)
{
	__asm
	{
		mov		eax, s_quantityMenuScript
		test	eax, eax
		jz		done
		mov		s_quantityMenuScript, 0
		mov		ecx, [esp+4]
		push	ecx
		push	1
		push	offset s_callRes
		push	0
		push	0
		push	eax
		call	CallFunction
		add		esp, 0x18
	done:
		retn
	}
}

bool Cmd_ShowQuantityMenu_Execute(COMMAND_ARGS)
{
	Script *callback;
	int maxCount;
	if (!*g_quantityMenu && !s_quantityMenuScript && ExtractArgs(EXTRACT_ARGS, &callback, &maxCount) && 
		IS_TYPE(callback, Script) && ShowQuantityMenu(maxCount, QuantityMenuCallback, maxCount))
		s_quantityMenuScript = callback;
	return true;
}

__declspec(naked) void MessageBoxCallback()
{
	__asm
	{
		mov		eax, s_messageBoxScript
		mov		s_messageBoxScript, 0
		mov		ecx, g_interfaceManager
		movzx	edx, byte ptr [ecx+0xE4]
		cmp		dl, 0xFF
		jz		done
		mov		byte ptr [ecx+0xE4], 0xFF
		test	eax, eax
		jz		done
		cmp		dword ptr [eax], kVtbl_Script
		jnz		done
		push	edx
		push	1
		push	offset s_callRes
		push	0
		push	0
		push	eax
		call	CallFunction
		add		esp, 0x18
	done:
		retn
	}
}

__declspec(naked) void __fastcall MessageBoxExAlt(char **msgStrings)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	0
	addButtons:
		mov		eax, [ecx]
		sub		ecx, 4
		test	eax, eax
		jz		doneButtons
		push	eax
		jmp		addButtons
	doneButtons:
		push	0
		push	0
		push	0x17
		push	0
		push	MessageBoxCallback
		push	dword ptr [ecx-4]
		push	0
		push	dword ptr [ecx]
		call	ShowMessageBox
		mov		esp, ebp
		pop		ebp
		retn
	}
}

bool Cmd_MessageBoxExAlt_Execute(COMMAND_ARGS)
{
	Script *callback;
	if (!s_messageBoxScript && ExtractFormatStringArgs(1, s_strArgBuffer, EXTRACT_ARGS_EX, kCommandInfo_MessageBoxExAlt.numParams, &callback))
	{
		s_messageBoxScript = callback;
		char *msgStrings[0x102], **buttonPtr = msgStrings + 2, *delim = s_strArgBuffer;
		*buttonPtr = NULL;
		for (UInt32 count = 0xFF; count; count--)
		{
			delim = GetNextToken(delim, '|');
			if (!*delim) break;
			*++buttonPtr = delim;
		}
		if (!*buttonPtr) *++buttonPtr = "OK";
		if ((s_strArgBuffer[0] == '^') && (delim = FindChr(s_strArgBuffer + 1, '^')))
		{
			*delim = 0;
			*msgStrings = s_strArgBuffer + 1;
			msgStrings[1] = delim + 1;
		}
		else
		{
			*msgStrings = NULL;
			msgStrings[1] = s_strArgBuffer;
		}
		MessageBoxExAlt(buttonPtr);
	}
	return true;
}

bool Cmd_GetVATSTargets_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (!*g_VATSMenu) return true;
	s_tempElements.Clear();
	ListNode<VATSTargetInfo> *iter = g_VATSTargetList->Head();
	VATSTargetInfo *targetInfo;
	do
	{
		targetInfo = iter->data;
		if (targetInfo && targetInfo->targetRef)
			s_tempElements.Append(targetInfo->targetRef);
	}
	while (iter = iter->next);
	if (!s_tempElements.Empty())
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	return true;
}

bool InCharGen()
{
	return g_tileMenuArray[kMenuType_CharGen - kMenuType_Min] || g_tileMenuArray[kMenuType_SpecialBook - kMenuType_Min] || 
		g_tileMenuArray[kMenuType_LoveTester - kMenuType_Min] || g_tileMenuArray[kMenuType_Trait - kMenuType_Min];
}

bool Cmd_IsInCharGen_Execute(COMMAND_ARGS)
{
	*result = InCharGen();
	return true;
}

bool Cmd_IsInCharGen_Eval(COMMAND_ARGS_EVAL)
{
	*result = InCharGen();
	return true;
}

bool s_craftingMessages = true;

bool Cmd_ToggleCraftingMessages_Execute(COMMAND_ARGS)
{
	*result = s_craftingMessages;
	UInt32 toggle;
	if (NUM_ARGS && ExtractArgs(EXTRACT_ARGS, &toggle) && (s_craftingMessages == !toggle))
	{
		s_craftingMessages = !s_craftingMessages;
		SafeWriteBuf(0x728933, s_craftingMessages ? "\x8D\x4D\xCC\xE8\x75" : "\xE9\xC4\x00\x00\x00", 5);
	}
	return true;
}

bool Cmd_SetCursorPos_Execute(COMMAND_ARGS)
{
	float posX, posY;
	if (ExtractArgs(EXTRACT_ARGS, &posX, &posY) && GetMenuMode())
	{
		NiNode *cursorNode = g_interfaceManager->cursor ? g_interfaceManager->cursor->node : NULL;
		if (cursorNode)
		{
			double mult = 480.0 / (int)g_screenHeight;
			cursorNode->m_localTranslate.x = ((posX * 2) - (int)g_screenWidth) * mult;
			cursorNode->m_localTranslate.z = ((int)g_screenHeight - (posY * 2)) * mult;
		}
	}
	return true;
}

bool Cmd_UnloadUIComponent_Execute(COMMAND_ARGS)
{
	if (ExtractArgs(EXTRACT_ARGS, &s_strArgBuffer))
	{
		Tile *component = GetTargetComponent(s_strArgBuffer);
		if (component) component->Destroy(true);
	}
	return true;
}

bool Cmd_ClearMessageQueue_Execute(COMMAND_ARGS)
{
	if (!g_HUDMainMenu->queuedMessages.Empty())
	{
		ListNode<HUDMainMenu::QueuedMessage> *headNode = g_HUDMainMenu->queuedMessages.Head(), *msgIter = headNode->next;
		while (msgIter)
		{
			GameHeapFree(msgIter->data);
			msgIter = msgIter->RemoveMe();
		}
		if (!g_HUDMainMenu->currMsgKey)
		{
			GameHeapFree(headNode->data);
			headNode->RemoveMe();
		}
		else if (headNode->next)
			headNode->RemoveNext();
	}
	return true;
}

bool Cmd_SetSystemColor_Execute(COMMAND_ARGS)
{
	UInt32 type, red, green, blue;
	if (ExtractArgs(EXTRACT_ARGS, &type, &red, &green, &blue) && type && (type <= 5))
	{
		DListNode<SystemColorManager::SystemColor> *colorNode = g_sysColorManager->sysColors.Head()->Advance(type - 1);
		if (colorNode && colorNode->data)
		{
			colorNode->data->SetColorRGB(red, green, blue);
			ThisCall(0xA0B350, g_interfaceManager->menuRoot, type, 0);
		}
	}
	return true;
}

__declspec(naked) void QueueQuestMsgHook(TESQuest *quest)
{
	__asm
	{
		mov		eax, [esp+4]
		mov		ecx, g_thePlayer
		mov		edx, [ecx+0x6B8]
		test	byte ptr [eax+0x3C], 0x42
		jnz		clrCurrent
		test	edx, edx
		jnz		done
		push	eax
		jmp		setCurrent
	clrCurrent:
		cmp		eax, edx
		jnz		done
		push	0
	setCurrent:
		mov		eax, 0x9529D0
		call	eax
	done:
		retn
	}
}

bool s_showQuestMessages = true, *g_showChallengeUpdates = (bool*)0x11CB4D8;

void ToggleQuestMessages()
{
	s_showQuestMessages = !s_showQuestMessages;
	*g_showChallengeUpdates = s_showQuestMessages;
	if (s_showQuestMessages)
	{
		SafeWriteBuf(0x77A480, "\x55\x8B\xEC\x83\xEC", 5);
		SafeWrite8(0x77A5B0, 0x55);
	}
	else
	{
		WriteRelJump(0x77A480, (UInt32)QueueQuestMsgHook);
		SafeWrite8(0x77A5B0, 0xC3);
	}
}

bool Cmd_SuppressQuestMessages_Execute(COMMAND_ARGS)
{
	if (s_showQuestMessages)
	{
		ToggleQuestMessages();
		MainLoopAddCallback(ToggleQuestMessages);
	}
	return true;
}

__declspec(naked) void RecipeMenuAcceptHook()
{
	__asm
	{
		push	kTileValue_target
		mov		ecx, [ebp-4]
		mov		ecx, [ecx+0x44]
		mov		eax, 0xA01230
		call	eax
		test	al, al
		jz		done
		push	0x7FFFFFFF
		push	0x7284F0
		push	dword ptr ds:[0x11D8EA8]
		mov		eax, 0x7ABA00
		call	eax
	done:
		mov		esp, ebp
		pop		ebp
		retn	8
	}
}

__declspec(naked) void RecipeMenuCloseHook()
{
	__asm
	{
		cmp		s_craftingMessages, 0
		jnz		doneSound
		push	0
		push	1
		push	dword ptr [ebp-0x20]
		mov		eax, 0x8ADCF0
		call	eax
		test	eax, eax
		jz		doneSound
		push	0
		push	0x121
		push	0
		push	eax
		lea		eax, [ebp-0x44]
		push	eax
		mov		ecx, g_thePlayer
		mov		eax, 0x933270
		call	eax
	doneSound:
		mov		esi, ds:[0x11D8E90]
		test	esi, esi
		jz		done
		push	1
		mov		ecx, g_thePlayer
		mov		ecx, [ecx+0x68]
		mov		eax, [ecx]
		call	dword ptr [eax+0x474]
		test	al, al
		jz		doRefresh
		mov		ecx, g_thePlayer
		mov		eax, 0x950030
		call	eax
	doRefresh:
		mov		ecx, esi
		call	RefreshRecipeMenu
		mov		ecx, g_interfaceManager
		mov		ecx, [ecx+0x28]
		test	ecx, ecx
		jz		done
		mov		ecx, [ecx+0x2C]
		test	ecx, ecx
		jz		done
		add		ecx, 0x58
		xor		eax, eax
		mov		[ecx], eax
		mov		[ecx+8], eax
	done:
		mov		ecx, [ebp-0xC]
		mov		fs:0, ecx
		pop		ecx
		pop		esi
		mov		esp, ebp
		pop		ebp
		retn
	}
}

bool Cmd_EnableImprovedRecipeMenu_Execute(COMMAND_ARGS)
{
	static bool enabled = false;
	if (!enabled)
	{
		enabled = true;
		SafeWrite32(0x727975, 0x7FFFFFFF);
		SafeWrite8(0x7274E1, 0xF);
		SafeWriteBuf(0x727675, "\x03\x03\x03\x03\x03\x03\x01", 7);
		SafeWrite32(0x727660, (UInt32)RecipeMenuAcceptHook);
		WriteRelJump(0x728A0C, (UInt32)RecipeMenuCloseHook);
	}
	return true;
}

bool Cmd_ClickMenuTile_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (ExtractArgs(EXTRACT_ARGS, &s_strArgBuffer) && GetMenuMode())
	{
		Tile *component = GetTargetComponent(s_strArgBuffer);
		if (component && (component->GetValueFloat(kTileValue_target) > 0))
		{
			component->FakeClick();
			*result = 1;
		}
	}
	return true;
}

__declspec(naked) void __stdcall TogglePipBoyLight(UInt32 turnON)
{
	__asm
	{
		push	esi
		mov		esi, g_pipBoyLight
		mov		ecx, g_thePlayer
		push	0
		cmp		dword ptr [esp+0xC], 0
		jz		turnOFF
		push	esi
		add		ecx, 0x88
		mov		eax, [ecx]
		call	dword ptr [eax]
		jmp		finish
	turnOFF:
		push	0
		add		esi, 0x18
		push	esi
		add		ecx, 0x94
		mov		eax, 0x824400
		call	eax
	finish:
		mov		esi, g_interfaceManager
		mov		esi, [esi+0x174]
		push	1
		push	dword ptr [esp+0xC]
		push	1
		mov		ecx, esi
		mov		eax, 0x7FA310
		call	eax
		push	1
		push	dword ptr [esp+0xC]
		push	0
		mov		ecx, esi
		mov		eax, 0x7FA310
		call	eax
		pop		esi
		retn	4
	}
}

bool Cmd_TogglePipBoyLight_Execute(COMMAND_ARGS)
{
	UInt32 turnON, currState = ThisCall<bool>(0x822B90, &g_thePlayer->magicTarget, &g_pipBoyLight->magicItem, 1);
	*result = (int)currState;
	if (NUM_ARGS && ExtractArgs(EXTRACT_ARGS, &turnON) && (turnON != currState))
		TogglePipBoyLight(turnON);
	return true;
}

char *s_itemFilterString = NULL;

__declspec(naked) bool __fastcall ShouldHideItem(TESForm *itemForm)
{
	__asm
	{
		mov		eax, ecx
		test	eax, eax
		jz		done
		mov		edx, s_itemFilterString
		mov		al, [edx]
		test	al, al
		jz		done
		mov		al, [ecx+4]
		cmp		al, kFormType_TESRecipe
		jnz		notRecipe
		mov		ecx, [ecx+0x1C]
		jmp		checkName
	notRecipe:
		cmp		al, kFormType_BGSNote
		jnz		notNote
		mov		ecx, [ecx+0x4C]
		jmp		checkName
	notNote:
		mov		ecx, [ecx+0x34]
	checkName:
		call	SubStrCI
		test	eax, eax
		setz	al
	done:
		retn
	}
}

__declspec(naked) void ItemFilterHook()
{
	__asm
	{
		test	al, al
		jnz		done
		mov		ecx, [ebp+8]
		test	ecx, ecx
		jz		done
		mov		ecx, [ecx+8]
		call	ShouldHideItem
	done:
		mov		esp, ebp
		pop		ebp
		retn
	}
}

__declspec(naked) void RecipeFilterHook()
{
	__asm
	{
		test	al, al
		jnz		done
		mov		ecx, [ebp+8]
		call	ShouldHideItem
	done:
		pop		esi
		pop		ebp
		retn
	}
}

bool Cmd_InitItemFilter_Execute(COMMAND_ARGS)
{
	if (!s_itemFilterString)
	{
		s_itemFilterString = (char*)malloc(0x100);
		*s_itemFilterString = 0;
		WriteRelJump(0x7827F1, (UInt32)ItemFilterHook);
		WriteRelJump(0x75E89F, (UInt32)ItemFilterHook);
		WriteRelJump(0x730671, (UInt32)ItemFilterHook);
		WriteRelJump(0x728D99, (UInt32)RecipeFilterHook);
	}
	return true;
}

bool Cmd_SetItemFilter_Execute(COMMAND_ARGS)
{
	if (s_itemFilterString)
	{
		*s_itemFilterString = 0;
		ExtractArgs(EXTRACT_ARGS, s_itemFilterString);
	}
	return true;
}

bool Cmd_ToggleHUDCursor_Execute(COMMAND_ARGS)
{
	*result = s_HUDCursorMode;
	UInt32 toggle;
	if (NUM_ARGS && ExtractArgs(EXTRACT_ARGS, &toggle) && (g_interfaceManager->currentMode == 1) && !s_controllerReady && (s_HUDCursorMode == !toggle))
	{
		s_HUDCursorMode = !s_HUDCursorMode;
		if (toggle)
		{
			g_interfaceManager->byte170 = 0;
			g_interfaceManager->isRenderedMenuOrPipboyManager = 0;
		}
		else
		{
			g_interfaceManager->cursor->SetFloat(kTileValue_visible, 0);
			g_interfaceManager->cursor->node->m_flags |= 1;
			if (g_interfaceManager->activeTile)
				g_interfaceManager->activeTile->SetFloat(kTileValue_mouseover, 0);
		}
		g_interfaceManager->activeTile = NULL;
		g_interfaceManager->activeMenu = NULL;
		g_DIHookCtrl->SetLMBDisabled(s_HUDCursorMode);
		s_lastLMBState = false;
	}
	return true;
}

bool Cmd_AddTileFromTemplate_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (ExtractFormatStringArgs(0, s_strArgBuffer, EXTRACT_ARGS_EX, kCommandInfo_AddTileFromTemplate.numParams))
	{
		char *tempName = GetNextToken(s_strArgBuffer, '|');
		if (!*tempName) return true;
		char *altName = GetNextToken(tempName, '|');
		TileMenu *menu;
		Tile *component = NULL;
		char *slashPos = SlashPos(s_strArgBuffer);
		if (slashPos)
		{
			*slashPos = 0;
			menu = GetMenuTile(s_strArgBuffer);
			if (!menu) return true;
			const char *trait = NULL;
			component = menu->GetComponent(slashPos + 1, &trait);
			if (trait) return true;
		}
		else
		{
			menu = GetMenuTile(s_strArgBuffer);
			component = menu;
		}
		if (component)
		{
			component = menu->menu->AddTileFromTemplate(component, tempName, 0);
			if (component)
			{
				*result = 1;
				if (*altName)
					component->name.Set(altName);
			}
		}
	}
	return true;
}

bool Cmd_SetUIFloatGradual_Execute(COMMAND_ARGS)
{
	static const UInt8 kChangeModeMatch[] = {0, 4, 1, 5};
	float startVal, endVal, timer;
	UInt32 changeMode = 0;
	UInt8 numArgs = NUM_ARGS;
	if (ExtractArgs(EXTRACT_ARGS, &s_strArgBuffer, &startVal, &endVal, &timer, &changeMode))
	{
		Tile::Value *tileVal = NULL;
		Tile *component = GetTargetComponent(s_strArgBuffer, &tileVal);
		if (component)
		{
			if (numArgs >= 4)
			{
				if (changeMode > 3) changeMode = 0;
				else changeMode = kChangeModeMatch[changeMode];
				component->GradualSetFloat(tileVal->id, startVal, endVal, timer, changeMode);
			}
			else
			{
				ThisCall(0xA07DC0, component, tileVal->id);
				if (numArgs >= 2)
					tileVal->SetFloat(startVal);
			}
		}
	}
	return true;
}

bool Cmd_CloseActiveMenu_Execute(COMMAND_ARGS)
{
	UInt32 closeAll = 0;
	if (ExtractArgs(EXTRACT_ARGS, &closeAll) && (g_interfaceManager->currentMode > 1))
	{
		UInt32 index = 10, *mnStack = &g_interfaceManager->menuStack[9], menuID;
		do
		{
			menuID = *mnStack--;
			if (!menuID) continue;
			if (menuID >= kMenuType_Min)
			{
				TileMenu *tileMenu = g_tileMenuArray[menuID - kMenuType_Min];
				if (tileMenu)
					tileMenu->Destroy(true);
			}
			else if (menuID == 1)
			{
				g_interfaceManager->pipBoyMode = 4;
				g_interfaceManager->pipBoyModeCallback = NULL;
			}
			if (!closeAll) break;
		}
		while (--index);
	}
	return true;
}

bool Cmd_ShowLevelUpMenuEx_Execute(COMMAND_ARGS)
{
	UInt32 skillPoints;
	if (ExtractArgs(EXTRACT_ARGS, &skillPoints))
	{
		CdeclCall(0x784C80);
		LevelUpMenu *menu = *(LevelUpMenu**)0x11D9FDC;
		if (menu)
		{
			menu->numSkillPointsToAssign = skillPoints;
			ThisCall(0x785830, menu, 0);
		}
	}
	return true;
}