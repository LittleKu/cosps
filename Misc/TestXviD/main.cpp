#include "xvid.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OUTPUT_FILE		stdout

void test1()
{
	xvid_gbl_info_t xvid_gbl_info;
	
	/* Clear xvid_gbl_info with 0s */
	memset(&xvid_gbl_info, 0, sizeof(xvid_gbl_info_t));
	/* Version */
	xvid_gbl_info.version = XVID_VERSION;
	/* Take effect */
	xvid_global(NULL, XVID_GBL_INFO, &xvid_gbl_info, NULL);
	
	/* print */
	printf("actual version: %d.%d.%d\n",
		XVID_VERSION_MAJOR(xvid_gbl_info.actual_version),
		XVID_VERSION_MINOR(xvid_gbl_info.actual_version),
		XVID_VERSION_PATCH(xvid_gbl_info.actual_version));

	fprintf(OUTPUT_FILE, "Detected CPU flags: ");
	if (xvid_gbl_info.cpu_flags & XVID_CPU_ASM)
		fprintf(OUTPUT_FILE, "ASM ");
	if (xvid_gbl_info.cpu_flags & XVID_CPU_MMX)
		fprintf(OUTPUT_FILE, "MMX ");
	fprintf(OUTPUT_FILE, "\n");

	if(xvid_gbl_info.build)
	{
		fprintf(OUTPUT_FILE, "build: %s\n", xvid_gbl_info.build);
	}
	fprintf(OUTPUT_FILE, "threads: %d\n", xvid_gbl_info.num_threads);
}

int main(int argc, char* argv[])
{	
	test1();
	return 0;
}