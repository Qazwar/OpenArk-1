#pragma once
#include "StdDialog.h"
#include "common.h"

class ThreadView :public StdDialog
{
	Q_OBJECT
public:

	enum Col {
		Tid,
		Ethread,
		Teb,
		Priority,
		StartAddress,
		Mod,
		ContextSwitchs,
		State,
		LastCol
	};
	typedef enum _KTHREAD_STATE {
		Initialized,
		Ready,
		Running,
		Standby,
		Terminated,
		Waiting,
		Transition,
		DeferredReady,
		GateWait

	} KTHREAD_STATE;

	ThreadView(QWidget *parent, LPVOID procId, QString procName);
	~ThreadView();
	void InitView();
	void SetContextMenu();
	void ProcessThreadInfo(ArkThreadInfo *ThreadInfo,ArkModInfo * ModInfo);

	LPVOID mProcId;
	QString mProcName;
	QString mStateTranslation[GateWait + 1];

	void OnRefresh();
private slots:
	void OnNouse();

private:
	QMenu mMenu;
};

