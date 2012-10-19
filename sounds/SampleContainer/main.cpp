#include "SampleContainer.h"
#include "WaveFileDecoder.h"
#include "WaveFileEncoder.h"
#include "EncoderEngine.h"

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
		printf("Usage: %s <wav> <wav>\n", argv[0]);
		return -1;
	}

	EncoderEngine engine;
	engine.SetSrcFile(CFL_C_STR_TO_T_STR(argv[1]));
	engine.SetDstFile(CFL_C_STR_TO_T_STR(argv[2]));

	SampleDecoder* pDecoder = new WaveFileDecoder();
	engine.SetDecoder(pDecoder);

	SampleEncoder* pEncoder = new WaveFileEncoder();
	engine.SetEncoder(pEncoder);

	int ret = engine.execute();

	delete pDecoder;
	delete pEncoder;

	return ret;
}