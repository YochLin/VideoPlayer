#include "RtspDialog.h"
#include "ui_RtspDialog.h"


RtspDialog::RtspDialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::RtspDialog)
{
    ui->setupUi(this);
}

RtspDialog::~RtspDialog()
{

}

void RtspDialog::SendUrl(QString url)
{

}

QString RtspDialog::getVideoUrl()
{
    url = ui->lineEdit_url->text();
    return url;
}
