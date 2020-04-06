#pragma once
#include "StdDialog.h"

class ModuleView :public StdDialog
{
	Q_OBJECT
public:

	enum Col{
		Path,
		RegionBase,
		RegionSize,
		Company
	};


	ModuleView(QWidget *parent,LPVOID procId,QString procName);
	~ModuleView();
	void InitView();
	void SetContextMenu();

	LPVOID mProcId;
	QString mProcName;

	void OnRefresh();
private slots:
	void OnNouse();
	void OnHideMod();




private:
	QMenu mMenu;


};

