TEMPLATE = app
CONFIG += console
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11
QMAKE_LFLAGS = -std=c++11

CXXFLAGS += -std=c++11


# lib common
SOURCES += \
    login/main.cpp \
    login/login-packet-handler.cpp

HEADERS += \
    libs/common/network/network-engine.hpp \
    libs/common/network/network-client.hpp \
    libs/common/network/packet.hpp \
    libs/common/utils/log.hpp \
    login/login-packet-handler.hpp \
    libs/common/database/database-engine.h \
    libs/common/network/packet-handler-helpers.h

OTHER_FILES += \
    SConstruct \
    libs/common/SConscriptCommon \
    login/SConscriptLogin

INCLUDEPATH += thirdparty/boost_1_53_0
INCLUDEPATH += libs
