#include "DriverModView.h"
#include "common.h"
#include "arknamespace.h"
#include "qtwrapper.h"
#include "ModuleView.h"


DriverModView::DriverModView(QWidget *parent) :StdTable(parent)
{

	InitView();
}


DriverModView::~DriverModView()
{
}

void DriverModView::InitView()
{
	
	mSourceModel->setHorizontalHeaderLabels(QStringList() << tr("DriverName") 
		<< tr("BaseAddress") << tr("ImageSize")
		<< tr("DriverObject") << tr("DriverPath") << tr("ServiceName")
		<< tr("LoadOrder") <<  tr("FileCompany"));
	mSortModel->mColSortFunctions.resize(Col::LastCol);

	mSortModel->mColSortFunctions[Col::DriverName] = MySortModel::SortBy::AsText;
	mSortModel->mColSortFunctions[Col::BaseAddress] = MySortModel::SortBy::AsHex;
	mSortModel->mColSortFunctions[Col::ImageSize] = MySortModel::SortBy::AsHex;
	mSortModel->mColSortFunctions[Col::DriverObject] = MySortModel::SortBy::AsHex;
	mSortModel->mColSortFunctions[Col::DriverPath] = MySortModel::SortBy::AsText;
	mSortModel->mColSortFunctions[Col::ServiceName] = MySortModel::SortBy::AsText;
	mSortModel->mColSortFunctions[Col::LoadOrder] = MySortModel::SortBy::AsInt;
	mSortModel->mColSortFunctions[Col::FileCompany] = MySortModel::SortBy::AsText;

	SetContextMenu();
}

void DriverModView::SetContextMenu()
{
	mMenu.addAction(tr("refresh"), this, &DriverModView::OnRefresh);

	setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	connect(this, &DriverModView::customContextMenuRequested, this, [=](const QPoint &pos)
		{
			mMenu.exec(QCursor::pos());

		}
	);


}

void DriverModView::OnRefresh()
{
	ArkDriverModInfo *driverModInfo = (ArkDriverModInfo *)Ark::Buffer;


	ParamInfo param;
	param.pOutData = (PCHAR)driverModInfo;
	param.cbOutData = SIZE4M;
	param.FunIdx = DrvCall::CallIdxGetDriverInfo;

	auto result = OpenArk::IoCallDriver(param);
	if (result == false) {
		return;
	}


	//Çå³ýÄÚÈÝ
	mSourceModel->removeRows(0, mSourceModel->rowCount());



	int i = 0;
	for (; i < driverModInfo->NumberOfMods; i++)
	{
		mSourceModel->setItem(i, Col::BaseAddress, MakeItem(driverModInfo->Mods[i].RegionBase));
		mSourceModel->setItem(i, Col::DriverName, MakeItem(driverModInfo->Mods[i].DriverName));
		if (driverModInfo->Mods[i].DriverObject)
		{
			mSourceModel->setItem(i, Col::DriverObject, MakeItem(driverModInfo->Mods[i].DriverObject));

		}
		else
		{
			mSourceModel->setItem(i, Col::DriverObject, MakeItem("-"));
		}
		
		QString fileCompany;
		ArkGetCovertPath(driverModInfo->Mods[i].DriverPath);
		
		QFile file(QString::fromWCharArray(driverModInfo->Mods[i].DriverPath));
		if (file.exists())
		{
			fileCompany = Ark::Ps->GetCompanyName(QString::fromWCharArray(driverModInfo->Mods[i].DriverPath));
		}
		else
		{
			fileCompany = tr("file no present");
		}
		mSourceModel->setItem(i, Col::DriverPath, MakeItem(driverModInfo->Mods[i].DriverPath));
		mSourceModel->setItem(i, Col::FileCompany, MakeItem(fileCompany));
		mSourceModel->setItem(i, Col::ImageSize, MakeItem((PVOID)driverModInfo->Mods[i].RegionSize));
		mSourceModel->setItem(i, Col::LoadOrder, MakeItem(driverModInfo->Mods[i].LoadOrder));
		mSourceModel->setItem(i, Col::ServiceName, MakeItem(driverModInfo->Mods[i].ServiceName));
	}

	QString msg = QString(tr("driver %1")).arg(driverModInfo->NumberOfMods);


	Ark::Instance->ShowMessage(msg);
	
}



void DriverModView::OnNouse()
{
}




