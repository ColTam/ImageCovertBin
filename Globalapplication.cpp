// 参考链接：https://blog.csdn.net/tax10240809163com/article/details/103206193
#include "Globalapplication.h"

#include <QThread>

GlobalApplication::GlobalApplication(int &argc, char **argv) : QApplication(argc, argv)
 , m_pTimer(Q_NULLPTR)
 , m_pThread(Q_NULLPTR)
 , m_pWidget(Q_NULLPTR) //该指针可以通过调用方法来设置
{
    int nTimer = 5; //设置自动锁屏的超时时间，该时间可由配置文件导入
    m_pThread = new QThread();
    m_pTimer = new QTimer(this);
    m_pTimer->moveToThread(m_pThread);  //开启一个线程用来进行计时器的运行
    connect(m_pThread, SIGNAL(started()), m_pTimer, SLOT(start()));
    connect(m_pTimer, &QTimer::timeout, this, &GlobalApplication::slotTimeOut);
    m_pTimer->setInterval(nTimer * 60000);
    m_pThread->start();
}

GlobalApplication::~GlobalApplication()
{

}

bool GlobalApplication::notify(QObject* obj, QEvent* e)
{
    if (e->type() == QEvent::MouseButtonPress || e->type() == QEvent::KeyPress)
    {
        m_pThread->quit();
    }
    else if (e->type() == QEvent::MouseButtonRelease || e->type() == QEvent::KeyRelease)
    {
        m_pThread->start();
    }
    if (e->type() == QEvent::KeyPress)
    {
        /***********
         *
         *  do something...
         *
        ***********/
    }
    return QApplication::notify(obj, e);
}

/**实现超时槽函数,可在次函数中实现一些定时操作的任务**/
void GlobalApplication::slotTimeOut()
{
//    if (!m_pWidget || m_pWidget->isLockScreen())
//    {
//        return;
//    }
//    m_pWidget->lockScreen();    //定时任务
}

//void GlobalApplication::setWindowInstance(Dialog *wnd)
//{
//    m_pWidget = wnd;	/**可以将外部的类传进来**/
//}
