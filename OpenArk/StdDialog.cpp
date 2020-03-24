#include "StdDialog.h"



StdDialog::StdDialog(QWidget *parent):StdTable(parent)
{
	setWindowFlags(Qt::Dialog| Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
	this->setWindowModality(Qt::WindowModal);
	
}


StdDialog::~StdDialog()
{
}
