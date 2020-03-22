#include "objdirectory.h"
#include "openark.h"
#include "processmgr.h"
#include "common.h"


namespace Ark {


	

	 HANDLE Device;
	 OpenArk* Instance;

};

OpenArk::OpenArk(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//tab
	auto CreateTabPage = [&](QWidget *widget, QWidget *origin,QTabWidget *tabwidget) {
		
		int idx = tabwidget->indexOf(origin);
		QString text = tabwidget->tabText(idx);
		tabwidget->removeTab(idx);
		tabwidget->insertTab(idx, widget, text);
	};

	//����tabҳ
	auto processmgr = new ProcessMgr(this);
	CreateTabPage(processmgr, ui.tabProcess,ui.tabWidget); 
	auto objView = new ObjectView(this);
	CreateTabPage(objView, ui.tabObjDir,ui.tabWidget);
	
	
	//��ʼ����Ա
	Ark::Instance = this;

	//�����źŲ�
	connect(ui.tabWidget, &QTabWidget::currentChanged, this,&OpenArk::OnTabChanged);

	//SeEnablePrivilege();
	//��������
	InitTargetDev();

	//��ʾ��ͼ
	OnTabChanged(false);
}

void OpenArk::OnTabChanged(bool checked)
{
	auto result = QMetaObject::invokeMethod(ui.tabWidget->currentWidget(), "OnRefresh"); 
	

}

bool OpenArk::InitTargetDev()
{
	bool result = 1;

	result =  SeEnablePrivilege();
	if (!result)  return false;

	//�򿪷�����ƹ�����
	SC_HANDLE scMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (scMgr == NULL){
		return false;
	}
	auto curDir =  QDir::current().absolutePath();
	
	QFile f(":/OpenArk/QHunTer.sys");
	f.open(QIODevice::ReadOnly);
	f.copy(QString( SERVICE_ANME )+ ".sys");
	QFileInfo info(QString(SERVICE_ANME) + ".sys");

	//������������Ӧ�ķ���
	SC_HANDLE scService = CreateServiceA(scMgr,
		SERVICE_ANME, //�����������ע����е�����  
		SERVICE_ANME, // ע������������ DisplayName ֵ  
		SERVICE_ALL_ACCESS, // ������������ķ���Ȩ��  
		SERVICE_KERNEL_DRIVER,// ��ʾ���صķ�������������  
		SERVICE_DEMAND_START, // ע������������ Start ֵ  
		SERVICE_ERROR_IGNORE, // ע������������ ErrorControl ֵ  
		info.absoluteFilePath().toLocal8Bit(), // ע������������ ImagePath ֵ  
		NULL,
		NULL,
		NULL,
		NULL,
		NULL);

	
	if (scService == NULL) {
		// ���������Ѿ���װ��ֻ��Ҫ��  
		DWORD dwErrNumber = GetLastError();
		if (dwErrNumber == ERROR_IO_PENDING || dwErrNumber == ERROR_SERVICE_EXISTS) {

			scService = OpenServiceA(scMgr, SERVICE_ANME, SERVICE_ALL_ACCESS);
		}else {
			return false;
		}
	}


	//�����������
	 result = StartServiceA(scService, NULL, NULL);

	//��ȡ�豸
	
	Ark::Device = CreateFileA(
		WIN32_LINK_NAME,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (Ark::Device == INVALID_HANDLE_VALUE) {
		result = false;
	}
	
	f.close();
	QFile::remove(SERVICE_ANME);//ɾ���ļ�

	return result;
}



void OpenArk::UnLoadTargetDev()
{
}

bool OpenArk::IoCallDriver(ParamInfo param)
{

	ULONG returnSize;

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

OpenArk::~OpenArk()
{
	UnLoadTargetDev();
}


