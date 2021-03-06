#pragma once
#include "StdDialog.h"

class HandleView :public StdDialog
{
	Q_OBJECT
public:

	enum Col {
		TypeName,
		HandleName,
		Handle,
		Object,
		TypeIndex,
		RefrenceCount
	};


	HandleView(QWidget *parent, LPVOID procId, QString procName);
	~HandleView();
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

