#include <windows.h>

#include <stdio.h>

#include <assert.h>

#include <mmreg.h>

#include <msacm.h>


#define MP3_BLOCK_SIZE 522

#define SOURCE_MP3 "E:\\music\\KTV\\dtqg.mp3"

#define OUTPUT_PCM_FILE "E:\\music\\KTV\\dump.pcm"


int g_mp3Drivers = 0;


BOOL CALLBACK acmDriverEnumCallback( HACMDRIVERID hadid, DWORD dwInstance, DWORD fdwSupport ){
	
	if( fdwSupport & ACMDRIVERDETAILS_SUPPORTF_CODEC ) {
		
		MMRESULT mmr;
		
		
		ACMDRIVERDETAILS details;
		
		details.cbStruct = sizeof(ACMDRIVERDETAILS);
		
		mmr = acmDriverDetails( hadid, &details, 0 );
		
		
		HACMDRIVER driver;
		
		mmr = acmDriverOpen( &driver, hadid, 0 );
		
		
		int i;
		
		for(i = 0; i < details.cFormatTags; i++ ){
			
			ACMFORMATTAGDETAILS fmtDetails;
			
			ZeroMemory( &fmtDetails, sizeof(fmtDetails) );
			
			fmtDetails.cbStruct = sizeof(ACMFORMATTAGDETAILS);
			
			fmtDetails.dwFormatTagIndex = i;
			
			mmr = acmFormatTagDetails( driver, &fmtDetails, ACM_FORMATTAGDETAILSF_INDEX );
			
			if( fmtDetails.dwFormatTag == WAVE_FORMAT_MPEGLAYER3 ){
				
				printf( "Found an MP3-capable ACM codec: " );
				printf( details.szLongName );
				printf( "\n" );
				
				g_mp3Drivers++;
				
			}

			if(fmtDetails.dwFormatTag != WAVE_FORMAT_PCM && fmtDetails.dwFormatTag != WAVE_FORMAT_UNKNOWN)
			{
				printf("format=%08X, %s\n", fmtDetails.dwFormatTag, fmtDetails.szFormatTag);
			}
			
		}
		
		mmr = acmDriverClose( driver, 0 );
		
	}
	
	return true;
	
}

void error_msg(MMRESULT hr)
{
	printf("error: %d\n", hr);
}



HACMSTREAM g_mp3stream = NULL;




convertMP3(){
	
	
	
	MMRESULT mmr;
	
	
	
	// try to find an MP3 codec
	
	acmDriverEnum( acmDriverEnumCallback, 0, 0 );
	
	if(g_mp3Drivers == 0){
		
		printf( "No MP3 decoders found!\n" );
		
		return E_FAIL;
		
	}
	
	
	
	// find the biggest format size
	
	DWORD maxFormatSize = 0;
	
	mmr = acmMetrics( NULL, ACM_METRIC_MAX_SIZE_FORMAT, &maxFormatSize );
	
	
	
	// define desired output format
	
	LPWAVEFORMATEX waveFormat = (LPWAVEFORMATEX) LocalAlloc( LPTR, maxFormatSize );
	
	waveFormat->wFormatTag = WAVE_FORMAT_PCM;
	
	waveFormat->nChannels = 2; // stereo
	
	waveFormat->nSamplesPerSec = 44100; // 44.1kHz
	
	waveFormat->wBitsPerSample = 16; // 16 bits
	
	waveFormat->nBlockAlign = 4; // 4 bytes of data at a time are useful (1 sample)
	
	waveFormat->nAvgBytesPerSec = 4 * 44100; // byte-rate
	
	waveFormat->cbSize = 0; // no more data to follow
	
	
	
	
	
	// define MP3 input format
	
	LPMPEGLAYER3WAVEFORMAT mp3format = (LPMPEGLAYER3WAVEFORMAT) LocalAlloc( LPTR, maxFormatSize );
	
	mp3format->wfx.cbSize = MPEGLAYER3_WFX_EXTRA_BYTES;
	
	mp3format->wfx.wFormatTag = WAVE_FORMAT_MPEGLAYER3;
	
	mp3format->wfx.nChannels = 2;
	
	mp3format->wfx.nAvgBytesPerSec = 128 * (1024 / 8);  // not really used but must be one of 64, 96, 112, 128, 160kbps
	
	mp3format->wfx.wBitsPerSample = 0;                  // MUST BE ZERO
	
	mp3format->wfx.nBlockAlign = 1;                     // MUST BE ONE
	
	mp3format->wfx.nSamplesPerSec = 44100;              // 44.1kHz
	
	mp3format->fdwFlags = MPEGLAYER3_FLAG_PADDING_OFF;
	
	mp3format->nBlockSize = MP3_BLOCK_SIZE;             // voodoo value #1
	
	mp3format->nFramesPerBlock = 1;                     // MUST BE ONE
	
	mp3format->nCodecDelay = 1393;                      // voodoo value #2
	
	mp3format->wID = MPEGLAYER3_ID_MPEG;
	
	
	
	g_mp3stream = NULL;
	
	mmr = acmStreamOpen( &g_mp3stream,               // open an ACM conversion stream
		
		NULL,                       // querying all ACM drivers
		
		(LPWAVEFORMATEX) mp3format, // converting from MP3
		
		waveFormat,                 // to WAV
		
		NULL,                       // with no filter
		
		0,                          // or async callbacks
		
		0,                          // (and no data for the callback)
		
		0                           // and no flags
		
		);
	
	
	
	LocalFree( mp3format );
	
	LocalFree( waveFormat );
	
	
	
	switch( mmr ) {
		
	case MMSYSERR_NOERROR:
		
		break; // success!
		
	case MMSYSERR_INVALPARAM:
		
		assert( !"Invalid parameters passed to acmStreamOpen" );
		
		return E_FAIL;
		
	case ACMERR_NOTPOSSIBLE:
		
		assert( !"No ACM filter found capable of decoding MP3" );
		
		return E_FAIL;
		
	default:
		
		assert( !"Some error opening ACM decoding stream!" );
		
		return E_FAIL;
		
	}
	
	
	
	// MP3 stream converter opened correctly
	
	// now, let's open a file, read in a bunch of MP3 data, and convert it!
	
	
	
	// open file
	
	FILE *fpIn = fopen( SOURCE_MP3, "rb" );
	
	if( fpIn == NULL ){
		
		assert( !"can't open MP3 file!" );
		
		return E_FAIL;
		
	}
	
	
	
	// find out how big the decompressed buffer will be
	
	unsigned long rawbufsize = 0;
	
	mmr = acmStreamSize( g_mp3stream, MP3_BLOCK_SIZE, &rawbufsize, ACM_STREAMSIZEF_SOURCE );
	
	assert( mmr == 0 );
	
	assert( rawbufsize > 0 );
	
	
	
	// allocate our I/O buffers
	
	LPBYTE mp3buf = (LPBYTE) LocalAlloc( LPTR, MP3_BLOCK_SIZE );
	
	LPBYTE rawbuf = (LPBYTE) LocalAlloc( LPTR, rawbufsize );
	
	
	
	// prepare the decoder
	
	ACMSTREAMHEADER mp3streamHead;
	
	ZeroMemory( &mp3streamHead, sizeof(ACMSTREAMHEADER ) );
	
	mp3streamHead.cbStruct = sizeof(ACMSTREAMHEADER );
	
	mp3streamHead.pbSrc = mp3buf;
	
	mp3streamHead.cbSrcLength = MP3_BLOCK_SIZE;
	
	mp3streamHead.pbDst = rawbuf;
	
	mp3streamHead.cbDstLength = rawbufsize;
	
	mmr = acmStreamPrepareHeader( g_mp3stream, &mp3streamHead, 0 );
	
	assert( mmr == 0 );
	
	
	
	// let's dump this data off to disk (for debug checking!)
	
	FILE *fpOut = fopen( OUTPUT_PCM_FILE, "wb" );
	
	if( fpOut == NULL ){
		
		assert( !"can't output output PCM!" );
		
		return E_FAIL;
		
	}
	
	
	
	while(1) {
		
		// suck in some MP3 data
		
		int count = fread( mp3buf, 1, MP3_BLOCK_SIZE, fpIn );
		
		if( count != MP3_BLOCK_SIZE )
			
			break;
		
		
		
		// convert the data
		
		mmr = acmStreamConvert( g_mp3stream, &mp3streamHead, ACM_STREAMCONVERTF_BLOCKALIGN );
		
		assert( mmr == 0 );
		
		
		
		// write the decoded PCM to disk
		
		count = fwrite( rawbuf, 1, mp3streamHead.cbDstLengthUsed, fpOut );
		
		assert( count == mp3streamHead.cbDstLengthUsed );
		
	};
	
	
	
	// clean up after yourself like a good little boy
	
	fclose( fpIn );
	
	fclose( fpOut );
	
	mmr = acmStreamUnprepareHeader( g_mp3stream, &mp3streamHead, 0 );
	
	assert( mmr == 0 );
	
	LocalFree(rawbuf);
	
	LocalFree(mp3buf);
	
	mmr = acmStreamClose( g_mp3stream, 0 );
	
	assert( mmr == 0 );
	
	
	
	return S_OK;
	
}

int main1()
{
	convertMP3();
	return 0;
}