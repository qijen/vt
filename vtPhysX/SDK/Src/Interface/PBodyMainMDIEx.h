#pragma once


// PBodyMainMDIEx frame with splitter

class PBodyMainMDIEx : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(PBodyMainMDIEx)
protected:
	PBodyMainMDIEx();           // protected constructor used by dynamic creation
	virtual ~PBodyMainMDIEx();

	CSplitterWnd m_wndSplitter;

public:
	virtual void OnFinalRelease();

protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


