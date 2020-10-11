#pragma once


// CGeneralOutputPage 对话框

class CGeneralOutputPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CGeneralOutputPage)

public:
	CGeneralOutputPage();
	virtual ~CGeneralOutputPage();

// 对话框数据
	enum { IDD = IDD_GENERALOUTPUTPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
