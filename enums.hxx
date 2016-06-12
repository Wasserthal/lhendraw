//Contains the enums needed for cdxml encoding
bienum CAMBRIDGE_ArrowType[]={
{{"NoHead"},0},
{{"HalfHead"},1},
{{"FullHead"},2},
{{"Resonance"},4},
{{"Equilibrium"},8},
{{"Hollow"},16},
{{"RetroSynthetic"},32},
};
int CAMBRIDGE_ArrowType_max=sizeof(CAMBRIDGE_ArrowType)/sizeof(bienum);
bienum CAMBRIDGE_ArrowheadHead[]={
{{"None"},1},
{{"Full"},2},
{{"HalfLeft"},3},
{{"HalfRight"},4},
};
int CAMBRIDGE_ArrowheadHead_max=sizeof(CAMBRIDGE_ArrowheadHead)/sizeof(bienum);
bienum CAMBRIDGE_ArrowheadTail[]={
{{"None"},1},
{{"Full"},2},
{{"HalfLeft"},3},
{{"HalfRight"},4},
};
int CAMBRIDGE_ArrowheadTail_max=sizeof(CAMBRIDGE_ArrowheadTail)/sizeof(bienum);
bienum CAMBRIDGE_Closed[]={//Unverified
	{{"no"},0},
	{{"yes"},1},
};
int CAMBRIDGE_Closed_max=sizeof(CAMBRIDGE_Closed)/sizeof(bienum);
bienum CAMBRIDGE_ArrowheadType[]={
{{"Solid"},1},//electron move,electronpair move, reaction, equillibrium, Resonance
{{"Hollow"},2},//Children's traffic arrow. Neat to show atom movements.
{{"Angle"},3},//Retrosynthetic
};
int CAMBRIDGE_ArrowheadType_max=sizeof(CAMBRIDGE_ArrowheadType)/sizeof(bienum);
bienum CAMBRIDGE_BracketType[]={
	{{"RoundPair"},0},
	{{"SquarePair"},1},
	{{"CurlyPair"},2},
	{{"Square"},3},
	{{"Curly"},4},
	{{"Round"},5},
};
int CAMBRIDGE_BracketType_max=sizeof(CAMBRIDGE_BracketType)/sizeof(bienum);
bienum CAMBRIDGE_GraphicType[]={
	{{"Undefined"},0},
	{{"Line"},1},
	{{"Arc"},2},
	{{"Rectangle"},3},
	{{"Oval"},4},
	{{"Orbital"},5},
	{{"Bracket"},6},
	{{"Symbol"},7},
};
int CAMBRIDGE_GraphicType_max=sizeof(CAMBRIDGE_GraphicType)/sizeof(bienum);
bienum CAMBRIDGE_SymbolType[]={
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
int CAMBRIDGE_SymbolType_max=sizeof(CAMBRIDGE_SymbolType)/sizeof(bienum);
bienum CAMBRIDGE_DoublePosition[]={
	{{"AutoCenter"},0},
	{{"AutoRight"},1},
	{{"AutoLeft"},2},
	{{"Center"},0x100},
	{{"Left"},0x101},
	{{"Right"},0x102},
};
int CAMBRIDGE_DoublePosition_max=sizeof(CAMBRIDGE_DoublePosition)/sizeof(bienum);
bienum CAMBRIDGE_OvalType[]={
	{{"Circle"},0x1},
	{{"Shaded"},0x2},
	{{"Filled"},0x4},
	{{"Dashed"},0x8},
	{{"Bold"},0x10},
	{{"Shadowed"},0x20},
};
int CAMBRIDGE_OvalType_max=sizeof(CAMBRIDGE_OvalType)/sizeof(bienum);
bienum CAMBRIDGE_Display2[]={
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
	{{"dative"},0x80},
};
int CAMBRIDGE_Display2_max=sizeof(CAMBRIDGE_Display2)/sizeof(bienum);
bienum CAMBRIDGE_Display[]={
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
	{{"dative"},0x80},
};
int CAMBRIDGE_Display_max=sizeof(CAMBRIDGE_Display)/sizeof(bienum);
bienum CAMBRIDGE_Justification[]={
	{{"Right"},-1},
	{{"Left"},0},
	{{"Center"},1},
	{{"Full"},2},
	{{"Above"},3},
	{{"Below"},4},
	{{"Auto"},5},
	{{"BestInitial"},6},
};
int CAMBRIDGE_Justification_max=sizeof(CAMBRIDGE_Justification)/sizeof(bienum);
bienum CAMBRIDGE_LabelAlignment[]={
	{{"Right"},-1},
	{{"Left"},0},
	{{"Center"},1},
	{{"Full"},2},
	{{"Above"},3},
	{{"Below"},4},
	{{"Auto"},5},
	{{"BestInitial"},6},
};
int CAMBRIDGE_LabelAlignment_max=sizeof(CAMBRIDGE_LabelAlignment)/sizeof(bienum);
bienum CAMBRIDGE_OrbitalType[]={
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
int CAMBRIDGE_OrbitalType_max=sizeof(CAMBRIDGE_OrbitalType)/sizeof(bienum);
bienum CAMBRIDGE_ExternalConnectionType[]={
	{{"Unspecified"},0},
	{{"Diamond"},1},
	{{"Star"},2},
	{{"PolymerBead"},3},
	{{"Wavy"},4},
};
int CAMBRIDGE_ExternalConnectionType_max=sizeof(CAMBRIDGE_ExternalConnectionType)/sizeof(bienum);
bienum CAMBRIDGE_RectangleType[]={
	{{"Plain"},0x0},
	{{"RoundEdge"},0x1},
	{{"Shadow"},0x2},
	{{"Shaded"},0x4},
	{{"Filled"},0x8},
	{{"Dashed"},0x10},
	{{"Bold"},0x20},
};
int CAMBRIDGE_RectangleType_max=sizeof(CAMBRIDGE_RectangleType)/sizeof(bienum);
bienum CAMBRIDGE_LineType[]={
	{{"Solid"},0x0},
	{{"Dashed"},0x1},
	{{"Bold"},0x2},
	{{"Wavy"},0x4},//or doubled in tlcspot and curve
};
int CAMBRIDGE_LineType_max=sizeof(CAMBRIDGE_LineType)/sizeof(bienum);
bienum CAMBRIDGE_CurveType[]={
	{{"dashed"},0x2},
	{{"filled"},0x80},
	{{"bold"},0x4},
	{{"doubled"},0x200},
};
int CAMBRIDGE_CurveType_max=sizeof(CAMBRIDGE_CurveType)/sizeof(bienum);
bienum CAMBRIDGE_FillType[]={
	{{"None"},0x0},
	{{"Solid"},0x1},//warning! 
	{{"Shaded"},0x2},
	{{"Horz"},0x4},//LHENDRAW EXTENSION
	{{"Vert"},0x8},//LHENDRAW EXTENSION
	{{"Respective_To_Internal_Cosy"},0x16},//LHENDRAW EXTENSION
};
int CAMBRIDGE_FillType_max=sizeof(CAMBRIDGE_FillType)/sizeof(bienum);
bienum CAMBRIDGE_Order[]={
	{{"any"},-1},
	{{"1"},0x1},
	{{"2"},0x2},
	{{"3"},0x4},
	{{"4"},0x8},
	{{"5"},0x10},
	{{"6"},0x20},
	{{"0.5"},0x40},
	{{"1.5"},0x80},
	{{"2.5"},0x100},
	{{"3.5"},0x200},
	{{"4.5"},0x400},
	{{"5.5"},0x800},
	{{"dative"},0x1000},
};
int CAMBRIDGE_Order_max=sizeof(CAMBRIDGE_Order)/sizeof(bienum);
bienum CAMBRIDGE_PrintTrimMarks[]={
	{{"no"},0},
	{{"yes"},1},
};
int CAMBRIDGE_PrintTrimMarks_max=sizeof(CAMBRIDGE_PrintTrimMarks)/sizeof(bienum);
bienum CAMBRIDGE_DrawingSpace[]={
	{{"pages"},0},
	{{"poster"},1},
};
int CAMBRIDGE_DrawingSpace_max=sizeof(CAMBRIDGE_DrawingSpace)/sizeof(bienum);
bienum CAMBRIDGE_charset[]={
	{{"Unknown"},0},
	{{"EBCDICOEM"},37},
	{{"MSDOSUS"},437},
	{{"EBCDIC500V1"},500},
	{{"ASMO-708"},708},
	{{"ArabicASMO449P"},709},
	{{"ArabicTransparent"},710},
	{{"DOS-720"},720},
	{{"Greek437G"},737},
	{{"cp775"},775},
	{{"windows-850"},850},
	{{"ibm852"},852},
	{{"cp855"},855},
	{{"cp857"},857},
	{{"cp860"},860},
	{{"cp861"},861},
	{{"DOS-862"},862},
	{{"cp863"},863},
	{{"cp864"},864},
	{{"cp865"},865},
	{{"cp866"},866},
	{{"cp869"},869},
	{{"windows-874"},874},
	{{"EBCDIC"},875},
	{{"shift_jis"},932},
	{{"gb2312"},936},
	{{"ks_c_5601-1987"},949},
	{{"big5"},950},
	{{"iso-10646"},1200},
	{{"windows-1250"},1250},
	{{"windows-1251"},1251},
	{{"iso-8859-1"},1252},
	{{"iso-8859-7"},1253},
	{{"iso-8859-9"},1254},
	{{"windows-1255"},1255},
	{{"windows-1256"},1256},
	{{"windows-1257"},1257},
	{{"windows-1258"},1258},
	{{"windows-1361"},1361},
	{{"x-mac-roman"},10000},
	{{"x-mac-japanese"},10001},
	{{"x-mac-tradchinese"},10002},
	{{"x-mac-korean"},10003},
	{{"x-mac-arabic"},10004},
	{{"x-mac-hebrew"},10005},
	{{"x-mac-greek"},10006},
	{{"x-mac-cyrillic"},10007},
	{{"x-mac-reserved"},10008},
	{{"x-mac-devanagari"},10009},
	{{"x-mac-gurmukhi"},10010},
	{{"x-mac-gujarati"},10011},
	{{"x-mac-oriya"},10012},
	{{"x-mac-nengali"},10013},
	{{"x-mac-tamil"},10014},
	{{"x-mac-telugu"},10015},
	{{"x-mac-kannada"},10016},
	{{"x-mac-Malayalam"},10017},
	{{"x-mac-sinhalese"},10018},
	{{"x-mac-burmese"},10019},
	{{"x-mac-khmer"},10020},
	{{"x-mac-thai"},10021},
	{{"x-mac-lao"},10022},
	{{"x-mac-georgian"},10023},
	{{"x-mac-armenian"},10024},
	{{"x-mac-simpChinese"},10025},
	{{"x-mac-tibetan"},10026},
	{{"x-mac-mongolian"},10027},
	{{"x-mac-ethiopic"},10028},
	{{"x-mac-ce"},10029},
	{{"x-mac-vietnamese"},10030},
	{{"x-mac-extArabic"},10031},
	{{"x-mac-uninterpreted"},10032},
	{{"x-mac-icelandic"},10079},
	{{"x-mac-turkish"},10081},
};
int CAMBRIDGE_charset_max=sizeof(CAMBRIDGE_charset)/sizeof(bienum);
bienum CONFIGBRIDGE_type[]={
	{{"Generic"},0},// but I believe correct numbers are irrelevant here.
	{{"Atom"},1},
	{{"Bond"},2},
	{{"Text"},3},
	{{"Arrow"},4},
	{{"Graphic"},5},
	{{"Curve"},6},
	{{"Matrix"},7},
	{{"Spectrum"},8},
	{{"TLCplate"},9},
	{{"None"},0x10000},
	{{"Some"},0x20000},
	{{"Continuous"},0x40000},
};
int CONFIGBRIDGE_type_max=sizeof(CONFIGBRIDGE_type)/sizeof(bienum);
bienum CAMBRIDGE_attribute[]={
	{{"Charge"},0x421},//TODO: correct numbers!
	{{"Radical"},0x422},
};
int CAMBRIDGE_attribute_max=sizeof(CAMBRIDGE_attribute)/sizeof(bienum);
bienum CAMBRIDGE_NodeType[]={
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
int CAMBRIDGE_NodeType_max=sizeof(CAMBRIDGE_NodeType)/sizeof(bienum);
bienum CONFIGBRIDGE_modifiers[]={
	{{"SHIFT"},0x1},
	{{"CTRL"},0x2},
	{{"ALT"},0x4},
	{{"HYPER"},0x8},
};
int CONFIGBRIDGE_modifiers_max=sizeof(CONFIGBRIDGE_modifiers)/sizeof(bienum);
bienum CONFIGBRIDGE_UNDO[]={
	{{"False"},0x0},
	{{"True"},0x1},
};
int CONFIGBRIDGE_UNDO_max=sizeof(CONFIGBRIDGE_UNDO)/sizeof(bienum);
bienum BKCHEM_type[]={
	{{"n1"},16},
	{{"n2"},32},
	{{"n3"},48},
	{{"n4"},64},
	{{"biradical"},0x101},
	{{"electronpair"},0x102},
	{{"radical"},0x103},
	{{"equilibrium"},0x201},
	{{"retro"},0x202},
	{{"retro2"},0x203},
	{{"electron"},0x204},
};
int BKCHEM_type_max=sizeof(BKCHEM_type)/sizeof(bienum);
#define BKCHEM_type_biradical 0x101
#define BKCHEM_type_electronpair 0x102
#define BKCHEM_type_radical 0x103
#define BKCHEM_type_equilibrium 0x201
#define BKCHEM_type_retro 0x202
#define BKCHEM_type_retro2 0x203
#define BKCHEM_type_electron 0x204
bienum BKCHEM_draw_circle[]={
	{{"no"},0},
	{{"yes"},1},
};
int BKCHEM_draw_circle_max=sizeof(BKCHEM_draw_circle)/sizeof(bienum);
bienum BKCHEM_end[]={
	{{"no"},0},
	{{"yes"},1},
};
int BKCHEM_end_max=sizeof(BKCHEM_end)/sizeof(bienum);
bienum BKCHEM_start[]={
	{{"no"},0},
	{{"yes"},1},
};
int BKCHEM_start_max=sizeof(BKCHEM_start)/sizeof(bienum);
bienum BKCHEM_spline[]={
	{{"no"},0},
	{{"yes"},1},
};
int BKCHEM_spline_max=sizeof(BKCHEM_spline)/sizeof(bienum);
