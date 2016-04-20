extern int edit_bienum_multi_string(bienum * ibienum,char * output,intl imax,intl value);
void conv_internalconfig_config(const char * filename)
{
	FILE * outfile=fopen(filename,"w");
	char objecttypestring[stringlength+1];
	fprintf(outfile,"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
	fprintf(outfile,"<Objects>\n");
	(*glob_CONFIGBRIDGE_Hotkey_multilist).filllevel=0;
	for (int ilv1=0;ilv1<hotkeylist_count;ilv1++)//Iterates over types
	{
		_i32 thistype=hotkeylist[ilv1].type;
		for (int ilv2=0;ilv2<ilv1;ilv2++)//What a waste! Just to check if we had that type before...(Non time-critical)
		{
			if (hotkeylist[ilv2].type==thistype) goto had_type_already;
		}
		edit_bienum_multi_string(CDXML_type,objecttypestring,CDXML_type_max,thistype);
		fprintf(outfile,"<Object type=\"%s\">\n<Hotkeys>\n",objecttypestring);
		for (int ilv2=ilv1;ilv2<hotkeylist_count;ilv2++)
		{
			if (hotkeylist[ilv2].type==thistype)
			{
				(*glob_CONFIGBRIDGE_Hotkey_multilist)[ilv2].INTERNALPropertyexistflags=0;
				(*glob_CONFIGBRIDGE_Hotkey_multilist)[ilv2].UNDO=hotkeylist[ilv2].UNDO;
				(*glob_CONFIGBRIDGE_Hotkey_multilist)[ilv2].modifiers=hotkeylist[ilv2].modifiers;
				strncpy((*glob_CONFIGBRIDGE_Hotkey_multilist)[ilv2].key.a,hotkeylist[ilv2].key,4);
				if (hotkeylist[ilv2].command!=NULL)
				{
					for (int ilv3=0;ilv3<REFLECTION_FUNCTION_ListSize;ilv3++)
					{
						if (REFLECTION_FUNCTION_List[ilv3].function==hotkeylist[ilv2].command)
						{
							strncpy((*glob_CONFIGBRIDGE_Hotkey_multilist)[ilv3].command.a,REFLECTION_FUNCTION_List[ilv3].name,19);
							goto ifound;
						}
					}
				}
				goto iskip;//do not save invalid commands
				ifound:;
				strncpy((*glob_CONFIGBRIDGE_Hotkey_multilist)[ilv2].value.a,hotkeylist[ilv2].value,20);
				strncpy((*glob_CONFIGBRIDGE_Hotkey_multilist)[ilv2].variable.a,hotkeylist[ilv2].variable,20);
				if (hotkeylist[ilv1].UNDO!=1)
				(*glob_CONFIGBRIDGE_Hotkey_multilist)[ilv2].UNDO=hotkeylist[ilv2].UNDO;
				if (hotkeylist[ilv2].variable[0]!=0)
				AUTOSTRUCT_EXISTS_SET_NAME((glob_CONFIGBRIDGE_Hotkey_multilist->bufferlist()+ilv2),variable);
				AUTOSTRUCT_EXISTS_SET_NAME((glob_CONFIGBRIDGE_Hotkey_multilist->bufferlist()+ilv2),value);
				AUTOSTRUCT_EXISTS_SET_NAME((glob_CONFIGBRIDGE_Hotkey_multilist->bufferlist()+ilv2),key);
				AUTOSTRUCT_EXISTS_SET_NAME((glob_CONFIGBRIDGE_Hotkey_multilist->bufferlist()+ilv2),command);
				if (hotkeylist[ilv2].modifiers!=0)
				AUTOSTRUCT_EXISTS_SET_NAME((glob_CONFIGBRIDGE_Hotkey_multilist->bufferlist()+ilv2),modifiers);
				output_object(outfile,glob_CONFIGBRIDGE_Hotkey_multilist->bufferlist()+ilv2);
				iskip:;
			}
		}
		fprintf(outfile,"</Hotkeys>\n</Object>\n");
		had_type_already:;
	}
	fprintf(outfile,"</Objects>\n");
	fclose(outfile);
}
