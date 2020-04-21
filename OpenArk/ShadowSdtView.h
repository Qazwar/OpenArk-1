#pragma once
#include "StdDialog.h"
#include "common.h"
#include "HookView.h"
#include "pe.h"


class ShadowSdtView :public HookView
{
	Q_OBJECT
public:


	ShadowSdtView(QWidget *parent = 0);
	~ShadowSdtView();
	void SetContextMenu();




private slots:
	void OnNouse();
	void OnRefresh();


private:
	static PCHAR mShadowSdtFunName[];
	PVOID *mShaodwSdtBase;
	long mLimit;
	QMenu mMenu;
	ULONG_PTR mWin32kBase;
};

