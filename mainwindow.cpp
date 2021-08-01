#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    videoDecoder = new Decoder();
    bufferSize = 0;
    buffer = nullptr;

    connect(ui->actionOpen_File, &QAction::triggered, this, &MainWindow::OpenFile);
    connect(ui->actionRtsp_Streaming, &QAction::triggered, this, &MainWindow::OpenRTSP);

    connect(this, &MainWindow::SendStreamPath, videoDecoder, &Decoder::InitDecoder);

    videoDecoder->moveToThread(&decodeThread);
    connect(videoDecoder, &Decoder::SendVideoData, this, &MainWindow::GetVideoData);
    decodeThread.start();       // 啟動 decode 執行緒

    qDebug() << "Main thread: " << QThread::currentThread();
}

MainWindow::~MainWindow()
{
    videoDecoder->decode_loop = false;
    decodeThread.quit();
    decodeThread.wait();

    delete [] buffer;

    delete ui;
}

void MainWindow::OpenFile()
{
    QString filePath;
    QFileDialog fileLoad(this);

    fileLoad.setWindowTitle(QStringLiteral("選擇文件"));
    fileLoad.setAcceptMode(QFileDialog::AcceptOpen);

    QStringList formatLists;
    formatLists.append("一般影像(*.flv *.rmvb *.avi *.MP4 *.mkv)");
    formatLists.append("所有文件(*.*)");
    fileLoad.setFileMode(QFileDialog::ExistingFile);
    fileLoad.setNameFilters(formatLists);

    if(fileLoad.exec() == QFileDialog::Accepted)
    {
        filePaths = fileLoad.selectedFiles();
        filePath = filePaths[0];
    }

    Q_EMIT SendStreamPath(filePath);
}

void MainWindow::OpenRTSP()
{
    RtspDialog rtsp;

    if(rtsp.exec() == QDialog::Accepted)
    {
        QString url = rtsp.getVideoUrl();
        if(!url.isEmpty())
        {
            Q_EMIT SendStreamPath(url);
        }
    }
}

void MainWindow::GetVideoData(uint8_t *data, int width, int height)
{
    if(bufferSize < width * height * 3/2)
    {
//        delete [] buffer;
        bufferSize = width * height * 3/2;
        buffer = new uint8_t[bufferSize];
    }


    std::copy(data, data+width*height*3/2, buffer);

    ui->video_render->getOneFrame(buffer, width, height);
}
