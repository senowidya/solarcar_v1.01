#include "interfacing.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    interfacing w;
    w.show();

    return a.exec();
}
