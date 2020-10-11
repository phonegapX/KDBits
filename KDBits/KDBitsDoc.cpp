// KDBitsDoc.cpp : CKDBitsDoc 类的实现
//

#include "stdafx.h"
#include "KDBits.h"

#include "KDBitsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKDBitsDoc

IMPLEMENT_DYNCREATE(CKDBitsDoc, CDocument)

BEGIN_MESSAGE_MAP(CKDBitsDoc, CDocument)
END_MESSAGE_MAP()


// CKDBitsDoc 构造/析构

CKDBitsDoc::CKDBitsDoc()
{
	// TODO: 在此添加一次性构造代码

}

CKDBitsDoc::~CKDBitsDoc()
{
}

BOOL CKDBitsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CKDBitsDoc 序列化

void CKDBitsDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CKDBitsDoc 诊断

#ifdef _DEBUG
void CKDBitsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CKDBitsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CKDBitsDoc 命令
