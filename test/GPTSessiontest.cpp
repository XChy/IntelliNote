#include "GPTSession.h"
#include <QDebug>
#include <QApplication>
#include <iostream>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    GPTSession* gptSession = new GPTSession(&app);
    gptSession->setAPIKey(
        "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");

    printf("Connecting...");

    QObject::connect(
        gptSession, &GPTSession::responseReceived, [&app](QString response) {
            if (response.isEmpty()) {
                qWarning() << "Empty response received from the server";
                app.exit(1);
                // TODO: add error handling logic
            } else {
                std::cout << response.toStdString();
                app.exit(0);
            }
        });

    gptSession->addPrompt("Hello,there");
    return app.exec();
}
