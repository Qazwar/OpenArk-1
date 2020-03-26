#include "ModuleView.h"
#include "common.h"
#include "arknamespace.h"


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
	mSortModel->AddColSortFun(MySortModel::SortBy::AsInt);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsInt);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsText);

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
		QStandardItem *nameItem = new  QStandardItem( QString::fromWCharArray(modInfo[i].Path));
		mSourceModel->setItem(i, Col::Path, nameItem);
		QStandardItem *modbaseItem = new  QStandardItem(QString::number(modInfo->RegionBase,16));
		mSourceModel->setItem(i, Col::RegionBase, modbaseItem);
		QStandardItem *modsizeItem = new  QStandardItem(QString::number(modInfo->RegionSize,16));
		mSourceModel->setItem(i, Col::RegionSize, modsizeItem);
		numberOfmod++;
	}
	mTableView->sortByColumn(Col::RegionBase, Qt::SortOrder::AscendingOrder);
	mTableView->resizeColumnToContents(Col::RegionBase);


	QString title = QString(tr("[%1],Processes mods: %2")).arg(mProcName).arg(numberOfmod);
	this->setWindowTitle(title);
}

void ModuleView::OnNouse()
{
}
