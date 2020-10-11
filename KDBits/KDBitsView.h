// KDBitsView.h : CKDBitsView 类的接口
//


#pragma once
#include "kdcontentsheet.h"


class CKDBitsView : public CView
{
protected: // 仅从序列化创建
	CKDBitsView();
	DECLARE_DYNCREATE(CKDBitsView)

// 属性
public:
	CKDBitsDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CKDBitsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	CKDContentSheet m_sheet;
public:
	virtual void OnInitialUpdate();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMainMenuLoadGamelist();
public:
	afx_msg void OnMenuDownlistStartTask();
	afx_msg void OnMenuDownlistStopTask();
	afx_msg void OnMenuDownlistDeleteTask();
	afx_msg void OnMainMenuStartAll();
	afx_msg void OnMainMenuStopAll();
	afx_msg void OnMainMenuDeleteAll();
};

#ifndef _DEBUG  // KDBitsView.cpp 中的调试版本
inline CKDBitsDoc* CKDBitsView::GetDocument() const
   { return reinterpret_cast<CKDBitsDoc*>(m_pDocument); }
#endif
