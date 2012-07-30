// Command line compile with CL
#pragma comment( lib, "msacm32.lib" ) 
#include <windows.h>
#include <math.h>
#include <mmsystem.h>
#include <mmreg.h>  
#include <msacm.h>    
#include <stdio.h>

BOOL CALLBACK FormatEnumProc(HACMDRIVERID hadid, LPACMFORMATDETAILS pafd, DWORD dwInstance, DWORD fdwSupport)
{
    printf("    %4.4lXH, %s\n", pafd->dwFormatTag, pafd->szFormat);
    return TRUE; // Continue enumerating.
}

BOOL CALLBACK DriverEnumProc(HACMDRIVERID hadid, DWORD dwInstance, DWORD fdwSupport)
{
    printf(" id: %8.8lxH", hadid);
    printf("  supports:\n");
    if (fdwSupport & ACMDRIVERDETAILS_SUPPORTF_ASYNC) printf("   async conversions\n");
    if (fdwSupport & ACMDRIVERDETAILS_SUPPORTF_CODEC) printf("   different format conversions\n");
    if (fdwSupport & ACMDRIVERDETAILS_SUPPORTF_CONVERTER) printf("   same format conversions\n");
    if (fdwSupport & ACMDRIVERDETAILS_SUPPORTF_FILTER) printf("   filtering\n");
    // Get some details.
    ACMDRIVERDETAILS dd;
    dd.cbStruct = sizeof(dd);
    MMRESULT mmr = acmDriverDetails(hadid, &dd, 0);
    if (mmr) {
        printf("we have an acmDriverDetails error\n");
    } 
    else {
        printf("   Short name: %s\n", dd.szShortName);
        printf("   Long name:  %s\n", dd.szLongName);
        printf("   Copyright:  %s\n", dd.szCopyright);
        printf("   Licensing:  %s\n", dd.szLicensing);
        printf("   Features:   %s\n", dd.szFeatures);
        printf("   Supports %u formats\n", dd.cFormatTags);
        printf("   Supports %u filter formats\n", dd.cFilterTags);
    }
    // Open the driver.
    HACMDRIVER had = NULL;
    mmr = acmDriverOpen(&had, hadid, 0);
    if (mmr) {
        printf("Driver open error\n");
    } 
    else {
        DWORD dwSize = 0;
        mmr = acmMetrics((HACMOBJ)had, ACM_METRIC_MAX_SIZE_FORMAT, &dwSize);
        if (dwSize < sizeof(WAVEFORMATEX)) dwSize = sizeof(WAVEFORMATEX); // for MS-PCM
        WAVEFORMATEX* pwf = (WAVEFORMATEX*) malloc(dwSize);
        memset(pwf, 0, dwSize);
        pwf->cbSize = LOWORD(dwSize) - sizeof(WAVEFORMATEX);
        pwf->wFormatTag = WAVE_FORMAT_UNKNOWN;
        ACMFORMATDETAILS fd;
        memset(&fd, 0, sizeof(fd));
        fd.cbStruct = sizeof(fd);
        fd.pwfx = pwf;
        fd.cbwfx = dwSize;
        fd.dwFormatTag = WAVE_FORMAT_UNKNOWN;
//         mmr = acmFormatEnum(had, &fd, FormatEnumProc, 0, 0);  
//         if (mmr) {
//             printf("FormatEnum Error\n");
//         }
        free(pwf);
        acmDriverClose(had, 0);
    }
    return TRUE; // Continue enumeration.
}


int main(void)
{
    // Get the ACM version.
    DWORD dwACMVer = acmGetVersion();
    printf("ACM version %u.%.02u build %u",
        HIWORD(dwACMVer) >> 8,
        HIWORD(dwACMVer) & 0x00FF,
        LOWORD(dwACMVer));
    if (LOWORD(dwACMVer) == 0) printf(" (Retail)");
    printf("\n");
	
    // Show some ACM metrics.
    printf("ACM metrics:\n");
	
    DWORD dwCodecs = 0;
    MMRESULT mmr = acmMetrics(NULL, ACM_METRIC_COUNT_CODECS, &dwCodecs);
    if (mmr) {
        printf("We have an acmMetrics error\n");
		
    } 
    else {
        printf("%lu codecs installed\n", dwCodecs);
    }
    // Enumerate the set of enabled drivers.
    printf("Enabled drivers:\n");
    mmr = acmDriverEnum(DriverEnumProc, 0, 0); 
    if (mmr)
        printf("We have an acmDriver Enum error\n");
    return 0;
}