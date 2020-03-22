#include "objdirectory.h"
#include "common.h"
#include "openark.h"
#include <qdebug.h>


PObInfo ObpInfo;

ObjectView::ObjectView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	

	ui.tableWidget->setSortingEnabled(true);
	ui.treeWidget->header()->hide();//隐藏标题
	ui.tableWidget->setWordWrap(false);//禁止换行
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);//最后一行扩展
	ui.tableWidget->setFont(QFont(QString::fromLocal8Bit("微软雅黑")));
	ui.treeWidget->setFont(QFont(QString::fromLocal8Bit("微软雅黑")));
	ui.tableWidget->verticalHeader()->setDefaultSectionSize(10);//设置默认行高度
	ui.tableWidget->verticalHeader()->hide();

	//比例
	ui.splitter->setStretchFactor(0, 3);
	ui.splitter->setStretchFactor(1, 7);



	//信号槽连接
	connect(ui.treeWidget, &QTreeWidget::currentItemChanged, this, &ObjectView::UpdataObTable);
}

ObjectView::~ObjectView()
{


}


 void ObjectView::SetTreeItemRecurSion( QTreeWidgetItem *parent)
{
	try
	{
		PObInfo pDirInfo = ObpInfo;
		


		parent->setText(0, QString::fromWCharArray(pDirInfo->ObjName));//设置目录名
		QList<ObInfo> *obList = new QList<ObInfo>;
		mObListList.push_back(obList);
		ObpInfo++;

		for (size_t i = 0; i < pDirInfo->SubItems; i++)
		{
			if (ObpInfo->IsDirectory){
				QTreeWidgetItem *pSubDir;
				pSubDir = new QTreeWidgetItem(parent);
				SetTreeItemRecurSion(pSubDir);
			}
			else {
				obList->push_back(ObInfo(*ObpInfo));
				ObpInfo++;
			}
		}
		
		parent->setData(0, Qt::UserRole, (ULONG_PTR)obList);
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
		 //清空视图
		 ui.treeWidget->clear();
		 ui.tableWidget->setRowCount(0);

		 if (!ui.treeWidget->topLevelItem(0))
			 ui.treeWidget->addTopLevelItem(new QTreeWidgetItem);

		 if (result) {

			 ObpInfo = (PObInfo)pBuf;
			 SetTreeItemRecurSion(ui.treeWidget->topLevelItem(0));
			 ui.treeWidget->expandItem(ui.treeWidget->topLevelItem(0));
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
	if (current == nullptr)
		return;
	QList<ObInfo> *obList = (QList<ObInfo>*)current->data(0, Qt::UserRole).toULongLong();

	ui.tableWidget->setRowCount(0);
	int i = 0;
	if (obList == NULL)
		return;

	ui.tableWidget->setSortingEnabled(false);
	for (ObInfo ob : *obList) 
	{
		ui.tableWidget->insertRow(i);
		ui.tableWidget->setItem(i, Name, new QTableWidgetItem(QString::fromWCharArray(ob.ObjName)));
		ui.tableWidget->setItem(i, Type, new QTableWidgetItem(QString::fromWCharArray(ob.TypeName)));
		ui.tableWidget->setItem(i, LinkSym, new QTableWidgetItem(QString::fromWCharArray(ob.SymLinkName)));
		i++;
	}
	ui.tableWidget->setSortingEnabled(true);

}


