#pragma once

#include "ui_processmgr.h"
#include "stdtable.h"
#include <qwidget.h>
#include <qmenu.h>
#include <QtGui>
#include <QtCore>
#include "common.h"




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

	struct QProcInfo{
	public:
		QString ProcName;
		QString ProcId;
		QString ParentProcId;
		QString ProcPath;
		QString	ProcAddr;
		QString Accessble;
		QString CorpName;
	};
	
	ProcessMgr(QWidget *parent);
	~ProcessMgr();
	QIcon GetFileIcon(QString filePath);
	bool IsAccessProcess(DWORD dwPsid);
	QString GetCompanyName(QString filePath);
	void ProcessProcInfo(StuProcInfo* procInfo, QVector<QProcInfo> &vprocInfo);
	
private slots:
	void OnRefresh();

private:
	void InitProcessView();


	
	QMenu *mMenu = nullptr;

	
	
};
