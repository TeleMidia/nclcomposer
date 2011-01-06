#include "NCLParser.h"

NCLSaxHandler::NCLSaxHandler(QTreeWidget *tree)
{
    treeWidget = tree;
    currentItem = 0;
}

bool NCLSaxHandler::startElement(const QString & /* namespaceURI */,
                                 const QString &localName,
                                 const QString &qName,
                                 const QXmlAttributes &attributes)
{
    qDebug() << qName;
    if (currentItem) {
        currentItem = new QTreeWidgetItem(currentItem);
    } else {
        currentItem = new QTreeWidgetItem(treeWidget);
    }
    currentItem->setText(0, localName);
    currentItem->setText(1, attributes.value("id"));
    currentItem->setText(2, QString::number(locator->lineNumber()) );
    return true;
}

bool NCLSaxHandler::characters(const QString &str)
{
    currentText += str;
    return true;
}

bool NCLSaxHandler::endElement(const QString & /* namespaceURI */,
                               const QString & /* localName */,
                               const QString & /* qName */)
{
    currentItem = currentItem->parent();
    /*if (currentItem) {
        QString allPages = currentItem->text(1);
        if (!allPages.isEmpty())
            allPages += ", ";
        allPages += currentText;
        currentItem->setText(1, allPages);
    }*/
    return true;
}

bool NCLSaxHandler::fatalError(const QXmlParseException &exception)
{
    qDebug() <<  QObject::tr("Parse error at line %1, column "
                                     "%2:\n%3.")
                         .arg(exception.lineNumber())
                         .arg(exception.columnNumber())
                         .arg(exception.message());
    return false;
}

void NCLSaxHandler::setDocumentLocator(QXmlLocator *locator)
{
    this->locator = locator;
}
