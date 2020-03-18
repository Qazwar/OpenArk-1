/********************************************************************************
** Form generated from reading UI file 'openark.ui'
**
** Created by: Qt User Interface Compiler version 5.12.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPENARK_H
#define UI_OPENARK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OpenArkClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tabProcess;
    QWidget *tabObjDir;
    QGridLayout *gridLayout_2;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *OpenArkClass)
    {
        if (OpenArkClass->objectName().isEmpty())
            OpenArkClass->setObjectName(QString::fromUtf8("OpenArkClass"));
        OpenArkClass->resize(662, 463);
        centralWidget = new QWidget(OpenArkClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabProcess = new QWidget();
        tabProcess->setObjectName(QString::fromUtf8("tabProcess"));
        tabWidget->addTab(tabProcess, QString());
        tabObjDir = new QWidget();
        tabObjDir->setObjectName(QString::fromUtf8("tabObjDir"));
        gridLayout_2 = new QGridLayout(tabObjDir);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        tabWidget->addTab(tabObjDir, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        OpenArkClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(OpenArkClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        OpenArkClass->setStatusBar(statusBar);

        retranslateUi(OpenArkClass);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(OpenArkClass);
    } // setupUi

    void retranslateUi(QMainWindow *OpenArkClass)
    {
        OpenArkClass->setWindowTitle(QApplication::translate("OpenArkClass", "OpenArk", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabProcess), QApplication::translate("OpenArkClass", "Process", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabObjDir), QApplication::translate("OpenArkClass", "ObjDirectory", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OpenArkClass: public Ui_OpenArkClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPENARK_H
