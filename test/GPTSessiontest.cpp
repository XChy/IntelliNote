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
    printf("Connecting...\n");

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
