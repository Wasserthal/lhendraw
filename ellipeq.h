void ARROW_ELLIPTIC(float ELLIPSE_REZIRADIUSX,float ELLIPSE_REZIRADIUSY,float ELLIPSE_STARTX,float ELLIPSE_STARTY,float &A,float &B,float &C,float &D,float &E)
{
	ELLIPSE_REZIRADIUSX*=ELLIPSE_REZIRADIUSX;
	ELLIPSE_REZIRADIUSY*=ELLIPSE_REZIRADIUSY;
	D=+4*ELLIPSE_STARTX*ELLIPSE_REZIRADIUSX-4*ELLIPSE_STARTX*fsqr(ELLIPSE_STARTY)*ELLIPSE_REZIRADIUSX*ELLIPSE_REZIRADIUSY-4*ELLIPSE_STARTX*ELLIPSE_REZIRADIUSY*ELLIPSE_REZIRADIUSX-4*ELLIPSE_STARTX*ELLIPSE_STARTX*ELLIPSE_STARTX*fsqr(ELLIPSE_REZIRADIUSX);
	C=+4*fsqr(ELLIPSE_STARTY)*fsqr(ELLIPSE_REZIRADIUSY)+2*ELLIPSE_REZIRADIUSY-2*fsqr(ELLIPSE_STARTY)*fsqr(ELLIPSE_REZIRADIUSY)-2*fsqr(ELLIPSE_REZIRADIUSY)-2*fsqr(ELLIPSE_STARTX)*ELLIPSE_REZIRADIUSX*ELLIPSE_REZIRADIUSY+4*fsqr(ELLIPSE_STARTX)*fsqr(ELLIPSE_REZIRADIUSX)-2*ELLIPSE_REZIRADIUSX+2*fsqr(ELLIPSE_STARTY)*ELLIPSE_REZIRADIUSY*ELLIPSE_REZIRADIUSX+2*ELLIPSE_REZIRADIUSX*ELLIPSE_REZIRADIUSY+2*fsqr(ELLIPSE_STARTX)*fsqr(ELLIPSE_REZIRADIUSX);
	B=+4*ELLIPSE_STARTX*ELLIPSE_REZIRADIUSX*ELLIPSE_REZIRADIUSY-4*ELLIPSE_STARTX*fsqr(ELLIPSE_REZIRADIUSX);
	A=+fsqr(ELLIPSE_REZIRADIUSY)-2*ELLIPSE_REZIRADIUSX*ELLIPSE_REZIRADIUSY+fsqr(ELLIPSE_REZIRADIUSX);
	E=+fsqr(fsqr(ELLIPSE_STARTX))*fsqr(ELLIPSE_REZIRADIUSX)+2*fsqr(ELLIPSE_STARTX)*ELLIPSE_REZIRADIUSX*ELLIPSE_REZIRADIUSY+2*fsqr(ELLIPSE_STARTX)*fsqr(ELLIPSE_STARTY)*ELLIPSE_REZIRADIUSX*ELLIPSE_REZIRADIUSY-2*fsqr(ELLIPSE_STARTX)*ELLIPSE_REZIRADIUSX+1*fsqr(ELLIPSE_REZIRADIUSY)+2*fsqr(ELLIPSE_STARTY)*fsqr(ELLIPSE_REZIRADIUSY)-2*ELLIPSE_REZIRADIUSY+fsqr(fsqr(ELLIPSE_STARTY))*fsqr(ELLIPSE_REZIRADIUSY)-2*fsqr(ELLIPSE_STARTY)*ELLIPSE_REZIRADIUSY+1-4*fsqr(ELLIPSE_STARTY)*fsqr(ELLIPSE_REZIRADIUSY);
}