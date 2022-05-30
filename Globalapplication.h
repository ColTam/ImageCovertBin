#ifndef GLOBALAPPLICATION_H
#define GLOBALAPPLICATION_H

#include <QApplication>
#include <QObject>
#include <QTimer>

class Dialog;

class GlobalApplication : public QApplication
{
    Q_OBJECT
public:
    GlobalApplication(int &argc, char **argv);
    ~GlobalApplication();

    bool notify(QObject *o, QEvent *e);

private:
    void slotTimeOut();

private:
    QTimer *m_pTimer;
    QThread *m_pThread;
    Dialog *m_pWidget;
};

#endif // GLOBALAPPLICATION_H
