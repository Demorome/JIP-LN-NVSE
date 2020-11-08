#pragma once

DEFINE_COMMAND_PLUGIN(GetNumEffects, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetNthEffectBase, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthEffectBase, , 0, 3, kParams_JIP_OneForm_OneInt_OneMagicEffect);
DEFINE_COMMAND_PLUGIN(GetNthEffectTraitNumeric, , 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetNthEffectTraitNumeric, , 0, 4, kParams_JIP_OneForm_ThreeInts);
DEFINE_COMMAND_PLUGIN(AddNewEffect, , 0, 6, kParams_JIP_OneForm_OneEffect_TwoInts_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(RemoveNthEffect, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetObjectEffect, , 0, 2, kParams_JIP_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetNumActorEffects, , 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetNthActorEffect, , 0, 2, kParams_JIP_OneInt_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetActorUnarmedEffect, , 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetActorUnarmedEffect, , 0, 2, kParams_JIP_OneForm_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetAddictionEffect, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetAddictionEffect, , 0, 2, kParams_JIP_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetAddictionChance, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetAddictionChance, , 0, 2, kParams_OneForm_OneFloat);
DEFINE_COMMAND_PLUGIN(GetIsPoison, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetActiveEffects, , 1, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetTempEffects, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(RemoveNthTempEffect, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetEffectFlag, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetEffectFlag, , 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(GetBaseEffectFlag, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetBaseEffectFlag, , 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(GetBaseEffectScript, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetBaseEffectScript, , 0, 2, kParams_JIP_TwoForms);
DEFINE_CMD_ALT_COND_PLUGIN(IsSpellTargetAlt, , , 1, kParams_JIP_OneMagicItem);
DEFINE_COMMAND_PLUGIN(CastImmediate, , 1, 2, kParams_JIP_OneMagicItem_OneOptionalActor);

bool Cmd_GetNumEffects_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form;
	if (ExtractArgs(EXTRACT_ARGS, &form))
	{
		EffectItemList *effList = DYNAMIC_CAST(form, TESForm, EffectItemList);
		if (effList) *result = (int)effList->list.Count();
	}
	return true;
}

bool Cmd_GetNthEffectBase_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form;
	UInt32 idx;
	if (ExtractArgs(EXTRACT_ARGS, &form, &idx))
	{
		EffectItemList *effList = DYNAMIC_CAST(form, TESForm, EffectItemList);
		if (effList)
		{
			EffectItem *effItem = effList->list.GetNthItem(idx);
			if (effItem && effItem->setting)
				REFR_RES = effItem->setting->refID;
		}
	}
	return true;
}

bool Cmd_SetNthEffectBase_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 idx;
	EffectSetting *effSetting;
	if (ExtractArgs(EXTRACT_ARGS, &form, &idx, &effSetting))
	{
		MagicItem *magicItem = DYNAMIC_CAST(form, TESForm, MagicItem);
		if (magicItem)
		{
			EffectItem *effItem = magicItem->list.list.GetNthItem(idx);
			if (effItem)
			{
				effItem->setting = effSetting;
				effItem->actorValueOrOther = effSetting->actorVal;
				UpdateEffectsAllActors(magicItem, effItem);
			}
		}
	}
	return true;
}

bool Cmd_GetNthEffectTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form;
	UInt32 idx, traitID;
	if (!ExtractArgs(EXTRACT_ARGS, &form, &idx, &traitID)) return true;
	EffectItemList *effList = DYNAMIC_CAST(form, TESForm, EffectItemList);
	if (!effList) return true;
	EffectItem *effItem = effList->list.GetNthItem(idx);
	if (!effItem) return true;
	switch (traitID)
	{
	case 0:
		*result = (int)effItem->magnitude;
		break;
	case 1:
		*result = (int)effItem->area;
		break;
	case 2:
		*result = (int)effItem->duration;
		break;
	case 3:
		*result = (int)effItem->range;
	}
	return true;
}

bool Cmd_SetNthEffectTraitNumeric_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 idx, traitID, val;
	if (!ExtractArgs(EXTRACT_ARGS, &form, &idx, &traitID, &val) || (traitID > 2)) return true;
	MagicItem *magicItem = DYNAMIC_CAST(form, TESForm, MagicItem);
	if (!magicItem) return true;
	EffectItem *effItem = magicItem->list.list.GetNthItem(idx);
	if (!effItem) return true;
	switch (traitID)
	{
	case 0:
		effItem->magnitude = val;
		break;
	case 1:
		effItem->area = val;
		break;
	case 2:
		effItem->duration = val;
	}
	UpdateEffectsAllActors(magicItem, effItem);
	return true;
}

bool Cmd_AddNewEffect_Execute(COMMAND_ARGS)
{
	*result = -1;
	TESForm *form;
	EffectSetting *effSetting;
	UInt32 duration, magnitude, range = 0, area = 0;
	if (!ExtractArgs(EXTRACT_ARGS, &form, &effSetting, &duration, &magnitude, &range, &area) || (range > 2)) return true;
	MagicItem *magicItem = DYNAMIC_CAST(form, TESForm, MagicItem);
	if (!magicItem) return true;
	if IS_TYPE(form, AlchemyItem) area = range = 0;
	else if IS_TYPE(form, EnchantmentItem)
	{
		if (((EnchantmentItem*)form)->type == 2) range = 1;
		else area = range = 0;
	}
	else
	{
		SpellItem *spelItem = (SpellItem*)form;
		if ((spelItem->type == 1) || (spelItem->type == 4) || (spelItem->type == 10)) range = 0;
		if (!range) area = 0;
	}
	if (!(effSetting->effectFlags & (16 << range))) return true;
	EffectItem *effItem = (EffectItem*)GameHeapAlloc(sizeof(EffectItem));
	effItem->magnitude = magnitude;
	effItem->area = area;
	effItem->duration = duration;
	effItem->range = range;
	effItem->actorValueOrOther = effSetting->actorVal;
	effItem->setting = effSetting;
	effItem->cost = 0;
	effItem->conditions.Init();
	magicItem->list.list.Insert(effItem);
	*result = (int)magicItem->list.list.Count();
	UpdateEffectsAllActors(magicItem, effItem, true);
	return true;
}

bool Cmd_RemoveNthEffect_Execute(COMMAND_ARGS)
{
	*result = -1;
	TESForm *form;
	SInt32 idx;
	if (ExtractArgs(EXTRACT_ARGS, &form, &idx))
	{
		EffectItemList *effList = DYNAMIC_CAST(form, TESForm, EffectItemList);
		if (effList && effList->RemoveNthEffect(idx))
			*result = (int)effList->list.Count();
	}
	return true;
}

bool Cmd_SetObjectEffect_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form;
	EnchantmentItem *effect = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &form, &effect)) return true;
	TESEnchantableForm *enchantable = DYNAMIC_CAST(form, TESForm, TESEnchantableForm);
	if (!enchantable) return true;
	if (effect && IS_TYPE(effect, EnchantmentItem))
	{
		if IS_TYPE(form, TESObjectARMO)
		{
			if (effect->type != 3) return true;
		}
		else if (effect->type != 2) return true;
		enchantable->enchantItem = effect;
	}
	else enchantable->enchantItem = NULL;
	*result = 1;
	return true;
}

bool Cmd_GetNumActorEffects_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = (TESActorBase*)thisObj->baseForm;
	}
	*result = (int)actorBase->spellList.spellList.Count();
	return true;
}

bool Cmd_GetNthActorEffect_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 idx;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &idx, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = (TESActorBase*)thisObj->baseForm;
	}
	SpellItem *splItem = actorBase->spellList.spellList.GetNthItem(idx);
	if (splItem) REFR_RES = splItem->refID;
	return true;
}

bool Cmd_GetActorUnarmedEffect_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = (TESActorBase*)thisObj->baseForm;
	}
	if (actorBase->touchSpell.unarmedEffect) REFR_RES = actorBase->touchSpell.unarmedEffect->refID;
	return true;
}

bool Cmd_SetActorUnarmedEffect_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *effForm;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &effForm, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = (TESActorBase*)thisObj->baseForm;
	}
	if IS_TYPE(effForm, EnchantmentItem)
	{
		if (((EnchantmentItem*)effForm)->type != 2) return true;
	}
	else if IS_TYPE(effForm, SpellItem)
	{
		if (((SpellItem*)effForm)->type) return true;
	}
	else return true;
	actorBase->touchSpell.unarmedEffect = effForm;
	*result = 1;
	return true;
}

bool Cmd_GetAddictionEffect_Execute(COMMAND_ARGS)
{
	*result = 0;
	AlchemyItem *alch;
	if (ExtractArgs(EXTRACT_ARGS, &alch) && IS_TYPE(alch, AlchemyItem) && alch->withdrawalEffect)
		REFR_RES = alch->withdrawalEffect->refID;
	return true;
}

bool Cmd_SetAddictionEffect_Execute(COMMAND_ARGS)
{
	AlchemyItem *alch;
	SpellItem *effect = NULL;
	if (ExtractArgs(EXTRACT_ARGS, &alch, &effect) && IS_TYPE(alch, AlchemyItem))
		alch->withdrawalEffect = (effect && IS_TYPE(effect, SpellItem)) ? effect : NULL;
	return true;
}

bool Cmd_GetAddictionChance_Execute(COMMAND_ARGS)
{
	AlchemyItem *alch;
	if (ExtractArgs(EXTRACT_ARGS, &alch) && IS_TYPE(alch, AlchemyItem) && alch->withdrawalEffect)
		*result = alch->addictionChance;
	else *result = 0;
	return true;
}

bool Cmd_SetAddictionChance_Execute(COMMAND_ARGS)
{
	AlchemyItem *alch;
	float chance;
	if (ExtractArgs(EXTRACT_ARGS, &alch, &chance) && IS_TYPE(alch, AlchemyItem) && alch->withdrawalEffect)
		alch->addictionChance = chance;
	return true;
}

bool Cmd_GetIsPoison_Execute(COMMAND_ARGS)
{
	AlchemyItem *alch;
	if (ExtractArgs(EXTRACT_ARGS, &alch) && IS_TYPE(alch, AlchemyItem))
		*result = alch->IsPoison() ? 1 : 0;
	else *result = 0;
	return true;
}

bool Cmd_GetActiveEffects_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 filter = 7;
	if (!ExtractArgs(EXTRACT_ARGS, &filter) || !thisObj->IsActor())
		return true;
	filter = ((filter & 1) ? kFormType_EnchantmentItem : 63) & ((filter & 2) ? kFormType_SpellItem : 63) & ((filter & 4) ? kFormType_AlchemyItem : 63);
	ActiveEffectList *effList = ((Actor*)thisObj)->magicTarget.GetEffectList();
	if (!effList) return true;
	s_tempFormList.Clear();
	ActiveEffect *activeEff;
	TESForm *form;
	ListNode<ActiveEffect> *iter = effList->Head();
	do
	{
		activeEff = iter->data;
		if (!activeEff || !activeEff->bApplied || !activeEff->magicItem || (activeEff->magnitude == 0))
			continue;
		form = DYNAMIC_CAST(activeEff->magicItem, MagicItem, TESForm);
		if (form && (!filter || ((form->typeID & filter) == filter)))
			s_tempFormList.Insert(form);
	}
	while (iter = iter->next);
	if (!s_tempFormList.Empty())
	{
		s_tempElements.Clear();
		for (auto refIter = s_tempFormList.Begin(); refIter; ++refIter)
			s_tempElements.Append(*refIter);
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	}
	return true;
}

bool __fastcall ValidTempEffect(EffectItem *effectItem)
{
	if (!effectItem || (effectItem->duration <= 0) || !effectItem->setting) return false;
	UInt8 archtype = effectItem->setting->archtype;
	return !archtype || ((archtype == 1) && (effectItem->setting->effectFlags & 0x2000)) || ((archtype > 10) && (archtype < 14)) || (archtype == 24) || (archtype > 33);
}

struct SortEffectsEntry
{
	NVSEArrayVar	*valuesArr;
	float			timeLeft;

	SortEffectsEntry(NVSEArrayVar *_valuesArr, float _timeLeft) : valuesArr(_valuesArr), timeLeft(_timeLeft) {}

	bool operator<(const SortEffectsEntry &rhs) const {return timeLeft < rhs.timeLeft;}
};

bool Cmd_GetTempEffects_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (!thisObj->IsActor()) return true;
	ActiveEffectList *effList = ((Actor*)thisObj)->magicTarget.GetEffectList();
	if (!effList) return true;
	Vector<SortEffectsEntry> sortEffects;
	ActiveEffect *activeEff;
	TESForm *form;
	float timeLeft;
	ListNode<ActiveEffect> *iter = effList->Head();
	do
	{
		activeEff = iter->data;
		if (!activeEff || !activeEff->bApplied || !ValidTempEffect(activeEff->effectItem) || !activeEff->magicItem) continue;
		form = DYNAMIC_CAST(activeEff->magicItem, MagicItem, TESForm);
		if (!form) continue;
		timeLeft = activeEff->duration - activeEff->timeElapsed;
		ArrayElementL elements[5] =
		{
			form,
			activeEff->effectItem->setting,
			(activeEff->effectItem->setting->effectFlags & 2) ? (int)activeEff->effectItem->magnitude : fabs(activeEff->magnitude),
			timeLeft,
			activeEff->caster ? activeEff->caster->GetActor() : NULL
		};
		sortEffects.InsertSorted(SortEffectsEntry(CreateArray(elements, 5, scriptObj), timeLeft));
	}
	while (iter = iter->next);
	if (!sortEffects.Empty())
	{
		s_tempElements.Clear();
		for (auto entry = sortEffects.Begin(); entry; ++entry)
			s_tempElements.Append(entry().valuesArr);
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	}
	return true;
}

bool Cmd_RemoveNthTempEffect_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 index;
	if (!ExtractArgs(EXTRACT_ARGS, &index) || !thisObj->IsActor()) return true;
	ActiveEffectList *effList = ((Actor*)thisObj)->magicTarget.GetEffectList();
	if (!effList) return true;
	ListNode<ActiveEffect> *iter = effList->Head();
	ActiveEffect *activeEff;
	do
	{
		activeEff = iter->data;
		if (!activeEff || !activeEff->bApplied || !ValidTempEffect(activeEff->effectItem) || !activeEff->magicItem ||
			!DYNAMIC_CAST(activeEff->magicItem, MagicItem, TESForm)) continue;
		if (!index--)
		{
			activeEff->Remove(true);
			*result = 1;
			break;
		}
	}
	while (iter = iter->next);
	return true;
}

bool Cmd_GetEffectFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *effForm;
	UInt32 flagID;
	if (!ExtractArgs(EXTRACT_ARGS, &effForm, &flagID) || (flagID > 7)) return true;
	UInt8 flags;
	switch (effForm->typeID)
	{
	case kFormType_SpellItem:
		flags = ((SpellItem*)effForm)->spellFlags;
		break;
	case kFormType_EnchantmentItem:
		flags = ((EnchantmentItem*)effForm)->enchFlags;
		break;
	case kFormType_AlchemyItem:
		flags = ((AlchemyItem*)effForm)->alchFlags;
		break;
	default:
		return true;
	}
	*result = (flags & (1 << flagID)) ? 1 : 0;
	return true;
}

bool Cmd_SetEffectFlag_Execute(COMMAND_ARGS)
{
	TESForm *effForm;
	UInt32 flagID, val;
	if (!ExtractArgs(EXTRACT_ARGS, &effForm, &flagID, &val) || (flagID > 7)) return true;
	UInt8 *flagsPtr;
	switch (effForm->typeID)
	{
	case kFormType_SpellItem:
		flagsPtr = &((SpellItem*)effForm)->spellFlags;
		break;
	case kFormType_EnchantmentItem:
		flagsPtr = &((EnchantmentItem*)effForm)->enchFlags;
		break;
	case kFormType_AlchemyItem:
		flagsPtr = &((AlchemyItem*)effForm)->alchFlags;
		break;
	default:
		return true;
	}
	if (val) *flagsPtr |= (1 << flagID);
	else *flagsPtr &= ~(1 << flagID);
	return true;
}

bool Cmd_GetBaseEffectFlag_Execute(COMMAND_ARGS)
{
	EffectSetting *effForm;
	UInt32 flagID;
	if (ExtractArgs(EXTRACT_ARGS, &effForm, &flagID) && IS_TYPE(effForm, EffectSetting) && (flagID <= 31))
		*result = (effForm->effectFlags & (1 << flagID)) ? 1 : 0;
	else *result = 0;
	return true;
}

bool Cmd_SetBaseEffectFlag_Execute(COMMAND_ARGS)
{
	EffectSetting *effSetting;
	UInt32 flagID, val;
	if (ExtractArgs(EXTRACT_ARGS, &effSetting, &flagID, &val) && IS_TYPE(effSetting, EffectSetting) && (flagID <= 31))
	{
		if (val) effSetting->effectFlags |= (1 << flagID);
		else effSetting->effectFlags &= ~(1 << flagID);
	}
	return true;
}

bool Cmd_GetBaseEffectScript_Execute(COMMAND_ARGS)
{
	*result = 0;
	EffectSetting *effect;
	if (ExtractArgs(EXTRACT_ARGS, &effect) && IS_TYPE(effect, EffectSetting) && (effect->archtype == 1) && effect->associatedItem)
		REFR_RES = effect->associatedItem->refID;
	return true;
}

bool Cmd_SetBaseEffectScript_Execute(COMMAND_ARGS)
{
	EffectSetting *effect;
	Script *script;
	if (ExtractArgs(EXTRACT_ARGS, &effect, &script) && IS_TYPE(effect, EffectSetting) && (effect->archtype == 1) && IS_TYPE(script, Script))
		effect->associatedItem = script;
	return true;
}

bool __fastcall IsSpellTargetAlt(Actor *actor, MagicItem *magicItem)
{
	if (!actor->IsActor()) return false;
	ActiveEffectList *effList = actor->magicTarget.GetEffectList();
	if (!effList) return false;
	ListNode<ActiveEffect> *iter = effList->Head();
	ActiveEffect *activeEff;
	do
	{
		activeEff = iter->data;
		if (activeEff && (activeEff->magicItem == magicItem) && activeEff->bApplied)
			return true;
	}
	while (iter = iter->next);
	return false;
}

bool Cmd_IsSpellTargetAlt_Execute(COMMAND_ARGS)
{
	MagicItem *magicItem;
	if (ExtractArgs(EXTRACT_ARGS, &magicItem))
		*result = IsSpellTargetAlt((Actor*)thisObj, magicItem);
	else *result = 0;
	return true;
}

bool Cmd_IsSpellTargetAlt_Eval(COMMAND_ARGS_EVAL)
{
	*result = IsSpellTargetAlt((Actor*)thisObj, (MagicItem*)arg1);
	return true;
}

bool Cmd_CastImmediate_Execute(COMMAND_ARGS)
{
	MagicItem *magicItem;
	Actor *target = (Actor*)thisObj, *caster = NULL;
	if (ExtractArgs(EXTRACT_ARGS, &magicItem, &caster) && target->IsActor() && target->parentCell && (magicItem->GetType() != 4) && (!caster || caster->IsActor()))
	{
		if (!caster) caster = target;
		UInt32 *reachMultPtr = (UInt32*)0x11CF1E4, reachMult = *reachMultPtr;
		*reachMultPtr = 0x43800000;
		caster->magicCaster.CastSpell(magicItem, 0, &target->magicTarget, 1, 0);
		*reachMultPtr = reachMult;
	}
	return true;
}