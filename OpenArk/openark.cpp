#include "objdirectory.h"
#include "openark.h"
#include "processmgr.h"
#include "common.h"
#include "SsdtView.h"
#include "ShadowSdtView.h"


#define ARKNAMESPACE

#include "arknamespace.h"




OpenArk::OpenArk(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//连接信号槽
	connect(ui.tabWidget, &QTabWidget::currentChanged, this, &OpenArk::OnTabChanged);
	connect(ui.tabWidgetKernelHook, &QTabWidget::currentChanged, this, &OpenArk::OnTabChanged);

	//初始化命名空间
	InitArkNameSpace();

	//SeEnablePrivilege();
	//加载驱动
	InitTargetDev();

	InitView();

	
	mInitSucess = 1;
	//显示视图
	OnTabChanged(false);
}

void OpenArk::OnTabChanged(bool checked)
{
	int index = ui.tabWidget->currentIndex();
	auto tabText = ui.tabWidget->tabText(index);
	QTabWidget *curTabWidget = 0;

	if (!mInitSucess) return;
	if (tabText.compare(tr("kernel hook"), Qt::CaseInsensitive) == 0)
	{
		curTabWidget = ui.tabWidgetKernelHook;
		auto dsd = curTabWidget->currentWidget();
		qDebug() << curTabWidget->currentWidget()->windowTitle();
		auto result = QMetaObject::invokeMethod(curTabWidget->currentWidget(), "OnRefresh");
		return;
	}
	auto result = QMetaObject::invokeMethod(ui.tabWidget->currentWidget(), "OnRefresh"); 
	

}

bool OpenArk::InitTargetDev()
{
	bool result = 1;

	result =  SeEnablePrivilege();
	if (!result)  return false;

	//打开服务控制管理器
	/*SC_HANDLE scMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (scMgr == NULL){
		return false;
	}
	auto curDir =  QDir::current().absolutePath();*/
	
	//QFile f(":/OpenArk/QHunTer.sys");
	//f.open(QIODevice::ReadOnly);
	//f.copy(QString( SERVICE_ANME )+ ".sys");
	//QFileInfo info(QString(SERVICE_ANME) + ".sys");

	////创建驱动所对应的服务
	//SC_HANDLE scService = CreateServiceA(scMgr,
	//	SERVICE_ANME, //驱动程序的在注册表中的名字  
	//	SERVICE_ANME, // 注册表驱动程序的 DisplayName 值  
	//	SERVICE_ALL_ACCESS, // 加载驱动程序的访问权限  
	//	SERVICE_KERNEL_DRIVER,// 表示加载的服务是驱动程序  
	//	SERVICE_DEMAND_START, // 注册表驱动程序的 Start 值  
	//	SERVICE_ERROR_IGNORE, // 注册表驱动程序的 ErrorControl 值  
	//	info.absoluteFilePath().toLocal8Bit(), // 注册表驱动程序的 ImagePath 值  
	//	NULL,
	//	NULL,
	//	NULL,
	//	NULL,
	//	NULL);

	//
	//if (scService == NULL) {
	//	// 驱动程序已经安装，只需要打开  
	//	qDebug() << "驱动程序已经安装，只需要打开  ";
	//	DWORD dwErrNumber = GetLastError();
	//	if (dwErrNumber == ERROR_IO_PENDING || dwErrNumber == ERROR_SERVICE_EXISTS) {
	//		scService = OpenServiceA(scMgr, SERVICE_ANME, SERVICE_ALL_ACCESS);
	//	}else {
	//		return false;
	//	}
	//}


	////开启此项服务
	// result = StartServiceA(scService, NULL, NULL);

	//获取设备
	
	Ark::Device = CreateFileA(
		WIN32_LINK_NAME,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (Ark::Device == INVALID_HANDLE_VALUE) {
		qDebug() << "INVALID_HANDLE_VALUE";
		result = false;
	}
	
	//f.close();
	//QFile::remove(SERVICE_ANME);//删除文件

	return result;
}

bool OpenArk::InitArkNameSpace()
{
	//初始化成员
	Ark::Instance = this;
	Ark::Buffer = new char[SIZE4M];
	if (Ark::Buffer) {
		return true;
	}
	exit(0);
}



void OpenArk::UnLoadTargetDev()
{
}

bool OpenArk::IoCallDriver(ParamInfo param)
{

	ULONG returnSize;

	RtlZeroMemory((LPVOID)param.pOutData, param.cbOutData);

	bool ret = DeviceIoControl(
		Ark::Device,
		IOCTL_HELLO_WORLD,
		&param,
		sizeof(param),
		NULL,
		0,
		&returnSize,
		NULL);
	

	return ret;
}

BOOLEAN OpenArk::SeEnablePrivilege()
{


		HANDLE hToken;
		LUID sedebugnameValue;
		TOKEN_PRIVILEGES tkp;
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		{
			return   FALSE;
		}
		if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
		{
			CloseHandle(hToken);
			return false;
		}
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Luid = sedebugnameValue;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
		{
			CloseHandle(hToken);
			return false;
		}
		return true;
}

void OpenArk::ShowMessage(QString text)
{
	ui.statusBar->showMessage(text, 0);
}

void OpenArk::InitView()
{
	

	//tab
	auto CreateTabPage = [&](QWidget *widget, QWidget *origin, QTabWidget *tabwidget) {

		int idx = tabwidget->indexOf(origin);
		QString text = tabwidget->tabText(idx);
		tabwidget->removeTab(idx);
		tabwidget->insertTab(idx, widget, text);
	};

	//创建tab页

	auto processmgr = new ProcessMgr(this);
	CreateTabPage(processmgr, ui.tabProcess, ui.tabWidget);
	auto ssdtView = new SsdtView;
	CreateTabPage(ssdtView, ui.tabSSDT, ui.tabWidgetKernelHook);
	auto objView = new ObjectView(this);
	CreateTabPage(objView, ui.tabObjDir, ui.tabWidget);
	auto shadowSdt = new ShadowSdtView(this);
	CreateTabPage(shadowSdt, ui.tabShadowSSDT, ui.tabWidgetKernelHook);

	ui.tabWidgetKernelHook->setCurrentIndex(0);
	ui.tabWidget->setCurrentIndex(0);
	
}

OpenArk::~OpenArk()
{
	UnLoadTargetDev();
}


