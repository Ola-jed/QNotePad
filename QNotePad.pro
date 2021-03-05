QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -Wall -Wextra -pedantic -O2
CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    about.cpp \
    main.cpp \
    notepad.cpp \
    popup.cpp \
    settings.cpp \
    style.cpp

HEADERS += \
    about.hpp \
    keywords.hpp \
    notepad.hpp \
    popup.hpp \
    settings.hpp \
    style.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    notepadressources.qrc
