/********************************************************************************
** Form generated from reading UI file 'RtspDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RTSPDIALOG_H
#define UI_RTSPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RtspDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *lineEdit_url;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *RtspDialog)
    {
        if (RtspDialog->objectName().isEmpty())
            RtspDialog->setObjectName(QString::fromUtf8("RtspDialog"));
        RtspDialog->resize(400, 162);
        verticalLayout = new QVBoxLayout(RtspDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(RtspDialog);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setSizeIncrement(QSize(0, 0));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\273\237\346\255\243\351\273\221\351\253\224"));
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        verticalLayout->addWidget(label);

        lineEdit_url = new QLineEdit(RtspDialog);
        lineEdit_url->setObjectName(QString::fromUtf8("lineEdit_url"));

        verticalLayout->addWidget(lineEdit_url);

        buttonBox = new QDialogButtonBox(RtspDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(RtspDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), RtspDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), RtspDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(RtspDialog);
    } // setupUi

    void retranslateUi(QDialog *RtspDialog)
    {
        RtspDialog->setWindowTitle(QCoreApplication::translate("RtspDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("RtspDialog", "\350\274\270\345\205\245\347\266\262\345\235\200\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RtspDialog: public Ui_RtspDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RTSPDIALOG_H
