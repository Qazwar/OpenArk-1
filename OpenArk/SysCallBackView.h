#pragma once
#include "StdDialog.h"
#include "common.h"



class SysCallBackView :public StdTable
{
	Q_OBJECT
public:

	enum Col {
		CallbackEntry,
		CallbakType,
		ModPath,
		Company,
		Note,
		LastCol
	};

	SysCallBackView(QWidget *parent = 0);
	~SysCallBackView();
	void InitView();
	void SetContextMenu();




private slots:
	void OnNouse();
	void OnRefresh();


private:
	QMenu mMenu;


};