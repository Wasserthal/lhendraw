//This one is "reverse engineered" from a GPLv2 program, zsnes (file hq2x32.asm). That program was written in i386 and MMX assembly
//of CURSE, that program is faster, but let's try to get max compatibility.

//The old program held the following license information:
//;This program is free software; you can redistribute it and/or
//;modify it under the terms of the GNU General Public License
//;version 2 as published by the Free Software Foundation.
//;----------------------------------------------------------
//; hq2x filter
//; Copyright (C) 2003 MaxSt ( maxst@hiend3d.com )
//;----------------------------------------------------------
//Since that program was completely understood in order to write this file, and this file contains nothing besides the data needed to give the exact same result, this file can be seen as an executable whitepaper.
//The onliest part I didn't understand was the HQ_normal_interpolation routine, but it is allowed to be in this whitepaper because it is pure specification data. So this file uses the same AGPLv3 as Lhendraw.

_RGB * HQ_inmemory;
_RGB * HQ_outmemory;
_u32 HQ_insizex,HQ_insizey;
_RGB HQ_threshold={0xFF300706};
_RGB HQ_1,HQ_2,HQ_3,HQ_4,HQ_5,HQ_6,HQ_7,HQ_8,HQ_9;
_RGB HQ_retval_00,HQ_retval_x0,HQ_retval_0y,HQ_retval_xy;
_RGB HQ_sub_color(_RGB par1,_RGB par2)
{
	_RGB backval;
	if (par1.c.A<par2.c.A) backval.c.A=0; else backval.c.A=par1.c.A-par2.c.A;
	if (par1.c.R<par2.c.R) backval.c.R=0; else backval.c.R=par1.c.R-par2.c.R;
	if (par1.c.G<par2.c.G) backval.c.G=0; else backval.c.G=par1.c.G-par2.c.G;
	if (par1.c.B<par2.c.B) backval.c.B=0; else backval.c.B=par1.c.B-par2.c.B;
	return backval;
}
_RGB HQ_sub_color_abs(_RGB par1,_RGB par2)
{
	_RGB backval;
	if (par1.c.A<par2.c.A) backval.c.A=backval.c.A=par2.c.A-par1.c.A; else backval.c.A=par1.c.A-par2.c.A;
	if (par1.c.R<par2.c.R) backval.c.R=backval.c.R=par2.c.R-par1.c.R; else backval.c.R=par1.c.R-par2.c.R;
	if (par1.c.G<par2.c.G) backval.c.G=backval.c.G=par2.c.G-par1.c.G; else backval.c.G=par1.c.G-par2.c.G;
	if (par1.c.B<par2.c.B) backval.c.B=backval.c.B=par2.c.B-par1.c.B; else backval.c.B=par1.c.B-par2.c.B;
	return backval;
}
_RGB HQ_YUV(_RGB RGB)
{
	_u32 Y=RGB.c.R+RGB.c.G+RGB.c.B/4;
	_u32 U=128+(RGB.c.R-RGB.c.B)/4;
	_u32 V=128+(RGB.c.G+RGB.c.G-RGB.c.R-RGB.c.B)/8;
	_RGB backval;
	backval.a[0]=Y;
	backval.a[1]=U;
	backval.a[2]=V;
	backval.a[3]=0;
	return backval;
}
_RGB Mix2Asym(_RGB par1,_RGB par2)
{
	_u8 r1=(((int)(((_RGB)par1).a[0]))+((int)(((_RGB)par1).a[0]))+((int)(((_RGB)par1).a[0]))+((int)(((_RGB)par2).a[0])))>>2;
	_u8 g1=(((int)(((_RGB)par1).a[1]))+((int)(((_RGB)par1).a[1]))+((int)(((_RGB)par1).a[1]))+((int)(((_RGB)par2).a[1])))>>2;
	_u8 b1=(((int)(((_RGB)par1).a[2]))+((int)(((_RGB)par1).a[2]))+((int)(((_RGB)par1).a[2]))+((int)(((_RGB)par2).a[2])))>>2;
	_u8 a1=(((int)(((_RGB)par1).a[3]))+((int)(((_RGB)par1).a[3]))+((int)(((_RGB)par1).a[3]))+((int)(((_RGB)par2).a[3])))>>2;
	_RGB backval;backval.a[0]=r1;backval.a[1]=g1;backval.a[2]=b1;backval.a[3]=a1;
	return backval;
}
_RGB Mix3(_RGB par1,_RGB par2,_RGB par3)
{
	_u8 r1=(((int)(((_RGB)par1).a[0]))+((int)(((_RGB)par1).a[0]))+((int)(((_RGB)par2).a[0]))+((int)(((_RGB)par3).a[0])))>>2;
	_u8 g1=(((int)(((_RGB)par1).a[1]))+((int)(((_RGB)par1).a[1]))+((int)(((_RGB)par2).a[1]))+((int)(((_RGB)par3).a[1])))>>2;
	_u8 b1=(((int)(((_RGB)par1).a[2]))+((int)(((_RGB)par1).a[2]))+((int)(((_RGB)par2).a[2]))+((int)(((_RGB)par3).a[2])))>>2;
	_u8 a1=(((int)(((_RGB)par1).a[3]))+((int)(((_RGB)par1).a[3]))+((int)(((_RGB)par2).a[3]))+((int)(((_RGB)par3).a[3])))>>2;
	_RGB backval;backval.a[0]=r1;backval.a[1]=g1;backval.a[2]=b1;backval.a[3]=a1;

	return backval;
}
_RGB Mix2Pure(_RGB par1,_RGB par2)
{
	_u16 r1=((_RGB)par1).a[0];
	_u16 g1=((_RGB)par1).a[1];
	_u16 b1=((_RGB)par1).a[2];
	_u16 a1=((_RGB)par1).a[3];
	_u16 r2=((_RGB)par2).a[0];
	_u16 g2=((_RGB)par2).a[1];
	_u16 b2=((_RGB)par2).a[2];
	_u16 a2=((_RGB)par2).a[3];
	_RGB backval;backval.a[0]=r1;backval.a[1]=g1;backval.a[2]=b1;backval.a[3]=a1;
	return backval;
}
_RGB Mix3Grad(_RGB par1,_RGB par2,_RGB par3)
{
	_RGB retval;
	retval.a[0]=(par1.a[0]*5+par2.a[0]*2+par3.a[0])/8;
	retval.a[1]=(par1.a[1]*5+par2.a[1]*2+par3.a[1])/8;
	retval.a[2]=(par1.a[2]*5+par2.a[2]*2+par3.a[2])/8;
	retval.a[3]=(par1.a[3]*5+par2.a[3]*2+par3.a[3])/8;
	return retval;
}
_RGB Mix3Weakly(_RGB par1,_RGB par2,_RGB par3)
{
	_RGB retval;
	retval.a[0]=(par1.a[0]*6+par2.a[0]+par3.a[0])/8;
	retval.a[1]=(par1.a[1]*6+par2.a[1]+par3.a[1])/8;
	retval.a[2]=(par1.a[2]*6+par2.a[2]+par3.a[2])/8;
	retval.a[3]=(par1.a[3]*6+par2.a[3]+par3.a[3])/8;
	return retval;
}

_RGB Mix3Strongly(_RGB par1,_RGB par2,_RGB par3)
{
	_RGB retval;
	retval.a[0]=(par1.a[0]*2+par2.a[0]*3+par3.a[0]*3)/8;
	retval.a[1]=(par1.a[1]*2+par2.a[1]*3+par3.a[1]*3)/8;
	retval.a[2]=(par1.a[2]*2+par2.a[2]*3+par3.a[2]*3)/8;
	retval.a[3]=(par1.a[3]*2+par2.a[3]*3+par3.a[3]*3)/8;
	return retval;
}
_RGB Mix3VeryWeakly(_RGB par1,_RGB par2,_RGB par3)
{
	_RGB retval;
	retval.a[0]=(par1.a[0]*14+par2.a[0]+par3.a[0])/16;
	retval.a[1]=(par1.a[1]*14+par2.a[1]+par3.a[1])/16;
	retval.a[2]=(par1.a[2]*14+par2.a[2]+par3.a[2])/16;
	retval.a[3]=(par1.a[3]*14+par2.a[3]+par3.a[3])/16;
	return retval;
}
char HQ_OverThreshold(_RGB par1,_RGB par2)
{
	return (HQ_sub_color(HQ_sub_color_abs(par1,par2),HQ_threshold).W>0);
}
void HQ_normal_interpolation(unsigned char value)
{
	switch (value)
	{
		case 0:
		case 1:
		case 4:
		case 32:
		case 128:
		case 5:
		case 132:
		case 160:
		case 33:
		case 129:
		case 36:
		case 133:
		case 164:
		case 161:
		case 37:
		case 165:
			HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
		break;
		case 2:
		case 34:
		case 130:
		case 162:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_4);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_6);
			HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
		break;
		case 16:
		case 17:
		case 48:
		case 49:
			HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_2);
			HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_8);
		break;
		case 64:
		case 65:
		case 68:
		case 69:
			HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_4);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_6);
		break;
		case 8:
		case 12:
		case 136:
		case 140:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
		break;
		case 3:
		case 35:
		case 131:
		case 163:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_6);
			HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
		break;
		case 6:
		case 38:
		case 134:
		case 166:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_4);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
		break;
		case 20:
		case 21:
		case 52:
		case 53:
			HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_8);
		break;
		case 144:
		case 145:
		case 176:
		case 177:
			HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_2);
			HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
		break;
		case 192:
		case 193:
		case 196:
		case 197:
			HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_4);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
		break;
		case 96:
		case 97:
		case 100:
		case 101:
			HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_6);
		break;
		case 40:
		case 44:
		case 168:
		case 172:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
		break;
		case 9:
		case 13:
		case 137:
		case 141:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
		break;
		case 18:
		case 50:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_4);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			}
			else
			{
				HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_8);
		break;
		case 80:
		case 81:
			HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_2);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_4);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
			}
			else
			{
				HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 72:
		case 76:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			}
			else
			{
				HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_6);
		break;
		case 10:
		case 138:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			}
			else
			{
				HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			}
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_6);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
		break;
		case 66:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_4);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_6);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_4);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_6);
		break;
		case 24:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_2);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_8);
		break;
		case 7:
		case 39:
		case 135:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
		break;
		case 148:
		case 149:
		case 180:
			HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
		break;
		case 224:
		case 228:
		case 225:
			HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
		break;
		case 41:
		case 169:
		case 45:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
		break;
		case 22:
		case 54:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_4);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_8);
		break;
		case 208:
		case 209:
			HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_2);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_4);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 104:
		case 108:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_6);
		break;
		case 11:
		case 139:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			}
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_6);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
		break;
		case 19:
		case 51:
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
				HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			}
			else
			{
				HQ_retval_00=Mix3Grad(HQ_5,HQ_2,HQ_4);
				HQ_retval_x0=Mix3Strongly(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_8);
		break;
		case 146:
		case 178:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_4);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
				HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
			}
			else
			{
				HQ_retval_x0=Mix3Strongly(HQ_5,HQ_2,HQ_6);
				HQ_retval_xy=Mix3Grad(HQ_5,HQ_6,HQ_8);
			}
			HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
		break;
		case 84:
		case 85:
			HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
				HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
			}
			else
			{
				HQ_retval_x0=Mix3Grad(HQ_5,HQ_6,HQ_2);
				HQ_retval_xy=Mix3Strongly(HQ_5,HQ_6,HQ_8);
			}
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_4);
		break;
		case 112:
		case 113:
			HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_2);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
				HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
			}
			else
			{
				HQ_retval_0y=Mix3Grad(HQ_5,HQ_8,HQ_4);
				HQ_retval_xy=Mix3Strongly(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 200:
		case 204:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
				HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
			}
			else
			{
				HQ_retval_0y=Mix3Strongly(HQ_5,HQ_8,HQ_4);
				HQ_retval_xy=Mix3Grad(HQ_5,HQ_8,HQ_6);
			}
		break;
		case 73:
		case 77:
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
				HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			}
			else
			{
				HQ_retval_00=Mix3Grad(HQ_5,HQ_4,HQ_2);
				HQ_retval_0y=Mix3Strongly(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_6);
		break;
		case 42:
		case 170:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
				HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
			}
			else
			{
				HQ_retval_00=Mix3Strongly(HQ_5,HQ_4,HQ_2);
				HQ_retval_0y=Mix3Grad(HQ_5,HQ_4,HQ_8);
			}
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_6);
			HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
		break;
		case 14:
		case 142:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
				HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			}
			else
			{
				HQ_retval_00=Mix3Strongly(HQ_5,HQ_4,HQ_2);
				HQ_retval_x0=Mix3Grad(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
		break;
		case 67:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_6);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_4);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_6);
		break;
		case 70:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_4);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_4);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_6);
		break;
		case 28:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_2);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_8);
		break;
		case 152:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_2);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_8);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
		break;
		case 194:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_4);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_6);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_4);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
		break;
		case 98:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_4);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_6);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_6);
		break;
		case 56:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_2);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_8);
		break;
		case 25:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_2);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_8);
		break;
		case 26:
		case 31:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			}
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_8);
		break;
		case 82:
		case 214:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_4);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_4);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 88:
		case 248:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_2);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			}
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 74:
		case 107:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			}
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_6);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_6);
		break;
		case 27:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			}
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_8);
		break;
		case 86:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_4);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_4);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
		break;
		case 216:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_2);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 106:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_6);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_6);
		break;
		case 30:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_8);
		break;
		case 210:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_4);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_4);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 120:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_2);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
		break;
		case 75:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			}
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_6);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_6);
		break;
		case 29:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_8);
		break;
		case 198:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_4);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_4);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
		break;
		case 184:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_2);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
		break;
		case 99:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_6);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_6);
		break;
		case 57:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_2);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_8);
		break;
		case 71:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_4);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_6);
		break;
		case 156:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_2);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_8);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
		break;
		case 226:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_4);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_6);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
		break;
		case 60:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_2);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_8);
		break;
		case 195:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_6);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_4);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
		break;
		case 102:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_4);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_6);
		break;
		case 153:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_2);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_8);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
		break;
		case 58:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			}
			else
			{
				HQ_retval_00=Mix3Weakly(HQ_5,HQ_4,HQ_2);
			}
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			}
			else
			{
				HQ_retval_x0=Mix3Weakly(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_8);
		break;
		case 83:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			}
			else
			{
				HQ_retval_x0=Mix3Weakly(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_4);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
			}
			else
			{
				HQ_retval_xy=Mix3Weakly(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 92:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_2);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			}
			else
			{
				HQ_retval_0y=Mix3Weakly(HQ_5,HQ_8,HQ_4);
			}
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
			}
			else
			{
				HQ_retval_xy=Mix3Weakly(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 202:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			}
			else
			{
				HQ_retval_00=Mix3Weakly(HQ_5,HQ_4,HQ_2);
			}
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_6);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			}
			else
			{
				HQ_retval_0y=Mix3Weakly(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
		break;
		case 78:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			}
			else
			{
				HQ_retval_00=Mix3Weakly(HQ_5,HQ_4,HQ_2);
			}
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			}
			else
			{
				HQ_retval_0y=Mix3Weakly(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_6);
		break;
		case 154:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			}
			else
			{
				HQ_retval_00=Mix3Weakly(HQ_5,HQ_4,HQ_2);
			}
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			}
			else
			{
				HQ_retval_x0=Mix3Weakly(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_8);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
		break;
		case 114:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_4);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			}
			else
			{
				HQ_retval_x0=Mix3Weakly(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
			}
			else
			{
				HQ_retval_xy=Mix3Weakly(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 89:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_2);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			}
			else
			{
				HQ_retval_0y=Mix3Weakly(HQ_5,HQ_8,HQ_4);
			}
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
			}
			else
			{
				HQ_retval_xy=Mix3Weakly(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 90:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			}
			else
			{
				HQ_retval_00=Mix3Weakly(HQ_5,HQ_4,HQ_2);
			}
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			}
			else
			{
				HQ_retval_x0=Mix3Weakly(HQ_5,HQ_2,HQ_6);
			}
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			}
			else
			{
				HQ_retval_0y=Mix3Weakly(HQ_5,HQ_8,HQ_4);
			}
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
			}
			else
			{
				HQ_retval_xy=Mix3Weakly(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 55:
		case 23:
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3Grad(HQ_5,HQ_2,HQ_4);
				HQ_retval_x0=Mix3Strongly(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_8);
		break;
		case 182:
		case 150:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_4);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
				HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
			}
			else
			{
				HQ_retval_x0=Mix3Strongly(HQ_5,HQ_2,HQ_6);
				HQ_retval_xy=Mix3Grad(HQ_5,HQ_6,HQ_8);
			}
			HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
		break;
		case 213:
		case 212:
			HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3Grad(HQ_5,HQ_6,HQ_2);
				HQ_retval_xy=Mix3Strongly(HQ_5,HQ_6,HQ_8);
			}
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_4);
		break;
		case 241:
		case 240:
			HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_2);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3Grad(HQ_5,HQ_8,HQ_4);
				HQ_retval_xy=Mix3Strongly(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 236:
		case 232:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
				HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
			}
			else
			{
				HQ_retval_0y=Mix3Strongly(HQ_5,HQ_8,HQ_4);
				HQ_retval_xy=Mix3Grad(HQ_5,HQ_8,HQ_6);
			}
		break;
		case 109:
		case 105:
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3Grad(HQ_5,HQ_4,HQ_2);
				HQ_retval_0y=Mix3Strongly(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_6);
		break;
		case 171:
		case 43:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
				HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
			}
			else
			{
				HQ_retval_00=Mix3Strongly(HQ_5,HQ_4,HQ_2);
				HQ_retval_0y=Mix3Grad(HQ_5,HQ_4,HQ_8);
			}
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_6);
			HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
		break;
		case 143:
		case 15:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
				HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			}
			else
			{
				HQ_retval_00=Mix3Strongly(HQ_5,HQ_4,HQ_2);
				HQ_retval_x0=Mix3Grad(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
		break;
		case 124:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_2);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
		break;
		case 203:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			}
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_6);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
		break;
		case 62:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_8);
		break;
		case 211:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_4);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 118:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_4);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
		break;
		case 217:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_2);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 110:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_6);
		break;
		case 155:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			}
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_8);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
		break;
		case 188:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_2);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
		break;
		case 185:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_2);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
		break;
		case 61:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_8);
		break;
		case 157:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_8);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
		break;
		case 103:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_6);
		break;
		case 227:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_6);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
		break;
		case 230:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_4);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
		break;
		case 199:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_4);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
		break;
		case 220:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_2);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			}
			else
			{
				HQ_retval_0y=Mix3Weakly(HQ_5,HQ_8,HQ_4);
			}
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 158:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			}
			else
			{
				HQ_retval_00=Mix3Weakly(HQ_5,HQ_4,HQ_2);
			}
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_8);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
		break;
		case 234:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			}
			else
			{
				HQ_retval_00=Mix3Weakly(HQ_5,HQ_4,HQ_2);
			}
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_6);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
		break;
		case 242:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_4);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			}
			else
			{
				HQ_retval_x0=Mix3Weakly(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 59:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			}
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			}
			else
			{
				HQ_retval_x0=Mix3Weakly(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_8);
		break;
		case 121:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_2);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			}
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
			}
			else
			{
				HQ_retval_xy=Mix3Weakly(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 87:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_4);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
			}
			else
			{
				HQ_retval_xy=Mix3Weakly(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 79:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			}
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			}
			else
			{
				HQ_retval_0y=Mix3Weakly(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_6);
		break;
		case 122:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			}
			else
			{
				HQ_retval_00=Mix3Weakly(HQ_5,HQ_4,HQ_2);
			}
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			}
			else
			{
				HQ_retval_x0=Mix3Weakly(HQ_5,HQ_2,HQ_6);
			}
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			}
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
			}
			else
			{
				HQ_retval_xy=Mix3Weakly(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 94:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			}
			else
			{
				HQ_retval_00=Mix3Weakly(HQ_5,HQ_4,HQ_2);
			}
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			}
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			}
			else
			{
				HQ_retval_0y=Mix3Weakly(HQ_5,HQ_8,HQ_4);
			}
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
			}
			else
			{
				HQ_retval_xy=Mix3Weakly(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 218:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			}
			else
			{
				HQ_retval_00=Mix3Weakly(HQ_5,HQ_4,HQ_2);
			}
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			}
			else
			{
				HQ_retval_x0=Mix3Weakly(HQ_5,HQ_2,HQ_6);
			}
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			}
			else
			{
				HQ_retval_0y=Mix3Weakly(HQ_5,HQ_8,HQ_4);
			}
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 91:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			}
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			}
			else
			{
				HQ_retval_x0=Mix3Weakly(HQ_5,HQ_2,HQ_6);
			}
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			}
			else
			{
				HQ_retval_0y=Mix3Weakly(HQ_5,HQ_8,HQ_4);
			}
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
			}
			else
			{
				HQ_retval_xy=Mix3Weakly(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 229:
			HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
		break;
		case 167:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
		break;
		case 173:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
		break;
		case 181:
			HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
		break;
		case 186:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			}
			else
			{
				HQ_retval_00=Mix3Weakly(HQ_5,HQ_4,HQ_2);
			}
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			}
			else
			{
				HQ_retval_x0=Mix3Weakly(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
		break;
		case 115:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			}
			else
			{
				HQ_retval_x0=Mix3Weakly(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
			}
			else
			{
				HQ_retval_xy=Mix3Weakly(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 93:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			}
			else
			{
				HQ_retval_0y=Mix3Weakly(HQ_5,HQ_8,HQ_4);
			}
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
			}
			else
			{
				HQ_retval_xy=Mix3Weakly(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 206:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			}
			else
			{
				HQ_retval_00=Mix3Weakly(HQ_5,HQ_4,HQ_2);
			}
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			}
			else
			{
				HQ_retval_0y=Mix3Weakly(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
		break;
		case 205:
		case 201:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			}
			else
			{
				HQ_retval_0y=Mix3Weakly(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
		break;
		case 174:
		case 46:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			}
			else
			{
				HQ_retval_00=Mix3Weakly(HQ_5,HQ_4,HQ_2);
			}
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
		break;
		case 179:
		case 147:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			}
			else
			{
				HQ_retval_x0=Mix3Weakly(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
		break;
		case 117:
		case 116:
			HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
			}
			else
			{
				HQ_retval_xy=Mix3Weakly(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 189:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
		break;
		case 231:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
		break;
		case 126:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			}
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
		break;
		case 219:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			}
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 125:
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3Grad(HQ_5,HQ_4,HQ_2);
				HQ_retval_0y=Mix3Strongly(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
		break;
		case 221:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3Grad(HQ_5,HQ_6,HQ_2);
				HQ_retval_xy=Mix3Strongly(HQ_5,HQ_6,HQ_8);
			}
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
		break;
		case 207:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
				HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			}
			else
			{
				HQ_retval_00=Mix3Strongly(HQ_5,HQ_4,HQ_2);
				HQ_retval_x0=Mix3Grad(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
		break;
		case 238:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
				HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
			}
			else
			{
				HQ_retval_0y=Mix3Strongly(HQ_5,HQ_8,HQ_4);
				HQ_retval_xy=Mix3Grad(HQ_5,HQ_8,HQ_6);
			}
		break;
		case 190:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
				HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
			}
			else
			{
				HQ_retval_x0=Mix3Strongly(HQ_5,HQ_2,HQ_6);
				HQ_retval_xy=Mix3Grad(HQ_5,HQ_6,HQ_8);
			}
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
		break;
		case 187:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
				HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
			}
			else
			{
				HQ_retval_00=Mix3Strongly(HQ_5,HQ_4,HQ_2);
				HQ_retval_0y=Mix3Grad(HQ_5,HQ_4,HQ_8);
			}
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
		break;
		case 243:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3Grad(HQ_5,HQ_8,HQ_4);
				HQ_retval_xy=Mix3Strongly(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 119:
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3Grad(HQ_5,HQ_2,HQ_4);
				HQ_retval_x0=Mix3Strongly(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
		break;
		case 237:
		case 233:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3VeryWeakly(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
		break;
		case 175:
		case 47:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3VeryWeakly(HQ_5,HQ_4,HQ_2);
			}
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
		break;
		case 183:
		case 151:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3VeryWeakly(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
		break;
		case 245:
		case 244:
			HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3VeryWeakly(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 250:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			}
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 123:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			}
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
		break;
		case 95:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			}
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
		break;
		case 222:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 252:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_2);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			}
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3VeryWeakly(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 249:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_2);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3VeryWeakly(HQ_5,HQ_8,HQ_4);
			}
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 235:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			}
			HQ_retval_x0=Mix3(HQ_5,HQ_3,HQ_6);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3VeryWeakly(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
		break;
		case 111:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3VeryWeakly(HQ_5,HQ_4,HQ_2);
			}
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_6);
		break;
		case 63:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3VeryWeakly(HQ_5,HQ_4,HQ_2);
			}
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
			HQ_retval_xy=Mix3(HQ_5,HQ_9,HQ_8);
		break;
		case 159:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			}
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3VeryWeakly(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_8);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
		break;
		case 215:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3VeryWeakly(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix3(HQ_5,HQ_7,HQ_4);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 246:
			HQ_retval_00=Mix3(HQ_5,HQ_1,HQ_4);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3VeryWeakly(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 254:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_1);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			}
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			}
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3VeryWeakly(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 253:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_2);
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_2);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3VeryWeakly(HQ_5,HQ_8,HQ_4);
			}
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3VeryWeakly(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 251:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			}
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_3);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3VeryWeakly(HQ_5,HQ_8,HQ_4);
			}
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 239:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3VeryWeakly(HQ_5,HQ_4,HQ_2);
			}
			HQ_retval_x0=Mix2Asym(HQ_5,HQ_6);
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3VeryWeakly(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_6);
		break;
		case 127:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3VeryWeakly(HQ_5,HQ_4,HQ_2);
			}
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3(HQ_5,HQ_2,HQ_6);
			}
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3(HQ_5,HQ_8,HQ_4);
			}
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_9);
		break;
		case 191:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3VeryWeakly(HQ_5,HQ_4,HQ_2);
			}
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3VeryWeakly(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_8);
			HQ_retval_xy=Mix2Asym(HQ_5,HQ_8);
		break;
		case 223:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3(HQ_5,HQ_4,HQ_2);
			}
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3VeryWeakly(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_7);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 247:
			HQ_retval_00=Mix2Asym(HQ_5,HQ_4);
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3VeryWeakly(HQ_5,HQ_2,HQ_6);
			}
			HQ_retval_0y=Mix2Asym(HQ_5,HQ_4);
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3VeryWeakly(HQ_5,HQ_6,HQ_8);
			}
		break;
		case 255:
			if (HQ_OverThreshold(HQ_4,HQ_2))
			{
				HQ_retval_00=HQ_5;
			}
			else
			{
				HQ_retval_00=Mix3VeryWeakly(HQ_5,HQ_4,HQ_2);
			}
			if (HQ_OverThreshold(HQ_2,HQ_6))
			{
				HQ_retval_x0=HQ_5;
			}
			else
			{
				HQ_retval_x0=Mix3VeryWeakly(HQ_5,HQ_2,HQ_6);
			}
			if (HQ_OverThreshold(HQ_8,HQ_4))
			{
				HQ_retval_0y=HQ_5;
			}
			else
			{
				HQ_retval_0y=Mix3VeryWeakly(HQ_5,HQ_8,HQ_4);
			}
			if (HQ_OverThreshold(HQ_6,HQ_8))
			{
				HQ_retval_xy=HQ_5;
			}
			else
			{
				HQ_retval_xy=Mix3VeryWeakly(HQ_5,HQ_6,HQ_8);
			}
		break;
	}
}
void HQ_gradient2()
{
	_RGB iother=Mix2Asym(HQ_5,HQ_2);
	HQ_retval_00=iother;
	HQ_retval_x0=iother;
	HQ_retval_0y=HQ_5;
	HQ_retval_xy=HQ_5;
}
void HQ_gradient4()
{
	_RGB iother=Mix2Asym(HQ_5,HQ_4);
	HQ_retval_00=iother;
	HQ_retval_x0=HQ_5;
	HQ_retval_0y=iother;
	HQ_retval_xy=HQ_5;
}
void HQ_gradient6()
{
	_RGB iother=Mix2Asym(HQ_5,HQ_6);
	HQ_retval_00=HQ_5;
	HQ_retval_x0=iother;
	HQ_retval_0y=HQ_5;
	HQ_retval_xy=iother;
}
void HQ_gradient8()
{
	_RGB iother=Mix2Asym(HQ_5,HQ_8);
	HQ_retval_00=HQ_5;
	HQ_retval_x0=HQ_5;
	HQ_retval_0y=iother;
	HQ_retval_xy=iother;
}
_u8 HQ_gradient(_u8 iflags)
{
	return 0;
}
_u8 HQ_flags;
_u8 HQ_gradientflags;
void HQ_checkflag(_RGB icenterYUV,_RGB px,_u32 nr)
{
	_i32 nr2=nr-1;
	if (nr>=5) nr2--;
	if (HQ_5.W!=px.W)
	{
		HQ_gradientflags|=1<<nr2;
		if (HQ_sub_color(HQ_sub_color_abs(icenterYUV,HQ_YUV(px)),HQ_threshold).W>0) HQ_flags|=1<<nr2;
	}
}
void HQ_steepness_dependent_interpolation()
{
	HQ_flags=0;
	_RGB centerYUV;
	centerYUV=HQ_YUV(HQ_5);
	HQ_checkflag(centerYUV,HQ_2,2);
	HQ_checkflag(centerYUV,HQ_4,4);
	HQ_checkflag(centerYUV,HQ_6,6);
	HQ_checkflag(centerYUV,HQ_8,8);
	if ((HQ_flags)==0)
	{
		if (HQ_gradientflags==0)
		{
			HQ_retval_00=HQ_5;
			HQ_retval_x0=HQ_5;
			HQ_retval_0y=HQ_5;
			HQ_retval_xy=HQ_5;
			return;
		}
		else
		{
			switch (HQ_gradientflags)
			{
				case 4: HQ_gradient2();return;
				case 0x10: HQ_gradient4();return;
				case 0x20: HQ_gradient6();return;
				case 0x80: HQ_gradient8();return;
			}
		}
	}
	else
	{
		HQ_checkflag(centerYUV,HQ_1,1);
		HQ_checkflag(centerYUV,HQ_3,3);
		HQ_checkflag(centerYUV,HQ_7,7);
		HQ_checkflag(centerYUV,HQ_9,9);
	}
	HQ_normal_interpolation(HQ_flags);
}
void HQ_mainloop()
{
	int ilv1,ilv2;
	for (ilv1=0;ilv1<HQ_insizey;ilv1++)
	{
		for (ilv2=0;ilv2<HQ_insizex;ilv2++)
		{
			HQ_1=HQ_inmemory[min(max(ilv1-1,0),HQ_insizey-1)*HQ_insizex+min(max(ilv2-1,0),HQ_insizex-1)];
			HQ_2=HQ_inmemory[min(max(ilv1-1,0),HQ_insizey-1)*HQ_insizex+min(max(ilv2+0,0),HQ_insizex-1)];
			HQ_3=HQ_inmemory[min(max(ilv1-1,0),HQ_insizey-1)*HQ_insizex+min(max(ilv2+1,0),HQ_insizex-1)];
			HQ_4=HQ_inmemory[min(max(ilv1+0,0),HQ_insizey-1)*HQ_insizex+min(max(ilv2-1,0),HQ_insizex-1)];
			HQ_5=HQ_inmemory[min(max(ilv1+0,0),HQ_insizey-1)*HQ_insizex+min(max(ilv2+0,0),HQ_insizex-1)];
			HQ_6=HQ_inmemory[min(max(ilv1+0,0),HQ_insizey-1)*HQ_insizex+min(max(ilv2+1,0),HQ_insizex-1)];
			HQ_7=HQ_inmemory[min(max(ilv1+1,0),HQ_insizey-1)*HQ_insizex+min(max(ilv2-1,0),HQ_insizex-1)];
			HQ_8=HQ_inmemory[min(max(ilv1+1,0),HQ_insizey-1)*HQ_insizex+min(max(ilv2+0,0),HQ_insizex-1)];
			HQ_9=HQ_inmemory[min(max(ilv1+1,0),HQ_insizey-1)*HQ_insizex+min(max(ilv2+1,0),HQ_insizex-1)];
			HQ_steepness_dependent_interpolation();
			HQ_outmemory[((ilv1*2)+0)*HQ_insizex*2+(ilv2*2)+0]=HQ_retval_00;
			HQ_outmemory[((ilv1*2)+0)*HQ_insizex*2+(ilv2*2)+1]=HQ_retval_x0;
			HQ_outmemory[((ilv1*2)+1)*HQ_insizex*2+(ilv2*2)+0]=HQ_retval_0y;
			HQ_outmemory[((ilv1*2)+1)*HQ_insizex*2+(ilv2*2)+1]=HQ_retval_xy;
		}
	}
}
