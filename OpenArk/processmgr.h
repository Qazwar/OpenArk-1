#pragma once

#include "ui_processmgr.h"
#include "stdtable.h"
#include <qwidget.h>
#include <qmenu.h>
#include <QtGui>
#include <QtCore>




class ProcessMgr : public StdTable
{
	Q_OBJECT

public:
	ProcessMgr(QWidget *parent);
	~ProcessMgr();

	void onRefresh();
	void onProcSelection(QString pid);
	void onShowProcess();


private:
	void InitProcessView();


	/*Ui::ProcessMgr  ui;*/
	QMenu *mMenu = nullptr;

	//QStandardItemModel* mProcModel;
	
};
