#include "WavePlayer.h"
#include "WaveFilePlayer.h"
#include <stdio.h>

static const int BUFFER_LEN = 17003;
static char buffer[BUFFER_LEN];

int testPlayRaw(int argc, char* argv[]);
void PlayRaw(CWavePlayer* player, const char* pFileName);

int testPlayFile(int argc, char* argv[]);
void PlayFile(CWaveFilePlayer* player, const char* pFileName);

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf ("\nUsage : %s <input sound file>\n\n", argv [0]) ;
		return 1 ;
	}

	testPlayFile(argc, argv);

	return 0;
}

int testPlayRaw(int argc, char* argv[])
{
	CWavePlayer player;
	if(!player.Init())
	{
		printf("%s\n", player.GetErrorMsg());
		return 2;
	}
	
	BOOL bRet;
	for(int k = 1 ; k < argc ; k++)
	{
		if(k % 2 == 1)
		{
			bRet = player.Open(44100, 16, 2);
		}
		else if(k % 2 == 0)
		{
			bRet = player.Open(44100, 8, 2);
		}
		
		if(!bRet)
		{
			printf("%s\n", player.GetErrorMsg());
			continue;
		}
		
		printf("start play %s\n", argv[k]);
		PlayRaw(&player, argv[k]);
		printf("end play %s\n", argv[k]);
	}
	
	return 0;
}

void PlayRaw(CWavePlayer* player, const char* pFileName)
{
	FILE* fp = fopen(pFileName, "rb");
	if(fp == NULL)
	{
		printf("Failed to open file %s\n", pFileName);
		return;
	}

	int nCount = 0;
	int nRead = 0;
	while( (nRead = fread(buffer, 1, BUFFER_LEN, fp)) > 0 )
	{
		nCount += nRead;
		player->PlayData(buffer, nRead);
//		player->Flush();
		if(nCount >= 11 * 44100 * 4)
		{
			printf("played 10 seconds data, stop now\n");
			break;
		}
	}
	player->Flush();
	player->Close();

	fclose(fp);
}

int testPlayFile(int argc, char* argv[])
{
	CWaveFilePlayer player;
	for(int k = 1; k < argc; k++)
	{
		printf("Play[%d]: %s\n", k, argv[k]);
		player.Play(argv[k]);
	}

	return 0;
}

void PlayFile(CWaveFilePlayer* player, const char* pFileName)
{
	player->Play(pFileName);
}