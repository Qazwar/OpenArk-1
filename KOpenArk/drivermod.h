#pragma once
class drivermod
{
public:
	drivermod();
	~drivermod();
};

//得到系统模块的基本信息
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


//通过模块的路径的到驱动名称
BOOLEAN ArkGetServiceName(PWCHAR ModPath, PWCHAR ReturnName);
