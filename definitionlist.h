typedef struct namelist_
{
	char input[5];
	char output[5];
}namelist_;
namelist_ list_greeklist[]={
{"A","Α"},
{"B","Β"},
{"C","Χ"},
{"D","Δ"},
{"E","Ε"},
{"F","Φ"},
{"G","Γ"},
{"H","Η"},
{"I","Ι"},
{"J","ϑ"},
{"K","Κ"},
{"L","Λ"},
{"M","Μ"},
{"N","Ν"},
{"O","Ο"},
{"P","Π"},
{"Q","Θ"},
{"R","Ρ"},
{"S","Σ"},
{"T","Τ"},
{"U","Υ"},
{"V","ς"},
{"W","Ω"},
{"X","Ξ"},
{"Y","Ψ"},
{"Z","Ζ"},
{"a","α"},
{"b","β"},
{"c","χ"},
{"d","δ"},
{"e","ε"},
{"f","ϕ"},
{"g","γ"},
{"h","η"},
{"i","ι"},
{"j","φ"},
{"k","κ"},
{"l","λ"},
{"m","μ"},
{"n","ν"},
{"o","ο"},
{"p","π"},
{"q","θ"},
{"r","ρ"},
{"s","σ"},
{"t","τ"},
{"u","υ"},
{"v","ϖ"},
{"w","ω"},
{"x","ξ"},
{"y","ψ"},
{"z","ζ"},
};
struct list_bookstavecode
{
	char name[12];
	char unicode[8];
};
list_bookstavecode list_xml[]
{
	{{"lt"},{"<"}},
	{{"gt"},{">"}},
	{{"amp"},{"&"}},
	{{"apos"},{"'"}},
	{{"quot"},{"\""}},
};
_small list_greeklist_size=sizeof(list_greeklist)/sizeof(list_greeklist[0]);
char * get_list_greeklist(const char * input)
{
	for (int ilv1=0;ilv1<list_greeklist_size;ilv1++)
	{
		if (strcmp(list_greeklist[ilv1].input,input)==0)
		{
			return list_greeklist[ilv1].output;
		}
	}
	return 0;
}
