// this file is covered by the General Public License version 2 or later
// please see GPL.html for more details and licensing issues
// copyright Etienne de Foras ( the author )  mailto: etienne.deforas@gmail.com

#include <QApplication>
#include "MainWindow.h"
#include <QLocale>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    myappTranslator.load("Foucault2_" + QLocale::system().name());
    app.installTranslator(&myappTranslator);

    MainWindow w;
    w.showMaximized();  

    if (argc==2)
    {
        w.load_file(string(argv[1]));
    }

    return app.exec();
}
