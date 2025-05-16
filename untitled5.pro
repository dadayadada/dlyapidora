QT       += core gui testlib
INCLUDEPATH += $$PWD/googletest/googletest/include
DEPENDPATH += $$PWD/googletest/googletest

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


CONFIG += c++17



SOURCES += \
    cursor.cpp \
    inputbtn.cpp \
    main.cpp \
    menu.cpp \
    minesweeper.cpp \
    playoption.cpp


HEADERS += \
    cursor.h \
    inputbtn.h \
    menu.h \
    minesweeper.h \
    playoption.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../../../Downloads/cur-Photoroom (1).png \
    ../../Изображения/Screenshots/cur.png \
    ../../Изображения/Screenshots/cur.png \
    ../../Изображения/Screenshots/Снимок экрана 2025-05-09 234110.png \
    ../../Изображения/Screenshots/Снимок экрана 2025-05-09 234110.png \
    mine.png \
    quicksave.ini \
    safeSaves.ini

RESOURCES +=


