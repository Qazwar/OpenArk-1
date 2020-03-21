#include "processmgr.h"
#include "common.h"
#include "openark.h"
#include <qvector.h>

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

void ProcessMgr::ProcessProcInfo(StuProcInfo * procInfo, QVector<QProcInfo> &vprocInfo)
{
	int procCnt;

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
		}

		vprocInfo.push_back(temp);
	}
}

void ProcessMgr::OnRefresh()
{

	StuProcInfo *buffer = (StuProcInfo *)new char[SIZE4M];

	if (!buffer) {
		return;
	}
	ParamInfo param;
	param.pInData = 0;
	param.cbInData = 0;
	param.pOutData = (PCHAR)buffer;
	param.cbOutData = SIZE4M;
	param.FunIdx = SYSCALL::ProcList;

	auto result = OpenArk::IoCallDriver(param);
	if (reset == false) {
		delete buffer;
		return;
	}
	int psCnt = buffer->ProcessCnt;
	//清除内容
	mSourceModel->removeRows(0, mSourceModel->rowCount());
	QVector<QProcInfo> vprocInfo;
	ProcessProcInfo(buffer, vprocInfo);//解析得到的数据转化为qstring
	int row = 0;
	for (auto procInfo : vprocInfo)
	{
		 QStandardItem *nameItem = new  QStandardItem(procInfo.ProcName);
		 nameItem->setIcon(GetFileIcon(procInfo.ProcPath));
		 mSourceModel->setItem(row, PHI::Name, nameItem);
		 mSourceModel->setItem(row, PHI::Pid, new QStandardItem(procInfo.ProcId));
		 mSourceModel->setItem(row, PHI::PPid, new QStandardItem(procInfo.ParentProcId));
		 mSourceModel->setItem(row, PHI::Addr, new QStandardItem(procInfo.ProcAddr));
		 mSourceModel->setItem(row, PHI::Access, new QStandardItem(procInfo.Accessble));
		 mSourceModel->setItem(row, PHI::Path, new QStandardItem(procInfo.ProcPath));
		 mSourceModel->setItem(row, PHI::Corp, new QStandardItem(procInfo.CorpName));

		 row++;
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

