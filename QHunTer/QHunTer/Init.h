#pragma once
#include "ntkrnl.h"

BOOLEAN InitSysCallTable();//调用表

BOOLEAN InitVarible();
BOOLEAN CheckVersion();

BOOLEAN InitNtInfo();
BOOLEAN InitExportByNtkrnl();

BOOLEAN InitDriver();
BOOLEAN InitExportByNtkrnl();
BOOLEAN InitUnExportByNtkrnl();
BOOLEAN InitOffset();

//初始化未导出变量
BOOLEAN InitPspCidTable();
BOOLEAN InitObTypeIndexTable();

 #define LOADEXPORT(name) \
    *((void**)&NT::##name)=PeGetProcRva(NT::ImageBaseCopyNt, #name) + NT::ImageBaseRunNt; \
    if(!NT::##name) \
    { \
        DbgPrint("Export %s could not be found!", #name); \
		return FALSE;		\
    }

#define LOADUNEXPORT(name)   if(!name())  return FALSE;
		