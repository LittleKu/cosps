#include "SampleContainer.h"
#include "SampleParams.h"
#include "WaveFileDecoder.h"
#include "WaveFileEncoder.h"
#include "LameFileDecoder.h"
#include "LameFileEncoder.h"
#include "EncoderEngine.h"
#include "cflbase/tstring.h"

int testCase1(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	int ret = 0;

	ret = testCase1(argc, argv);

	return ret;
}

SampleDecoder* GetDecoder(const TCHAR* lpFileName)
{
	SampleDecoder* pDecoder = NULL;
	TCHAR* lpFileExt = _tcsrchr(lpFileName, _T('.'));
	if(lpFileExt != NULL)
	{
		if(_tcsicmp(lpFileExt, _T(".mp3")) == 0)
		{
			pDecoder = new LameFileDecoder();
		}
		else if(_tcsicmp(lpFileExt, _T(".wav")) == 0)
		{
			pDecoder = new WaveFileDecoder();
		}
	}

	if(pDecoder == NULL)
		pDecoder = new WaveFileDecoder();

	return pDecoder;
}

SampleEncoder* GetEncoder(const TCHAR* lpFileName)
{
	SampleEncoder* pEncoder = NULL;
	TCHAR* lpFileExt = _tcsrchr(lpFileName, _T('.'));
	if(lpFileExt != NULL)
	{
		if(_tcsicmp(lpFileExt, _T(".mp3")) == 0)
		{
			pEncoder = new LameFileEncoder();
		}
		else if(_tcsicmp(lpFileExt, _T(".wav")) == 0)
		{
			pEncoder = new WaveFileEncoder();
		}
	}
	
	if(pEncoder == NULL)
		pEncoder = new WaveFileEncoder();
	
	return pEncoder;
}

int testCase1(int argc, char* argv[])
{
	if(argc < 3)
	{
		printf("Usage: %s <wav> <wav> <bps>\n", argv[0]);
		return -1;
	}

	SampleContext context;

	NS_CFL::tstring srcFile = CFL_C_STR_TO_TSTRING(argv[1]);
	NS_CFL::tstring dstFile = CFL_C_STR_TO_TSTRING(argv[2]);
	context.Set(SampleParams::INPUT_FILE_NAME, &srcFile);
	context.Set(SampleParams::OUTPUT_FILE_NAME, &dstFile);

	NS_CFL::tstring errorStr;
	context.Set(SampleParams::ERROR_STR, &errorStr);

	EncoderEngine engine;

	SampleDecoder* pDecoder = GetDecoder(srcFile.c_str());
	engine.SetDecoder(pDecoder);

	SampleEncoder* pEncoder = GetEncoder(dstFile.c_str());
	engine.SetEncoder(pEncoder);
	
	if(argc >= 4)
	{
		int bps = atoi(argv[3]);
		context.SetInt(WaveFileEncoder::OUT_BITS_PER_SAMPLE, bps);
	}
	int ret = engine.execute(context);

	if(ret != 0)
	{
		CFL_TRACET(_T("Engine error: %s\n"), errorStr.c_str());
	}

	delete pDecoder;
	delete pEncoder;

	return ret;
}