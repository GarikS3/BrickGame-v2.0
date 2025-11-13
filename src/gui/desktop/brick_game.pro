QT += core gui widgets

CONFIG += c++20
TARGET = brickgame_desktop
TEMPLATE = app

SOURCES += \
    main.cpp \
    menu_window.cpp \
    snake_window.cpp \
    tetris_window.cpp \
    ../../brick_game/tetris/backend.c \
    ../../brick_game/tetris/control.c \
    ../../brick_game/tetris/figures.c \
    ../../brick_game/tetris/utils.c

HEADERS += \
    menu_window.h \
    snake_window.h \
    tetris_window.h \
    ../../brick_game/tetris/backend.h


SOURCES += \
    ../../brick_game/snake/model/model.cpp \
    ../../brick_game/snake/controller/controller.cpp \

HEADERS += \
    ../../brick_game/snake/model/model.h \
    ../../brick_game/snake/controller/controller.h


INCLUDEPATH += ../../..
DEPENDPATH += ../../..

target.path = $$HOME/brickgame
INSTALLS += target