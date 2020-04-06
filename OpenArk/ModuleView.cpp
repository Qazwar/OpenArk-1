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
	mMenu.addAction(tr("refresh"), this, &ModuleView::OnRefresh);
	mMenu.addAction(tr("hide moudle"), this, &ModuleView::OnHideMod);

	setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	connect(this, &ProcessMgr::customContextMenuRequested, this, [=](const QPoint &pos)
		{
			mMenu.exec(QCursor::pos());

		}
	);


}

ArkModInfo * ModuleView::GetModInfo(PVOID ProcId)
{
	ArkModInfo *modInfo = (ArkModInfo *)new char[SIZE4M];

	if (!modInfo) {
		return 0;
	}
	ParamInfo param;
	param.pInData = (PCHAR)&ProcId;
	param.cbInData = sizeof(ProcId);
	param.pOutData = (PCHAR)modInfo;
	param.cbOutData = SIZE4M;
	param.FunIdx = DrvCall::ModList;

	auto result = OpenArk::IoCallDriver(param);
	if (result == false) {
		delete modInfo;
		return 0;
	}
	return modInfo;
}

void ModuleView::OnRefresh()
{

	ArkModInfo *modInfo = (ArkModInfo *)Ark::Buffer;

	if (!modInfo) {
		return;
	}
	ParamInfo param;
	param.pInData = (PCHAR)&mProcId;
	param.cbInData = sizeof(mProcId);
	param.pOutData = (PCHAR)modInfo;
	param.cbOutData = SIZE4M;
	param.FunIdx = DrvCall::ModList;

	auto result = OpenArk::IoCallDriver(param);
	if (result == false) {
		return;
	}

	//Çå³ýÄÚÈÝ
	mSourceModel->removeRows(0, mSourceModel->rowCount());
	int numberOfmod = modInfo->NumberOfMods;
	for(int i = 0;i< numberOfmod;i++)
	{
		if (wcsicmp(modInfo->Path, L"C:\\Windows\\System32\\apisetschema.dll") == 0) 
		{
			modInfo++;
			numberOfmod--;
			if (i >= numberOfmod)
				break;
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


void ModuleView::OnHideMod()
{
	auto index = GetIndexForCurRowCol(Col::RegionBase);
	

	HideModParam *pIndata = (HideModParam *)Ark::Buffer;
	pIndata->Id = mProcId;
	pIndata->ModBase = index.data().toString().toULongLong(0, 16);;
	
	ParamInfo param;
	param.pInData = (PCHAR)pIndata;
	param.cbInData = sizeof(HideModParam);
	param.pOutData = 0;
	param.cbOutData = 0;
	param.FunIdx = DrvCall::HideMod;

	auto result = OpenArk::IoCallDriver(param);
	if (reset == false) {
		qDebug() << "hide mode failed";
	}

}

void ModuleView::OnNouse()
{
}
