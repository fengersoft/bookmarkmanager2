QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/api/controls/category/datacategory.cpp \
    src/api/controls/setvaluedialog.cpp \
    src/api/http/synchttp.cpp \
    src/api/http/webinfo.cpp \
    src/api/libqrencode/bitstream.c \
    src/api/libqrencode/mask.c \
    src/api/libqrencode/mmask.c \
    src/api/libqrencode/mqrspec.c \
    src/api/libqrencode/qrcodewidget.cpp \
    src/api/libqrencode/qrencode.c \
    src/api/libqrencode/qrinput.c \
    src/api/libqrencode/qrspec.c \
    src/api/libqrencode/rsecc.c \
    src/api/libqrencode/split.c \
    src/api/sql/mssqldao.cpp \
    src/api/sql/sqldef.cpp \
    src/api/sql/sqlitedao.cpp \
    src/api/sql/sqlitewrapper.cpp \
    src/bookmark.cpp \
    src/editurldialog.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/showqrcodewindow.cpp

HEADERS += \
    src/api/controls/category/datacategory.h \
    src/api/controls/setvaluedialog.h \
    src/api/http/synchttp.h \
    src/api/http/webinfo.h \
    src/api/libqrencode/bitstream.h \
    src/api/libqrencode/mask.h \
    src/api/libqrencode/mmask.h \
    src/api/libqrencode/mqrspec.h \
    src/api/libqrencode/qrcodewidget.h \
    src/api/libqrencode/qrencode.h \
    src/api/libqrencode/qrencode_inner.h \
    src/api/libqrencode/qrinput.h \
    src/api/libqrencode/qrspec.h \
    src/api/libqrencode/rsecc.h \
    src/api/libqrencode/split.h \
    src/api/sql/mssqldao.h \
    src/api/sql/sqldef.h \
    src/api/sql/sqlitedao.h \
    src/api/sql/sqlitewrapper.h \
    src/bookmark.h \
    src/editurldialog.h \
    src/mainwindow.h \
    src/showqrcodewindow.h

FORMS += \
    src/api/controls/category/datacategory.ui \
    src/api/controls/setvaluedialog.ui \
    src/bookmark.ui \
    src/editurldialog.ui \
    src/mainwindow.ui \
    src/showqrcodewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}
DESTDIR=$$PWD/bin
RC_ICONS=app.ico

RESOURCES += \
    app.qrc
