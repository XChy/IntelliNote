#include "MDViewer.h"
#include <QDesktopServices>
#include <QWebChannel>
#include <QWebEngineSettings>

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

MDViewer::MDViewer(QWidget* parent):QWebEngineView(parent),m_page(new PreviewPage(parent))
{
    setPage(m_page);

    settings()->setAttribute(QWebEngineSettings::AutoLoadIconsForPage,false);
    settings()->setAttribute(QWebEngineSettings::WebGLEnabled,false);
    settings()->setAttribute(QWebEngineSettings::Accelerated2dCanvasEnabled,true);
    settings()->setAttribute(QWebEngineSettings::DnsPrefetchEnabled,false);
    QWebChannel *channel = new QWebChannel(parent);
    channel->registerObject(QStringLiteral("content"), &m_content);
    m_page->setWebChannel(channel);

    setUrl(QUrl("qrc:/res/index.html"));
}

void MDViewer::setText(const QString& text){
    m_content.setText(text);
}
