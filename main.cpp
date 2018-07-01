#include "controller.h"
#include "singleton.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Singleton<Controller>::getInstance();
    return a.exec();
}
