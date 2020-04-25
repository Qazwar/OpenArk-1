#pragma once
class drivermod
{
public:
	drivermod();
	~drivermod();
};


BOOLEAN ArkGetDriverMods(PVOID,PVOID,PVOID,  ArkDriverModInfo* driverModInfo);