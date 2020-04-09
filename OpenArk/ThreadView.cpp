#include "ThreadView.h"
#include "common.h"
#include "arknamespace.h"
#include "qtwrapper.h"
#include "ModuleView.h"

ThreadView::ThreadView(QWidget *parent, LPVOID procId, QString procName) :StdDialog(parent)
{
	mProcId = procId;
	mProcName = procName;
	mStateTranslation[KTHREAD_STATE::DeferredReady] = tr("DeferredReady");
	mStateTranslation[KTHREAD_STATE::GateWait] = tr("GateWait");
	mStateTranslation[KTHREAD_STATE::Initialized] = tr("Initialized");
	mStateTranslation[KTHREAD_STATE::Ready] = tr("Ready");
	mStateTranslation[KTHREAD_STATE::Running] = tr("Running");
	mStateTranslation[KTHREAD_STATE::Standby] = tr("Standby");
	mStateTranslation[KTHREAD_STATE::Terminated] = tr("Terminated");
	mStateTranslation[KTHREAD_STATE::Transition] = tr("Transition");
	mStateTranslation[KTHREAD_STATE::Waiting] = tr("Waiting");


	InitView();
	OnRefresh();
}


ThreadView::~ThreadView()
{
}

void ThreadView::InitView()
{
	mSourceModel->setHorizontalHeaderLabels(QStringList() << tr("thread id") << tr("Ethread ") << tr("Teb") 
		<< tr("Priority") << tr("startAddress") << tr("module")  
		<< tr("contextSwitches") << tr("state"));

	
	mSortModel->mColSortFunctions.resize(Col::LastCol);

	mSortModel->mColSortFunctions[Col::ContextSwitchs] = MySortModel::SortBy::AsInt;
	mSortModel->mColSortFunctions[Col::Ethread] = MySortModel::SortBy::AsHex;
	mSortModel->mColSortFunctions[Col::Mod] = MySortModel::SortBy::AsHex;
	mSortModel->mColSortFunctions[Col::Priority] = MySortModel::SortBy::AsInt;
	mSortModel->mColSortFunctions[Col::StartAddress] = MySortModel::SortBy::AsHex;
	mSortModel->mColSortFunctions[Col::State] = MySortModel::SortBy::AsText;
	mSortModel->mColSortFunctions[Col::Teb] = MySortModel::SortBy::AsHex;
	mSortModel->mColSortFunctions[Col::Tid] = MySortModel::SortBy::AsInt;

	
	SetContextMenu();
}

void ThreadView::SetContextMenu()
{
	mMenu.addAction(tr("refresh"), this, &ThreadView::OnRefresh);
	mMenu.addAction(tr("suspend thread"), this, &ThreadView::OnSuspendThread);
	mMenu.addAction(tr("resume thread"), this, &ThreadView::OnResumeThread);

	setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	connect(this, &ProcessMgr::customContextMenuRequested, this, [&](const QPoint &pos)
		{
			ParamInfo param;
			ulong suspendCount;
			BOOLEAN result;
			auto actList = mMenu.actions();

			if (mTableView->currentIndex().isValid() == 0)
			{
				actList.at(1)->setEnabled(false);
				actList.at(2)->setEnabled(false);
			}
			else
			{

				auto threadId = GetColDataFromInt(Col::Tid);
				RtlZeroMemory(&param, sizeof(param));
				param.pInData = (PCHAR)&threadId;
				param.cbInData = sizeof(threadId);
				param.pOutData = (PCHAR)&suspendCount;
				param.cbOutData = sizeof(suspendCount);
				param.FunIdx = DrvCall::SusPendCount;

				result = OpenArk::IoCallDriver(param);
				if (result)
				{
					if (suspendCount)
					{
						actList.at(1)->setEnabled(false);
						actList.at(2)->setEnabled(true);
					}
					else
					{
						actList.at(1)->setEnabled(true);
						actList.at(2)->setEnabled(false);
					}
				}
			}


			mMenu.exec(QCursor::pos());

		}
	);


}

void ThreadView::ProcessThreadInfo(ArkThreadInfo * ThreadInfo, ArkModInfo * ModInfo)
{
	int numOfThreads = ThreadInfo->ThreadCnt;

	for (int i = 0; i < numOfThreads; i++)
	{
		ULONG_PTR cmpAddr = mProcId == (LPVOID)4 ? (ULONG_PTR)ThreadInfo->Threads[i].StartAddress : 
			(ULONG_PTR)ThreadInfo->Threads[i].Win32StartAddress;
		for (int j = 0; j < ModInfo->NumberOfMods; j++)
		{
			if (cmpAddr >= ModInfo[j].RegionBase  && cmpAddr < ModInfo[j].RegionBase + ModInfo[j].RegionSize)
			{
				auto p = wcsrchr(ModInfo[j].Path, L'\\');
				if (p)
				{
					memcpy(ThreadInfo->Threads[i].InModName, p+1, wcslen(p+1)*sizeof(WCHAR));
					break;
				}
			}
		}
	}


}

void ThreadView::OnRefresh()
{

	ArkThreadInfo *threadInfo = (ArkThreadInfo *)Ark::Buffer;


	ParamInfo param;
	param.pInData = (PCHAR)&mProcId;
	param.cbInData = sizeof(mProcId);
	param.pOutData = (PCHAR)threadInfo;
	param.cbOutData = SIZE4M;
	param.FunIdx = DrvCall::ProcThreadList;

	auto result = OpenArk::IoCallDriver(param);
	if (reset == false)
	{
		return;
	}

	//Çå³ýÄÚÈÝ
	mSourceModel->removeRows(0, mSourceModel->rowCount());
	int numOfthreads = threadInfo->ThreadCnt;
	ArkThreadInfoEntry *nextThreadInfo = threadInfo->Threads;
	ArkModInfo *modInfo;
	
	if (mProcId == (PVOID)4)
	{
		modInfo = ModuleView::GetSystemModInfo();
	}
	else
	{
		modInfo = ModuleView::GetModInfo(mProcId);
	}
	if (modInfo)
	{
		ProcessThreadInfo(threadInfo, modInfo);
		delete modInfo;
	}
	else
	{
		return;
	}

	for (int i = 0; i < numOfthreads; i++, nextThreadInfo++)
	{
		mSourceModel->setItem(i, Col::Tid, MakeItem((ULONG_PTR)nextThreadInfo->ThreadId));
		mSourceModel->setItem(i, Col::Ethread, MakeItem(nextThreadInfo->Ethread));
		mSourceModel->setItem(i, Col::Teb, MakeItem(nextThreadInfo->Teb));
		mSourceModel->setItem(i, Col::ContextSwitchs, MakeItem(nextThreadInfo->ContextSwitches));
		mSourceModel->setItem(i, Col::Mod, MakeItem(nextThreadInfo->InModName));
		mSourceModel->setItem(i, Col::Priority, MakeItem(nextThreadInfo->Priority));
		if (mProcId == (LPVOID)4)
		{

			mSourceModel->setItem(i, Col::StartAddress, MakeItem(nextThreadInfo->StartAddress));
		}
		else
		{
			mSourceModel->setItem(i, Col::StartAddress, MakeItem(nextThreadInfo->Win32StartAddress));

		}
		mSourceModel->setItem(i, Col::State, MakeItem(mStateTranslation[nextThreadInfo->State]));
		
	}

	QString title = QString(tr("[%1],Processes threads: (%2)")).arg(mProcName).arg(numOfthreads);
	this->setWindowTitle(title);
}

void ThreadView::OnSuspendThread()
{
	ParamInfo param;
	ulong suspendCount;
	BOOLEAN result;
	ArkThreadSuspendParam temp;

	temp.ThreadId = (PVOID)GetColDataFromInt(Col::Tid);
	 temp.ToSuspend = true;
	RtlZeroMemory(&param, sizeof(param));
	param.pInData = (PCHAR)&temp;
	param.cbInData = sizeof(temp);
	param.FunIdx = DrvCall::SuspendThreadEnum;

	result = OpenArk::IoCallDriver(param);
}

void ThreadView::OnResumeThread()
{
	ParamInfo param;
	ulong suspendCount;
	BOOLEAN result;
	ArkThreadSuspendParam temp;

	temp.ThreadId = (PVOID)GetColDataFromInt(Col::Tid);
	temp.ToSuspend = false;
	RtlZeroMemory(&param, sizeof(param));
	param.pInData = (PCHAR)&temp;
	param.cbInData = sizeof(temp);
	param.FunIdx = DrvCall::SuspendThreadEnum;

	result = OpenArk::IoCallDriver(param);
}



void ThreadView::OnNouse()
{
}



