
#include <propertybuttons.h>
#include<QObject>
#include<QPushButton>
#include<QFileDialog>
#include<QWidget>
#include<QDialog>
#include<QString>

propertybuttons::propertybuttons(QString key)
{
  this->key = key;
}

void propertybuttons::openfile()
{
    QFileDialog dialog;

    dialog.setFileMode(QFileDialog::AnyFile);

    QString filename = dialog.getOpenFileName(NULL);
    if(!filename.isEmpty())
        emit newValue(key, filename);

}




