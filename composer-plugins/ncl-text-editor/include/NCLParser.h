#ifndef NCLSAXHANDLER_H
#define NCLSAXHANDLER_H

#include <QtXml/QtXml>

#include <QList>
#include <QMessageBox>

#include <QTreeWidget>

class NCLSaxHandler : public QXmlDefaultHandler {
public:
    NCLSaxHandler(QTreeWidget *tree);
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
};
#endif
