/********************************************************************************
** Form generated from reading UI file 'dialog_about.ui'
**
** Created: Tue 6. Dec 20:00:48 2011
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_ABOUT_H
#define UI_DIALOG_ABOUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_dialogAbout
{
public:
    QLabel *label;
    QLabel *label_3;
    QLabel *label_5;
    QLabel *label_7;
    QLabel *label_4;
    QLabel *label_6;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QPushButton *pushButton;
    QLabel *label_2;
    QLabel *label_11;

    void setupUi(QDialog *dialogAbout)
    {
        if (dialogAbout->objectName().isEmpty())
            dialogAbout->setObjectName(QString::fromUtf8("dialogAbout"));
        dialogAbout->resize(450, 301);
        dialogAbout->setMouseTracking(true);
        dialogAbout->setContextMenuPolicy(Qt::NoContextMenu);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/russian_federation.png"), QSize(), QIcon::Normal, QIcon::Off);
        dialogAbout->setWindowIcon(icon);
        dialogAbout->setWindowOpacity(1);
        dialogAbout->setAutoFillBackground(true);
        label = new QLabel(dialogAbout);
        label->setObjectName(QString::fromUtf8("label"));
        label->setEnabled(true);
        label->setGeometry(QRect(-20, -110, 491, 511));
        label->setAutoFillBackground(false);
        label->setFrameShape(QFrame::NoFrame);
        label->setFrameShadow(QFrame::Plain);
        label->setPixmap(QPixmap(QString::fromUtf8(":/images/about.png")));
        label->setScaledContents(true);
        label->setWordWrap(false);
        label_3 = new QLabel(dialogAbout);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(50, 50, 271, 16));
        QFont font;
        font.setFamily(QString::fromUtf8("Times New Roman"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        label_3->setFont(font);
        label_3->setTextFormat(Qt::RichText);
        label_5 = new QLabel(dialogAbout);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(50, 120, 101, 16));
        label_5->setFont(font);
        label_5->setTextFormat(Qt::RichText);
        label_5->setScaledContents(false);
        label_7 = new QLabel(dialogAbout);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(70, 180, 271, 16));
        label_7->setFont(font);
        label_7->setTextFormat(Qt::RichText);
        label_7->setScaledContents(false);
        label_4 = new QLabel(dialogAbout);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(184, 90, 90, 21));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Times New Roman"));
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setItalic(false);
        font1.setUnderline(false);
        font1.setWeight(75);
        label_4->setFont(font1);
        label_4->setTextFormat(Qt::RichText);
        label_6 = new QLabel(dialogAbout);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(70, 140, 201, 16));
        label_6->setFont(font);
        label_6->setTextFormat(Qt::RichText);
        label_6->setScaledContents(false);
        label_8 = new QLabel(dialogAbout);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(70, 160, 271, 16));
        label_8->setFont(font);
        label_8->setTextFormat(Qt::RichText);
        label_8->setScaledContents(false);
        label_9 = new QLabel(dialogAbout);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(70, 200, 271, 16));
        label_9->setFont(font);
        label_9->setTextFormat(Qt::RichText);
        label_9->setScaledContents(false);
        label_10 = new QLabel(dialogAbout);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(70, 220, 271, 16));
        label_10->setFont(font);
        label_10->setTextFormat(Qt::RichText);
        label_10->setScaledContents(false);
        pushButton = new QPushButton(dialogAbout);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(380, 230, 51, 51));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/exit_32_32.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon1);
        pushButton->setIconSize(QSize(32, 32));
        label_2 = new QLabel(dialogAbout);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(50, 30, 351, 21));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Broadway"));
        font2.setPointSize(10);
        font2.setBold(true);
        font2.setWeight(75);
        label_2->setFont(font2);
        label_2->setTextFormat(Qt::RichText);
        label_2->setScaledContents(false);
        label_11 = new QLabel(dialogAbout);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(50, 70, 271, 16));
        label_11->setFont(font);
        label_11->setTextFormat(Qt::RichText);

        retranslateUi(dialogAbout);
        QObject::connect(pushButton, SIGNAL(clicked()), dialogAbout, SLOT(close()));

        QMetaObject::connectSlotsByName(dialogAbout);
    } // setupUi

    void retranslateUi(QDialog *dialogAbout)
    {
        dialogAbout->setWindowTitle(QApplication::translate("dialogAbout", "TSMFB", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        label_3->setText(QApplication::translate("dialogAbout", "TSMFB v1.0.0.0 (Final Release 06.12.11)", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("dialogAbout", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Times New Roman'; font-size:10pt; font-weight:600; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#ffffff;\">\320\235\320\265\321\207\320\260\320\265\320\262 \320\225\320\262\320\263\320\265\320\275\320\270\320\271</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("dialogAbout", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Times New Roman'; font-size:10pt; font-weight:600; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#ffffff;\">\320\237\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265 \320\264\320\276\320\277\320\276\320\273\320\275\320\270\321\202\320\265\320\273\321\214\320\275\321\213\321\205 \321\215\321\204\321\204\320\265\320\272\321\202\320\276\320\262</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("dialogAbout", "\320\241\320\276\320\267\320\264\320\260\321\202\320\265\320\273\320\270:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("dialogAbout", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Times New Roman'; font-size:10pt; font-weight:600; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#ffffff;\">\320\233\320\270\320\264\320\265\321\200 \320\277\321\200\320\276\320\265\320\272\321\202\320\260, \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("dialogAbout", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Times New Roman'; font-size:10pt; font-weight:600; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#ffffff;\">\320\224\320\270\320\267\320\260\320\271\320\275</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("dialogAbout", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Times New Roman'; font-size:10pt; font-weight:600; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#ffffff;\">\320\232\320\276\320\275\321\201\321\203\320\273\321\214\321\202\320\260\320\275\321\202 \320\277\321\200\320\276\320\265\320\272\321\202\320\260 \320\270\320\267 \320\221\320\276\320\275\321\207 \320\256\320\275\320\270\320\262\320\265\321\200\321\201\320\270\321\202\320\270</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("dialogAbout", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Times New Roman'; font-size:10pt; font-weight:600; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#ffffff;\">\320\242\320\265\321\201\321\202\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QString());
        label_2->setText(QApplication::translate("dialogAbout", "Tree-Structured Multichannel Filter Banks", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("dialogAbout", "Copyright \302\251 2011 \320\235\320\265\321\207\320\260\320\225\320\262\320\223\320\265\320\275\320\270\320\271", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dialogAbout: public Ui_dialogAbout {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_ABOUT_H
