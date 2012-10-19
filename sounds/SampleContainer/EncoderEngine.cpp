#include "EncoderEngine.h"

EncoderEngine::EncoderEngine()
{
}

EncoderEngine::~EncoderEngine()
{
}

int EncoderEngine::execute()
{
	int ret = 0;
	
	do 
	{
		ret = m_pDecoder->Init(m_srcFile.c_str(), m_samples);
		if(ret != 0)
		{
			break;
		}
		
		ret = m_pEncoder->Init(m_dstFile.c_str(), m_samples);
		if(ret != 0)
		{
			break;
		}

		ret = doExecute();
		if(ret != 0)
		{
			break;
		}


	} while (false);

	m_pDecoder->Done();
	m_pEncoder->Done();
	

	return ret;
}

int EncoderEngine::doExecute()
{
	int ret = 0;
	do 
	{
		ret = m_pDecoder->Decode(m_samples);
		if(ret <= 0)
		{
			return ret;
		}
		
		ret = m_pEncoder->Encode(m_samples);
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

void EncoderEngine::SetSrcFile(const TCHAR* pFileName)
{
	m_srcFile = pFileName;
}

void EncoderEngine::SetDstFile(const TCHAR* pFileName)
{
	m_dstFile = pFileName;
}