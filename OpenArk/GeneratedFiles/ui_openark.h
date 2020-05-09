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
#include <mytabwidget.h>

QT_BEGIN_NAMESPACE

class Ui_OpenArkClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tabProcess;
    QWidget *tabDriverMod;
    QWidget *tabKernel;
    QGridLayout *gridLayout_4;
    MyTabWidget *tabWidgetKernel;
    QWidget *tabSysCallbak;
    QWidget *tabGdt;
    QGridLayout *gridLayout_5;
    QWidget *tabKernelHook;
    QGridLayout *gridLayout_3;
    MyTabWidget *tabWidgetKernelHook;
    QWidget *tabSSDT;
    QWidget *tabShadowSSDT;
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
        tabDriverMod = new QWidget();
        tabDriverMod->setObjectName(QString::fromUtf8("tabDriverMod"));
        tabWidget->addTab(tabDriverMod, QString());
        tabKernel = new QWidget();
        tabKernel->setObjectName(QString::fromUtf8("tabKernel"));
        gridLayout_4 = new QGridLayout(tabKernel);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        tabWidgetKernel = new MyTabWidget(tabKernel);
        tabWidgetKernel->setObjectName(QString::fromUtf8("tabWidgetKernel"));
        tabSysCallbak = new QWidget();
        tabSysCallbak->setObjectName(QString::fromUtf8("tabSysCallbak"));
        tabWidgetKernel->addTab(tabSysCallbak, QString());
        tabGdt = new QWidget();
        tabGdt->setObjectName(QString::fromUtf8("tabGdt"));
        gridLayout_5 = new QGridLayout(tabGdt);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        tabWidgetKernel->addTab(tabGdt, QString());

        gridLayout_4->addWidget(tabWidgetKernel, 0, 0, 1, 1);

        tabWidget->addTab(tabKernel, QString());
        tabKernelHook = new QWidget();
        tabKernelHook->setObjectName(QString::fromUtf8("tabKernelHook"));
        gridLayout_3 = new QGridLayout(tabKernelHook);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        tabWidgetKernelHook = new MyTabWidget(tabKernelHook);
        tabWidgetKernelHook->setObjectName(QString::fromUtf8("tabWidgetKernelHook"));
        tabSSDT = new QWidget();
        tabSSDT->setObjectName(QString::fromUtf8("tabSSDT"));
        tabWidgetKernelHook->addTab(tabSSDT, QString());
        tabShadowSSDT = new QWidget();
        tabShadowSSDT->setObjectName(QString::fromUtf8("tabShadowSSDT"));
        tabWidgetKernelHook->addTab(tabShadowSSDT, QString());

        gridLayout_3->addWidget(tabWidgetKernelHook, 0, 0, 1, 1);

        tabWidget->addTab(tabKernelHook, QString());
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

        tabWidget->setCurrentIndex(3);
        tabWidgetKernel->setCurrentIndex(0);
        tabWidgetKernelHook->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(OpenArkClass);
    } // setupUi

    void retranslateUi(QMainWindow *OpenArkClass)
    {
        OpenArkClass->setWindowTitle(QApplication::translate("OpenArkClass", "OpenArk", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabProcess), QApplication::translate("OpenArkClass", "Process", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabDriverMod), QApplication::translate("OpenArkClass", "\351\251\261\345\212\250\346\250\241\345\235\227", nullptr));
        tabWidgetKernel->setTabText(tabWidgetKernel->indexOf(tabSysCallbak), QApplication::translate("OpenArkClass", "\347\263\273\347\273\237\345\233\236\350\260\203", nullptr));
        tabWidgetKernel->setTabText(tabWidgetKernel->indexOf(tabGdt), QApplication::translate("OpenArkClass", "Gdt", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabKernel), QApplication::translate("OpenArkClass", "\345\206\205\346\240\270", nullptr));
        tabWidgetKernelHook->setTabText(tabWidgetKernelHook->indexOf(tabSSDT), QApplication::translate("OpenArkClass", "SSDT", nullptr));
        tabWidgetKernelHook->setTabText(tabWidgetKernelHook->indexOf(tabShadowSSDT), QApplication::translate("OpenArkClass", "Shadow SSDT", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabKernelHook), QApplication::translate("OpenArkClass", "\345\206\205\346\240\270\351\222\251\345\255\220", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabObjDir), QApplication::translate("OpenArkClass", "ObjDirectory", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OpenArkClass: public Ui_OpenArkClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPENARK_H
