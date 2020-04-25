#pragma once
#include "StdDialog.h"
#include "common.h"




class GdtView :public StdTable
{
	Q_OBJECT
public:

	enum Col {
		CpuSerial,
		Selector,
		Base,
		Limit,
		Grantity,
		Dpl,
		Type,
		LastCol
	};

	GdtView(QWidget *parent);
	~GdtView();
	void InitView();
	void SetContextMenu();


private slots:
	void OnNouse();
	void OnRefresh();


private:
	QMenu mMenu;
};

