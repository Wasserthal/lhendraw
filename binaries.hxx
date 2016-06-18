void binaries_LOAD_mol64(FILE * infile)
{
	_u64 atomcount;
	_u64 bondcount;
	_u64 dummy;
	fread(&dummy,8,1,infile);
	fread(&atomcount,8,1,infile);
	fread(&bondcount,8,1,infile);
	fread(&dummy,8,1,infile);
	_i32 atom_first_id=glob_n_multilist->maxid+1;
	for (int ilv1=0;ilv1<atomcount;ilv1++)
	{
		_i64 x,y,z;
		_u64 element;
		intl iindex=0;
		fread(&x,8,1,infile);
		fread(&y,8,1,infile);
		fread(&z,8,1,infile);
		fread(&element,8,1,infile);
		n_instance * tl_n_instance=edit_summonatom((int*)&iindex);
		tl_n_instance->xyz.x=x/4294967296.0;
		tl_n_instance->xyz.y=y/4294967296.0;
		tl_n_instance->xyz.z=z/4294967296.0;
		LABELTEXT("",(char*)&element,glob_n_multilist,tl_n_instance,iindex);
		tl_n_instance->id=ilv1+atom_first_id;
	}
	for (int ilv1=0;ilv1<bondcount;ilv1++)
	{
		_i32 start,end,params,dummy;
		intl iindex=0;
		fread(&start,4,1,infile);
		fread(&end,4,1,infile);
		fread(&params,4,1,infile);
		fread(&dummy,4,1,infile);
		b_instance * tl_b_instance=edit_summonbond(start+atom_first_id,end+atom_first_id,edit_load_lastfilllevel[STRUCTURE_OBJECTTYPE_n]+start,edit_load_lastfilllevel[STRUCTURE_OBJECTTYPE_n]+end);
	}
}
