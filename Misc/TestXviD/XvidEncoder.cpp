// XvidEncoder.cpp: implementation of the CXvidEncoder class.
//
//////////////////////////////////////////////////////////////////////

#include "XvidEncoder.h"

#include <string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const int CXvidEncoder::motion_presets[MAX_QUALITY+1] =
{
	// quality 0
	0,
		
	// quality 1
	XVID_ME_ADVANCEDDIAMOND16,
	
	// quality 2
	XVID_ME_ADVANCEDDIAMOND16 | XVID_ME_HALFPELREFINE16,
	
	// quality 3
	XVID_ME_ADVANCEDDIAMOND16 | XVID_ME_HALFPELREFINE16 |
	XVID_ME_ADVANCEDDIAMOND8 | XVID_ME_HALFPELREFINE8,
	
	// quality 4
	XVID_ME_ADVANCEDDIAMOND16 | XVID_ME_HALFPELREFINE16 |
	XVID_ME_ADVANCEDDIAMOND8 | XVID_ME_HALFPELREFINE8 |
	XVID_ME_CHROMA_PVOP | XVID_ME_CHROMA_BVOP,
	
	// quality 5
	XVID_ME_ADVANCEDDIAMOND16 | XVID_ME_HALFPELREFINE16 |
	XVID_ME_ADVANCEDDIAMOND8 | XVID_ME_HALFPELREFINE8 |
	XVID_ME_CHROMA_PVOP | XVID_ME_CHROMA_BVOP,
	
	// quality 6
	XVID_ME_ADVANCEDDIAMOND16 | XVID_ME_HALFPELREFINE16 | XVID_ME_EXTSEARCH16 |
	XVID_ME_ADVANCEDDIAMOND8 | XVID_ME_HALFPELREFINE8 | XVID_ME_EXTSEARCH8 |
	XVID_ME_CHROMA_PVOP | XVID_ME_CHROMA_BVOP,
};

const int CXvidEncoder::vop_presets[MAX_QUALITY+1] =
{
	// quality 0
	0,
		
	// quality 1
	0,
		
	// quality 2
	XVID_VOP_HALFPEL,
	
	// quality 3
	XVID_VOP_HALFPEL | XVID_VOP_INTER4V,
	
	// quality 4
	XVID_VOP_HALFPEL | XVID_VOP_INTER4V,
	
	// quality 5
	XVID_VOP_HALFPEL | XVID_VOP_INTER4V |
	XVID_VOP_TRELLISQUANT,
	
	// quality 6
	XVID_VOP_HALFPEL | XVID_VOP_INTER4V |
	XVID_VOP_TRELLISQUANT | XVID_VOP_HQACPRED,
};

CXvidEncoder::CXvidEncoder()
{
	m_frameBuffer = NULL;
	m_nBufferLength = 0;

	m_xvidEncoderHandle = NULL;

	// Initialize XviD core
    xvid_gbl_init_t xvid_gbl_init;
    memset(&xvid_gbl_init, 0, sizeof(xvid_gbl_init));
	xvid_gbl_init.version = XVID_VERSION;
    xvid_gbl_init.debug = 0;
    xvid_gbl_init.cpu_flags = 0;
	xvid_global(NULL, XVID_GBL_INIT, &xvid_gbl_init, NULL);
	
	Reset();
}

CXvidEncoder::~CXvidEncoder()
{
	Reset();
}

void CXvidEncoder::Reset()
{
	if(m_frameBuffer != NULL)
	{
		delete [] m_frameBuffer;
		m_frameBuffer = NULL;
	}

	m_nBufferLength = 0;
	
	if (m_xvidEncoderHandle != NULL)
    {
		int xerr = xvid_encore(m_xvidEncoderHandle, XVID_ENC_DESTROY, NULL, NULL);
        m_xvidEncoderHandle = NULL;
    }
}

int CXvidEncoder::CreateEncoder(int width, int height)
{
	if(m_frameBuffer != NULL)
	{
		delete []m_frameBuffer;
		m_frameBuffer = NULL;
	}
	m_nBufferLength = width * height * 4;
	m_frameBuffer = new char[m_nBufferLength];
    
    // Initialize XviD encoder
    xvid_enc_create_t xvid_enc_create;
	memset(&xvid_enc_create, 0, sizeof(xvid_enc_create));
	xvid_enc_create.version = XVID_VERSION;
	xvid_enc_create.width = width;
	xvid_enc_create.height = height;
	xvid_enc_create.profile = XVID_PROFILE_AS_L4;
	
    xvid_enc_create.num_threads = 0;
	
    xvid_enc_create.fincr = 1;
	xvid_enc_create.fbase = 25;
    xvid_enc_create.max_key_interval = xvid_enc_create.fbase * 10;
	
    xvid_enc_create.max_bframes = 0;
	xvid_enc_create.bquant_ratio = 150;
	xvid_enc_create.bquant_offset = 100;
	
    xvid_enc_create.frame_drop_ratio = 0;
    xvid_enc_create.global = 0;
	
    int xerr = xvid_encore(NULL, XVID_ENC_CREATE, &xvid_enc_create, NULL);
	
	// Retrieve the encoder instance from the structure
	m_xvidEncoderHandle = xvid_enc_create.handle;
	
	return xerr;
}

int CXvidEncoder::EncodeFrame(void* image, int stride, int csp, int quality)
{
	// Version for the frame and the stats
	xvid_enc_frame_t xvid_enc_frame;
	memset(&xvid_enc_frame, 0, sizeof(xvid_enc_frame));
	xvid_enc_frame.version = XVID_VERSION;
	
	// Bind output buffer
	xvid_enc_frame.bitstream = m_frameBuffer;
	xvid_enc_frame.length = m_nBufferLength;
	
	// Initialize input image fields
	xvid_enc_frame.input.csp = csp;
	xvid_enc_frame.input.plane[0] = image;
	xvid_enc_frame.input.stride[0] = stride;
	
	// Set up core's general features
	xvid_enc_frame.vol_flags = 0;
	
	// Set up core's general features
	xvid_enc_frame.vop_flags = vop_presets[quality];
	
	// Frame type -- let core decide for us
	xvid_enc_frame.type = XVID_TYPE_AUTO;
	
	// Force the right quantizer -- It is internally managed by RC plugins
	xvid_enc_frame.quant = 0;
	
	// Set up motion estimation flags
	xvid_enc_frame.motion = motion_presets[quality];
	
	// We don't use special matrices
	xvid_enc_frame.quant_intra_matrix = NULL;
	xvid_enc_frame.quant_inter_matrix = NULL;

	// Initialize encoder stats
	xvid_enc_stats_t xvid_enc_stats;
	memset(&xvid_enc_stats, 0, sizeof(xvid_enc_stats));
	xvid_enc_stats.version = XVID_VERSION;
	
	// Encode the frame
	int frameBytes = xvid_encore(m_xvidEncoderHandle, XVID_ENC_ENCODE,
        &xvid_enc_frame, &xvid_enc_stats);
	
	return frameBytes;
}
