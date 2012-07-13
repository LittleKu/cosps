#ifndef __RIFF_FILE_H__
#define __RIFF_FILE_H__

#include <vector>

class RiffNode
{
public:
	//Returns the child at index 
	virtual RiffNode* getChildAt(int childIndex) = 0;

	//Returns the number of children
	virtual int getChildrenCount() = 0;

	//Returns the parent
	virtual RiffNode* getParent() = 0;

	//Returns true if the receiver is a leaf.
	virtual bool isLeaf() = 0;

	//Returns the children list
	virtual void getChildren(std::vector<RiffNode*>& children) = 0;
};

class RiffChunk : public RiffNode
{
public:
	RiffChunk();
	RiffChunk(const char* header, size_t nOffset = -1);
	RiffChunk(size_t ckID, size_t ckSize, char* pData);
	virtual ~RiffChunk();

	//Returns the child at index 
	virtual RiffNode* getChildAt(int childIndex);
	
	//Returns the number of children
	virtual int getChildrenCount();
	
	//Returns the parent
	virtual RiffNode* getParent();
	
	//Returns true if the receiver is a leaf.
	virtual bool isLeaf();
	
	//Returns the children list
	virtual void getChildren(std::vector<RiffNode*>& children);

	virtual void setParent(RiffNode* newParent);

private:
	//data fields
	size_t	m_chunkID;
	size_t	m_chunkSize;
	char*	m_pChunkData;

	//structure overhead
	RiffNode* m_pParent;
	size_t	m_nOffset;	//the offset in the riff file
};

class RiffList : public RiffNode
{
public:
	RiffList();
	RiffList(const char* listHdr, char* pData = NULL, size_t nOffset = -1);
	RiffList(size_t name, size_t size, size_t type, char* pData = NULL);
	virtual ~RiffList();
	
	//Returns the child at index 
	virtual RiffNode* getChildAt(int childIndex);
	
	//Returns the number of children
	virtual int getChildrenCount();
	
	//Returns the parent
	virtual RiffNode* getParent();
	
	//Returns true if the receiver is a leaf.
	virtual bool isLeaf();
	
	//Returns the children list
	virtual void getChildren(std::vector<RiffNode*>& children);


	virtual void addChild(RiffNode* child);
	virtual void setParent(RiffNode* newParent);
	
private:
	//data fields
	size_t	m_nListName;
	size_t	m_nListSize;
	size_t	m_nListType;
	char*	m_pListData;

	//structure overhead
	RiffNode* m_pParent;
	std::vector<RiffNode*> m_children;
	size_t	m_nOffset;	//the offset in the riff file
};

#define PAD_EVEN(x) ( ((x)+1) & ~1 )

class CRiffFile
{
public:
	CRiffFile(const char* lpFileName);
	~CRiffFile();
private:
	enum
	{
		ERROR_MSG_SIZE = 256
	};
	void read(char* pData, int nDataSize, RiffNode* pParent);

	void read(FILE* fp, int nOffset, int nDataSize, RiffNode* pParent);

	int parseFile(FILE* fp);
	int parseFile0(FILE* fp);
private:
	RiffList* m_pRoot;
	char m_szErrorMsg[ERROR_MSG_SIZE];
};

#endif