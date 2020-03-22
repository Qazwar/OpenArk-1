#pragma once

#include <QtWidgets/QMainWindow>
#include <qlabel.h>
#include "ui_openark.h"
#include "common.h"


class OpenArk : public QMainWindow
{
	Q_OBJECT

public:
	OpenArk(QWidget *parent = Q_NULLPTR);
	void OnTabChanged(bool checked);//tab控件下标
	bool InitTargetDev();//打开驱动设备
	void UnLoadTargetDev();//退出卸载设备
	static bool IoCallDriver(ParamInfo param);
	BOOLEAN SeEnablePrivilege();
	void ShowMessage(QString text);
		 

	~OpenArk();

private:
	Ui::OpenArkClass ui;
};
