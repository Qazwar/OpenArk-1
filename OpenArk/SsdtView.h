#pragma once
#include "StdDialog.h"
#include "common.h"



class SsdtView :public StdDialog
{
	Q_OBJECT
public:

	enum Col {
		Serial,
		FunName,
		CurFunAddr,
		Hook,
		OriginalAddr,
		InMod,
		LastCol
	};

	SsdtView(QWidget *parent = 0);
	~SsdtView();
	void InitView();
	void SetContextMenu();




private slots:
	void OnNouse();
	void OnRefresh();


private:
	QMenu mMenu;
	static PCHAR mSsdtFunName[];
	
};

