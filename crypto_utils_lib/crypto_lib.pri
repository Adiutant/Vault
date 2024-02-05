TARGET = crypto_lib
TEMPLATE = lib
CONFIG += staticlib
DEFINES += CRYPTOLIB_LIBRARY
SOURCES += $${PWD}/crypto_utils.cpp\
 $${PWD}/utils.cpp
HEADERS += $${PWD}/crypto_utils.h\
 $${PWD}/utils.h