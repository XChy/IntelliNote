#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFile>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile f(":qdarkstyle/dark/darkstyle.qss");
    QFile f2(":qlightstyle/light/lightstyle.qss");
    QFile f3(":qlightstyle/trial.qss");
    f.open(QFile::ReadOnly );
    a.setStyleSheet(f.readAll());
    f.close();
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "IntelliNote_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}
