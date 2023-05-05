#include "GPTSession.h"
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    GPTSession* gptSession= new GPTSession(&app);
    gptSession->setAPIKey("ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAACAQDaJJNGHFUvim5e5O6m6xOAHS/RB5A7H5DiAK7957ZqFB1ZbqG37LlVTPEtLHR6rXGoviP2HjLV7SoAkJe9r4Sf2K4pgEPlL0/KKqM7pUsWlsPfhvRurMCNhnMkCEf+oKgQDkcOJualhbxtky9knwTGyRu8FXOG1yP2eVCKiSRr1GOd+PwgnE/Ljy0UGkD1r1TMLf8twmMNcwaE5SI/BLSTsFma/oBkkQyziIzR3tY6PgesAURKH1hSqhLN/lFoy3DKQJ3/LfgHm0s9nlhVTQgQ5hHDfAhN3nU23jEU8naTlVA6xv9TutInPPCQSqJhAhvwDqRkBRCNy91lU7vc9uBmSRdKmJi++XUV4bTu3ou07h8uh0BrVA/iLUSy0nBjJkjv1XgziLt8gfqDP5ULbQFtHDnmT53xEfbPmcIh95OV3cTQb95oynhlMi1puHG5wSYetV6/JzEq6fzM+oTNDMLw2anT2dAhWAEbuyULcExH5c/s1ggPdU6OeWuWrphpNVNTzyhpAYovbGaIjfiLmZQZW3b5gVAUEyL/8fcAkf3pgtUlXxz6QlIFlDiLuFrzu4K52I6y5GKwCGoCJNiHQRKvRt/43gkumOkA6FtkldiRnC5EbY7Aem4qnOGwLDhZWj+AMezkNKsa2kYPOsllEV+9Bbrk0BqF9uvP4mXsxCYlsw==");
    QObject::connect(gptSession, &GPTSession::responseReceived, [](QString response) {
        qDebug() << response;
    });
    gptSession->addPrompt("Hello,there");
    return app.exec();
}
