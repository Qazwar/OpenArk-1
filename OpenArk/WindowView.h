#pragma once
#include "StdDialog.h"
#include "common.h"

class WindowView :public StdDialog
{
	Q_OBJECT
public:

	enum Col {
		Hwnd,
		Title,
		ClassName,
		IsVisible,
		ProcessId,
		ThreadId,
		LastCol
	};
	
	WindowView(QWidget *parent, LPVOID procId, QString procName);
	~WindowView();
	void InitView();
	void SetContextMenu();
	

	LPVOID mProcId;
	QString mProcName;
	

	void OnRefresh();
private slots:
	void OnNouse();
	

private:
	QMenu mMenu;
};

