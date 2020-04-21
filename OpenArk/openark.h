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
	void OnTabChanged(bool checked);//tab�ؼ��±�
	bool InitTargetDev();//�������豸
	bool InitArkNameSpace();
	void UnLoadTargetDev();//�˳�ж���豸
	static bool IoCallDriver(ParamInfo param);
	BOOLEAN SeEnablePrivilege();
	void ShowMessage(QString text);
	void InitView();
		 

	~OpenArk();

private:
	Ui::OpenArkClass ui;
	BOOLEAN mInitSucess = 0;
};
