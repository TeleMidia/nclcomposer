#ifndef STRUCTURALLINKDIALOG_H
#define STRUCTURALLINKDIALOG_H

#include <QDebug>
#include <QDialog>
#include <QShowEvent>

#include <QStringListModel>
#include <QSortFilterProxyModel>

#include "ui_StructuralLinkForm.h"

#include <QLineEdit>
#include <QStringList>

class QListView;
class QStringListModel;
class QModelIndex;

class CompleteLineEdit : public QLineEdit
{
  Q_OBJECT

public:
  CompleteLineEdit(QStringList words, QWidget *parent = 0);
  void setStringList(const QStringList &words);

public slots:
  void setCompleter(const QString &text);
  void completeText(const QModelIndex &index);

protected:
  virtual void keyPressEvent(QKeyEvent *e);
  virtual void focusInEvent(QFocusEvent *e);
  virtual void hideEvent ( QHideEvent * event );
  // virtual void focusOutEvent(QFocusEvent *e);
  virtual bool eventFilter(QObject *object, QEvent *event);

private:
  QStringList words;
  QListView *listView;
  QStringListModel *model;

};

class StructuralLinkDialog : public QDialog
{
  Q_OBJECT

public:
  StructuralLinkDialog(QWidget* parent = 0);

  ~StructuralLinkDialog();

  void init(QMap<QString, QVector<QString> > conditions,
            QMap<QString, QVector<QString> > actions,
            QMap<QString, QVector<QString> > params);

protected slots:
  void adjustBinds(QString conn);

protected:
  virtual void showEvent(QShowEvent *evt);

public:
  //TODO: This shouldn't be public
  Ui::StructuralLinkForm form;

  QMap<QString, QVector<QString> > _conditions;
  QMap<QString, QVector<QString> > _actions;
  QMap<QString, QVector<QString> > _params;

  QString getCurrentConnector();

private:
  bool firstTime, changeModel;
  CompleteLineEdit *connLineEdit;
};


#endif // STRUCTURALLINKDIALOG_H
