#ifndef PROPERTYBUTTONS_H
#define PROPERTYBUTTONS_H




#include <QWidget>
#include<QVBoxLayout>
#include<QFileDialog>
#include<QString>


class propertybuttons : public QFileDialog
{
  Q_OBJECT

private:
 QWidget *window;

  public:
    propertybuttons(QString);
    QFileDialog dialog;
    QString key;




public slots:
  void openfile();

signals:
  void newValue(QString name, QString value);
};









#endif // PROPERTYBUTTONS_H
