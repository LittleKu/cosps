#include "WavePlayer.h"
#include "WaveFilePlayer.h"
#include "MP3Player.h"
#include <stdio.h>

static const int BUFFER_LEN = 17003;
static char buffer[BUFFER_LEN];

int testPlayRaw(int argc, char* argv[]);
void PlayRaw(CWavePlayer* player, const char* pFileName);

int testPlayFile(int argc, char* argv[]);
void PlayFile(CWaveFilePlayer* player, const char* pFileName);

int testPlayMP3(int argc, char* argv[]);

int main(int argc, char* argv[])
{
//	testPlayRaw(argc, argv);
	testPlayFile(argc, argv);
//	testPlayMP3(argc, argv);

	return 0;
}

int testPlayRaw(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("Usage: %s <input file> <sample rate> <bits> <channels>\n", "RawPlayer");
		return -1;
	}
	CWavePlayer player;
	if(!player.Init())
	{
		printf("%s\n", player.GetErrorMsg());
		return 2;
	}

	DWORD nSampleRate = 44100;
	WORD wBitsPerSample = 16;
	WORD wChannel = 2;

	if(argc >= 3)
	{
		nSampleRate = atoi(argv[2]);
	}
	if(argc >= 4)
	{
		wBitsPerSample = atoi(argv[3]);
	}
	if(argc >= 5)
	{
		wChannel = atoi(argv[4]);
	}
	
	BOOL bRet = player.Open(nSampleRate, wBitsPerSample, wChannel);
	if(!bRet)
	{
		printf("%s\n", player.GetErrorMsg());
		return -1;
	}
	printf("start play %s\n", argv[1]);
	PlayRaw(&player, argv[1]);
	printf("end play %s\n", argv[1]);
	
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
// 		if(nCount >= 11 * 44100 * 4)
// 		{
// 			printf("played 10 seconds data, stop now\n");
// 			break;
// 		}
	}
	player->Flush();
	player->Close();

	fclose(fp);
}

int testPlayFile(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf ("\nUsage : %s <input sound file>\n\n", argv [0]) ;
		return 1 ;
	}

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

int testPlayMP3(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("Usage: %s <input file> ...\n", "mp3Player");
		return -1;
	}
	CMP3Player player;
	for(int i = 1; i < argc; i++)
	{
		printf("\n\n===========Play %s =============\n\n", argv[i]);
		player.Play(argv[i]);
	}

	return 0;
}