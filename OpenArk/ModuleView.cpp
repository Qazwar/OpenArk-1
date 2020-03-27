#include "ModuleView.h"
#include "common.h"
#include "arknamespace.h"
#include "qtwrapper.h"

ModuleView::ModuleView(QWidget *parent, LPVOID procId, QString procName):StdDialog(parent)
{
	mProcId = procId;
	mProcName = procName;
	InitView();
}


ModuleView::~ModuleView()
{
}

void ModuleView::InitView()
{
	mSourceModel->setHorizontalHeaderLabels(QStringList() << tr("mod path") << tr("base addr") << tr("mod size") << tr("company"));
	mSortModel->AddColSortFun(MySortModel::SortBy::AsText);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsHex);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsHex);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsText);

	mTableView->setColumnWidth(Path,250);
	mTableView->setColumnWidth(Col::RegionBase,130);
	mTableView->setColumnWidth(Col::RegionSize,130);
	mTableView->setColumnWidth(Col::Company,150);
	
	this->resize(700,300);
	SetContextMenu();

	OnRefresh();

}

void ModuleView::SetContextMenu()
{
}

void ModuleView::OnRefresh()
{

	ModInfo *modInfo = (ModInfo *)Ark::Buffer;

	if (!modInfo) {
		return;
	}
	ParamInfo param;
	param.pInData = (PCHAR)&mProcId;
	param.cbInData = sizeof(mProcId);
	param.pOutData = (PCHAR)modInfo;
	param.cbOutData = SIZE4M;
	param.FunIdx = SYSCALL::ModList;

	auto result = OpenArk::IoCallDriver(param);
	if (reset == false) {
		return;
	}

	//Çå³ýÄÚÈÝ
	mSourceModel->removeRows(0, mSourceModel->rowCount());
	int numberOfmod = modInfo->NumberOfMods;
	for(int i = 0;i< numberOfmod;i++)
	{
		if (wcsicmp(modInfo[i].Path, L"C:\\Windows\\System32\\apisetschema.dll") == 0) 
		{
			modInfo++;
			numberOfmod--;
		}
		QStandardItem *nameItem = new  QStandardItem( QString::fromWCharArray(modInfo->Path));
		mSourceModel->setItem(i, Col::Path, nameItem);
		QStandardItem *modbaseItem = new  QStandardItem(ToHexQstring(modInfo->RegionBase));
		mSourceModel->setItem(i, Col::RegionBase, modbaseItem);
		QStandardItem *modsizeItem = new  QStandardItem(ToHexQstring(modInfo->RegionSize));
		mSourceModel->setItem(i, Col::RegionSize, modsizeItem);
		QStandardItem *companyItem = new QStandardItem(Ark::Ps->GetCompanyName(QString::fromWCharArray(modInfo->Path)));
		mSourceModel->setItem(i, Col::Company, companyItem);
		modInfo++;
	}
	mTableView->sortByColumn(Col::RegionBase, Qt::SortOrder::AscendingOrder);
	QString title = QString(tr("[%1],Processes mods: %2")).arg(mProcName).arg(numberOfmod);
	this->setWindowTitle(title);
}

void ModuleView::OnNouse()
{
}
