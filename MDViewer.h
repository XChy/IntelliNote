#ifndef MDVIEWER_H
#define MDVIEWER_H

#include <QWebEngineView>

class Document : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text MEMBER m_text NOTIFY textChanged FINAL)
public:
    explicit Document(QObject *parent = nullptr) : QObject(parent) {}

    void setText(const QString &text);

signals:
    void textChanged(const QString &text);

private:
    QString m_text;
};

class PreviewPage : public QWebEnginePage
{
    Q_OBJECT
public:
    using QWebEnginePage::QWebEnginePage;

protected:
    bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame) override;
};


class MDViewer : public QWebEngineView
{
    Q_OBJECT
public:
    MDViewer(QWidget* parent);

    void setText(const QString& text);
private:
    Document m_content;
    PreviewPage *m_page;
};

#endif // MDVIEWER_H
