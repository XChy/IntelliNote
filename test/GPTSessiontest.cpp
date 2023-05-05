#include "GPTSession.h"
#include <QDebug>
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    GPTSession* gptSession= new GPTSession(&app);
    gptSession->setAPIKey("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");
    printf("Connecting...");
    bool success = QObject::connect(gptSession, &GPTSession::responseReceived, [](QString response) {
        if (response.isEmpty()) {
            qWarning() << "Empty response received from the server";
            // TODO: add error handling logic
        } else {
            std::cout<<response.toStdString();
        }
    });
    if (!success) {
        qWarning() << "Failed to connect signals and slots";
        // TODO: add error handling logic
    }
    char str[100];
    //while(scanf("%s", str) != EOF) {
        //gptSession->addPrompt(str);
    //}
    gptSession->addPrompt("Hello,there");
    return app.exec();
}
