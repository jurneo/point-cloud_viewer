#include <iostream>
#include <QApplication>
#include <testwindow.h>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    TestWindow test;
    test.show();

    return app.exec();
}
