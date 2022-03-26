#include "HilbertImage.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HilbertImage w;
    w.show();
    return a.exec();
}
