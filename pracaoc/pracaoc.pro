# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src
# El destino es una aplicaci??n: ../bin/pracoc

FORMS += mainForm.ui \
           escalaForm.ui \
           filtroForm.ui \
           umbralForm.ui
HEADERS += pracaoc.h \
           imageprocess.h 
SOURCES += pracaoc.cpp \
           main.cpp \
           imageprocess.cpp 

TEMPLATE = app
QT += core gui opengl


QMAKE_CXXFLAGS_DEBUG += -fno-omit-frame-pointer
QMAKE_CXXFLAGS_RELEASE += -fno-omit-frame-pointer
QMAKE_CFLAGS_DEBUG += -fno-omit-frame-pointer
QMAKE_CFLAGS_RELEASE += -fno-omit-frame-pointer

QMAKE_LFLAGS += -no-pie


TARGET = pracAOC
