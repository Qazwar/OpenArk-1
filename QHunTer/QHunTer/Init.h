#pragma once
#include "ntkrnl.h"

BOOLEAN InitSysCallTable();//���ñ�

BOOLEAN InitVarible();
BOOLEAN CheckVersion();

BOOLEAN InitNtInfo();
BOOLEAN InitExportByNtkrnl();

BOOLEAN InitDriver();
BOOLEAN InitExportByNtkrnl();
BOOLEAN InitUnExportByNtkrnl();
BOOLEAN InitOffset();

//��ʼ��δ��������
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
		