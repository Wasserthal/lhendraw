void sdl_popupmenucommon()
{
	menu_list_count=0;
}
struct single_property_
{
	char name[33];
	char value[44];
	char modified;
};
single_property_ popup_list_of_singleproperties[100];
int popup_list_of_singleproperties_count;
int entry[STRUCTURE_OBJECTTYPE_ListSize];
void printbyte(char * inpointer)
{
	printf("%hhX ",*inpointer);
}
void draw_popupmenuinfo()
{
	SDL_color=0;
	popup_list_of_singleproperties_count=0;
	printmenutext(100,10,"POPUP! PRESS ESCAPE TO CLOSE POPUP",34,1);
	int verticounter=0;
	char value;
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		intl counter=0;
		basicmultilist * imultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		selection_datatype icompare=1<<ilv1;
		int isize=STRUCTURE_OBJECTTYPE_List[ilv1].size;
		for (int ilv2=0;ilv2<imultilist->filllevel;ilv2++)
		{
			if (selection_currentselection[ilv2] & icompare)
			{
				basic_instance * iinstance=(basic_instance*)(imultilist->pointer+ilv2*isize);
				if (iinstance->exist)
				{
					counter++;
				}
			}
		}
		if (counter)
		{
			for (int ilv2=0;ilv2<imultilist->_->properties_count;ilv2++)
			{
				char * outpointer;
				for (int ilv3=0;ilv3<popup_list_of_singleproperties_count;ilv3++)
				{
					if (strcmp(popup_list_of_singleproperties[ilv3].name,imultilist->_->properties[ilv2].name)==0)
					{
						goto popup_wehadthisnamealready;
					}
				}
				sprintf(popup_list_of_singleproperties[popup_list_of_singleproperties_count].name,"%s",imultilist->_->properties[ilv2].name);
				outpointer=(char*)"0";
				for (int ilv4=0;ilv4<sizeof(popup_list_of_singleproperties[popup_list_of_singleproperties_count].value);ilv4++)
				{
					popup_list_of_singleproperties[popup_list_of_singleproperties_count].value[ilv4]=0;
				}
				{
				#ifndef NOPOSIX
					FILE * ifile;
					while (shm_unlink("/lhendraw_output_filedescriptor")>0);//in case the program crashed ONCE here.
					int ifd=shm_open("/lhendraw_output_filedescriptor",O_CREAT|O_RDWR,O_RDWR);
					ifile=fdopen(ifd,"w+");
					outpointer=(char*)mmap(NULL,constants_VALUEREPRESENTATION_BUFFERSIZE,PROT_READ|PROT_WRITE,MAP_FILE|MAP_SHARED,ifd,0);
					imultilist->_->properties[ilv2].writedelegate(((char*)(imultilist->pointer))+imultilist->_->properties[ilv2].ref,ifile);
					fwrite("",1,1,ifile);
					for (int ilv1=0;ilv1<constants_VALUEREPRESENTATION_BUFFERSIZE-ftell(ifile);ilv1++)
					{
						fwrite("",1,1,ifile);
					}
					fclose(ifile);
				#endif
					sprintf(popup_list_of_singleproperties[popup_list_of_singleproperties_count].value,"%s",outpointer);
				#ifndef NOPOSIX
					munmap(outpointer,constants_VALUEREPRESENTATION_BUFFERSIZE);
					shm_unlink("/lhendraw_output_filedescriptor");
				#endif
				}
				popup_list_of_singleproperties_count++;
				popup_wehadthisnamealready:;
			}
			char outstring[11];
			sprintf(outstring,"%5i",(int)counter);
			printmenutext(8,20+16*verticounter,outstring,strlen(outstring));
			printmenutext(50,20+16*verticounter,STRUCTURE_OBJECTTYPE_List[ilv1].name,strlen(STRUCTURE_OBJECTTYPE_List[ilv1].name));
			verticounter++;
		}
	}
	verticounter=0;
	for (int ilv2=0;ilv2<popup_list_of_singleproperties_count;ilv2++)
	{
		char outstring[77];
		sprintf(outstring,"%s,%s",popup_list_of_singleproperties[ilv2].name,popup_list_of_singleproperties[ilv2].value);
		printmenutext(8,200+16*verticounter,outstring,strlen(outstring));
		verticounter++;
	}
}
