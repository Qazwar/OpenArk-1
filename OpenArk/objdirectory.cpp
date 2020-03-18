#include "objdirectory.h"
#include "common.h"
#include "openark.h"
#include <qdebug.h>
ObjectView::ObjectView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	

	


	//比例
	ui.splitter->setStretchFactor(0, 3);
	ui.splitter->setStretchFactor(1, 7);



	//信号槽连接
	connect(ui.treeWidget, &QTreeWidget::currentItemChanged, this, &ObjectView::UpdataObTable);
}

ObjectView::~ObjectView()
{


}


 void ObjectView::SetTreeItemRecurSion(PObInfo pObInfo,QTreeWidgetItem *parent)
{
	try
	{
		PObInfo pDirInfo = pObInfo;
		auto pCurDirItem = new QTreeWidgetItem(parent);//添加子目录
		pCurDirItem->setText(0, QString::fromWCharArray(pDirInfo->ObjName));//设置目录名
		QList<ObInfo> *obList = new QList<ObInfo>;
		mObListList.push_back(obList);
		pObInfo++;

		for (size_t i = 0; i < pDirInfo->SubItems; i++)
		{
			if (pObInfo->IsDirectory){
				SetTreeItemRecurSion(pObInfo, pCurDirItem);
			}
			else {
				obList->push_back(ObInfo(*pObInfo));
			}
			pObInfo++;
		}
		pCurDirItem->setData(0, Qt::UserRole, (ULONG_PTR)obList);
	}
	catch (const std::exception&)
	{
		qDebug() << __FILE__ << __LINE__;
	}
	
}

void ObjectView::OnRefresh()
{
	try
	{

		auto pBuf = new char[SIZE4M];

		if (!pBuf){
			qDebug() << __FILE__ << __LINE__;
			return;
		}
		ParamInfo temp;
		temp.pInData = 0;
		temp.cbInData = 0;
		temp.pOutData = pBuf;
		temp.cbOutData = SIZE4M;
		temp.FunIdx = SYSCALL::ObjDirectory;

		 auto result =  OpenArk::IoCallDriver(temp);

	 
		 //释放树部件中残存的数据信息
		 for (size_t i = 0; i < mObListList.count(); i++)
		 {
			delete mObListList.back();
			mObListList.pop_back();
		 }

		 if (result) {
			 SetTreeItemRecurSion((PObInfo)pBuf, ui.treeWidget->topLevelItem(0));
		 }


		 //释放缓冲
		 delete pBuf;
	}
	catch (const std::exception&)
	{
		qDebug() << "void ObjectView::OnRefresh()";
	}

}

void ObjectView::UpdataObTable(QTreeWidgetItem * current, QTreeWidgetItem * previous)
{
	QList<ObInfo> *obList = (QList<ObInfo>*)current->data(0, Qt::UserRole).toULongLong();

	ui.tableWidget->setRowCount(0);
	int i = 0;
	
	for (ObInfo ob : *obList) 
	{
		ui.tableWidget->insertRow(i++);
		ui.tableWidget->setItem(ui.tableWidget->currentRow(), Name, new QTableWidgetItem(QString::fromWCharArray(ob.ObjName)));
		ui.tableWidget->setItem(ui.tableWidget->currentRow(), Type, new QTableWidgetItem(QString::fromWCharArray(ob.TypeName)));
		ui.tableWidget->setItem(ui.tableWidget->currentRow(), LinkSym, new QTableWidgetItem(QString::fromWCharArray(ob.SymLinkName)));
	}


}


