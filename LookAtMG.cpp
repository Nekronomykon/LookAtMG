#include "FrameBrowser.h"

#include <QSurfaceFormat>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QApplication>

#include <QPointer>
#include <QMainWindow>
#include <QSurfaceFormat>
#include <QCommandLineParser>

#include <QVTKOpenGLNativeWidget.h>
#include <QVTKApplication.h>

int main(int argc, char *argv[])
{
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
    QVTKApplication theApp(argc, argv);

    QApplication::setQuitOnLastWindowClosed(true);
    // set false if one would make use of
    // the tray activation system. Who would dare?
    // --------------------------------------------
    QCoreApplication::setOrganizationName("Nekronomykon");
    QCoreApplication::setApplicationName("Look At Molecular GraPh files");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file(s)", "One or more files to open in the present workspace");
    parser.process(theApp);

    QPointer<FrameBrowser> pNew(FrameBrowser::createNewFrame());
    if (!(!pNew))
    {
        auto posArgs = parser.positionalArguments();
        pNew->provideFilePaths(posArgs.begin(), posArgs.end());
        pNew->show();
    }
    return theApp.exec();
}
