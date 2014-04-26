//Contains the enums needed for cdxml encoding
bienum CDXML_ArrowType[]={
{{"NoHead"},0},
{{"HalfHead"},1},
{{"FullHead"},2},
{{"Resonance"},4},
{{"Equilibrium"},8},
{{"Hollow"},16},
{{"RetroSynthetic"},32},
};
int CDXML_ArrowType_max=sizeof(CDXML_ArrowType)/sizeof(CDXML_ArrowType[0]);
bienum CDXML_ArrowheadHead[]={
{{"None"},0},
{{"Full"},1},
{{"HalfLeft"},2},
{{"HalfRight"},3},
};
int CDXML_ArrowheadHead_max=sizeof(CDXML_ArrowheadHead)/sizeof(CDXML_ArrowheadHead[0]);
bienum CDXML_ArrowheadTail[]={
{{"None"},0},
{{"Full"},1},
{{"HalfLeft"},2},
{{"HalfRight"},3},
};
int CDXML_ArrowheadTail_max=sizeof(CDXML_ArrowheadTail)/sizeof(CDXML_ArrowheadTail[0]);
bienum CDXML_Closed[]={//Unverified
	{{"no"},0},
	{{"yes"},1},
};
int CDXML_Closed_max=sizeof(CDXML_Closed)/sizeof(CDXML_Closed[0]);
bienum CDXML_ArrowheadType[]={
{{"Solid"},0},//electron move,electronpair move, reaction, equillibrium, Resonance
{{"Hollow"},1},//Children's traffic arrow. Neat to show atom movements.
{{"Angle"},2},//Retrosynthetic
};
int CDXML_ArrowheadType_max=sizeof(CDXML_ArrowheadType)/sizeof(CDXML_ArrowheadType[0]);
bienum CDXML_BracketType[]={
	{{"RoundPair"},0},
	{{"SquarePair"},1},
	{{"CurlyPair"},2},
	{{"Square"},3},
	{{"Curly"},4},
	{{"Round"},5},
};
int CDXML_BracketType_max=sizeof(CDXML_BracketType)/sizeof(CDXML_BracketType[0]);
bienum CDXML_GraphicType[]={
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
bienum CDXML_SymbolType[]={
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
bienum CDXML_DoublePosition[]={
	{{"AutoCenter"},0},
	{{"AutoRight"},1},
	{{"AutoLeft"},2},
	{{"Center"},0x100},
	{{"Left"},0x101},
	{{"Right"},0x102},
};
int CDXML_DoublePosition_max=sizeof(CDXML_DoublePosition)/sizeof(CDXML_DoublePosition[0]);
bienum CDXML_OvalType[]={
	{{"Circle"},0x1},
	{{"Shaded"},0x2},
	{{"Filled"},0x4},
	{{"Dashed"},0x8},
	{{"Bold"},0x10},
	{{"Shadowed"},0x20},
};
int CDXML_OvalType_max=sizeof(CDXML_OvalType)/sizeof(CDXML_OvalType[0]);
bienum CDXML_Display2[]={
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
bienum CDXML_Display[]={
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
bienum CDXML_Justification[]={
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
bienum CDXML_LabelAlignment[]={
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
bienum CDXML_OrbitalType[]={
	{{"s"},0},
	{{"oval"},1},
	{{"lobe"},2},
	{{"p"},3},
	{{"hybridPlus"},4},
	{{"hybridMinus"},5},
	{{"dz2Plus"},6},
	{{"dz2Minus"},7},
	{{"dxy"},8},
	{{"sShaded"},0x100},
	{{"ovalShaded"},0x101},
	{{"lobeShaded"},0x102},
	{{"pShaded"},0x103},
	{{"sFilled"},0x200},
	{{"ovalFilled"},0x201},
	{{"lobeFilled"},0x202},
	{{"pFilled"},0x203},
	{{"hybridPlusFilled"},0x204},
	{{"hybridMinusFilled"},0x205},
	{{"dz2PlusFilled"},0x206},
	{{"dz2MinusFilled"},0x207},
	{{"dxyFilled"},0x208},
};
int CDXML_OrbitalType_max=sizeof(CDXML_OrbitalType)/sizeof(CDXML_OrbitalType[0]);
bienum CDXML_ExternalConnectionType[]={
	{{"Unspecified"},0},
	{{"Diamond"},1},
	{{"Star"},2},
	{{"PolymerBead"},3},
	{{"Wavy"},4},
};
int CDXML_ExternalConnectionType_max=sizeof(CDXML_ExternalConnectionType)/sizeof(CDXML_ExternalConnectionType[0]);
bienum CDXML_RectangleType[]={
	{{"Plain"},0x0},
	{{"RoundEdge"},0x1},
	{{"Shadow"},0x2},
	{{"Shaded"},0x4},
	{{"Filled"},0x8},
	{{"Dashed"},0x10},
	{{"Bold"},0x20},
};
int CDXML_RectangleType_max=sizeof(CDXML_RectangleType)/sizeof(CDXML_RectangleType[0]);
bienum CDXML_LineType[]={
	{{"Solid"},0x0},
	{{"Dashed"},0x1},
	{{"Bold"},0x2},
	{{"Wavy"},0x4},
};
int CDXML_LineType_max=sizeof(CDXML_LineType)/sizeof(CDXML_LineType[0]);
bienum CDXML_FillType[]={
	{{"None"},0x0},
	{{"Solid"},0x1},//warning! 
	{{"Shaded"},0x2},
	{{"Horz"},0x4},//LHENDRAW EXTENSION
	{{"Vert"},0x8},//LHENDRAW EXTENSION
	{{"Respective_To_Internal_Cosy"},0x16},//LHENDRAW EXTENSION
};
int CDXML_FillType_max=sizeof(CDXML_FillType)/sizeof(CDXML_FillType[0]);
bienum CDXML_type[]={
	{{"Generic"},0},
	{{"Atom"},1},
	{{"Bond"},2},
	{{"None"},0x10000},
	{{"Some"},0x20000},
	{{"Continuous"},0x40000},
};
int CDXML_type_max=sizeof(CDXML_type)/sizeof(CDXML_type[0]);
bienum CDXML_attribute[]={
	{{"Charge"},0x421},//TODO: correct numbers!
	{{"Radical"},0x422},
};
int CDXML_attribute_max=sizeof(CDXML_attribute)/sizeof(CDXML_attribute[0]);
bienum CDXML_NodeType[]={
	{{"Unspecified"},0},
	{{"Element"},1},
	{{"ElementList"},2},
	{{"ElementListNickname"},3},
	{{"Nickname"},4},
	{{"Fragment"},5},
	{{"Formula"},6},
	{{"GenericNickname"},7},
	{{"AnonymousAlternativeGroup"},8},
	{{"NamedAlternativeGroup"},9},
	{{"MultiAttachment"},10},
	{{"VariableAttachment"},11},
	{{"ExternalConnectionPoint"},12},
	{{"LinkNode"},13},
};
int CDXML_NodeType_max=sizeof(CDXML_NodeType)/sizeof(CDXML_NodeType[0]);
bienum CDXML_modifier[]={
	{{"SHIFT"},0x1},
	{{"CTRL"},0x2},
	{{"ALT"},0x4},
	{{"HYPER"},0x8},
};
int CDXML_modifier_max=sizeof(CDXML_modifier)/sizeof(CDXML_modifier[0]);
