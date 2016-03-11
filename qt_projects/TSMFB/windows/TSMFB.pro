#-------------------------------------------------
#
# Project created by QtCreator 2011-11-22T16:22:22
#
#-------------------------------------------------

QT          += core gui

TARGET       = TSMFB
TEMPLATE     = app

include(TSMFB.prf)

SOURCES     += main.cpp\
    plot.cpp \
    fir_filter.cpp \
    cool_tsmfb.cpp \
    dec_detail.cpp \
    lbl_scheme.cpp \
    rec_detail.cpp \
    dialog_about.cpp

HEADERS     += \
    plot.h \
    fir_filter.h \
    fir_const.h \
    fir_coef.h \
    cool_tsmfb.h \
    dec_detail.h \
    lbl_scheme.h \
    rec_detail.h \
    dialog_about.h

RESOURCES    += resource.qrc

RC_FILE       = icon_resource.rc

TRANSLATIONS += cool_tsmfb_ru.ts

unix {
}
win32 {
    LIBS     += -L"E:/program_files/qwt-6.0.1/lib" -lqwt
}

OTHER_FILES  += icon_resource.rc

FORMS += \
    dialog_about.ui








































































