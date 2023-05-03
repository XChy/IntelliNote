#include "MDViewer.h"
#include <QDesktopServices>
#include <QWebChannel>

void Document::setText(const QString &text)
{
    if (text == m_text)
        return;
    m_text = text;
    emit textChanged(m_text);
}

bool PreviewPage::acceptNavigationRequest(const QUrl &url,
                                          QWebEnginePage::NavigationType /*type*/,
                                          bool /*isMainFrame*/)
{
    // Only allow qrc:/index.html.
    if (url.scheme() == QString("qrc"))
        return true;
    QDesktopServices::openUrl(url);
    return false;
}

#include <QFile>
#include <iostream>
MDViewer::MDViewer(QWidget* parent):QWebEngineView(parent),m_page(new PreviewPage(parent))
{
    setPage(m_page);

    QWebChannel *channel = new QWebChannel(parent);
    channel->registerObject(QStringLiteral("content"), &m_content);
    m_page->setWebChannel(channel);

    setUrl(QUrl("qrc:/res/index.html"));
}

void MDViewer::setText(const QString& text){
    m_content.setText(text);
}
