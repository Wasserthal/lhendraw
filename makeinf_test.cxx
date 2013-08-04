#include "stdio.h"
#include "stdlib.h"
#include "makeinf.hxx"
int main(void)
{

	picturesortiment.sortiment_length=1;
	picturesortiment.sortiment[0].number=2;
	makeinf_sortimentcount=1;
	makeinf("makeinf_test.inf");
}
