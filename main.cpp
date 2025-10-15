
#include <QApplication>

#include "tpg300.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    TPG300 window;
    window.show();
    app.exec();

    return 0;
}
