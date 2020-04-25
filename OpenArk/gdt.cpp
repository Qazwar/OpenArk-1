#include "gdt.h"
#include "common.h"
#include "arknamespace.h"
#include "qtwrapper.h"

GdtView::GdtView(QWidget *parent) :StdTable(parent)
{

	InitView();
}


GdtView::~GdtView()
{
}

void GdtView::InitView()
{
	
	mSourceModel->setHorizontalHeaderLabels(QStringList() << "Cpu" << "Sel" << "Base"
		<< "Limit" << "Type"<< "pl"
		 << "Size" << "Gran" << "pres"
		<<"Long" );
	mTableView->setSortingEnabled(false);
	
	mSortModel->mColSortFunctions.resize(10);

	for (int i = 0; i < 10; i++)
	{
		mSortModel->mColSortFunctions[i] = MySortModel::SortBy::AsText;
	}

	/*mSortModel->mColSortFunctions[Col::Base] = MySortModel::SortBy::AsHex;
	mSortModel->mColSortFunctions[Col::CpuSerial] = MySortModel::SortBy::AsInt;
	mSortModel->mColSortFunctions[Col::Dpl] = MySortModel::SortBy::AsInt;
	mSortModel->mColSortFunctions[Col::Grantity] = MySortModel::SortBy::AsText;
	mSortModel->mColSortFunctions[Col::Limit] = MySortModel::SortBy::AsHex;
	mSortModel->mColSortFunctions[Col::Selector] = MySortModel::SortBy::AsHex;
	mSortModel->mColSortFunctions[Col::Type] = MySortModel::SortBy::AsText;*/

	OnRefresh();
}

void GdtView::SetContextMenu()
{
	mMenu.addAction(tr("refresh"), this, &GdtView::OnRefresh);

	setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	connect(this, &ProcessMgr::customContextMenuRequested, this, [=](const QPoint &pos)
		{
			mMenu.exec(QCursor::pos());

		}
	);
}

void GdtView::OnRefresh()
{

	ArkGdtInfo *gdtInfo = (ArkGdtInfo *)Ark::Buffer;


	ParamInfo param;
	param.pOutData = (PCHAR)gdtInfo;
	param.cbOutData = SIZE4M;
	param.FunIdx = DrvCall::CallIdxGdtInfo;

	auto result = OpenArk::IoCallDriver(param);
	if (result == false) {
		return;
	}
	//清除内容
	mSourceModel->removeRows(0, mSourceModel->rowCount());

	int i = 0;
	int col = 0;
	int row = 0;
	for (int i = 0; gdtInfo->Size != 0; i++)
	{
		for (int j = 0; j < (gdtInfo->Size - 4) / 8; j++)
		{
			if (gdtInfo->Gdts[j].Bits.Pres)
			{//如果是有效的描述符
				col = 0;
				ArkBase base;
				base.BaseLow = gdtInfo->Gdts[j].BaseLow;
				base.BaseMid = gdtInfo->Gdts[j].Bits.BaseMid;
				base.BaseHi = gdtInfo->Gdts[j].Bits.BaseHi;
				ULONG_PTR limit;


				mSourceModel->setItem(row, col++, MakeItem(i));  //cpu序号
				mSourceModel->setItem(row, col++, MakeItem((PVOID)(j * 8)));  //选择子
				mSourceModel->setItem(row, col++, MakeItem((PVOID)*(DWORD*)&base));  //基质

				limit = gdtInfo->Gdts[j].LimitLow | (gdtInfo->Gdts[j].Bits.LimitHi << 16);
				if (gdtInfo->Gdts[j].Bits.L == 0)
				{//32位段
					if (gdtInfo->Gdts[j].Bits.Granularity)
					{//页粒度
						limit = limit *0x1000 + 0xfff;
					}
				}

				mSourceModel->setItem(row, col++, MakeItem((PVOID)limit));  //限制

				QString type;
				if (gdtInfo->Gdts[j].Bits.C && gdtInfo->Gdts[j].Bits.S)
				{//代码段
					type = "Code RE ";
					if (gdtInfo->Gdts[j].Bits.A)
					{
						type += "Ac";
					}
				}
				else if (!gdtInfo->Gdts[j].Bits.C && gdtInfo->Gdts[j].Bits.S)
				{//数据段
					type = "Data RW Ac";
				}
				else
				{
					type = "TSS32 ";
					if (gdtInfo->Gdts[j].Bits.R_W)
					{
						type += "Busy";
					}
					else
					{
						type += "avilable";
					}
				}
				mSourceModel->setItem(row, col++, MakeItem(type));//类型
				mSourceModel->setItem(row, col++, MakeItem(gdtInfo->Gdts[j].Bits.Dpl));//特权级
				if (gdtInfo->Gdts[j].Bits.Default_Big)
				{
					mSourceModel->setItem(row, col++, MakeItem("Bg"));//size
				}
				else
				{
					mSourceModel->setItem(row, col++, MakeItem("Nb"));//size
				}
				

				QString granularity = "By";
				if (gdtInfo->Gdts[j].Bits.Granularity)
				{
					granularity = "Pg";
				}
				
				mSourceModel->setItem(row, col++, MakeItem(granularity));//Granularity

				mSourceModel->setItem(row, col++, MakeItem("P"));//存在
				QString mode = "-";
				if (
					gdtInfo->Gdts[j].Bits.S &&
					gdtInfo->Gdts[j].Bits.C)
				{
					if(gdtInfo->Gdts[j].Bits.L == 1 &&
						gdtInfo->Gdts[j].Bits.Default_Big == 0 )
					{
						mode = "Lo";
					}
					else
					{
						mode = "Nl";
					}
				}
				mSourceModel->setItem(row++, col++, MakeItem(mode));//moshi
			}

		}



		gdtInfo = (ArkGdtInfo *)((ULONG_PTR)gdtInfo + gdtInfo->Size);
	}
}


void GdtView::OnNouse()
{


}
