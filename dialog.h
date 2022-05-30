#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QDir>
#include <QLabel>
#include <QTranslator>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

    bool imageConvertBin(const QString filaName);
    void reTrans(const QString &l);

signals:
    void retransRequest();

protected:
    void keyPressEvent(QKeyEvent *e) override;

    void mouseDoubleClickEvent(QMouseEvent *e) override;

    void mousePressEvent(QMouseEvent *e) override;

    bool eventFilter(QObject *, QEvent *) override;
    
private slots:
    void on_pBtnSave_clicked();

    void on_pBtnOpen_clicked();

    void on_pBtnCovert_clicked();

    void on_pBtnClose_clicked();

    void on_lineEditOpen_editingFinished();

    void on_lineEditSave_editingFinished();

    void on_checkBoxOpenUrl_stateChanged(int arg1);

    void on_comboBoxLanguage_currentIndexChanged(int index);

private:
    Ui::Dialog *ui;

    QString currentOpenDir;
    QString currentSaveDir;
    QString imageFileName;

    QLabel *mLabelIcon;
    QLabel *mLabelTitle;
    QPushButton *mBtnMin;
//    QPushButton *mBtnMax;
    QPushButton *mBtnClose;

    QMap<QString, QTranslator *> mTrans;
    QString mLastLan;

    void updateMaximize();
};
#endif // DIALOG_H
