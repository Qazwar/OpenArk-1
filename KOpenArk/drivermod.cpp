#include "kark.h"



drivermod::drivermod()
{
}


drivermod::~drivermod()
{
}

BOOLEAN ArkGetDriverMods(PVOID, PVOID, ArkDriverModInfo * driverModInfo,PVOID )
{

	/*PLDR_DATA_TABLE_ENTRY nextMod = (PLDR_DATA_TABLE_ENTRY)NT::PsLoadedModuleList->Flink;
	int numOfMod = 0;
	

	for (int i = 0; (PLDR_DATA_TABLE_ENTRY)NT::PsLoadedModuleList != nextMod;
		nextMod = (PLDR_DATA_TABLE_ENTRY)nextMod->InLoadOrderLinks.Flink,
		i++
		)
	{
		PGROUP_AFFINITY
		driverModInfo->Mods[i].RegionBase = nextMod->DllBase;
		driverModInfo->Mods[i].RegionSize = nextMod->SizeOfImage;
		driverModInfo->Mods[i].LoadOrder = i;

		if(nextMod.dri)
		memcpy(ModInfo->Path, nextMod->FullDllName.Buffer, nextMod->FullDllName.Length);
	}

	tempModInfo->NumberOfMods = numOfMod;
	return true;*/



	return BOOLEAN();
}
