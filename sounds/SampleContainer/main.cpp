#include "SampleContainer.h"
#include "WaveFileDecoder.h"
#include "WaveFileEncoder.h"
#include "EncoderEngine.h"
#include "cflbase/tstring.h"

int testCase1(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	int ret = 0;

	ret = testCase1(argc, argv);

	return ret;
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

	SampleDecoder* pDecoder = new WaveFileDecoder();
	engine.SetDecoder(pDecoder);

	SampleEncoder* pEncoder = new WaveFileEncoder();
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