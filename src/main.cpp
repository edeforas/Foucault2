// this file is covered by the General Public License version 2 or later
// please see GPL.html for more details and licensing issues
// copyright Etienne de Foras ( the author )  mailto: etienne.deforas@gmail.com

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QLibraryInfo>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString sPath=QCoreApplication::applicationDirPath();

    QTranslator qtTranslator;
    (void)qtTranslator.load("qt_" + QLocale::system().name(),QLibraryInfo::path(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    (void)myappTranslator.load("Foucault2_" + QLocale::system().name(),sPath);
    app.installTranslator(&myappTranslator);

    MainWindow w;
    w.showMaximized();  

    if (argc==2)
    {
        w.load_file(std::string(argv[1]));
    }

    return app.exec();
}
