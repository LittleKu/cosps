/***********************************************************************
 * wavetest.c
 *  
 *    Audio Library Test
 *
 *
 *  Supports .WAV files, Very Simplistic Player
 *
 *
 * Toby Opferman Copyright (c) 2004
 *
 ***********************************************************************/
 
 
 #include <windows.h>
 #include <stdio.h>
 #include <conio.h>
 #include "wavelib.h"

/************************************************************************
 * Prototypes 
 ************************************************************************/
 void WaveTest_Play(char *pszFileName);
 void WaveTest_PrintArgs(void);
 void WaveTest_PrintError(void);
 
 /***********************************************************************
  * main()
  *  
  *    Entry Point
  *
  * Parameters
  *     Number Of Arguements, Arguements
  * 
  * Return Value
  *     0
  *
  ***********************************************************************/
  int main(int argc, char *argv[])
  {
      if(argc == 2)
      {
          WaveTest_Play(argv[1]);
      }
      else
      {
          WaveTest_PrintArgs();
      }

      return 0;
  }


 /***********************************************************************
  * WaveTest_Play
  *  
  *    Play A Wave File
  *
  * Parameters
  *     File Name
  * 
  * Return Value
  *     0
  *
  ***********************************************************************/
 void WaveTest_Play(char *pszFileName)
 {
     HWAVELIB hWaveLib = NULL;
         
     hWaveLib = WaveLib_Init(pszFileName, FALSE);
     
     if(hWaveLib)
     {
         printf(" Press a key to stop> ");
         getch();

         WaveLib_UnInit(hWaveLib);
     }
     else
     {
         WaveTest_PrintError();
     }
     
 }

 /***********************************************************************
  * WaveTest_PrintArgs
  *  
  *    Display Program Parameters
  *
  * Parameters
  *     Nothing
  * 
  * Return Value
  *     0
  *
  ***********************************************************************/
 void WaveTest_PrintArgs(void)
 {
     printf("WaveTest .WAV File Player!\n");

     printf("Usage:\n");
     printf("\n   WaveTest <FileName>\n\n");
 }

 /***********************************************************************
  * WaveTest_PrintError
  *  
  *    Display Program Error
  *
  * Parameters
  *     Nothing
  * 
  * Return Value
  *     0
  *
  ***********************************************************************/
 void WaveTest_PrintError(void)
 {
     printf(" An Error Occured Attempting to play sound\n");
     WaveTest_PrintArgs();
 }
