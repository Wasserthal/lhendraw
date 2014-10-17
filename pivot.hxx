cdx_Point3D edit_pivot;
intl edit_pivot_counter;
void get_selection_pivot()
{
	intl ioffset;
	char * ibufferpos;
	_u32 icompare;
	float tl_x,tl_y,tl_z;
	int internalpointcount;
	char iAllofthem=0;
	edit_pivot_counter=0;
	edit_pivot.x=0;edit_pivot.y=0;edit_pivot.z=0;
	edit_judgeselection(-1);
	if (edit_singlepointselected)
	{
		float tl_startx,tl_starty,tl_startz;
		if (retrievepoints_basic(glob_n_multilist->bufferlist+edit_partner,&edit_pivot.x,&edit_pivot.y,&edit_pivot.z,0,STRUCTURE_OBJECTTYPE_n)>0)
		{
			return;
		}
	}
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		n_instance * tl_n_instance=(*glob_n_multilist).bufferlist+ilv1;
		if ((*tl_n_instance).exist)
		{
			if (selection_currentselection[ilv1]&(1<<STRUCTURE_OBJECTTYPE_n))
			{
				edit_pivot.x+=(*tl_n_instance).xyz.x;
				edit_pivot.y+=(*tl_n_instance).xyz.y;
				edit_pivot.z+=(*tl_n_instance).xyz.z;
				edit_pivot_counter++;
			}
		}
	}
	for (int ilv1=2;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		internalpointcount=retrieveprops_basic(-1,ilv1);
		icompare=1<<ilv1;
		int isize= STRUCTURE_OBJECTTYPE_List[ilv1].size;
		basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		if (tlmultilist==NULL) goto i_standard_fertig;
		CDXMLREAD_functype tldummy;
		ibufferpos=(char*)((*tlmultilist).pointer);
		for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
		{
			if ((*((basic_instance*)(ibufferpos+isize*ilv2))).exist)
			{
				iAllofthem=((selection_currentselection[ilv2]) & icompare)>0;
				for (int ilv3=-1;retrievepoints_basic(((basic_instance*)(ibufferpos+isize*ilv2)),&tl_x,&tl_y,&tl_z,ilv3,ilv1)>0;ilv3--)
				{
					if (ilv3<=-3) continue;//HACK: this is a hack because some components are undefined
					if ((selection_currentselection[ilv2*internalpointcount-ilv3+1] & (1<<(STRUCTURE_OBJECTTYPE_ListSize+ilv1))) || (iAllofthem))
					{
						edit_pivot.x+=tl_x;
						edit_pivot.y+=tl_y;
						edit_pivot.z+=tl_z;
						edit_pivot_counter++;
					}
				}
			}
		}
		i_standard_fertig:;
	}
	edit_pivot.x/=edit_pivot_counter;
	edit_pivot.y/=edit_pivot_counter;
	edit_pivot.z/=edit_pivot_counter;
}
void applytransform_single(float matrix[3][3],cdx_Point3D * input,cdx_Point3D * output,cdx_Point3D * pivot)//input and output may be the same
{
	cdx_Point3D xyz;
	xyz.x=input->x-pivot->x;
	xyz.y=input->y-pivot->y;
	xyz.z=input->z-pivot->z;
	output->x=xyz.x*matrix[0][0]+xyz.y*matrix[1][0]+xyz.z*matrix[2][0]+pivot->x;
	output->y=xyz.x*matrix[0][1]+xyz.y*matrix[1][1]+xyz.z*matrix[2][1]+pivot->y;
	output->z=xyz.x*matrix[0][2]+xyz.y*matrix[1][2]+xyz.z*matrix[2][2]+pivot->z;
}
void applytransform(float matrix[3][3])
{
	intl ioffset;
	float tl_x,tl_y,tl_z;
	char * ibufferpos;
	_u32 icompare;
	cdx_Point3D xyz;
	int internalpointcount;
	char iAllofthem=0;
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		n_instance * tl_n_instance=(*glob_n_multilist).bufferlist+ilv1;
		if ((*tl_n_instance).exist)
		{
			if (selection_currentselection[ilv1]&(1<<STRUCTURE_OBJECTTYPE_n))
			{
				xyz.x=(*tl_n_instance).xyz.x-edit_pivot.x;
				xyz.y=(*tl_n_instance).xyz.y-edit_pivot.y;
				xyz.z=(*tl_n_instance).xyz.z-edit_pivot.z;
				(*tl_n_instance).xyz.x=xyz.x*matrix[0][0]+xyz.y*matrix[1][0]+xyz.z*matrix[2][0]+edit_pivot.x;
				(*tl_n_instance).xyz.y=xyz.x*matrix[0][1]+xyz.y*matrix[1][1]+xyz.z*matrix[2][1]+edit_pivot.y;
				(*tl_n_instance).xyz.z=xyz.x*matrix[0][2]+xyz.y*matrix[1][2]+xyz.z*matrix[2][2]+edit_pivot.z;
			}
		}
	}
	for (int ilv1=2;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		internalpointcount=retrieveprops_basic(-1,ilv1);
		icompare=1<<ilv1;
		int isize= STRUCTURE_OBJECTTYPE_List[ilv1].size;
		basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		if (tlmultilist==NULL) goto i_standard_fertig;
		CDXMLREAD_functype tldummy;
		ibufferpos=(char*)((*tlmultilist).pointer);
		for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
		{
			if ((*((basic_instance*)(ibufferpos+isize*ilv2))).exist)
			{
				iAllofthem=((selection_currentselection[ilv2]) & icompare);
				for (int ilv3=-1;retrievepoints_basic(((basic_instance*)(ibufferpos+isize*ilv2)),&tl_x,&tl_y,&tl_z,ilv3,ilv1)>0;ilv3--)
				{
					if ((selection_currentselection[ilv2*internalpointcount+ilv3-1] & (1<<(STRUCTURE_OBJECTTYPE_ListSize+ilv1))) || (iAllofthem))
					{
						xyz.x=tl_x-edit_pivot.x;
						xyz.y=tl_y-edit_pivot.y;
						xyz.z=tl_z-edit_pivot.z;
						tl_x=xyz.x*matrix[0][0]+xyz.y*matrix[1][0]+xyz.z*matrix[2][0]+edit_pivot.x;
						tl_y=xyz.x*matrix[0][1]+xyz.y*matrix[1][1]+xyz.z*matrix[2][1]+edit_pivot.y;
						tl_z=xyz.x*matrix[0][2]+xyz.y*matrix[1][2]+xyz.z*matrix[2][2]+edit_pivot.z;
						placepoints_basic(((basic_instance*)(ibufferpos+isize*ilv2)),tl_x,tl_y,tl_z,ilv3,ilv1);
					}
				}
			}
		}
		i_standard_fertig:;
	}
	return;
}
catalogized_command_funcdef(PIVOT_TURNZ)
{
	float angle=atof(value);
	float matrix[3][3];
	get_selection_pivot();
	matrix[0][0]=cos(angle);
	matrix[0][1]=sin(angle);
	matrix[0][2]=0;
	matrix[1][0]=-sin(angle);
	matrix[1][1]=cos(angle);
	matrix[1][2]=0;
	matrix[2][0]=0;
	matrix[2][1]=0;
	matrix[2][2]=1;
	applytransform(matrix);
	return 1;
}
catalogized_command_funcdef(PIVOT_TURNX)
{
	float angle=atof(value);
	float matrix[3][3];
	get_selection_pivot();
	matrix[0][0]=1;
	matrix[0][1]=0;
	matrix[0][2]=0;
	matrix[1][0]=0;
	matrix[1][1]=cos(angle);
	matrix[1][2]=sin(angle);
	matrix[2][0]=0;
	matrix[2][1]=-sin(angle);
	matrix[2][2]=cos(angle);
	applytransform(matrix);
	return 1;
}
catalogized_command_funcdef(PIVOT_TURNY)
{
	float angle=atof(value);
	float matrix[3][3];
	get_selection_pivot();
	matrix[0][0]=cos(angle);
	matrix[0][1]=0;
	matrix[0][2]=-sin(angle);
	matrix[1][0]=0;
	matrix[1][1]=1;
	matrix[1][2]=0;
	matrix[2][0]=sin(angle);
	matrix[2][1]=0;
	matrix[2][2]=cos(angle);
	applytransform(matrix);
	return 1;
}
