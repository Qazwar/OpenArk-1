#include <QtWidgets/QApplication>
#include "openark.h"

#include "processmgr.h"

int OpenArkInit(int argc, char *argv[])
{
	//UNONE::SeEnableDebugPrivilege();
	//bool is_ark64 = UNONE::PeX64((CHAR*)GetModuleHandleW(NULL));
	/*if (!is_ark64 && UNONE::OsIs64()) {
		auto &&path = UNONE::PsGetProcessDirW() + L"\\OpenArk64.exe";
		if (UNONE::FsIsExistedW(path)) {
			UNONE::PsCreateProcessW(path);Z                                                            
			exit(0);
		}
	}*/

	
	
	
	

	
	

	return 0;
}



int main(int argc, char *argv[])
{

	QApplication app(argc, argv);
	app.setWindowIcon(QIcon(":/OpenArk/OpenArk.ico"));
	//QApplication::setFont(QFont(QString::fromLocal8Bit("Î¢ÈíÑÅºÚ")));
	QTranslator translator;
	translator.load(":/OpenArk/openark_zh.qm");
	app.installTranslator(&translator);

	OpenArk w;
	w.show();
	app.exec();


	

	return 0;
}
