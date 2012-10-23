#include "EncoderEngine.h"

EncoderEngine::EncoderEngine()
{
}

EncoderEngine::~EncoderEngine()
{
}

int EncoderEngine::execute(SampleContext& context)
{
	int ret = 0;
	
	do 
	{
		ret = m_pDecoder->Open(m_samples, context);
		if(ret != 0)
		{
			break;
		}
		
		ret = m_pEncoder->Open(m_samples, context);
		if(ret != 0)
		{
			break;
		}

		ret = doExecute(context);
		if(ret != 0)
		{
			break;
		}


	} while (false);

	m_pDecoder->Close(m_samples, context);
	m_pEncoder->Close(m_samples, context);

	return ret;
}

int EncoderEngine::doExecute(SampleContext& context)
{
	int ret = 0;
	do 
	{
		ret = m_pDecoder->Decode(m_samples, context);
		if(ret <= 0)
		{
			return ret;
		}
		
		ret = m_pEncoder->Encode(m_samples, context);
		if(ret < 0)
		{
			return ret;
		}

	} while (true);

	return 0;
}

void EncoderEngine::SetDecoder(SampleDecoder* pDecoder)
{
	this->m_pDecoder = pDecoder;
}
void EncoderEngine::SetEncoder(SampleEncoder* pEncoder)
{
	this->m_pEncoder = pEncoder;
}
