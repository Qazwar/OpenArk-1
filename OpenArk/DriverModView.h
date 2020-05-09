#pragma once
#include "StdDialog.h"
#include "common.h"



class DriverModView :public StdTable
{
	Q_OBJECT
public:

	enum Col {
		DriverName,
		BaseAddress,
		ImageSize,
		DriverObject,
		DriverPath,
		ServiceName,
		LoadOrder,
		FileCompany,
		LastCol
	};

	DriverModView(QWidget *parent = 0);
	~DriverModView();
	void InitView();
	void SetContextMenu();




private slots:
	void OnNouse();
	void OnRefresh();


private:
	QMenu mMenu;
	

};