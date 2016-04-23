struct hotkeylist_
{
	int type;//the type of the object it belongs to.
	_u32 modifiers;
	char key[4+1];
	catalogized_command_functype command;
	char variable[20+1];
	char value[20+1];
	_u32 UNDO;
	int functype;//type type of the function. See tools/reflection.hxx
};
hotkeylist_ hotkeylist[1000];
int hotkeylist_count=0;
void conv_config_internalconfig()
{
	int ilv1=0;
	int currenthotkey=0;
	int oldcount=hotkeylist_count;
	int input_count=(*glob_CONFIGBRIDGE_Hotkey_multilist).filllevel;
	if (input_count>=1000) error("too many hotkeys defined");
	for (ilv1=0;ilv1<input_count;ilv1++)
	{
		CONFIGBRIDGE_Object_instance * tl_CONFIGBRIDGE_Object_instance=(CONFIGBRIDGE_Object_instance*)((*(CONFIGBRIDGE_Hotkeys_instance*)((*glob_CONFIGBRIDGE_Hotkey_multilist)[ilv1].master)).master);
		for (int ilv2=0;ilv2<oldcount;ilv2++)
		{
			if (hotkeylist[ilv2].type==(*tl_CONFIGBRIDGE_Object_instance).type)
			{
				if (hotkeylist[ilv2].modifiers==(*glob_CONFIGBRIDGE_Hotkey_multilist)[ilv1].modifiers)
				{
					if (strncmp(hotkeylist[currenthotkey].key,(*glob_CONFIGBRIDGE_Hotkey_multilist)[ilv1].key.a,4)==0)
					{
						goto we_had_such_a_combination_already;
					}
				}
			}
		}
		hotkeylist[currenthotkey].type=(*tl_CONFIGBRIDGE_Object_instance).type;
		hotkeylist[currenthotkey].UNDO=(*glob_CONFIGBRIDGE_Hotkey_multilist)[ilv1].UNDO;
		hotkeylist[currenthotkey].modifiers=(*glob_CONFIGBRIDGE_Hotkey_multilist)[ilv1].modifiers;
		strncpy(hotkeylist[currenthotkey].key,(*glob_CONFIGBRIDGE_Hotkey_multilist)[ilv1].key.a,4);
		hotkeylist[currenthotkey].command=NULL;
		hotkeylist[currenthotkey].functype=0;
		for (int ilv2=0;ilv2<REFLECTION_FUNCTION_ListSize;ilv2++)
		{
			if (strcmp(REFLECTION_FUNCTION_List[ilv2].name,(*glob_CONFIGBRIDGE_Hotkey_multilist)[ilv1].command.a)==0)
			{
				hotkeylist[currenthotkey].command=(catalogized_command_functype)REFLECTION_FUNCTION_List[ilv2].function;
				hotkeylist[currenthotkey].functype=REFLECTION_FUNCTION_List[ilv2].type;
			}
		}
		strncpy(hotkeylist[currenthotkey].variable,(*glob_CONFIGBRIDGE_Hotkey_multilist)[ilv1].variable.a,20);
		hotkeylist[currenthotkey].variable[19]=0;
		strncpy(hotkeylist[currenthotkey].value,(*glob_CONFIGBRIDGE_Hotkey_multilist)[ilv1].value.a,20);
		hotkeylist[currenthotkey].value[19]=0;
		currenthotkey++;
		we_had_such_a_combination_already:;
	}
	hotkeylist_count=ilv1;
}
