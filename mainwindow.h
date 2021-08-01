#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QThread>

#include "RtspDialog.h"
#include "decoder.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

Q_SIGNALS:
    void SendStreamPath(QString path);

public Q_SLOTS:
    void GetVideoData(uint8_t *data, int width, int height);

private:
    Ui::MainWindow *ui;

    QStringList filePaths;

    RtspDialog *rtsp;
    Decoder *videoDecoder;

    QThread decodeThread;

    uint8_t *buffer;
    int bufferSize;

    void OpenFile();
    void OpenRTSP();
};
#endif // MAINWINDOW_H
