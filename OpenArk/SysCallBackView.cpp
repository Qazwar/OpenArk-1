#include "SysCallBackView.h"
#include "ModuleView.h"
#include "DriverModView.h"
#include "common.h"
#include "arknamespace.h"
#include "qtwrapper.h"


SysCallBackView::SysCallBackView(QWidget *parent) :StdTable(parent)
{

	InitView();
}


SysCallBackView::~SysCallBackView()
{
}

void SysCallBackView::InitView()
{


	mSourceModel->setHorizontalHeaderLabels(QStringList() << tr("CallbackEntry")
		<< tr("CallbakType") << tr("ModPath")
		<< tr("Company") << tr("Note"));
	mSortModel->mColSortFunctions.resize(Col::LastCol);

	mSortModel->mColSortFunctions[Col::CallbackEntry] = MySortModel::SortBy::AsHex;
	mSortModel->mColSortFunctions[Col::CallbakType] = MySortModel::SortBy::AsText;
	mSortModel->mColSortFunctions[Col::ModPath] = MySortModel::SortBy::AsText;
	mSortModel->mColSortFunctions[Col::Company] = MySortModel::SortBy::AsText;
	mSortModel->mColSortFunctions[Col::Note] = MySortModel::SortBy::AsText;


	SetContextMenu();
	OnRefresh();
}

void SysCallBackView::SetContextMenu()
{
	mMenu.addAction(tr("refresh"), this, &SysCallBackView::OnRefresh);

	setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	connect(this, &DriverModView::customContextMenuRequested, this, [=](const QPoint &pos)
		{
			mMenu.exec(QCursor::pos());

		}
	);


}

void SysCallBackView::OnRefresh()
{
	ArkSysCallBack *callbackInfo = (ArkSysCallBack *)Ark::Buffer;


	ParamInfo param;
	param.pOutData = (PCHAR)callbackInfo;
	param.cbOutData = SIZE4M;
	param.FunIdx = DrvCall::CallIdxGetCallbackInfo;

	auto result = OpenArk::IoCallDriver(param);
	if (result == false) {
		return;
	}


	ArkModInfo *modInfo = ModuleView::GetSystemModInfo();




	//Çå³ýÄÚÈÝ
	mSourceModel->removeRows(0, mSourceModel->rowCount());

	int createProcessCnt = 0;
	int createThreadCnt = 0;
	int loadImageCnt = 0;
	int cmpCallbackCnt = 0;

	
	for (int i = 0; callbackInfo->CallBackRoutine; callbackInfo++,i++)
	{

		mSourceModel->setItem(i, Col::CallbackEntry, MakeItem(callbackInfo->CallBackRoutine));

		switch (callbackInfo->Type)
		{
		case ArkSysCallBack::CreateProcessType:
		{
			createProcessCnt++;
			mSourceModel->setItem(i, Col::CallbakType, MakeItem(("CreateProcess")));
			break;
		}
		case ArkSysCallBack::CreateThread:
		{
			createThreadCnt++;
			mSourceModel->setItem(i, Col::CallbakType, MakeItem(("CreateThread")));
			break;
		}
		case ArkSysCallBack::LoadImageType:
		{
			loadImageCnt++;
			mSourceModel->setItem(i, Col::CallbakType, MakeItem(("LoadImage")));
			break;
		}
		case ArkSysCallBack::CmpCallback:
		{
			cmpCallbackCnt++;
			mSourceModel->setItem(i, Col::CallbakType, MakeItem(("CmpCallback")));
			break;
		}
		default:
			break;
		}

		mSourceModel->setItem(i, Col::Note, MakeItem(callbackInfo->CallBackRecord));

		


		for (int j = 0; j < modInfo->NumberOfMods; j++)
		{
			if ((ULONG_PTR)callbackInfo->CallBackRoutine >= modInfo[j].RegionBase
				&& (ULONG_PTR)callbackInfo->CallBackRoutine < (modInfo[j].RegionBase + modInfo[j].RegionSize))
			{

				QString fileCompany;
				ArkGetCovertPath(modInfo[j].Path);
				mSourceModel->setItem(i, Col::ModPath, MakeItem(modInfo[j].Path));

				QFile file(QString::fromWCharArray(modInfo[j].Path));
				if (file.exists())
				{
					fileCompany = Ark::Ps->GetCompanyName(QString::fromWCharArray(modInfo[j].Path));
				}
				else
				{
					fileCompany = tr("file no present");
				}
				mSourceModel->setItem(i, Col::Company, MakeItem(fileCompany));

			}
		}

		

	}
	if(modInfo)
	{
		delete modInfo;
	}
	
	QString msg = QString(tr("createprocess  %1  thread %2   loadimage %3  reg %4"))
		.arg(createProcessCnt).arg(createThreadCnt).arg(loadImageCnt).arg(cmpCallbackCnt);
	Ark::Instance->ShowMessage(msg);

}



void SysCallBackView::OnNouse()
{
}




