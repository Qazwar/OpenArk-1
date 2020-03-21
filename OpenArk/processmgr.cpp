#include "processmgr.h"
#include "common.h"
#include "openark.h"

#pragma comment (lib,"Version.lib")
ProcessMgr::ProcessMgr(QWidget *parent)
	: StdTable(parent)
{
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

void ProcessMgr::OnRefresh()
{

	StuProcInfo *curProcInfo = (StuProcInfo *)new char[SIZE4M];

	if (!curProcInfo) {
		return;
	}
	ParamInfo param;
	param.pInData = 0;
	param.cbInData = 0;
	param.pOutData = (PCHAR)curProcInfo;
	param.cbOutData = SIZE4M;
	param.FunIdx = SYSCALL::ProcList;

	auto result = OpenArk::IoCallDriver(param);
	if (reset == false) {
		delete curProcInfo;
		return;
	}
	int psCnt = curProcInfo->ProcessCnt;
	//Çå³ýÄÚÈÝ
	mSourceModel->removeRows(0, mSourceModel->rowCount());
	for (int i = 0; i < psCnt; i++, curProcInfo++)
	{
		QFileInfo fileInfo(QString::fromWCharArray(curProcInfo->wStrProcessPath));
		QFileIconProvider icon;

		 icon.icon(fileInfo);
		 mSourceModel->setItem(i, PHI::Name, new QStandardItem(fileInfo.fileName()));
		 mSourceModel->setItem(i, PHI::Pid, new QStandardItem(QString::number(curProcInfo->ProcessId)));
		 if (curProcInfo->ParentProcessId) {
			 mSourceModel->setItem(i, PHI::PPid, new QStandardItem(QString::number(curProcInfo->ParentProcessId)));
		 } else {
			 mSourceModel->setItem(i, PHI::PPid, new QStandardItem("-"));
		 }
		 mSourceModel->setItem(i, PHI::Addr, new QStandardItem(QString::number(curProcInfo->Process,16)));

		 bool isAccessble = IsAccessProcess(curProcInfo->ProcessId);
		 if (isAccessble) {
			 mSourceModel->setItem(i, PHI::Access, new QStandardItem(tr("-")));
		 }
		 else {
			 mSourceModel->setItem(i, PHI::Access, new QStandardItem(tr("deny")));
		 }

		 if (curProcInfo->ProcessId == 0 || curProcInfo->ProcessId == 4) {
			 continue;
		 }
		 mSourceModel->setItem(i, PHI::Path, new QStandardItem(fileInfo.absoluteFilePath()));

		 QString company = GetCompanyName(fileInfo.absoluteFilePath());
		 mSourceModel->setItem(i, PHI::Corp, new QStandardItem(company));

		
	}

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
	

}

