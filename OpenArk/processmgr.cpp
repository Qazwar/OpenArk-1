#include "processmgr.h"
#include "common.h"
#include "openark.h"
#include "arknamespace.h"
#include "ModuleView.h"
#include <qvector.h>
#include <TlHelp32.h>
#include "HandleView.h"
#include "ThreadView.h"
#include "WindowView.h"

#pragma comment (lib,"Version.lib")
ProcessMgr::ProcessMgr(QWidget *parent)
	: StdTable(parent)
{
	Ark::Ps = this;
	InitProcessView();
}

ProcessMgr::~ProcessMgr()
{



}

QIcon ProcessMgr::GetFileIcon(QString filePath)
{
	QFileInfo fileInfo(filePath);
	QFileIconProvider icon;
	return icon.icon(filePath);
}

bool ProcessMgr::IsAccessProcess(DWORD dwPsid)
{
	HANDLE hProcess =  OpenProcess(PROCESS_TERMINATE | PROCESS_VM_OPERATION,0,dwPsid);
	if (hProcess) {
		if (GetCurrentProcessId() == dwPsid)
			return false;
		else {
			return true;
		}
	}
	return false;
}

QString ProcessMgr::GetCompanyName(QString filePath)
{
	QByteArray cb;
	DWORD reserved1;
	DWORD reserved2 = NULL;
	std::wstring pBlock;
	
	
	DWORD versize = GetFileVersionInfoSizeW(filePath.toStdWString().data(), &reserved1);

	if (versize == 0) {
		return QString();
	}
	pBlock.resize(versize);
	memset((void*)pBlock.c_str(), 0, versize * 2);
	if (!GetFileVersionInfoW(filePath.toStdWString().data(), reserved2, versize, (LPVOID)pBlock.c_str())) {
		return QString();
	}

	struct LANGANDCODEPAGE {
		WORD language;
		WORD codepage;
	}*lpTranslate;
	UINT size = 0;
	WCHAR lpSubBlock[MAX_PATH] = { 0 };


	if (!VerQueryValueW(pBlock.c_str(), L"\\VarFileInfo\\Translation", (LPVOID*)&lpTranslate, &size)) {
		return QString();
	}
	_snwprintf_s(lpSubBlock, MAX_PATH - 1, L"\\StringFileInfo\\%04X%04X\\%s",
		lpTranslate->language, lpTranslate->codepage, L"CompanyName");
	
	LPVOID buf = NULL;
	if (VerQueryValueW(pBlock.data(), lpSubBlock, (LPVOID*)&buf, &size)) {
		if (buf != NULL) {
			return QString::fromWCharArray((PWCHAR)buf);
		}
	}

	return QString();
}

int ProcessMgr::ProcessProcInfo(StuProcInfo * procInfo, QVector<QProcInfo> &vprocInfo)
{
	int procCnt;
	int denyCnt = 0;

	procCnt = procInfo->ProcessCnt;

	for (int i = 0; i < procCnt; i++,procInfo++)
	{
		QProcInfo temp;
		int pos = 0;
		QString path; 

		if (procInfo->ProcessId == 0) {
			temp.ProcName = "Idle";
			temp.ProcPath = "Idle";
		}
		else if(procInfo->ProcessId == 4){
			temp.ProcName = "System";
			temp.ProcPath = "System";
		}
		else {
			path = QString::fromWCharArray(procInfo->wStrProcessPath);
			pos = path.lastIndexOf("\\");
			temp.ProcName = path.right(path.length() - pos - 1);
			if (procInfo->IsWoW64Process) {
				temp.ProcName += " *32";
			}
			temp.ProcPath = path;
			temp.CorpName = GetCompanyName(temp.ProcPath);
		}

		temp.ProcId = QString::number(procInfo->ProcessId);

		if (procInfo->ParentProcessId) {
			temp.ParentProcId = QString::number(procInfo->ParentProcessId);
		}
		else {
			temp.ParentProcId = "-";
		}

		temp.ProcAddr = QString::number(procInfo->Process,16).toUpper();

		bool isAccessble = IsAccessProcess(procInfo->ProcessId);
		if (isAccessble) {
			temp.Accessble = "-";
		}
		else {
			temp.Accessble = tr("deny");
			denyCnt++;
		}

		vprocInfo.push_back(temp);
	}


	return denyCnt;
}

int ProcessMgr::GetHideProcessCnt(StuProcInfo * procInfo)
{
	int totalProcess = procInfo->ProcessCnt;
	HANDLE hProcessSnap;
	int hideProcessCnt = 0;
	PROCESSENTRY32 pe32;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		qDebug() << (TEXT("CreateToolhelp32Snapshot (of processes)"));
		return(FALSE);
	}
	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		qDebug() << (TEXT("Process32First")); // show cause of failure
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return(FALSE);
	}



	int i = 1;
	while (Process32Next(hProcessSnap, &pe32))
	{
		i++;
	}

	CloseHandle(hProcessSnap);

	return totalProcess - i;
}

void ProcessMgr::SetContextMenu()
{


	mMenu.addAction(tr("refresh"), this,&ProcessMgr::OnRefresh);
	mMenu.addAction(tr("hide process"), this,&ProcessMgr::OnHideProcess);
	mMenu.addAction(tr("look process moudle"), this,&ProcessMgr::OnLookProcMod);
	mMenu.addAction(tr("look process handle"), this,&ProcessMgr::OnLookProcHandle);
	mMenu.addAction(tr("look process threads"), this,&ProcessMgr::OnLookProcThreads);
	mMenu.addAction(tr("terminate process"), this,&ProcessMgr::OnTerminateProcess);
	auto lookUp = mMenu.addMenu(tr("look up"));
	lookUp->addAction(tr("look process windows"), this, &ProcessMgr::OnLookProcWindows);


	setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	connect(this, &ProcessMgr::customContextMenuRequested, this, [=](const QPoint &pos)
		{
			mMenu.exec(QCursor::pos());

		}
	);

	
	
}

void ProcessMgr::OnRefresh()
{

	StuProcInfo *buffer = (StuProcInfo *)new char[SIZE4M];
	int denyCnt = 0;

	if (!buffer) {
		return;
	}
	ParamInfo param;
	param.pInData = 0;
	param.cbInData = 0;
	param.pOutData = (PCHAR)buffer;
	param.cbOutData = SIZE4M;
	param.FunIdx = DrvCall::ProcList;

	auto result = OpenArk::IoCallDriver(param);
	if (reset == false) {
		delete buffer;
		return;
	}
	int psCnt = buffer->ProcessCnt;
	//清除内容
	mSourceModel->removeRows(0, mSourceModel->rowCount());
	QVector<QProcInfo> vprocInfo;
	denyCnt = ProcessProcInfo(buffer, vprocInfo);//解析得到的数据转化为qstring
	int row = 0;
	for (auto procInfo : vprocInfo)
	{
		 QStandardItem *nameItem = new  QStandardItem(procInfo.ProcName);
		 if (/* procInfo.ProcId.toULong() != 0 && procInfo.ProcId.toULong() != 4*/procInfo.ProcId != "0" && procInfo.ProcId != "4") {
			 nameItem->setIcon(GetFileIcon(procInfo.ProcPath));
		 }
		 mSourceModel->setItem(row, PHI::Name, nameItem);
		 mSourceModel->setItem(row, PHI::Pid, new QStandardItem(procInfo.ProcId));
		 mSourceModel->setItem(row, PHI::PPid, new QStandardItem(procInfo.ParentProcId));
		 mSourceModel->setItem(row, PHI::Addr, new QStandardItem(procInfo.ProcAddr));
		 mSourceModel->setItem(row, PHI::Access, new QStandardItem(procInfo.Accessble));
		 mSourceModel->item(row, PHI::Access)->setTextAlignment(Qt::AlignHCenter);
		 mSourceModel->setItem(row, PHI::Path, new QStandardItem(procInfo.ProcPath));
		 mSourceModel->setItem(row, PHI::Corp, new QStandardItem(procInfo.CorpName));
		 row++;
	}

	mTableView->sortByColumn(PHI::Pid, Qt::SortOrder::AscendingOrder);
	mTableView->resizeColumnToContents(PHI::Addr);

	int hideProc = 0;
	int totalProc = vprocInfo.size();

	if (result) {
		int hideProc = GetHideProcessCnt(buffer);
	}
	

	QString msg = QString(tr("Processes: %1, Hidden Processes: %2, Ring3 Inaccessible Processes: %3")).arg(totalProc).arg(hideProc).arg(denyCnt);


	Ark::Instance->ShowMessage(msg);
	
}
void ProcessMgr::OnNouse()
{

}

void ProcessMgr::OnHideProcess()
{
	 auto index = ui.tableView->currentIndex();
	 int row = index.row();
	 QModelIndex idIndex = mSortModel->index(row, PHI::Pid);
	 auto procId = mSortModel->data(idIndex).toLongLong();

	 ParamInfo param;
	 param.pInData = (PCHAR)&procId;
	 param.cbInData = 8;
	 param.pOutData = 0;
	 param.cbOutData = 0;
	 param.FunIdx = DrvCall::HideProcess;

	 auto result = OpenArk::IoCallDriver(param);
	 qDebug() << "hide" << result;

}

void ProcessMgr::OnLookProcMod()
{
	static ModuleView *modView = 0;


	auto index = ui.tableView->currentIndex();
	int row = index.row();
	QModelIndex idIndex = mSortModel->index(row, PHI::Pid);
	auto procId = mSortModel->data(idIndex).toLongLong();

	if (!modView)
	{
		modView = new ModuleView(this, (LPVOID)procId, mSortModel->data(mSortModel->index(row, PHI::Name)).toString());
	}
	else
	{
		modView->mProcId = (LPVOID)procId;
		modView->mProcName = mSortModel->data(mSortModel->index(row, PHI::Name)).toString();
		modView->OnRefresh();
	}
	modView->show();
	
	
}

void ProcessMgr::OnLookProcHandle()
{
	static HandleView *handleView = 0;

	auto Idindex = GetIndexForCurRowCol(Col::Pid);
	auto nameIndex = GetIndexForCurRowCol(Col::Name);
	QString procName;
	auto procId = mSortModel->data(Idindex).toLongLong();
	procName = mSortModel->data(nameIndex).toString();


	if (!handleView)
	{
		handleView = new HandleView(this, (LPVOID)procId, procName);
	}
	else
	{
		handleView->mProcId = (LPVOID)procId;
		handleView->mProcName = procName;
		handleView->OnRefresh();
	}
	handleView->show();
	
	
}

void ProcessMgr::OnLookProcThreads()
{
	static ThreadView *threadView = 0;
	auto Idindex = GetIndexForCurRowCol(Col::Pid);
	auto nameIndex = GetIndexForCurRowCol(Col::Name);
	QString procName;
	auto procId = mSortModel->data(Idindex).toLongLong();
	procName = mSortModel->data(nameIndex).toString();

	if (!threadView)
	{
		threadView = new ThreadView(this, (LPVOID)procId, procName);
	}
	else
	{
		threadView->mProcId = (LPVOID)procId;
		threadView->mProcName = procName;
		threadView->OnRefresh();
	}
	threadView->show();

}

void ProcessMgr::OnLookProcWindows()
{
	static WindowView *windowView = 0;
	QString procName;

	auto procId = GetColDataFromInt(Col::Pid);
	procName = GetColDataFromString(Col::Name);

	if (!windowView)
	{
		windowView = new WindowView(this, (LPVOID)procId, procName);
	}
	else
	{
		windowView->mProcId = (LPVOID)procId;
		windowView->mProcName = procName;
		windowView->OnRefresh();
	}
	windowView->show();
}

void ProcessMgr::OnTerminateProcess()
{
	QString procName;
	auto procId = GetColDataFromInt(Col::Pid);
	
	

	ParamInfo param;
	param.pInData = (PCHAR)procId;
	param.cbInData = 8;
	param.pOutData = 0;
	param.cbOutData = 0;
	param.FunIdx = DrvCall::CallIdxTerminate;

	auto result = OpenArk::IoCallDriver(param);
}

void ProcessMgr::OnPrintTest()
{
	/*ParamInfo param;
	param.pInData = 0;
	param.cbInData = 0;
	param.pOutData = (PCHAR)0;
	param.cbOutData = 0;
	param.FunIdx = DrvCall::TestHook;

	auto result = OpenArk::IoCallDriver(param);*/


	
}

void ProcessMgr::InitProcessView()
{

	mSourceModel->setHorizontalHeaderLabels(QStringList() << tr("Process") << tr("PID") << tr("PPID") << tr("Path") << tr("Eprocess") << tr("Access") << tr("Company Name"));
	mSortModel->AddColSortFun(MySortModel::SortBy::AsText);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsInt);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsInt);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsText);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsHex);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsText);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsText);

	//设置宽度
	mTableView->setColumnWidth(PHI::Name, 150);
	mTableView->setColumnWidth(PHI::Pid, 50);
	mTableView->setColumnWidth(PHI::PPid, 60);
	SetContextMenu();
	

}

