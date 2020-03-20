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

	typedef enum  ProcHdeaderIdx {
		Name,
		Pid,
		PPid,
		Path,
		Addr,
		Access,
		Corp
	}PHI;

	
	ProcessMgr(QWidget *parent);
	~ProcessMgr();
	QIcon GetFileIcon(QString filePath);
	bool IsAccessProcess(DWORD dwPsid);
	QString GetCompanyName(QString filePath);
	void onProcSelection(QString pid);
	void onShowProcess();
private slots:
	void OnRefresh();

private:
	void InitProcessView();


	
	QMenu *mMenu = nullptr;

	
	
};
