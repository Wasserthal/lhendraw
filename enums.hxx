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
bienum CDXML_ArrowHeadHead[]{
{{"None"},0},
{{"Full"},1},
{{"HalfLeft"},2},
{{"HalfRight"},3},
};
int CDXML_ArrowHeadHead_max=sizeof(CDXML_ArrowHeadHead)/sizeof(CDXML_ArrowHeadHead[0]);
bienum CDXML_ArrowHeadTail[]{
{{"None"},0},
{{"Full"},1},
{{"HalfLeft"},2},
{{"HalfRight"},3},
};
int CDXML_ArrowHeadTail_max=sizeof(CDXML_ArrowHeadTail)/sizeof(CDXML_ArrowHeadTail[0]);
bienum CDXML_ArrowHeadType[]{
{{"Solid"},0},//electron move,electronpair move, reaction, equillibrium, Resonance
{{"Hollow"},1},//Children's traffic arrow. Neat to show atom movements.
{{"Angle"},2},//Retrosynthetic
{{"LhendrawInternalOffByOne"},55},//Tells me that that arrowhead is shifted, as in equillibrium.
};
int CDXML_ArrowHeadType_max=sizeof(CDXML_ArrowHeadType)/sizeof(CDXML_ArrowHeadType[0]);
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
bienum CDXML_SymbolType[] {
	{{"LonePair"},0},
	{{"Electron"},1},
	{{"RadicalCation"},2},
	{{"RadicalAnion"},3},
	{{"CirclePlus"},4},
	{{"CircleMinus"},5},
	{{"Dagger"},6},
	{{"DoubleDagger"},7},
	{{"Plus"},8},
	{{"Minus"},9},
	{{"Racemic"},10},
	{{"Absolute"},11},
	{{"Relative"},12},
};
int CDXML_SymbolType_max=sizeof(CDXML_SymbolType)/sizeof(CDXML_SymbolType[0]);
bienum CDXML_DoublePosition[] {
	{{"AutoCenter"},0},
	{{"AutoRight"},1},
	{{"AutoLeft"},2},
	{{"Center"},0x100},
	{{"Left"},0x101},
	{{"Right"},0x102},
};
int CDXML_DoublePosition_max=sizeof(CDXML_DoublePosition)/sizeof(CDXML_DoublePosition[0]);
bienum CDXML_OvalType[] {
	{{"Circle"},0x1},
	{{"Shaded"},0x2},
	{{"Filled"},0x4},
	{{"Dashed"},0x8},
	{{"Bold"},0x10},
	{{"Shadowed"},0x20},
};
int CDXML_OvalType_max=sizeof(CDXML_OvalType)/sizeof(CDXML_OvalType[0]);
bienum CDXML_Display2[] {
	{{"Solid"},0},
	{{"Dash"},1},
	{{"Hash"},2},
	{{"WedgedHashBegin"},3},
	{{"WedgedHashEnd"},4},
	{{"Bold"},5},
	{{"WedgeBegin"},6},
	{{"WedgeEnd"},7},
	{{"Wavy"},8},
	{{"HollowWedgeBegin"},9},
	{{"HollowWedgeEnd"},10},
	{{"WavyWedgeBegin"},11},
	{{"WavyWedgeEnd"},12},
	{{"Dot"},13},
	{{"DashDot"},14},
};
int CDXML_Display2_max=sizeof(CDXML_Display2)/sizeof(CDXML_Display2[0]);
bienum CDXML_Display[] {
	{{"Solid"},0},
	{{"Dash"},1},
	{{"Hash"},2},
	{{"WedgedHashBegin"},3},
	{{"WedgedHashEnd"},4},
	{{"Bold"},5},
	{{"WedgeBegin"},6},
	{{"WedgeEnd"},7},
	{{"Wavy"},8},
	{{"HollowWedgeBegin"},9},
	{{"HollowWedgeEnd"},10},
	{{"WavyWedgeBegin"},11},
	{{"WavyWedgeEnd"},12},
	{{"Dot"},13},
	{{"DashDot"},14},
};
int CDXML_Display_max=sizeof(CDXML_Display)/sizeof(CDXML_Display[0]);
bienum CDXML_Justification[] {
	{{"Right"},-1},
	{{"Left"},0},
	{{"Center"},1},
	{{"Full"},2},
	{{"Above"},3},
	{{"Below"},4},
	{{"Auto"},5},
	{{"BestInitial"},6},
};
int CDXML_Justification_max=sizeof(CDXML_Justification)/sizeof(CDXML_Justification[0]);
bienum CDXML_LabelAlignment[] {
	{{"Right"},-1},
	{{"Left"},0},
	{{"Center"},1},
	{{"Full"},2},
	{{"Above"},3},
	{{"Below"},4},
	{{"Auto"},5},
	{{"BestInitial"},6},
};
int CDXML_LabelAlignment_max=sizeof(CDXML_LabelAlignment)/sizeof(CDXML_LabelAlignment[0]);
bienum CDXML_OrbitalType[] {
	{{"OrbitalType_s"},0},
	{{"OrbitalType_oval"},1},
	{{"OrbitalType_lobe"},2},
	{{"OrbitalType_p"},3},
	{{"OrbitalType_hybridPlus"},4},
	{{"OrbitalType_hybridMinus"},5},
	{{"OrbitalType_dz2Plus"},6},
	{{"OrbitalType_dz2Minus"},7},
	{{"OrbitalType_dxy"},8},
	{{"OrbitalType_sShaded"},0x100},
	{{"OrbitalType_ovalShaded"},0x101},
	{{"OrbitalType_lobeShaded"},0x102},
	{{"OrbitalType_pShaded"},0x103},
	{{"OrbitalType_sFilled"},0x200},
	{{"OrbitalType_ovalFilled"},0x201},
	{{"OrbitalType_lobeFilled"},0x202},
	{{"OrbitalType_pFilled"},0x203},
	{{"OrbitalType_hybridPlusFilled"},0x204},
	{{"OrbitalType_hybridMinusFilled"},0x205},
	{{"OrbitalType_dz2PlusFilled"},0x206},
	{{"OrbitalType_dz2MinusFilled"},0x207},
	{{"OrbitalType_dxyFilled"},0x208},
};
int CDXML_OrbitalType_max=sizeof(CDXML_OrbitalType)/sizeof(CDXML_OrbitalType[0]);
