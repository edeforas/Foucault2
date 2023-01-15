TEMPLATE = app
DESTDIR = ..
QT += widgets printsupport

CONFIG(debug, debug|release):TARGET = Foucault2_debug
CONFIG(release, debug|release):TARGET = Foucault2

win32:RC_FILE = Foucault2.rc
RESOURCES += Foucault2.qrc
SOURCES += main.cpp \
    Mirror.cpp \
    MirrorIo.cpp \
    MirrorComment.cpp \
    MirrorCouderMeasure.cpp \
    MirrorItem.cpp \
    TimelineScene.cpp \
    TaskItem.cpp \
    TaskItemComment.cpp \
    TaskItemCouderMeasure.cpp \
    TaskItemUnmaskedMeasure.cpp \
    DialogNewMeasure.cpp \
    DialogNewMirror.cpp \
    DialogNewComment.cpp \
    DialogNewUnmaskedMeasure.cpp \
    MainWindow.cpp \
    TaskItemNewMirror.cpp \
    MirrorWork.cpp \
    DialogNewWork.cpp \
	UnmaskedFoucaultImgSet.cpp \
    TaskItemWork.cpp

HEADERS += \
    Mirror.h \
    MirrorIo.h \
    MirrorItem.h \
    MirrorComment.h \
    MirrorCouderMeasure.h \
    MirrorUnmaskedMeasure.h \
    TimelineScene.h \
    TaskItem.h \
    TaskItemComment.h \
    TaskItemCouderMeasure.h \
    TaskItemUnmaskedMeasure.h \
    DialogNewMeasure.h \
    DialogNewMirror.h \
    DialogNewComment.h \
	DialogNewUnmaskedMeasure.h \
    MainWindow.h \
    TaskItemNewMirror.h \
    MirrorWork.h \
    DialogNewWork.h \
    TaskItemWork.h \
	UnmaskedFoucaultImgSet.h \
    Foucault2Defines.h

FORMS += mainwindow.ui \
    DialogNewMirror.ui \
    DialogNewMeasure.ui \
    DialogNewComment.ui \
	DialogNewUnmaskedMeasure.ui \
    DialogNewWork.ui
INCLUDEPATH += Util
SOURCES += Util/Properties.cpp
HEADERS += Util/Properties.h

TRANSLATIONS = Foucault2_fr.ts
