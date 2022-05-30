#include "dialog.h"
#include <QQmlApplicationEngine>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;

    QQmlApplicationEngine engine;
    QObject::connect(&w, &Dialog::retransRequest, &engine, &QQmlApplicationEngine::retranslate);
    w.show();
    return a.exec();
}
