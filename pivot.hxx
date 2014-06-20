cdx_Point3D edit_pivot;
intl edit_pivot_counter;
void get_selection_pivot()
{
	edit_pivot_counter=0;
	edit_pivot.x=0;edit_pivot.y=0;edit_pivot.z=0;
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		n_instance * tl_n_instance=(*glob_n_multilist).bufferlist+ilv1;
		if ((*tl_n_instance).exist)
		{
			edit_pivot.x+=(*tl_n_instance).xyz.x;
			edit_pivot.y+=(*tl_n_instance).xyz.y;
			edit_pivot.z+=(*tl_n_instance).xyz.z;
			edit_pivot_counter++;
		}
	}
	edit_pivot.x/=edit_pivot_counter;
	edit_pivot.y/=edit_pivot_counter;
	edit_pivot.z/=edit_pivot_counter;
}
void applytransform(float matrix[3][3])
{
	cdx_Point3D xyz;
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		n_instance * tl_n_instance=(*glob_n_multilist).bufferlist+ilv1;
		if ((*tl_n_instance).exist)
		{
			xyz.x=(*tl_n_instance).xyz.x-edit_pivot.x;
			xyz.y=(*tl_n_instance).xyz.y-edit_pivot.y;
			xyz.z=(*tl_n_instance).xyz.z-edit_pivot.z;
			(*tl_n_instance).xyz.x=xyz.x*matrix[0][0]+xyz.y*matrix[1][0]+xyz.z*matrix[2][0]+edit_pivot.x;
			(*tl_n_instance).xyz.y=xyz.x*matrix[0][1]+xyz.y*matrix[1][1]+xyz.z*matrix[2][1]+edit_pivot.y;
			(*tl_n_instance).xyz.z=xyz.x*matrix[0][2]+xyz.y*matrix[1][2]+xyz.z*matrix[2][2]+edit_pivot.z;
		}
	}
	return;
}
