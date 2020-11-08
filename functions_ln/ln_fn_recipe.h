#pragma once

DEFINE_COMMAND_PLUGIN(GetFormRecipes, , 0, 2, kParams_JIP_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetFormRecipeOutputs, , 0, 2, kParams_JIP_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetRecipeRequiredSkill, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetRecipeRequiredSkill, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetRecipeRequiredSkillLevel, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetRecipeRequiredSkillLevel, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetRecipeInputForms, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetRecipeOutputForms, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(AddRecipeInputForm, , 0, 3, kParams_JIP_TwoForms_OneInt);
DEFINE_COMMAND_PLUGIN(AddRecipeOutputForm, , 0, 3, kParams_JIP_TwoForms_OneInt);
DEFINE_COMMAND_PLUGIN(RemoveRecipeInputForm, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(RemoveRecipeOutputForm, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(ReplaceRecipeInputForm, , 0, 3, kParams_JIP_ThreeForms);
DEFINE_COMMAND_PLUGIN(ReplaceRecipeOutputForm, , 0, 3, kParams_JIP_ThreeForms);
DEFINE_COMMAND_PLUGIN(GetRecipeInputCount, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(SetRecipeInputCount, , 0, 3, kParams_JIP_TwoForms_OneInt);
DEFINE_COMMAND_PLUGIN(GetRecipeOutputCount, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(SetRecipeOutputCount, , 0, 3, kParams_JIP_TwoForms_OneInt);
DEFINE_COMMAND_PLUGIN(GetRecipeCategory, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetRecipeCategory, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(GetRecipeSubcategory, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetRecipeSubcategory, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(AddRecipeCondition, , 0, 5, kParams_FormCondition);

bool Cmd_GetFormRecipes_Execute(COMMAND_ARGS)
{
	TESForm *form, *filter = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &form, &filter)) return true;
	s_tempElements.Clear();
	if (filter && NOT_TYPE(filter, TESRecipeCategory)) filter = NULL;
	ListNode<TESRecipe> *rcpeIter = g_dataHandler->recipeList.Head();
	TESRecipe *recipe;
	ListNode<RecipeComponent> *entryIter;
	RecipeComponent *component;
	do
	{
		recipe = rcpeIter->data;
		if (!recipe || (filter && (recipe->category != filter) && (recipe->subCategory != filter))) continue;
		entryIter = recipe->inputs.Head();
		do
		{
			component = entryIter->data;
			if (component && (component->item == form))
				s_tempElements.Append(recipe);
		}
		while (entryIter = entryIter->next);
	}
	while (rcpeIter = rcpeIter->next);
	AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	return true;
}

bool Cmd_GetFormRecipeOutputs_Execute(COMMAND_ARGS)
{
	TESForm *form, *filter;
	if (!ExtractArgs(EXTRACT_ARGS, &form, &filter))
		return true;
	s_tempFormList.Clear();
	if (filter && NOT_TYPE(filter, TESRecipeCategory)) filter = NULL;
	ListNode<TESRecipe> *rcpeIter = g_dataHandler->recipeList.Head();
	TESRecipe *recipe;
	ListNode<RecipeComponent> *inputIter, *outputIter;
	RecipeComponent *component;
	do
	{
		recipe = rcpeIter->data;
		if (!recipe || (filter && (recipe->category != filter) && (recipe->subCategory != filter))) continue;
		inputIter = recipe->inputs.Head();
		do
		{
			component = inputIter->data;
			if (!component || (component->item != form)) continue;
			outputIter = recipe->outputs.Head();
			do
			{
				if (component = outputIter->data)
					s_tempFormList.Insert(component->item);
			}
			while (outputIter = outputIter->next);
		}
		while (inputIter = inputIter->next);
	}
	while (rcpeIter = rcpeIter->next);
	if (!s_tempFormList.Empty())
	{
		s_tempElements.Clear();
		for (auto refIter = s_tempFormList.Begin(); refIter; ++refIter)
			s_tempElements.Append(*refIter);
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	}
	return true;
}

bool Cmd_GetRecipeRequiredSkill_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	if (ExtractArgs(EXTRACT_ARGS, &recipe) && IS_TYPE(recipe, TESRecipe))
		*result = (int)recipe->reqSkill;
	else *result = -1;
	return true;
}

bool Cmd_SetRecipeRequiredSkill_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	UInt32 skill;
	if (ExtractArgs(EXTRACT_ARGS, &recipe, &skill) && IS_TYPE(recipe, TESRecipe))
		recipe->reqSkill = skill;
	return true;
}

bool Cmd_GetRecipeRequiredSkillLevel_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	if (ExtractArgs(EXTRACT_ARGS, &recipe) && IS_TYPE(recipe, TESRecipe))
		*result = (int)recipe->reqSkillLevel;
	else *result = -1;
	return true;
}

bool Cmd_SetRecipeRequiredSkillLevel_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	UInt32 skillLvl;
	if (ExtractArgs(EXTRACT_ARGS, &recipe, &skillLvl) && IS_TYPE(recipe, TESRecipe))
		recipe->reqSkillLevel = skillLvl;
	return true;
}

bool Cmd_GetRecipeInputForms_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	if (ExtractArgs(EXTRACT_ARGS, &recipe) && IS_TYPE(recipe, TESRecipe))
		AssignCommandResult((NVSEArrayVar*)recipe->inputs.GetComponents(scriptObj), result);
	else *result = 0;
	return true;
}

bool Cmd_GetRecipeOutputForms_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	if (ExtractArgs(EXTRACT_ARGS, &recipe) && IS_TYPE(recipe, TESRecipe))
		AssignCommandResult((NVSEArrayVar*)recipe->outputs.GetComponents(scriptObj), result);
	else *result = 0;
	return true;
}

bool Cmd_AddRecipeInputForm_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESForm *form;
	UInt32 count;
	if (ExtractArgs(EXTRACT_ARGS, &recipe, &form, &count) && IS_TYPE(recipe, TESRecipe))
		recipe->inputs.AddComponent(form, count);
	return true;
}

bool Cmd_AddRecipeOutputForm_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESForm *form;
	UInt32 count;
	if (ExtractArgs(EXTRACT_ARGS, &recipe, &form, &count) && IS_TYPE(recipe, TESRecipe))
		recipe->outputs.AddComponent(form, count);
	return true;
}

bool Cmd_RemoveRecipeInputForm_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESForm *form;
	if (ExtractArgs(EXTRACT_ARGS, &recipe, &form) && IS_TYPE(recipe, TESRecipe))
		*result = (int)recipe->inputs.RemoveComponent(form);
	else *result = 0;
	return true;
}

bool Cmd_RemoveRecipeOutputForm_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESForm *form;
	if (ExtractArgs(EXTRACT_ARGS, &recipe, &form) && IS_TYPE(recipe, TESRecipe))
		*result = (int)recipe->outputs.RemoveComponent(form);
	else *result = 0;
	return true;
}

bool Cmd_ReplaceRecipeInputForm_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESForm *form, *replace;
	if (ExtractArgs(EXTRACT_ARGS, &recipe, &form, &replace) && IS_TYPE(recipe, TESRecipe))
		recipe->inputs.ReplaceComponent(form, replace);
	return true;
}

bool Cmd_ReplaceRecipeOutputForm_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESForm *form, *replace;
	if (ExtractArgs(EXTRACT_ARGS, &recipe, &form, &replace) && IS_TYPE(recipe, TESRecipe))
		recipe->outputs.ReplaceComponent(form, replace);
	return true;
}

bool Cmd_GetRecipeInputCount_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESRecipe *recipe;
	TESForm *form;
	if (ExtractArgs(EXTRACT_ARGS, &recipe, &form) && IS_TYPE(recipe, TESRecipe))
		*result = (int)recipe->inputs.GetQuantity(form);
	else *result = 0;
	return true;
}

bool Cmd_SetRecipeInputCount_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESForm *form;
	UInt32 count;
	if (ExtractArgs(EXTRACT_ARGS, &recipe, &form, &count) && IS_TYPE(recipe, TESRecipe))
		recipe->inputs.SetQuantity(form, count);
	return true;
}

bool Cmd_GetRecipeOutputCount_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESForm *form;
	if (ExtractArgs(EXTRACT_ARGS, &recipe, &form) && IS_TYPE(recipe, TESRecipe))
		*result = (int)recipe->outputs.GetQuantity(form);
	else *result = 0;
	return true;
}

bool Cmd_SetRecipeOutputCount_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESForm *form;
	UInt32 count;
	if (ExtractArgs(EXTRACT_ARGS, &recipe, &form, &count) && IS_TYPE(recipe, TESRecipe))
		recipe->outputs.SetQuantity(form, count);
	return true;
}

bool Cmd_GetRecipeCategory_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESRecipe *recipe;
	if (ExtractArgs(EXTRACT_ARGS, &recipe) && IS_TYPE(recipe, TESRecipe) && recipe->category)
		REFR_RES = recipe->category->refID;
	return true;
}

bool Cmd_SetRecipeCategory_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESRecipeCategory *category;
	if (ExtractArgs(EXTRACT_ARGS, &recipe, &category) && IS_TYPE(recipe, TESRecipe) && IS_TYPE(category, TESRecipeCategory))
	{
		recipe->category = category;
		recipe->categoryID = category->refID;
	}
	return true;
}

bool Cmd_GetRecipeSubcategory_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESRecipe *recipe;
	if (ExtractArgs(EXTRACT_ARGS, &recipe) && IS_TYPE(recipe, TESRecipe) && recipe->subCategory)
		REFR_RES = recipe->subCategory->refID;
	return true;
}

bool Cmd_SetRecipeSubcategory_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESRecipeCategory *category;
	if (ExtractArgs(EXTRACT_ARGS, &recipe, &category) && IS_TYPE(recipe, TESRecipe) && IS_TYPE(category, TESRecipeCategory))
	{
		recipe->subCategory = category;
		recipe->subCategoryID = category->refID;
	}
	return true;
}

bool Cmd_AddRecipeCondition_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESForm *param;
	UInt32 func, comp;
	float value;
	if (!ExtractArgs(EXTRACT_ARGS, &recipe, &func, &param, &comp, &value) || NOT_TYPE(recipe, TESRecipe)) return true;
	Condition *condition = (Condition*)GameHeapAlloc(sizeof(Condition));
	condition->runOnType = 0;
	condition->opcode = func;
	condition->parameter1.form = param;
	condition->type = comp;
	condition->comparisonValue.value = value;
	recipe->conditions.Append(condition);
	return true;
}