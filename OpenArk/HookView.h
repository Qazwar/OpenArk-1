#pragma once
#include "StdDialog.h"
#include "common.h"




class HookView :public StdDialog
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

	HookView(QWidget *parent = 0);
	~HookView();
	void InitView();
	//void SetContextMenu();




private slots:
	void OnNouse();
	void OnRefresh();


private:
	QMenu mMenu;
	

};
