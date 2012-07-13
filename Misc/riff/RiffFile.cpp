#include "RiffFile.h"

	/* Copy n into dst as a 4 byte, little endian number.
	Should also work on big endian machines */
	static void int2str(char *dst, int n)
	{
		dst[0] = (n    )&0xff;
		dst[1] = (n>> 8)&0xff;
		dst[2] = (n>>16)&0xff;
		dst[3] = (n>>24)&0xff;
	}

	/* Convert a string of 4 or 2 bytes to a number,
	also working on big endian machines */
	static unsigned int ustr2uint(const unsigned char *str)
	{
		return ( str[0] | (str[1]<<8) | (str[2]<<16) | (str[3]<<24) );
	}

	static unsigned int str2uint(const char *s)
	{
		const unsigned char * str = (const unsigned char *)s;
		return ( str[0] | (str[1]<<8) | (str[2]<<16) | (str[3]<<24) );
	}

	static unsigned int str2ushort(const unsigned char *str)
	{
		return ( str[0] | (str[1]<<8) );
	}

RiffChunk::RiffChunk()
{
	m_chunkID = 0;
	m_chunkSize = 0;
	m_pChunkData = NULL;
	m_pParent = NULL;
}

RiffChunk::RiffChunk(size_t ckID, size_t ckSize, char* pData)
{
	m_chunkID = ckID;
	m_chunkSize = ckSize;
	m_pChunkData = pData;

	m_pParent = NULL;

	char name[5] = {0};
	int2str(name, m_chunkID);
	
	printf("CHNK: name=%s, size=%010d(0x%08X), off=0x%08X\n", name, m_chunkSize, m_chunkSize, m_nOffset);
}

RiffChunk::RiffChunk(const char* header, size_t nOffset)
{
	m_chunkID = str2uint(header);
	m_chunkSize = str2uint(header + 4);

	m_nOffset = nOffset;

	char name[5] = {0};
	int2str(name, m_chunkID);
	printf("CHNK: name=%s, size=%010d(0x%08X), off=0x%08X\n", name, m_chunkSize, m_chunkSize, m_nOffset);
}

RiffChunk::~RiffChunk()
{
	if(m_pChunkData != NULL)
	{
		delete [] m_pChunkData;
		m_pChunkData = NULL;
	}
}

RiffNode* RiffChunk::getChildAt(int childIndex)
{
	return NULL;
}

//Returns the number of children
int RiffChunk::getChildrenCount()
{
	return 0;
}

//Returns the parent
RiffNode* RiffChunk::getParent()
{
	return m_pParent;
}

//Returns true if the receiver is a leaf.
bool RiffChunk::isLeaf()
{
	return true;
}

//Returns the children list
void RiffChunk::getChildren(std::vector<RiffNode*>& children)
{
}

void RiffChunk::setParent(RiffNode* newParent)
{
	m_pParent = newParent;
}

RiffList::RiffList()
{
	m_nListName = 0;
	m_nListSize = 0;
	m_nListType = 0;
	m_pListData = NULL;

	m_pParent = NULL;
	m_children.clear();
}

RiffList::RiffList(const char* listHdr, char* pData, size_t nOffset)
{
	m_nListName = str2uint(listHdr);
	m_nListSize = str2uint(listHdr + 4);
	m_nListType = str2uint(listHdr + 8);

	m_pListData = pData;

	m_nOffset = nOffset;

	m_pParent = NULL;
	m_children.clear();

	char name[5] = {0};
	int2str(name, m_nListName);

	char type[5] = {0};
	int2str(type, m_nListType);

	printf("LIST: name=%s, size=%010d(0x%08X), off=0x%08X, type=%s\n", name, m_nListSize, m_nListSize, m_nOffset, type);
}
RiffList::RiffList(size_t name, size_t size, size_t type, char* pData)
{
	m_nListName = name;
	m_nListSize = size;
	m_nListType = type;
	
	m_pListData = pData;
	
	m_pParent = NULL;
	m_children.clear();
}

RiffList::~RiffList()
{
	//clear children
	std::vector<RiffNode*>::iterator iter = m_children.begin();

	RiffNode* child = NULL;
	for( ; iter != m_children.end(); ++iter)
	{
		child = *iter;
		delete child;
	}

	m_children.clear();

	//clear data
	if(m_pListData != NULL)
	{
		delete [] m_pListData;
		m_pListData = NULL;
	}
}


//Returns the child at index 
RiffNode* RiffList::getChildAt(int childIndex)
{
	if(childIndex < 0 || childIndex >= m_children.size())
	{
		return NULL;
	}

	return m_children.at(childIndex);
}

//Returns the number of children
int RiffList::getChildrenCount()
{
	return m_children.size();
}

//Returns the parent
RiffNode* RiffList::getParent()
{
	return m_pParent;
}

//Returns true if the receiver is a leaf.
bool RiffList::isLeaf()
{
	return false;
}

//Returns the children list
void RiffList::getChildren(std::vector<RiffNode*>& children)
{
	std::vector<RiffNode*>::iterator iter = m_children.begin();
	
	for( ; iter != m_children.end(); ++iter)
	{
		children.push_back(*iter);
	}
}

void RiffList::addChild(RiffNode* child)
{
	m_children.push_back(child);
}

void RiffList::setParent(RiffNode* newParent)
{
	m_pParent = newParent;
}

CRiffFile::CRiffFile(const char* lpFileName)
{
	m_pRoot = new RiffList(0, 0, 0, NULL);

	FILE* fp = fopen(lpFileName, "rb");
	if(fp == NULL)
	{
		sprintf(m_szErrorMsg, "failed to open file: %s", lpFileName);
		return;
	}

	parseFile(fp);

	fclose(fp);
}
int CRiffFile::parseFile0(FILE* fp)
{
	char buffer[256];
	do 
	{
		//read root node first
		if(fread(buffer, 1, 12, fp) != 12)
		{
			sprintf(m_szErrorMsg, "failed to read list head");
			break;
		}
		
		int n = str2uint(buffer + 4);
		n = PAD_EVEN(n);
		
		char* pData = new char[n - 4];
		if(fread(pData, 1, n - 4, fp) != (n - 4))
		{
			sprintf(m_szErrorMsg, "failed to read list data");
			break;
		}
		RiffList* pList = new RiffList(buffer, pData);
		pList->setParent(m_pRoot);
		
		read(pData, n - 4, pList);

		delete [] pData;
		pData = NULL;
		
	} while (false);

	return 0;
}
int CRiffFile::parseFile(FILE* fp)
{
	int nRet = -1;
	int nPos = 0;
	char buffer[12];
	int n;

	//read root node first
	if(fread(buffer, 1, 12, fp) != 12)
	{
		sprintf(m_szErrorMsg, "fread failed");
		return nRet;
	}
	n = PAD_EVEN(str2uint(buffer + 4));

	RiffList* pList = new RiffList(buffer, 0, nPos);
	pList->setParent(m_pRoot);

	read(fp, 12, n - 4, pList);

	return 0;
}

CRiffFile::~CRiffFile()
{
	memset(m_szErrorMsg, 0, ERROR_MSG_SIZE);

	if(m_pRoot != NULL)
	{
		delete m_pRoot;
		m_pRoot = NULL;
	}
}

void CRiffFile::read(FILE* fp, int nOffset, int nDataSize, RiffNode* pParent)
{
	char buffer[12];
	int n, nPos = 0;
	while(nPos < nDataSize)
	{
		if(fread(buffer, 1, 8, fp) != 8)
		{
			sprintf(m_szErrorMsg, "fread failed");
			return;
		}

		//LIST
		if(_strnicmp(buffer, "LIST", 4) == 0)
		{
			if(fread(buffer + 8, 1, 4, fp) != 4)
			{
				sprintf(m_szErrorMsg, "fread failed");
				return;
			}
			
			RiffList* pList = new RiffList(buffer, 0, nOffset);
			pList->setParent(pParent);

			n = PAD_EVEN(str2uint(buffer + 4));
			nPos += n + 8;

			nOffset += 12;
			
			read(fp, nOffset, n - 4, pList);

			nOffset += n - 4;
		}
		else
		{
			n = PAD_EVEN(str2uint(buffer + 4));
			if(fseek(fp, n, SEEK_CUR) != 0)
			{
				sprintf(m_szErrorMsg, "fseek failed");
				return;
			}
			nPos += n + 8;
			
			RiffChunk* pChunk = 0;
			pChunk = new RiffChunk(buffer, nOffset);
			pChunk->setParent(pParent);

			nOffset += n + 8;
		}
	}
}

void CRiffFile::read(char* pData, int nDataSize, RiffNode* pParent)
{
	if(nDataSize < 0)
	{
		return;
	}

	int n;
	int nPos = 0;
	while(nPos < nDataSize)
	{
		//LIST
		if(_strnicmp(pData + nPos,"LIST",4) == 0)
		{
			n = str2uint(pData + nPos + 4);
			n = PAD_EVEN(n);

			RiffList* pList = new RiffList(pData + nPos, pData + nPos + 12);
			pList->setParent(pParent);
			
			read(pData + nPos + 12, n - 4, pList);

			nPos += n + 8;
		}
		else
		{
			n = str2uint(pData + nPos + 4);
			n = PAD_EVEN(n);
			
			size_t ckID = str2uint(pData + nPos);
			
			RiffChunk* pChunk = new RiffChunk(ckID, str2uint(pData + nPos + 4), pData + nPos + 8);
			pChunk->setParent(pParent);

			nPos += n + 8;
		}
	}
}