#pragma once


// PBDodyTab

class PBDodyTab : public CTabCtrl
{
	DECLARE_DYNAMIC(PBDodyTab)

public:
	PBDodyTab();
	virtual ~PBDodyTab();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


