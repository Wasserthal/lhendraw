//Converts the internal format to the cambridge structure both for cdx as cdxml.
//Note: The created CAMBRIDGE structure is created as IMMUTABLE structure, for example, pointers in in may point to other buffers which may not be changed just in order to change the CAMBRIDGE structure.
//If these data changes because the other datastructure (usually the corresponding internal one) changes, they go out of scope, so the CAMBRIDGE structure is temporary only.
#define CONVCAMBRIDGE_COLORCONV(AUTOPARM) \
{\
	int tl_color;\
	unsigned char r,g,b;\
	AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_ ## AUTOPARM ## _instance,color);\
	tl_color=(*tl_ ## AUTOPARM ## _instance).color;\
	(*tl_CAMBRIDGE_ ## AUTOPARM ## _instance).color=0;\
	for (int ilv1=0;ilv1<(*glob_CAMBRIDGE_color_multilist).filllevel;ilv1++)\
	{\
		r=(tl_color&0xFF0000)>>16;\
		g=(tl_color&0xFF00)>>8;\
		b=(tl_color&0xFF);\
		if ((((unsigned char)((*((CAMBRIDGE_color_instance*)&((*glob_CAMBRIDGE_color_multilist)[ilv1]))).r*255))==r) && \
		(((unsigned char)((*((CAMBRIDGE_color_instance*)&((*glob_CAMBRIDGE_color_multilist)[ilv1]))).g*255))==g) && \
		(((unsigned char)((*((CAMBRIDGE_color_instance*)&((*glob_CAMBRIDGE_color_multilist)[ilv1]))).b*255))==b))\
		{\
			(*tl_CAMBRIDGE_ ## AUTOPARM ## _instance).color=ilv1+2;\
		}\
	}\
}
#define CONVCAMBRIDGE_COLORCONV2(AUTOPARM,MACROPARAM_color) \
{\
	int tl_color;\
	unsigned char r,g,b;\
	AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_ ## AUTOPARM ## _instance,color);\
	tl_color=(MACROPARAM_color);\
	(*tl_CAMBRIDGE_ ## AUTOPARM ## _instance).color=0;\
	for (int ilv1=0;ilv1<(*glob_CAMBRIDGE_color_multilist).filllevel;ilv1++)\
	{\
		r=(tl_color&0xFF0000)>>16;\
		g=(tl_color&0xFF00)>>8;\
		b=(tl_color&0xFF);\
		if ((((unsigned char)((*((CAMBRIDGE_color_instance*)&((*glob_CAMBRIDGE_color_multilist)[ilv1]))).r*255))==r) && \
		(((unsigned char)((*((CAMBRIDGE_color_instance*)&((*glob_CAMBRIDGE_color_multilist)[ilv1]))).g*255))==g) && \
		(((unsigned char)((*((CAMBRIDGE_color_instance*)&((*glob_CAMBRIDGE_color_multilist)[ilv1]))).b*255))==b))\
		{\
			(*tl_CAMBRIDGE_ ## AUTOPARM ## _instance).color=ilv1+2;\
		}\
	}\
}

int CONVCAMBRIDGE_s_f(CAMBRIDGE_t_instance * master,edit_formatstruct * iformat,char * ibuffer)
{
	char * ibuffer2;
	ibuffer2=filestructure_text_buffer.buffer+filestructure_text_buffer.count;
	if (filestructure_text_buffer.count+strlen(ibuffer)+1>=LHENDRAW_buffersize)
	{
		return -1;
	}
	if (strlen(ibuffer)==0) return 0;
	strcpy(ibuffer2,ibuffer);
	filestructure_text_buffer.count+=1+strlen(ibuffer);
	multilist<CAMBRIDGE_s_instance> * tl_CAMBRIDGE_s_multilist=retrievemultilist<CAMBRIDGE_s_instance>();
	ADD_TO_MULTILISTREFERENCE(master,s);
	CONVCAMBRIDGE_COLORCONV2(s,(*iformat).color);
	if ((*master).color==(*tl_CAMBRIDGE_s_instance).color)
	{
		AUTOSTRUCT_EXISTS_CLEAR_NAME(tl_CAMBRIDGE_s_instance,color);
	}
	(*tl_CAMBRIDGE_s_instance).font=1;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_s_instance,font);
	(*tl_CAMBRIDGE_s_instance).face=(*iformat).face|0x60;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_s_instance,face);
	(*tl_CAMBRIDGE_s_instance).size=12;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_s_instance,size);
	(*tl_CAMBRIDGE_s_instance).PCTEXT.a=ibuffer2;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_s_instance,PCTEXT);
	(*tl_CAMBRIDGE_s_instance).PCTEXT.count=strlen(ibuffer);
	(*tl_CAMBRIDGE_s_instance).PCTEXTcounter=strlen(ibuffer);
	return 1;
}
int CONVCAMBRIDGE_latin_vs_greek(CAMBRIDGE_t_instance * master,s_instance * tl_s_instance,int * icursor,int count,char * ibufferptr)
{
	int ilv2;
	int currenttype=0;
	int thereismore=0;
	ADD_TO_MULTILISTREFERENCE(master,s);
	iback:;
	if ((*icursor)>=count){thereismore=0;goto done;}
	if (ibufferptr[(*icursor)]==0){thereismore=0;goto done;}
	for (ilv2=0;ilv2<list_greeklist_size;ilv2++)
	{
		if (strncmp(list_greeklist[ilv2].output,ibufferptr+(*icursor),strlen(list_greeklist[ilv2].output))==0)
		{
			if (currenttype==0) {currenttype=2;}else{if ((currenttype)!=2){thereismore=1;goto done;}}
			printf("2");
			currenttype=2;
			(*icursor)+=strlen(list_greeklist[ilv2].output);
			goto ifound;
		}
	}
	if (currenttype==0) {currenttype=1;}else{if ((currenttype)!=1){thereismore=1;goto done;}}
	(*icursor)++;
	ifound:;
	goto iback;
	done:;
	if ((*icursor)>=count)thereismore=0;
	if (ibufferptr[(*icursor)]==0)thereismore=0;
	CONVCAMBRIDGE_COLORCONV(s);
	(*tl_CAMBRIDGE_s_instance).font=currenttype;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_s_instance,font);
	(*tl_CAMBRIDGE_s_instance).face=(*tl_s_instance).face;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_s_instance,face);
	(*tl_CAMBRIDGE_s_instance).size=(*tl_s_instance).size;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_s_instance,size);
	(*tl_CAMBRIDGE_s_instance).PCTEXT.a=ibufferptr;
	(*tl_CAMBRIDGE_s_instance).PCTEXT.count=(*icursor);
	AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_s_instance,PCTEXT);
	return thereismore;
};
void CONVCAMBRIDGE_s(CAMBRIDGE_t_instance * master,s_instance * tl_s_instance,char * CAMBRIDGE_s_instance_buffer)
{
	int icursor=0;
	while (CONVCAMBRIDGE_latin_vs_greek(master,tl_s_instance,&icursor,strlen(CAMBRIDGE_s_instance_buffer),CAMBRIDGE_s_instance_buffer)>0)
	{
		CAMBRIDGE_s_instance_buffer+=icursor;
		icursor=0;
	}
}
void CONVCAMBRIDGE_atoms(CAMBRIDGE_fragment_instance * master,cdx_Rectangle * iBoundingBox)
{
	int backval;
	float i_bond_sum;
	_u32 icompare=1<<STRUCTURE_OBJECTTYPE_n;
	multilist<CAMBRIDGE_n_instance> * tl_CAMBRIDGE_n_multilist=retrievemultilist<CAMBRIDGE_n_instance>();
	multilist<n_instance> * tl_n_multilist=retrievemultilist<n_instance>();
	multilist<CAMBRIDGE_t_instance> * tl_CAMBRIDGE_t_multilist=retrievemultilist<CAMBRIDGE_t_instance>();
	for (int ilv1=0;ilv1<(*tl_n_multilist).filllevel;ilv1++)
	{
		if ((*tl_n_multilist).bufferlist()[ilv1].exist==0) continue;
		if (selection_fragmentselection[ilv1] & icompare)
		{
			i_bond_sum=0;
			n_instance * tl_n_instance=(*tl_n_multilist).bufferlist()+ilv1;
			ADD_TO_MULTILISTREFERENCE(master,n);
			CONVCAMBRIDGE_COLORCONV(n);
			(*tl_CAMBRIDGE_n_instance).Charge=(*tl_n_instance).charge;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_n_instance,Charge);
			int tl_Element=(*tl_n_instance).Element;
			TELESCOPE_aggressobject(tl_n_multilist,ilv1);
			backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
			if (backval)
			{
ADD_TO_MULTILISTREFERENCE(tl_CAMBRIDGE_n_instance,t);
TELESCOPE_aggressobject(tl_n_multilist,ilv1);
backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
while (backval>0)
{
	s_instance * nd_s_instance=(s_instance*)TELESCOPE_getproperty();
	CONVCAMBRIDGE_s(tl_CAMBRIDGE_t_instance,(s_instance*)TELESCOPE_getproperty(),(char*)TELESCOPE_getproperty_contents());
	backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
}
CONVCAMBRIDGE_COLORCONV2(t,(*tl_n_instance).color);
(*tl_CAMBRIDGE_t_instance).p.x=(*tl_n_instance).xyz.x;//BACKWARDS COMPATIBILITY
(*tl_CAMBRIDGE_t_instance).p.y=(*tl_n_instance).xyz.y;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_t_instance,p);//BACKWARDS COMPATIBILITY
(*tl_CAMBRIDGE_t_instance).xyz=(*tl_n_instance).xyz;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_t_instance,xyz);
(*tl_CAMBRIDGE_t_instance).Z=(*tl_n_instance).Z;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_t_instance,Z);
			}
			else
			{
				if ((tl_Element>=0) && (tl_Element<element_max))
				{
					for (int ilv2=0;ilv2<atom_actual_node[ilv1].bondcount;ilv2++)
					{
						i_bond_sum+=(*glob_b_multilist)[atom_actual_node[ilv1].bonds[ilv2]].Order/16.0;
					}
					if (fmod(i_bond_sum,1.0)>0.4)
					{
						i_bond_sum=trunc(i_bond_sum)+1;
					}
					if ((*tl_n_instance).Element!=constants_Element_implicitcarbon)
					{
	ADD_TO_MULTILISTREFERENCE(tl_CAMBRIDGE_n_instance,t);
	TELESCOPE_aggressobject(tl_n_multilist,ilv1);
	backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
	CONVCAMBRIDGE_COLORCONV2(t,(*tl_n_instance).color);
						char istring[10];
						char tl_fill;
						char imatch;
						char tl_format=0;
						s_f_instance * tl_s_f_instance;
						edit_formatstruct * tl_formatpointer;
						edit_formatstruct iformatstruct;
						edit_formatstruct * currentformatpointer=&iformatstruct;
						iformatstruct.color=(*tl_n_instance).color;
						iformatstruct.face=0x60;
						backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s_f);
						if (backval)
						{
							tl_format=(*(s_f_instance*)TELESCOPE_getproperty()).valids;
							tl_formatpointer=(edit_formatstruct*)TELESCOPE_getproperty_contents();
						}
						for (int ilv1=0;ilv1<5;ilv1++)
						{
							if (tl_s_f_instance)
							{
								tl_fill=0;
								for (int ilv2=0;ilv2<=ilv1;ilv2++)
								{
									imatch=1<<ilv2;
									if (tl_format & imatch)
									{
										currentformatpointer=tl_formatpointer+tl_fill;
										tl_fill++;
									}
								}
							}
							switch (ilv1)
							{
								case 0 : if (element[tl_Element].name[0]==0) {ilv1=2;goto ifertig;}sprintf(istring,"%c",element[tl_Element].name[0]);break;
								case 1 : if (element[tl_Element].name[1]==0) {ilv1=2;goto ifertig;}sprintf(istring,"%c",element[tl_Element].name[1]);break;
								case 2 : if (element[tl_Element].name[2]==0) {ilv1=2;goto ifertig;}sprintf(istring,"%c",element[tl_Element].name[2]);break;
								case 3 : sprintf(istring,"%s",((*tl_n_instance).protons-(int)i_bond_sum<=0)?"":"H");break;
								case 4 : if ((*tl_n_instance).protons-(int)i_bond_sum>1) sprintf(istring,"%i",(*tl_n_instance).protons-(int)i_bond_sum); else istring[0]=0;break;
								//TODO: the following ones are not used, because they do not make any sense usually. However, one might want to add charge in the text...
								case 5 :
								if ((*tl_n_instance).charge<0) {sprintf(istring,"%i-",-(*tl_n_instance).charge);break;}
								if ((*tl_n_instance).charge>0) {sprintf(istring,"%i+",(*tl_n_instance).charge);break;}
								if ((*tl_n_instance).charge==0) {istring[0]=0;break;}
							}
							CONVCAMBRIDGE_s_f(tl_CAMBRIDGE_t_instance,currentformatpointer,istring);
							ifertig:;
						}
	(*tl_CAMBRIDGE_t_instance).p.x=(*tl_n_instance).xyz.x;//BACKWARDS COMPATIBILITY
	(*tl_CAMBRIDGE_t_instance).p.y=(*tl_n_instance).xyz.y;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_t_instance,p);//BACKWARDS COMPATIBILITY
	(*tl_CAMBRIDGE_t_instance).xyz=(*tl_n_instance).xyz;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_t_instance,xyz);
	(*tl_CAMBRIDGE_t_instance).Z=(*tl_n_instance).Z;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_t_instance,Z);
					}
				}
			}
			if ((tl_Element>=0) && (tl_Element<element_max))
			{
				(*tl_CAMBRIDGE_n_instance).Element=element[tl_Element].protons;//DON'T WORRY; these are NUCLEAR protons!
				AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_n_instance,Element);
			}
			if (tl_Element==-2)
			{
				AUTOSTRUCT_EXISTS_CLEAR_NAME(tl_CAMBRIDGE_n_instance,Element);
				AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_n_instance,NodeType);
				AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_n_instance,ExternalConnectionType);
				(*tl_CAMBRIDGE_n_instance).NodeType=12;
				(*tl_CAMBRIDGE_n_instance).ExternalConnectionType=4;
				TELESCOPE_aggressobject(tl_n_multilist,ilv1);
				backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_wildcard);
				if (backval>0)
				{
					(*tl_CAMBRIDGE_n_instance).ExternalConnectionType=*(_u32*)TELESCOPE_getproperty_contents();
				}
			}
			(*tl_CAMBRIDGE_n_instance).id=janitor_id_list[STRUCTURE_OBJECTTYPE_n-1]+(*tl_n_instance).id;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_n_instance,id);
			(*tl_CAMBRIDGE_n_instance).Z=(*tl_n_instance).Z;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_n_instance,Z);
			(*tl_CAMBRIDGE_n_instance).xyz=(*tl_n_instance).xyz;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_n_instance,xyz);
			if (tl_Element!=constants_Element_implicitcarbon)
			{
				(*tl_CAMBRIDGE_n_instance).NumHydrogens=(*tl_n_instance).protons-(int)i_bond_sum;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_n_instance,NumHydrogens);
			}
			if (iBoundingBox!=NULL)
			{
				if ((*tl_CAMBRIDGE_n_instance).xyz.x<(*iBoundingBox).left){(*iBoundingBox).left=(*tl_CAMBRIDGE_n_instance).xyz.x;}
				if ((*tl_CAMBRIDGE_n_instance).xyz.x>(*iBoundingBox).right){(*iBoundingBox).right=(*tl_CAMBRIDGE_n_instance).xyz.x;}
				if ((*tl_CAMBRIDGE_n_instance).xyz.y<(*iBoundingBox).top){(*iBoundingBox).top=(*tl_CAMBRIDGE_n_instance).xyz.y;}
				if ((*tl_CAMBRIDGE_n_instance).xyz.y>(*iBoundingBox).bottom){(*iBoundingBox).bottom=(*tl_CAMBRIDGE_n_instance).xyz.y;}
			}
		}
		else
		{
			if ((selection_clickselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_n))==0)
			{
				for (int ilv2=0;ilv2<atom_actual_node[ilv1].bondcount;ilv2++)
				{
					if (selection_fragmentselection[atom_actual_node[ilv1].bonds[ilv2]] & (1<<STRUCTURE_OBJECTTYPE_b))
					{
						n_instance * tl_n_instance=(*tl_n_multilist).bufferlist()+ilv1;
						ADD_TO_MULTILISTREFERENCE(master,n);
						CONVCAMBRIDGE_COLORCONV(n);
	(*tl_CAMBRIDGE_n_instance).p.x=(*tl_n_instance).xyz.x;//BACKWARDS COMPATIBILITY
	(*tl_CAMBRIDGE_n_instance).p.y=(*tl_n_instance).xyz.y;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_n_instance,p);//BACKWARDS COMPATIBILITY
	(*tl_CAMBRIDGE_n_instance).xyz=(*tl_n_instance).xyz;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_n_instance,xyz);
	(*tl_CAMBRIDGE_n_instance).Z=(*tl_n_instance).Z;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_n_instance,Z);
	(*tl_CAMBRIDGE_n_instance).id=janitor_id_list[STRUCTURE_OBJECTTYPE_n-1]+(*tl_n_instance).id;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_n_instance,id);
						goto iatomfertig;
					}
				}
			}
		}
		iatomfertig:;
	}
}

void CONVCAMBRIDGE_bonds(CAMBRIDGE_fragment_instance * master)
{
	_u32 icompare=1<<STRUCTURE_OBJECTTYPE_b;
	multilist<CAMBRIDGE_b_instance> * tl_CAMBRIDGE_b_multilist=retrievemultilist<CAMBRIDGE_b_instance>();
	multilist<b_instance> * tl_b_multilist=retrievemultilist<b_instance>();
	for (int ilv1=0;ilv1<(*tl_b_multilist).filllevel;ilv1++)
	{
		if ((*tl_b_multilist).bufferlist()[ilv1].exist==0) continue;
		if (selection_fragmentselection[ilv1] & icompare)
		{
			b_instance * tl_b_instance=(*tl_b_multilist).bufferlist()+ilv1;
			ADD_TO_MULTILISTREFERENCE(master,b);
			CONVCAMBRIDGE_COLORCONV(b);
			(*tl_CAMBRIDGE_b_instance).id=janitor_id_list[STRUCTURE_OBJECTTYPE_b-1]+(*tl_b_instance).id;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_b_instance,id);
			(*tl_CAMBRIDGE_b_instance).Z=(*tl_b_instance).Z;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_b_instance,Z);
			(*tl_CAMBRIDGE_b_instance).B=(*tl_b_instance).B+janitor_id_list[STRUCTURE_OBJECTTYPE_n-1];AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_b_instance,B);
			(*tl_CAMBRIDGE_b_instance).E=(*tl_b_instance).E+janitor_id_list[STRUCTURE_OBJECTTYPE_n-1];AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_b_instance,E);
			_u8 iOrder=(*tl_b_instance).Order;
			_i32 iOrderout=1;
if (iOrder>8){ if (iOrder>16) {if (iOrder>24) {if (iOrder>32) {if (iOrder>40) {if (iOrder>48) {if (iOrder>56) {if (iOrder>64) {if (iOrder>72) {if (iOrder>80) {if (iOrder>88) {
iOrderout=0x20;}else iOrderout=0x800;}else iOrderout=0x10;}else iOrderout=0x400;}else iOrderout=0x8;}else iOrderout=0x200;}else iOrderout=0x4;}else iOrderout=0x100;}else
iOrderout=0x2;}else iOrderout=0x80;}else iOrderout=0x1;}else iOrderout=0x40;
			(*tl_CAMBRIDGE_b_instance).Display=(*tl_b_instance).Display & 0x7F;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_b_instance,Display);
			if (iOrder>16)
			{
				if ((*tl_b_instance).Display2 & 0x80)
				{
					iOrderout|=0x1000;//ChemDraw cannot process orred dative
				}
				(*tl_CAMBRIDGE_b_instance).Display2=(*tl_b_instance).Display2 & 0x7F;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_b_instance,Display2);
			}
			if ((iOrder<=16))
			{
				if ((*tl_b_instance).Display & 0x80)
				{
					iOrderout=0x1000;//which is why it gets omitted at least here.
				}
			}
			(*tl_CAMBRIDGE_b_instance).Order=iOrderout;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_b_instance,Order);
		}
	}
}

void CONVCAMBRIDGE_text(CAMBRIDGE_page_instance * master)
{
	int backval=0;
	multilist<CAMBRIDGE_t_instance> * tl_CAMBRIDGE_t_multilist=retrievemultilist<CAMBRIDGE_t_instance>();
	multilist<t_instance> * tl_t_multilist=retrievemultilist<t_instance>();
	for (int ilv1=0;ilv1<(*tl_t_multilist).filllevel;ilv1++)
	{
		if ((*tl_t_multilist).bufferlist()[ilv1].exist==0) continue;
		if (edit_fileoperationrefersonlytopartofdocument)
		{
			if ((selection_currentselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_t))==0)
			{
				continue;
			}
		}
		t_instance * tl_t_instance=(*tl_t_multilist).bufferlist()+ilv1;
		
		ADD_TO_MULTILISTREFERENCE(master,t);
		CONVCAMBRIDGE_COLORCONV(t);
		TELESCOPE_aggressobject(tl_t_multilist,ilv1);
		backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
		while (backval>0)
		{
			s_instance * nd_s_instance=(s_instance*)TELESCOPE_getproperty();
			CONVCAMBRIDGE_s(tl_CAMBRIDGE_t_instance,(s_instance*)TELESCOPE_getproperty(),(char*)TELESCOPE_getproperty_contents());
			backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
		}
		CONVCAMBRIDGE_COLORCONV(t);
		(*tl_CAMBRIDGE_t_instance).BoundingBox=(*tl_t_instance).BoundingBox;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_t_instance,BoundingBox);
		(*tl_CAMBRIDGE_t_instance).Justification=(*tl_t_instance).Justification;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_t_instance,Justification);
		(*tl_CAMBRIDGE_t_instance).LabelAlignment=(*tl_t_instance).LabelAlignment;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_t_instance,LabelAlignment);
		(*tl_CAMBRIDGE_t_instance).p.x=(*tl_t_instance).xyz.x;//BACKWARDS COMPATIBILITY
		(*tl_CAMBRIDGE_t_instance).p.y=(*tl_t_instance).xyz.y;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_t_instance,p);//BACKWARDS COMPATIBILITY
		(*tl_CAMBRIDGE_t_instance).xyz=(*tl_t_instance).xyz;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_t_instance,xyz);
		if ((*tl_t_instance).RotationAngle!=0)
		{
			(*tl_CAMBRIDGE_t_instance).RotationAngle=((*tl_t_instance).RotationAngle)/Pi*180.0*65536.0;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_t_instance,RotationAngle);
		}
		(*tl_CAMBRIDGE_t_instance).id=janitor_id_list[STRUCTURE_OBJECTTYPE_t-1]+(*tl_t_instance).id;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_t_instance,id);
		(*tl_CAMBRIDGE_t_instance).Z=(*tl_t_instance).Z;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_t_instance,Z);
	}
}

void CONVCAMBRIDGE_fragments(CAMBRIDGE_page_instance * master)
{
	int iatomnr=0;
	multilist<CAMBRIDGE_fragment_instance> * tl_CAMBRIDGE_fragment_multilist=retrievemultilist<CAMBRIDGE_fragment_instance>();
	selection_clearselection(selection_clickselection);
	selection_clickselection_found=0;
	ibackn:
	selection_clearselection(selection_fragmentselection);
	selection_fragmentselection_found=0;
	iatomnr=findunfragmentedatom();
	if (iatomnr!=-1)
	{
		ibackb:
		select_fragment_by_atom(iatomnr);
		if (edit_fileoperationrefersonlytopartofdocument) selection_ANDselection(selection_fragmentselection,selection_currentselection);

		ADD_TO_MULTILISTREFERENCE(master,fragment);
		tl_CAMBRIDGE_fragment_instance->BoundingBox.left=32767;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_fragment_instance,BoundingBox);
		tl_CAMBRIDGE_fragment_instance->BoundingBox.right=0;
		tl_CAMBRIDGE_fragment_instance->BoundingBox.top=32767;
		tl_CAMBRIDGE_fragment_instance->BoundingBox.bottom=0;
		CONVCAMBRIDGE_atoms(tl_CAMBRIDGE_fragment_instance,&(tl_CAMBRIDGE_fragment_instance->BoundingBox));
		CONVCAMBRIDGE_bonds(tl_CAMBRIDGE_fragment_instance);
		goto ibackn;
	}
	selection_clearselection(selection_fragmentselection);
	selection_fragmentselection_found=0;
	int ibondnr=findunfragmentedbond();
	if (ibondnr!=-1)
	{
		iatomnr=bond_actual_node[ibondnr].start;
		goto ibackb;
	}
}

void CONVCAMBRIDGE_arrow(CAMBRIDGE_page_instance * master)
{
	multilist<CAMBRIDGE_arrow_instance> * tl_CAMBRIDGE_arrow_multilist=retrievemultilist<CAMBRIDGE_arrow_instance>();
	multilist<arrow_instance> * tl_arrow_multilist=retrievemultilist<arrow_instance>();
	for (int ilv1=0;ilv1<(*tl_arrow_multilist).filllevel;ilv1++)
	{
		if (edit_fileoperationrefersonlytopartofdocument)
		{
			if ((selection_currentselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_arrow))==0)
			{
				continue;
			}
		}
		arrow_instance * tl_arrow_instance=(*tl_arrow_multilist).bufferlist()+ilv1;
		if (tl_arrow_instance->exist)
		{
			ADD_TO_MULTILISTREFERENCE(master,arrow);
			CONVCAMBRIDGE_COLORCONV(arrow);
			(*tl_CAMBRIDGE_arrow_instance).Head3D=(*tl_arrow_instance).Head3D;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,Head3D);
			(*tl_CAMBRIDGE_arrow_instance).Tail3D=(*tl_arrow_instance).Tail3D;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,Tail3D);
			(*tl_CAMBRIDGE_arrow_instance).Center3D=(*tl_arrow_instance).Center3D;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,Center3D);
			(*tl_CAMBRIDGE_arrow_instance).LineType=(*tl_arrow_instance).LineType;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,LineType);
			(*tl_CAMBRIDGE_arrow_instance).FillType=(*tl_arrow_instance).FillType;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,FillType);
			(*tl_CAMBRIDGE_arrow_instance).AngularSize=(*tl_arrow_instance).AngularSize;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,AngularSize);
			(*tl_CAMBRIDGE_arrow_instance).MajorAxisEnd3D=(*tl_arrow_instance).MajorAxisEnd3D;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,MajorAxisEnd3D);
			(*tl_CAMBRIDGE_arrow_instance).MinorAxisEnd3D=(*tl_arrow_instance).MinorAxisEnd3D;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,MinorAxisEnd3D);
			(*tl_CAMBRIDGE_arrow_instance).ArrowheadType=(*tl_arrow_instance).ArrowheadType;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,ArrowheadType);
			(*tl_CAMBRIDGE_arrow_instance).ArrowheadHead=(*tl_arrow_instance).ArrowheadHead;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,ArrowheadHead);
			(*tl_CAMBRIDGE_arrow_instance).ArrowheadTail=(*tl_arrow_instance).ArrowheadTail;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,ArrowheadTail);
			(*tl_CAMBRIDGE_arrow_instance).ArrowShaftSpacing=(*tl_arrow_instance).ArrowShaftSpacing;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,ArrowShaftSpacing);
			(*tl_CAMBRIDGE_arrow_instance).ArrowShaftSpacing=(*tl_CAMBRIDGE_arrow_instance).ArrowShaftSpacing*150;
			(*tl_CAMBRIDGE_arrow_instance).Z=(*tl_arrow_instance).Z;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_arrow_instance,Z);
		}
	}
}
void CONVCAMBRIDGE_graphic(CAMBRIDGE_page_instance * master)
{
	multilist<CAMBRIDGE_graphic_instance> * tl_CAMBRIDGE_graphic_multilist=retrievemultilist<CAMBRIDGE_graphic_instance>();
	multilist<graphic_instance> * tl_graphic_multilist=retrievemultilist<graphic_instance>();
	for (int ilv1=0;ilv1<(*tl_graphic_multilist).filllevel;ilv1++)
	{
		if (edit_fileoperationrefersonlytopartofdocument)
		{
			if ((selection_currentselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_graphic))==0)
			{
				continue;
			}
		}
		graphic_instance * tl_graphic_instance=(*tl_graphic_multilist).bufferlist()+ilv1;
		if (tl_graphic_instance->exist)
		{
			ADD_TO_MULTILISTREFERENCE(master,graphic);
			CONVCAMBRIDGE_COLORCONV(graphic);
			(*tl_CAMBRIDGE_graphic_instance).BoundingBox=(*tl_graphic_instance).BoundingBox;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,BoundingBox);
			(*tl_CAMBRIDGE_graphic_instance).GraphicType=(*tl_graphic_instance).GraphicType;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,GraphicType);
			switch ((*tl_graphic_instance).GraphicType)
			{
				case 1://Line
				case 2://Arc
				case 6://Bracket
				{
					(*tl_CAMBRIDGE_graphic_instance).LineType=(*tl_graphic_instance).LineType;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,LineType);
					break;
				}
				case 3://Rectangle
				{
					if ((*tl_graphic_instance).CornerRadius>0)
					{
						(*tl_CAMBRIDGE_graphic_instance).CornerRadius=(*tl_graphic_instance).CornerRadius;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,CornerRadius);
						(*tl_CAMBRIDGE_graphic_instance).CornerRadius*=100;
						(*tl_CAMBRIDGE_graphic_instance).RectangleType|=1;
					}
					int tl_f=tl_graphic_instance->LineType;
					(*tl_CAMBRIDGE_graphic_instance).RectangleType|=((tl_f & 1)!=0)*0x10+((tl_f & 2)!=0)*0x20;
					tl_f=tl_graphic_instance->FillType;
					(*tl_CAMBRIDGE_graphic_instance).RectangleType|=((tl_f & 1)!=0)*0x8+((tl_f & 2)!=0)*0x4;
					AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,RectangleType);
					break;
				}
				case 4://Oval
				{
					int tl_f=tl_graphic_instance->LineType;
					(*tl_CAMBRIDGE_graphic_instance).OvalType|=((tl_f & 1)!=0)*0x8+((tl_f & 2)!=0)*0x10;
					tl_f=tl_graphic_instance->FillType;
					(*tl_CAMBRIDGE_graphic_instance).OvalType|=((tl_f & 1)!=0)*0x4+((tl_f & 2)!=0)*0x2;
					AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,OvalType);
					break;
				}
				case 5://Orbital
				{
					break;
				}
				case 7://Symbol
				{
					(*tl_CAMBRIDGE_graphic_instance).SymbolType=(*tl_graphic_instance).SymbolType;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,SymbolType);
					break;
				}
			}
			(*tl_CAMBRIDGE_graphic_instance).BracketType=(*tl_graphic_instance).BracketType;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,BracketType);
			if (edit_getBoundingBoxMode(tl_graphic_instance)>0)
			{
				(*tl_CAMBRIDGE_graphic_instance).Center3D=(*tl_graphic_instance).Center3D;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,Center3D);
				(*tl_CAMBRIDGE_graphic_instance).MajorAxisEnd3D=(*tl_graphic_instance).MajorAxisEnd3D;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,MajorAxisEnd3D);
				(*tl_CAMBRIDGE_graphic_instance).MinorAxisEnd3D=(*tl_graphic_instance).MinorAxisEnd3D;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,MinorAxisEnd3D);
			}
			(*tl_CAMBRIDGE_graphic_instance).id=janitor_id_list[STRUCTURE_OBJECTTYPE_graphic-1]+(*tl_graphic_instance).id;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,id);
			(*tl_CAMBRIDGE_graphic_instance).Z=(*tl_graphic_instance).Z;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,Z);
		}
	}
}
void CONVCAMBRIDGE_represent(CAMBRIDGE_graphic_instance * master,int attribute,int object)
{
	multilist<CAMBRIDGE_represent_instance> * tl_CAMBRIDGE_represent_multilist=retrievemultilist<CAMBRIDGE_represent_instance>();
	ADD_TO_MULTILISTREFERENCE(master,represent);
	(*tl_CAMBRIDGE_represent_instance).object=object;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_represent_instance,object);
	(*tl_CAMBRIDGE_represent_instance).attribute=attribute;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_represent_instance,attribute);
}
void CONVCAMBRIDGE_Symbol(CAMBRIDGE_page_instance * master)
{
	_u32 icompare;
	int tl_size;
	int ifilllevel;
	int i_offset;
	int i_id_offset;
	int backval;
	CDXMLREAD_functype dummy;
	multilist<CAMBRIDGE_graphic_instance> * tl_CAMBRIDGE_graphic_multilist=retrievemultilist<CAMBRIDGE_graphic_instance>();
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		basicmultilist * tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		icompare=(1<<ilv1);
		tl_size=(*tl_multilist).itemsize;
		i_offset=(*tl_multilist).getproperties("xyz",&dummy);
		if (i_offset<0) i_offset=(*tl_multilist).getproperties("Center3D",&dummy);
		if (i_offset<0) goto skipthismultilist;
		i_id_offset=(*tl_multilist).getproperties("id",&dummy);
		if (i_id_offset<0) goto skipthismultilist;
		ifilllevel=(*tl_multilist).filllevel;
		for (int ilv2=0;ilv2<ifilllevel;ilv2++)
		{
			if (edit_fileoperationrefersonlytopartofdocument)
			{
				if ((selection_currentselection[ilv2] & (1<<ilv1))==0)
				{
					continue;
				}
			}
			if ((*tl_multilist)[ilv2].exist==0) continue;
			TELESCOPE_aggressobject(tl_multilist,ilv2);
			backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_Symbol);
			while (backval>0)
			{
				Symbol_instance * tl_Symbol_instance=(Symbol_instance*)TELESCOPE_getproperty();
				ADD_TO_MULTILISTREFERENCE(master,graphic);
				CONVCAMBRIDGE_COLORCONV2(graphic,(*tl_Symbol_instance).color);
				cdx_Point3D tl_cdx_Point3D;
				tl_cdx_Point3D.x=(*tl_Symbol_instance).dxyz.x+((cdx_Point3D*)(((char*)((*tl_multilist).pointer))+tl_size*ilv2+i_offset))->x;
				tl_cdx_Point3D.y=(*tl_Symbol_instance).dxyz.y+((cdx_Point3D*)(((char*)((*tl_multilist).pointer))+tl_size*ilv2+i_offset))->y;
				tl_cdx_Point3D.z=(*tl_Symbol_instance).dxyz.z+((cdx_Point3D*)(((char*)((*tl_multilist).pointer))+tl_size*ilv2+i_offset))->z;
				(*tl_CAMBRIDGE_graphic_instance).Center3D=tl_cdx_Point3D;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,Center3D);
				(*tl_CAMBRIDGE_graphic_instance).BoundingBox.left=tl_cdx_Point3D.x;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,BoundingBox);
				(*tl_CAMBRIDGE_graphic_instance).BoundingBox.top=tl_cdx_Point3D.y;
				(*tl_CAMBRIDGE_graphic_instance).BoundingBox.right=tl_cdx_Point3D.x;
				(*tl_CAMBRIDGE_graphic_instance).BoundingBox.bottom=tl_cdx_Point3D.y+10;
				//TODO: Major/Minor axis
				(*tl_CAMBRIDGE_graphic_instance).SymbolType=(*tl_Symbol_instance).SymbolType;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,SymbolType);
				(*tl_CAMBRIDGE_graphic_instance).GraphicType=7;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_graphic_instance,GraphicType);
				int iid=*((_i32*)(((char*)((*tl_multilist).pointer))+tl_size*ilv2+i_id_offset))+janitor_id_list[ilv1-1];
				switch ((*tl_Symbol_instance).SymbolType)
				{
					case 2: case 3: case 4: case 5: case 8: case 9: CONVCAMBRIDGE_represent(tl_CAMBRIDGE_graphic_instance,0x421,iid);break;
				}
				switch ((*tl_Symbol_instance).SymbolType)
				{
					case 0: case 1: case 2: case 3: CONVCAMBRIDGE_represent(tl_CAMBRIDGE_graphic_instance,0x422,iid);break;
				}
				backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_Symbol);
			}
		}
		skipthismultilist:;
	}
}
void CONVCAMBRIDGE_curve(CAMBRIDGE_page_instance * master)
{
	multilist<CAMBRIDGE_curve_instance> * tl_CAMBRIDGE_curve_multilist=retrievemultilist<CAMBRIDGE_curve_instance>();
	multilist<curve_instance> * tl_curve_multilist=retrievemultilist<curve_instance>();
	for (int ilv1=0;ilv1<(*tl_curve_multilist).filllevel;ilv1++)
	{
		if (edit_fileoperationrefersonlytopartofdocument)
		{
			if ((selection_currentselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_curve))==0)
			{
				continue;
			}
		}
		curve_instance * tl_curve_instance=(*tl_curve_multilist).bufferlist()+ilv1;
		if (tl_curve_instance->exist)
		{
			ADD_TO_MULTILISTREFERENCE(master,curve);
			CONVCAMBRIDGE_COLORCONV(curve);
			(*tl_CAMBRIDGE_curve_instance).Z=(*tl_curve_instance).Z;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_curve_instance,Z);
			(*tl_CAMBRIDGE_curve_instance).ArrowheadType=(*tl_curve_instance).ArrowheadType;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_curve_instance,ArrowheadType);
			(*tl_CAMBRIDGE_curve_instance).ArrowheadHead=(*tl_curve_instance).ArrowheadHead;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_curve_instance,ArrowheadHead);
			(*tl_CAMBRIDGE_curve_instance).ArrowheadTail=(*tl_curve_instance).ArrowheadTail;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_curve_instance,ArrowheadTail);
			(*tl_CAMBRIDGE_curve_instance).CurvePoints=(*tl_curve_instance).CurvePoints;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_curve_instance,CurvePoints);
			(*tl_CAMBRIDGE_curve_instance).FillType=(*tl_curve_instance).FillType;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_curve_instance,FillType);
			(*tl_CAMBRIDGE_curve_instance).Closed=(*tl_curve_instance).Closed;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_curve_instance,Closed);
			(*tl_CAMBRIDGE_curve_instance).CurveType=(((*tl_curve_instance).LineType&3)<<1)|(((*tl_curve_instance).LineType&4)<<7);AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_curve_instance,CurveType);
		}
	}
}

void CONVCAMBRIDGE_tlcplate(CAMBRIDGE_page_instance * master)
{
	multilist<CAMBRIDGE_tlcplate_instance> * tl_CAMBRIDGE_tlcplate_multilist=retrievemultilist<CAMBRIDGE_tlcplate_instance>();
	multilist<tlcplate_instance> * tl_tlcplate_multilist=retrievemultilist<tlcplate_instance>();
	for (int ilv1=0;ilv1<(*tl_tlcplate_multilist).filllevel;ilv1++)
	{
		if (edit_fileoperationrefersonlytopartofdocument)
		{
			if ((selection_currentselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_tlcplate))==0)
			{
				continue;
			}
		}
		tlcplate_instance * tl_tlcplate_instance=(*tl_tlcplate_multilist).bufferlist()+ilv1;
		if (tl_tlcplate_instance->exist)
		{
			print("PLATE\n");
			int tl_backval=0;
			ADD_TO_MULTILISTREFERENCE(master,tlcplate);
			CONVCAMBRIDGE_COLORCONV(tlcplate);
			(*tl_CAMBRIDGE_tlcplate_instance).Z=(*tl_tlcplate_instance).Z;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_tlcplate_instance,Z);
			(*tl_CAMBRIDGE_tlcplate_instance).TopLeft=(*tl_tlcplate_instance).TopLeft;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_tlcplate_instance,TopLeft);
			(*tl_CAMBRIDGE_tlcplate_instance).TopRight=(*tl_tlcplate_instance).TopRight;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_tlcplate_instance,TopRight);
			(*tl_CAMBRIDGE_tlcplate_instance).BottomLeft=(*tl_tlcplate_instance).BottomLeft;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_tlcplate_instance,BottomLeft);
			(*tl_CAMBRIDGE_tlcplate_instance).BottomRight=(*tl_tlcplate_instance).BottomRight;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_tlcplate_instance,BottomRight);
/*			(*tl_CAMBRIDGE_tlcplate_instance).OriginFraction=(*tl_tlcplate_instance).OriginFraction;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_tlcplate_instance,OriginFraction);
			(*tl_CAMBRIDGE_tlcplate_instance).SolventFrontFraction=(*tl_tlcplate_instance).SolventFrontFraction;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_tlcplate_instance,SolventFrontFraction);*/
			TELESCOPE_aggressobject(tl_tlcplate_multilist,ilv1);
			tl_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_tlclane);
			while (tl_backval>0)
			{
				print("LANE\n");
				ADD_TO_MULTILISTREFERENCE(tl_CAMBRIDGE_tlcplate_instance,tlclane);
				tlclane_instance * tl_tlclane_instance=(tlclane_instance*)TELESCOPE_getproperty();
				for (int ilv2=0;ilv2<tl_tlclane_instance->length-sizeof(tlclane_instance);ilv2+=sizeof(cdx_tlcspot))
				{
					cdx_tlcspot * tl_tlcspot_instance=(cdx_tlcspot*)((char*)TELESCOPE_getproperty_contents()+ilv2);
					ADD_TO_MULTILISTREFERENCE(tl_CAMBRIDGE_tlclane_instance,tlcspot);
					CONVCAMBRIDGE_COLORCONV(tlcspot);
					(*tl_CAMBRIDGE_tlcspot_instance).Rf=(*tl_tlcspot_instance).Rf;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_tlcspot_instance,Rf);
					(*tl_CAMBRIDGE_tlcspot_instance).CurveType=(*tl_tlcspot_instance).CurveType;AUTOSTRUCT_EXISTS_SET_NAME(tl_CAMBRIDGE_tlcspot_instance,CurveType);
					print("SPOT\n");
				}
				tl_backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_tlclane);
			}
		}
	}
}

void CONVCAMBRIDGE_internaltomain(CAMBRIDGE_page_instance * master)
{
	filestructure_text_buffer.count=0;
	filestructure_curve_buffer.count=0;
	CONVCAMBRIDGE_fragments(master);
	CONVCAMBRIDGE_text(master);
	CONVCAMBRIDGE_arrow(master);
	CONVCAMBRIDGE_graphic(master);
	CONVCAMBRIDGE_Symbol(master);
	CONVCAMBRIDGE_curve(master);
	CONVCAMBRIDGE_tlcplate(master);
}
