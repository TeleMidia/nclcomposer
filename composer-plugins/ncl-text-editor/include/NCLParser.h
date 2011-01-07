#ifndef NCLSAXHANDLER_H
#define NCLSAXHANDLER_H

#include <QtXml/QtXml>

#include <QList>
#include <QMessageBox>
#include <QTreeWidget>

class NCLParser : public QObject, public QXmlDefaultHandler {
    Q_OBJECT

public:
    NCLParser(QTreeWidget *tree);
    bool startElement(const QString &namespaceURI,
                      const QString &localName,
                      const QString &qName,
                      const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI,
                    const QString &localName,
                    const QString &qName);
    bool characters(const QString &str);
    bool fatalError(const QXmlParseException &exception);
    void setDocumentLocator(QXmlLocator *locator);

private:
    QTreeWidget *treeWidget;
    QTreeWidgetItem *currentItem;
    QString currentText;
    QXmlLocator *locator;

signals:
    void fatalErrorFound ( QString message,
                           QString file,
                           int line,
                           int column,
                           int severity);
};
#endif
