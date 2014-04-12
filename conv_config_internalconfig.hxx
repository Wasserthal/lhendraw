struct hotkeylist_
{
	int type;//the type of the object it belongs to.
	int modifiers;
	char key[4];
	catalogized_command_functype command;
	char variable[20];
	char value[20];
};
hotkeylist_ hotkeylist[1000];
int hotkeylist_count=0;
void conv_config_internalconfig()
{
	hotkeylist_count=(*glob_CONFIGBRIDGE_Hotkey_multilist).filllevel;
	if (hotkeylist_count>=1000) {fprintf(stderr,"too many hotkeys defined");exit(1);}
	for (int ilv1=0;ilv1<(*glob_CONFIGBRIDGE_Hotkey_multilist).filllevel;ilv1++)
	{
		CONFIGBRIDGE_Object_instance * tl_CONFIGBRIDGE_Object_instance=(CONFIGBRIDGE_Object_instance*)((*(CONFIGBRIDGE_Hotkeys_instance*)((*glob_CONFIGBRIDGE_Hotkey_multilist).bufferlist[ilv1].master)).master);
		hotkeylist[ilv1].type=(*tl_CONFIGBRIDGE_Object_instance).type;
		hotkeylist[ilv1].modifiers=(*glob_CONFIGBRIDGE_Hotkey_multilist).bufferlist[ilv1].modifier;
		strncpy(hotkeylist[ilv1].key,(*glob_CONFIGBRIDGE_Hotkey_multilist).bufferlist[ilv1].key.a,4);
		hotkeylist[ilv1].command=NULL;
		for (int ilv2=0;ilv2<REFLECTION_FUNCTION_ListSize;ilv2++)
		{
			if (strcmp(REFLECTION_FUNCTION_List[ilv2].name,(*glob_CONFIGBRIDGE_Hotkey_multilist).bufferlist[ilv1].command.a)==0)
			{
				hotkeylist[ilv1].command=(catalogized_command_functype)REFLECTION_FUNCTION_List[ilv2].function;
			}
		}
		strncpy(hotkeylist[ilv1].variable,(*glob_CONFIGBRIDGE_Hotkey_multilist).bufferlist[ilv1].variable.a,20);
		hotkeylist[ilv1].variable[19]=0;
		strncpy(hotkeylist[ilv1].value,(*glob_CONFIGBRIDGE_Hotkey_multilist).bufferlist[ilv1].value.a,20);
		hotkeylist[ilv1].value[19]=0;
	}
}
