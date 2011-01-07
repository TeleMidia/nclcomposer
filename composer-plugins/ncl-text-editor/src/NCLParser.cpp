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
    // qDebug() << qName;
    if (currentItem) {
        currentItem = new QTreeWidgetItem(currentItem);
    } else {
        currentItem = new QTreeWidgetItem(treeWidget);
    }
    QIcon icon;
    if(qName == "media")
            icon = QIcon (":/images/media.png");
    else if(qName == "descriptor")
        icon = QIcon (":/images/descriptor.png");
    else if(qName == "link")
        icon = QIcon (":/images/link-icon.png");
    else
        icon = QIcon (":/images/new.png");

    currentItem->setIcon(0, icon);
    currentItem->setText(0, localName);
    currentItem->setText(1, attributes.value("id"));
    currentItem->setText(2, QString::number(locator->lineNumber()));
    currentItem->setText(3, QString::number(locator->columnNumber()));
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
