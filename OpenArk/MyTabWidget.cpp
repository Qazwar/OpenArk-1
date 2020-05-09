#include "MyTabWidget.h"
#include "arknamespace.h"


MyTabWidget::MyTabWidget(QWidget * parent):QTabWidget(parent)
{
	connect(this, &MyTabWidget::currentChanged, this, &MyTabWidget::OnTabChanged);
}


MyTabWidget::~MyTabWidget()
{
}


void MyTabWidget::OnTabChanged(bool checked)
{

	if (Ark::Instance)
	{
		Ark::Instance->ShowMessage(QString());

	}
	auto result = QMetaObject::invokeMethod(this->currentWidget(), "OnRefresh");


}