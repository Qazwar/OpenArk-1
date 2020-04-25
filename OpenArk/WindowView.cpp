#include "WindowView.h"
#include "common.h"
#include "arknamespace.h"
#include "qtwrapper.h"

WindowView::WindowView(QWidget *parent, LPVOID procId, QString procName) :StdDialog(parent)
{
	mProcId = procId;
	mProcName = procName;
	InitView();
}


WindowView::~WindowView()
{
}

void WindowView::InitView()
{
	mSourceModel->setHorizontalHeaderLabels(QStringList() << tr("windows handle") << tr("window title") << tr("window class name") 
		<< tr("is visble")<< tr("process id") << tr("thread id"));
	mSortModel->mColSortFunctions.resize(Col::LastCol);

	mSortModel->mColSortFunctions[Col::ClassName] = MySortModel::SortBy::AsText;
	mSortModel->mColSortFunctions[Col::Hwnd] = MySortModel::SortBy::AsHex;
	mSortModel->mColSortFunctions[Col::IsVisible] = MySortModel::SortBy::AsText;
	mSortModel->mColSortFunctions[Col::ProcessId] = MySortModel::SortBy::AsInt;
	mSortModel->mColSortFunctions[Col::ThreadId] = MySortModel::SortBy::AsInt;
	
	SetContextMenu();

	OnRefresh();

}

void WindowView::SetContextMenu()
{
	/*mMenu.addAction(tr("refresh"), this, &ModuleView::OnRefresh);
	mMenu.addAction(tr("hide moudle"), this, &ModuleView::OnHideMod);*/

	setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	connect(this, &ProcessMgr::customContextMenuRequested, this, [=](const QPoint &pos)
		{
			mMenu.exec(QCursor::pos());

		}
	);


}
//
//ArkModInfo * ModuleView::GetModInfo(PVOID ProcId)
//{
//	ArkModInfo *modInfo = (ArkModInfo *)new char[SIZE4M];
//
//	if (!modInfo) {
//		return 0;
//	}
//	ParamInfo param;
//	param.pInData = (PCHAR)&ProcId;
//	param.cbInData = sizeof(ProcId);
//	param.pOutData = (PCHAR)modInfo;
//	param.cbOutData = SIZE4M;
//	param.FunIdx = DrvCall::ModList;
//
//	auto result = OpenArk::IoCallDriver(param);
//	if (result == false) {
//		delete modInfo;
//		return 0;
//	}
//	return modInfo;
//}
//
//ArkModInfo * ModuleView::GetSystemModInfo()
//{
//	ArkModInfo *modInfo = (ArkModInfo *)new char[SIZE4M];
//
//	if (!modInfo) {
//		return 0;
//	}
//	ParamInfo param;
//	param.pInData = 0;
//	param.cbInData = 0;
//	param.pOutData = (PCHAR)modInfo;
//	param.cbOutData = SIZE4M;
//	param.FunIdx = DrvCall::SystemMods;
//
//	auto result = OpenArk::IoCallDriver(param);
//	if (result == false) {
//		delete modInfo;
//		return 0;
//	}
//	return modInfo;
//}
//
void WindowView::OnRefresh()
{

	ArkWindowInfo *hwndInfo = (ArkWindowInfo *)Ark::Buffer;

	
	ParamInfo param;
	param.pOutData = (PCHAR)hwndInfo;
	param.cbOutData = SIZE4M;
	param.FunIdx = DrvCall::QueryHwnd;

	auto result = OpenArk::IoCallDriver(param);
	if (result == false) {
		return;
	}

	//Çå³ýÄÚÈÝ
	mSourceModel->removeRows(0, mSourceModel->rowCount());

	int hwndCnt = 0;
	WCHAR strClassName[MAX_PATH] = { 0 };
	BOOLEAN isVisble;
	QString visble[2] = { "-" ,tr("visble") };
	DWORD threadId;
	WCHAR windowTitle[MAX_PATH];


	for (; hwndInfo->Hwnd != 0; hwndInfo++)
	{
		if (hwndInfo->ProcessId == mProcId)
		{
			isVisble = IsWindowVisible(hwndInfo->Hwnd);
			threadId = GetWindowThreadProcessId(hwndInfo->Hwnd, 0);
			GetWindowTextW(hwndInfo->Hwnd, windowTitle, MAX_PATH);
			GetClassNameW(hwndInfo->Hwnd, strClassName, MAX_PATH);

			mSourceModel->setItem(hwndCnt, Col::ClassName, MakeItem(strClassName));
			mSourceModel->setItem(hwndCnt, Col::Hwnd, MakeItem(hwndInfo->Hwnd));
			mSourceModel->setItem(hwndCnt, Col::IsVisible, MakeItem(visble[isVisble]));
			mSourceModel->setItem(hwndCnt, Col::ProcessId, MakeItem(*(DWORD*)&hwndInfo->ProcessId));
			mSourceModel->setItem(hwndCnt, Col::ThreadId, MakeItem(threadId));
			mSourceModel->setItem(hwndCnt, Col::Title,MakeItem(windowTitle));
			hwndCnt++;
		}
	}
	
	QString title = QString(tr("[%1],Processes window: %2")).arg(mProcName).arg(hwndCnt);
	this->setWindowTitle(title);
}



void WindowView::OnNouse()
{
}


