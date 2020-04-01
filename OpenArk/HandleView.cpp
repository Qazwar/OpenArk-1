#include "HandleView.h"
#include "common.h"
#include "arknamespace.h"
#include "qtwrapper.h"

HandleView::HandleView(QWidget *parent, LPVOID procId, QString procName) :StdDialog(parent)
{
	mProcId = procId;
	mProcName = procName;
	InitView();
}


HandleView::~HandleView()
{
}

void HandleView::InitView()
{
	mSourceModel->setHorizontalHeaderLabels(QStringList() << tr("handle type name") << tr("handle name") << tr("handle") << tr("object") << tr("index")<<tr("refrece count"));
	mSortModel->AddColSortFun(MySortModel::SortBy::AsText);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsText);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsHex);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsHex);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsInt);
	mSortModel->AddColSortFun(MySortModel::SortBy::AsInt);

	

	SetContextMenu();

	OnRefresh();

}

void HandleView::SetContextMenu()
{
	mMenu.addAction(tr("refresh"), this, &HandleView::OnRefresh);

	setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	connect(this, &ProcessMgr::customContextMenuRequested, this, [=](const QPoint &pos)
		{
			mMenu.exec(QCursor::pos());

		}
	);


}

void HandleView::OnRefresh()
{

	ArkHandleInfo *handleInfo = (ArkHandleInfo *)Ark::Buffer;


	ParamInfo param;
	param.pInData = (PCHAR)&mProcId;
	param.cbInData = sizeof(mProcId);
	param.pOutData = (PCHAR)handleInfo;
	param.cbOutData = SIZE4M;
	param.FunIdx = DrvCall::ProcHandleList;

	auto result = OpenArk::IoCallDriver(param);
	if (reset == false)
	{
		return;
	}

	//Çå³ýÄÚÈÝ
	mSourceModel->removeRows(0, mSourceModel->rowCount());
	int handleCnt = handleInfo->HandleCnt;
	for (int i = 0; i < handleCnt; i++)
	{
		mSourceModel->setItem(i, Col::TypeName, MakeItem(handleInfo->TypeName));
		mSourceModel->setItem(i, Col::HandleName, MakeItem(handleInfo->HandleName));
		mSourceModel->setItem(i, Col::Handle, MakeItem(handleInfo->Handle));
		mSourceModel->setItem(i, Col::Object, MakeItem(handleInfo->Object));
		mSourceModel->setItem(i, Col::TypeIndex, MakeItem(handleInfo->TypeIndex));
		mSourceModel->setItem(i, Col::RefrenceCount, MakeItem(handleInfo->RefreceCount));
		handleInfo++;
	}
	
	QString title = QString(tr("[%1],Processes handles: (%2)")).arg(mProcName).arg(handleCnt);
	this->setWindowTitle(title);
}



void HandleView::OnNouse()
{
}
