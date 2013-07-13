bienum CDXML_ArrowType[]{
{{"NoHead"},0},
{{"HalfHead"},1},
{{"FullHead"},2},
{{"Resonance"},4},
{{"Equilibrium"},8},
{{"Hollow"},16},
{{"RetroSynthetic"},32},
};
int CDXML_ArrowType_max=sizeof(CDXML_ArrowType)/sizeof(CDXML_ArrowType[0]);
bienum CDXML_GraphicType[] {
	{{"Undefined"},0},
	{{"Line"},1},
	{{"Arc"},2},
	{{"Rectangle"},3},
	{{"Oval"},4},
	{{"Orbital"},5},
	{{"Bracket"},6},
	{{"Symbol"},7},
};
int CDXML_GraphicType_max=sizeof(CDXML_GraphicType)/sizeof(CDXML_GraphicType[0]);
bienum CDXML_DoublePosition[] {
	{{"AutoCenter"},0},
	{{"AutoRight"},1},
	{{"AutoLeft"},2},
	{{"Center"},0x100},
	{{"Left"},0x101},
	{{"Right"},0x102},
};
int CDXML_DoublePosition_max=sizeof(CDXML_DoublePosition)/sizeof(CDXML_DoublePosition[0]);
