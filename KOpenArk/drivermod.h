#pragma once
class drivermod
{
public:
	drivermod();
	~drivermod();
};

//�õ�ϵͳģ��Ļ�����Ϣ
BOOLEAN ArkGetDriverModsInfo(
	PVOID,
	PVOID,  
	ArkDriverModInfo* driverModInfo,
	ULONG BufSize);




BOOLEAN ArkGetDriversInfo(
	PVOID,
	PVOID,
	ArkDriverModInfo* driverModInfo,
	ULONG BufSize);


//ͨ��ģ���·���ĵ���������
BOOLEAN ArkGetServiceName(PWCHAR ModPath, PWCHAR ReturnName);
