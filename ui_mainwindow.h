/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "video_render.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen_File;
    QAction *actionRtsp_Streaming;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    VideoRender *video_render;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_play;
    QPushButton *pushButton_pause;
    QPushButton *pushButton_stop;
    QSlider *horizontalSlider;
    QPushButton *pushButton_volume;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(783, 595);
        actionOpen_File = new QAction(MainWindow);
        actionOpen_File->setObjectName(QString::fromUtf8("actionOpen_File"));
        actionRtsp_Streaming = new QAction(MainWindow);
        actionRtsp_Streaming->setObjectName(QString::fromUtf8("actionRtsp_Streaming"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        video_render = new VideoRender(centralwidget);
        video_render->setObjectName(QString::fromUtf8("video_render"));
        video_render->setMinimumSize(QSize(765, 490));

        verticalLayout->addWidget(video_render);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_play = new QPushButton(centralwidget);
        pushButton_play->setObjectName(QString::fromUtf8("pushButton_play"));
        pushButton_play->setMinimumSize(QSize(36, 36));
        pushButton_play->setStyleSheet(QString::fromUtf8("QPushButton{ \n"
"image: url(:start.png);\n"
"border-radius:0px; \n"
"}  \n"
"QPushButton:hover{ \n"
"image: url(:start_hover.png);\n"
"border-radius:0px; \n"
"} \n"
""));

        horizontalLayout->addWidget(pushButton_play);

        pushButton_pause = new QPushButton(centralwidget);
        pushButton_pause->setObjectName(QString::fromUtf8("pushButton_pause"));
        pushButton_pause->setMinimumSize(QSize(36, 36));
        pushButton_pause->setStyleSheet(QString::fromUtf8("QPushButton{ \n"
"image: url(:pause.png);\n"
"border-radius:0px; \n"
"}  \n"
"QPushButton:hover{ \n"
"image: url(:pause_hover.png);\n"
"border-radius:0px; \n"
"} \n"
""));

        horizontalLayout->addWidget(pushButton_pause);

        pushButton_stop = new QPushButton(centralwidget);
        pushButton_stop->setObjectName(QString::fromUtf8("pushButton_stop"));
        pushButton_stop->setMinimumSize(QSize(36, 36));
        pushButton_stop->setStyleSheet(QString::fromUtf8("QPushButton{ \n"
"image: url(:stop.png);\n"
"border-radius:0px; \n"
"}  \n"
"QPushButton:hover{ \n"
"image: url(:stop_hover.png);\n"
"border-radius:0px; \n"
"} \n"
""));

        horizontalLayout->addWidget(pushButton_stop);

        horizontalSlider = new QSlider(centralwidget);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setStyleSheet(QString::fromUtf8("QSlider::groove:horizontal {\n"
"border: 0px solid #bbb;\n"
"}\n"
"\n"
"QSlider::sub-page:horizontal {\n"
"background-color: #ff9900;\n"
"border-bottom-left-radius: 2px; \n"
"border-top-left-radius: 2px; \n"
"\n"
"border-radius: 2px; \n"
"margin-top:5px;\n"
"margin-bottom:5px;\n"
"}\n"
"\n"
"QSlider::add-page:horizontal {\n"
"background-color: rgba(228, 228, 228, 50);v\n"
"border: 0px solid #777;\n"
"border-radius: 2px;\n"
"margin-top:5px;\n"
"margin-bottom:5px;\n"
"}\n"
"/*\n"
"QSlider::handle:horizontal {\n"
"background: rgb(255,153,102); \n"
"border: 1px solid rgb(255,153,102); \n"
"width: 14px;\n"
"height:10px;\n"
"border-radius: 7px;\n"
"margin-top:2px;\n"
"margin-bottom:2px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal:hover {\n"
"background: rgb(255,128,6); \n"
"border: 1px solid rgba(102,102,102,102);\n"
"border-radius: 7px;\n"
"}*/\n"
"\n"
"QSlider::sub-page:horizontal:disabled {\n"
"background: #bbb;\n"
"border-color: #999;\n"
"}\n"
"\n"
"QSlider::add-page:horizontal:disabled {\n"
"background: #eee;\n"
""
                        "border-color: #999;\n"
"}\n"
"\n"
"QSlider::handle:horizontal:disabled {\n"
"background: #eee;\n"
"border: 1px solid #aaa;\n"
"border-radius: 2px;\n"
"}\n"
""));
        horizontalSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(horizontalSlider);

        pushButton_volume = new QPushButton(centralwidget);
        pushButton_volume->setObjectName(QString::fromUtf8("pushButton_volume"));
        pushButton_volume->setMinimumSize(QSize(36, 36));
        pushButton_volume->setStyleSheet(QString::fromUtf8("QPushButton{ \n"
"image: url(:sound.png);\n"
"border-radius:0px; \n"
"padding:5px;\n"
"}  \n"
"QPushButton:hover{ \n"
"image: url(:stop_focus.png);\n"
"border-radius:0px; \n"
"padding:3px;\n"
"} \n"
"QPushButton:!checked{ \n"
"image: url(:sound.png);\n"
"} \n"
"QPushButton:checked{ \n"
"image: url(:sound_mute.png);\n"
"} \n"
"\n"
""));

        horizontalLayout->addWidget(pushButton_volume);


        verticalLayout->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 783, 20));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen_File);
        menuFile->addAction(actionRtsp_Streaming);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionOpen_File->setText(QCoreApplication::translate("MainWindow", "Open File", nullptr));
        actionRtsp_Streaming->setText(QCoreApplication::translate("MainWindow", "Rtsp Streaming", nullptr));
        pushButton_play->setText(QString());
        pushButton_pause->setText(QString());
        pushButton_stop->setText(QString());
        pushButton_volume->setText(QString());
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
