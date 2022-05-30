#include "dialog.h"
#include "ui_dialog.h"
#include <iostream>
#include <stdio.h>
#include <QDebug>

#include <QFile>
#include <QFileDevice>
#include <QFileDialog>
#include <QDir>
#include <QUrl>
#include <QImage>
#include <QBuffer>
#include <QMessageBox>
#include <QKeyEvent>
#include <QDesktopServices>

#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
    , currentOpenDir("./")
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | windowFlags());
    this->setWindowIcon(QIcon(":/img/resource/images/logo.png"));
    this->setWindowTitle("Image Covert TFT");

    ui->textEditLog->setReadOnly(true);


    QTranslator *en = new QTranslator(this);
    QTranslator *cn = new QTranslator(this);
    bool ret = en->load(":/language/zh_CN.qm");
    qWarning() << ret;
    ret = cn->load(":/language/zh_CN.qm");
    qWarning() << ret;
    mTrans["en"] = en;
    mTrans["zh"] = cn;
////    this->setFixedHeight(30);

//    mLabelIcon = new QLabel(this);
//    mLabelTitle = new QLabel(this);
//    mBtnMin = new QPushButton(this);
////    mBtnMax = new QPushButton(this);
//    mBtnClose = new QPushButton(this);

//    mLabelIcon->setFixedSize(20,20);
//    mLabelIcon->setScaledContents(true);

//    mLabelTitle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

//    mBtnMin->setIconSize(QSize(27,22));
//    mBtnMin->setIcon(QIcon());
//    mBtnMin->setFlat(true);
//    connect(mBtnMin, QPushButton::clicked, [=] () {this->window()->showMinimized();});

//    mBtnClose->setIconSize(QSize(27,22));
//    mBtnClose->setIcon(QIcon());
//    mBtnClose->setFlat(true);
//    connect(mBtnClose, QPushButton::clicked, [=](){this->window()->close();});

//    mLabelTitle->setObjectName("");
//    mBtnMin->setObjectName("");
//    mBtnClose->setObjectName("");

//    mBtnMin->setToolTip("Minimize");
//    mBtnClose->setToolTip("Close");

//    QHBoxLayout *pLayout = new QHBoxLayout(this);
//    pLayout->addWidget(mLabelIcon);
//    pLayout->addSpacing(5);
//    pLayout->addWidget(mLabelTitle);
//    pLayout->addWidget(mBtnMin);
//    pLayout->addWidget(mBtnClose);

//    pLayout->setSpacing(0);
//    pLayout->setContentsMargins(5, 0, 5, 0);

//    setLayout(pLayout);
}

Dialog::~Dialog()
{
    delete ui;
}

bool Dialog::imageConvertBin(const QString filaName)
{
    QFileInfo inf(filaName);
    QString outFilaName = QString("%1/%2.%3").arg(currentSaveDir).arg(inf.baseName()).arg("bin");

    QImage image(filaName);
    image = image.convertToFormat(QImage::Format_RGB16);
    uchar *uimg = image.bits();

    //save
    QFile file(outFilaName);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    else {
        QDataStream out(&file);

        uint32_t hear3 = 4 | image.width() << 10 | image.height() << 21;
        char hear4[4];
        hear4[0] = hear3;
        hear4[1] = hear3 >> 8;
        hear4[2] = hear3 >> 16;
        hear4[3] = hear3 >> 24;

        out.writeRawData(hear4, 4);
        out.writeRawData((char *)uimg, image.width()*image.height()*2);

        file.close();
    }

    return true;
}

void Dialog::reTrans(const QString &l)
{
    if (mTrans.contains(l)) {
        if (!mLastLan.isEmpty()) {
            QCoreApplication::removeTranslator(mTrans[mLastLan]);
        }
        mLastLan = l;
        QCoreApplication::installTranslator(mTrans[mLastLan]);
        emit retransRequest();
    }
}

void Dialog::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Escape:
        break;
    default:
        QDialog::keyPressEvent(e);
    }
}

void Dialog::mouseDoubleClickEvent(QMouseEvent *e)
{

}

void Dialog::mousePressEvent(QMouseEvent *e)
{

}

bool Dialog::eventFilter(QObject *, QEvent *)
{

}

void Dialog::on_pBtnOpen_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    currentOpenDir,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    if (dir.isEmpty()) {
        QMessageBox::critical(this, tr("Error"), tr("Open Directory failed"));
        return;
    }

    currentOpenDir = dir;
    ui->lineEditOpen->setText(currentOpenDir);
    ui->textEditLog->append(QString(tr("Selected open path: %1")).arg(currentOpenDir));

    if (ui->lineEditSave->text().isEmpty()) {
        currentSaveDir = dir;
        ui->lineEditSave->setText(currentSaveDir);
        ui->textEditLog->append(QString(tr("Selected save path: %1")).arg(currentSaveDir));
    }
}

void Dialog::on_pBtnSave_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    currentSaveDir,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    if (dir.isEmpty()) {
        QMessageBox::critical(this, tr("Error"), tr("Open Directory failed"));
        return;
    }

    currentSaveDir = dir;
    ui->lineEditSave->setText(currentSaveDir);
    ui->textEditLog->append(QString(tr("Selected save path: %1")).arg(currentSaveDir));
}

void Dialog::on_pBtnCovert_clicked()
{
    QDir dir(currentOpenDir);
    if (!dir.exists()) {
        QMessageBox::critical(this, tr("Error"), tr("Directory does not exist!"));
        return;
    }

    QStringList filters;
    filters << "*.bmp" << "*.jpg" << "*.png";
    dir.setNameFilters(filters);

    QFileInfoList fileInfoList = dir.entryInfoList();
    if (fileInfoList.isEmpty()) {
        // 空文件夹
        QMessageBox::critical(this, tr("Error"), tr("Image not found!"));
        return;
    }

    foreach (const QFileInfo fileInfo, fileInfoList) {
        if (imageConvertBin(fileInfo.canonicalFilePath())) {
            ui->textEditLog->append(QString(tr("%1 convert sucessed.")).arg(fileInfo.canonicalFilePath()));
        }
        else {
            ui->textEditLog->append(QString(tr("%1 convert failed.")).arg(fileInfo.canonicalFilePath()));
        }
    }

    if (ui->checkBoxOpenUrl->isEnabled()) {
        QDesktopServices::openUrl(QUrl(QString("file:///%1").arg(currentSaveDir), QUrl::TolerantMode));
    }
}

void Dialog::on_pBtnClose_clicked()
{

}

void Dialog::updateMaximize()
{

}


void Dialog::on_lineEditOpen_editingFinished()
{
    currentOpenDir = ui->lineEditOpen->text();
    ui->textEditLog->append(QString(tr("Selected open path: %1")).arg(currentOpenDir));

    if (ui->lineEditSave->text().isEmpty()) {
        currentSaveDir = currentOpenDir;
        ui->lineEditSave->setText(currentSaveDir);
        ui->textEditLog->append(QString(tr("Selected save path: %1")).arg(currentSaveDir));
    }
}


void Dialog::on_lineEditSave_editingFinished()
{
    currentSaveDir = ui->lineEditSave->text();
    ui->textEditLog->append(QString(tr("Selected save path: %1").arg(currentSaveDir)));
}


void Dialog::on_checkBoxOpenUrl_stateChanged(int arg1)
{
    if (arg1 == Qt::Unchecked) {
        ui->textEditLog->append(QString(tr("Don't open folder when done.")));
    }
    else if (arg1 == Qt::Checked) {
        ui->textEditLog->append(QString(tr("Open the folder when done.")));
    }
}


void Dialog::on_comboBoxLanguage_currentIndexChanged(int index)
{
    qDebug() << index;
    if (index == 0) {
        reTrans("cn");
    }
    else {
        reTrans("1");
    }
}

