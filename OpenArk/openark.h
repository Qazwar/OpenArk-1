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
	void SetLabelText(QString str);//���ñ�ǩ�ı�
	void UnLoadTargetDev();//�˳�ж���豸
	static bool IoCallDriver(ParamInfo param);
	BOOLEAN SeEnablePrivilege();
		 

	~OpenArk();

private:
	Ui::OpenArkClass ui;
	QLabel *mLable;
};
