#ifndef RTSPDIALOG_H
#define RTSPDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class RtspDialog; }
QT_END_NAMESPACE

class RtspDialog : public QDialog
{
    Q_OBJECT

public:
    RtspDialog(QWidget *parent = nullptr);
    ~RtspDialog();

    QString getVideoUrl();

Q_SIGNALS:
    void EmitAccept(QString);

private Q_SLOTS:

    void SendUrl(QString);
    //    void RejectState();

private:
    Ui::RtspDialog *ui;

    QString url;
};

#endif // RTSPDIALOG_H
