#ifndef NCLSAXHANDLER_H
#define NCLSAXHANDLER_H

#include <QtXml>

#include <QList>
#include <QtGui>
#include <QMessageBox>
#include <NCLTreeWidget.h>

class NCLTreeWidget;

class NCLParser : public QObject, public QXmlDefaultHandler {
    Q_OBJECT

public:
    NCLParser(NCLTreeWidget *tree);
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
    NCLTreeWidget *treeWidget;
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
