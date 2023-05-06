#include "GPTSession.h"
#include <QDebug>
#include <QApplication>
#include <iostream>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    GPTSession* gptSession = new GPTSession(&app);
    gptSession->setAPIKey("");
    printf("Connecting...");

    QObject::connect(
        gptSession, &GPTSession::responseReceived, [](QString response) {
            if (response.isEmpty()) {
                qWarning() << "Empty response received from the server";
                // TODO: add error handling logic
            } else {
                std::cout << response.toStdString();
            }
        });

    gptSession->addPrompt("Hello,there");
    return app.exec();
}
