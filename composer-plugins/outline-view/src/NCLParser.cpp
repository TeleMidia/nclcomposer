#include "NCLParser.h"

NCLParser::NCLParser(NCLTreeWidget *tree)
{
    treeWidget = tree;
    currentItem = 0;
}

bool NCLParser::startElement(const QString & /* namespaceURI */,
                                 const QString & /*localName */,
                                 const QString &qName,
                                 const QXmlAttributes &attributes)
{
    if (currentItem) {
        currentItem = treeWidget->addElement(currentItem,
                                             currentItem->childCount(),
                                             qName,
                                             attributes.value("id"),
                                             locator->lineNumber(),
                                             locator->columnNumber());
    } else {
        currentItem = treeWidget->addElement(0,
                                            -1,
                                            qName,
                                            attributes.value("id"),
                                            locator->lineNumber(),
                                            locator->columnNumber());
    }
    //TODO: send a signal to all interested plugins.
    return true;
}

bool NCLParser::characters(const QString &str)
{
    currentText += str;
    return true;
}

bool NCLParser::endElement(const QString & /* namespaceURI */,
                               const QString & /* localName */,
                               const QString & /* qName */)
{
    currentItem = currentItem->parent();
    return true;
}

bool NCLParser::fatalError(const QXmlParseException &exception)
{
    if(currentItem != NULL) {
        currentItem->setIcon(0, QIcon(":/images/error-icon-16.png"));
        currentItem->setTextColor(0, QColor("#FF0000"));
//      currentItem->setBackgroundColor(0, QColor("#FF0000"));
    }

    qDebug() <<  QObject::tr("Parse error at line %1, column "
                                     "%2:\n%3.")
                         .arg(exception.lineNumber())
                         .arg(exception.columnNumber())
                         .arg(exception.message());

    emit fatalErrorFound ( exception.message(),
                           QString("FILENAME"),
                           exception.lineNumber(),
                           exception.columnNumber(),
                           0);

    return false;
}

void NCLParser::setDocumentLocator(QXmlLocator *locator)
{
    this->locator = locator;
}
