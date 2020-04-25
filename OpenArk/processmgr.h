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

	typedef enum  Col {
		Name,
		Pid,
		PPid,
		Path,
		Addr,
		Access,
		Corp
	}PHI;

	/*typedef enum Function
	{
		TerminateProcess
	};*/

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
	int ProcessProcInfo(StuProcInfo* procInfo, QVector<QProcInfo> &vprocInfo);
	int GetHideProcessCnt(StuProcInfo* procInfo);
	void SetContextMenu();

	
private slots:
	void OnNouse();
	void OnRefresh();
	void OnHideProcess();
	void OnLookProcMod();
	void OnLookProcHandle();
	void OnLookProcThreads();
	void OnLookProcWindows();
	void OnTerminateProcess();
	void OnPrintTest();

private:
	void InitProcessView();

	QMenu mMenu;
	
};
