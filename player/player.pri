CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += $$PWD/src


win32 {
    INCLUDEPATH += $$PWD/lib/win64_msvc/ffmpeg/build_vs64/include
    INCLUDEPATH += $$PWD/lib/win64_msvc/SDL2-2.0.14/include

    contains(QT_ARCH, x86_64){
        message("64 bit")
        LIBS += -L$$PWD/lib/win64_msvc/ffmpeg/build_vs64/lib \
                -lavcodec \
                -lavdevice \
                -lavfilter \
                -lavformat \
                -lavutil \
                -lswresample \
                -lswscale

        LIBS += -L$$PWD/lib/win64_msvc/SDL2-2.0.14/x64 -lSDL2

    } else{
        message("32 bit, compiler by yoursel, thx!")
    }
}


SOURCES += \
    $$PWD/src/audio_decoder.cpp \
    $$PWD/src/decoder.cpp \
    $$PWD/src/video_decoder.cpp \
    $$PWD/src/video_render.cpp

HEADERS += \
    $$PWD/src/decoder.h \
    $$PWD/src/video_render.h

