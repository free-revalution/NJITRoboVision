QT       += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console c++11

TARGET = NJITRoboVision
CONFIG -= qt
CONFIG -= app_bundle

TEMPLATE = app

#LIBS +=  -lpthread \
#         -lX11
#         -lgxiapi \



unix: PKGCONFIG += opencv
#INCLUDEPATH += /usr/local/include/opencv4 \
#               /usr/local/include/opencv4/opencv \
#               /usr/local/include/opencv4/opencv2

QMAKE_CFLAGS  =`pkg-config --cflags opencv`
LIBS += `pkg-config --libs opencv` -lMVSDK \
        -lpthread \
        -lX11
INCLUDEPATH = -I./include

# 编译生成的so文件（类似于windows下的dll文件）
LIBS += /usr/local/lib/libopencv_calib3d.so \
        /usr/local/lib/libopencv_core.so    \
        /usr/local/lib/libopencv_highgui.so \
        /usr/local/lib/libopencv_imgproc.so \
       /usr/local/lib/libopencv_imgcodecs.so\
        /usr/local/lib/libopencv_objdetect.so\
        /usr/local/lib/libopencv_photo.so \
        /usr/local/lib/libopencv_dnn.so \
        /usr/local/lib/libopencv_shape.so\
        /usr/local/lib/libopencv_features2d.so \
        /usr/local/lib/libopencv_stitching.so \
        /usr/local/lib/libopencv_flann.so\
        /usr/local/lib/libopencv_superres.so \
        /usr/local/lib/libopencv_videoio.so \
        /usr/local/lib/libopencv_video.so\
        /usr/local/lib/libopencv_videostab.so \
        /usr/local/lib/libopencv_ml.so

SOURCES += \
        AngleSolver/AngleSolver.cpp \
        Armor/ArmorBox.cpp \
        Armor/ArmorDetector.cpp \
        Armor/ArmorNumClassifier.cpp \
        Armor/findLights.cpp \
        Armor/LightBar.cpp \
        Armor/matchArmors.cpp \
        Camera/MVVideoCapture.cpp \
        General/General.cpp \
        Main/ArmorDetecting.cpp \
        Main/ImageUpdating.cpp \
        Main/main.cpp \
        Serial/Serial.cpp

HEADERS += \
    AngleSolver/AngleSolver.h \
    Armor/Armor.h \
    Camera/include/CameraApi.h \
    Camera/include/CameraDefine.h \
    Camera/include/CameraStatus.h \
    Camera/MVVideoCapture.h \
    General/General.h \
    Serial/Serial.h

DISTFILES += \
    General/setting_file/123svm.xml \
    General/setting_file/Camera752-hero1.xml \
    General/setting_file/Param_Other_XS.yml \
    General/setting_file/Camera_Params_JL.xml
