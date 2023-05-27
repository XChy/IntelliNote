//
// Created by shenjiawei on 2023/5/10.
//
#include "TransSession.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TransSession *transSession = new TransSession(&app);
    printf("Connecting...\n");
    ushort curtime=QDateTime::currentDateTimeUtc().toSecsSinceEpoch();
    //change curtime to seconds;
    printf("curtime: %d\n", curtime);
    QObject::connect(
            transSession, &TransSession::completed, [&app](QString response) {
                if (response.isEmpty()) {
                    qWarning() << "Empty response received from the server";
                    app.exit(1);
                    // TODO: add error handling logic
                } else {
                    std::cout << response.toStdString();
                    app.exit(0);
                }
            });
    std::string s;
    getline(std::cin, s);
    transSession->translate(QString::fromStdString(s));
    return app.exec();
}