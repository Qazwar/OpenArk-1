# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = app
TARGET = OpenArk
DESTDIR = ./bin
CONFIG += debug
DEFINES += $(Qt_DEFINES_) \"$(INHERIT)\" OPENARK
LIBS += -L"$(Qt_LIBPATH_)" \
    -L"../../../../../$(INHERIT)" \
    -l$(Qt_LIBS_) \
    -l/"$(INHERIT)/" \
    -lunone-v140-static-debug64
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/$(ConfigurationName)
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(OpenArk.pri)
TRANSLATIONS += openark_zh.ts
