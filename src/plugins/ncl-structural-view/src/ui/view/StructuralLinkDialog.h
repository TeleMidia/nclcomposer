#ifndef STRUCTURALLINKDIALOG_H
#define STRUCTURALLINKDIALOG_H

#include <QDebug>
#include <QDialog>
#include <QShowEvent>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QLineEdit>
#include <QStringList>

#include "Structural.h"

#include "ui_StructuralLinkForm.h"

class QListView;
class QStringListModel;
class QModelIndex;

class CompleteLineEdit : public QLineEdit
{
  Q_OBJECT

public:
  CompleteLineEdit(const QStringList &_words, QWidget *parent = 0);
  void setStringList(const QStringList &_words);

  void updateListPos();
  void updateListSize();
  void setCompleter(const QString &text);

public slots:
  void updateCompleter(const QString &text);
  void completeText(const QModelIndex &index);

protected:
  virtual void keyPressEvent(QKeyEvent *e);
  virtual void focusInEvent(QFocusEvent *e);
  virtual void focusOutEvent(QFocusEvent *e);
  virtual bool eventFilter(QObject *object, QEvent *event);

private:
  QStringList _words;
  QListView *_listView;
  QStringListModel *_model;

};

class StructuralLinkDialog : public QDialog
{
  Q_OBJECT

public:
  StructuralLinkDialog(QWidget* parent = 0);
  virtual ~StructuralLinkDialog();

  enum Mode
  {
    CreateLink,
    CreateAction,
    CreateCondition,
    EditLink,
    EditAction,
    EditCondition
  };

  QString getConnector();
  void setConnector(const QString &connector);

  QMap<QString, QString> getConnectorParams();
  void setConnectorParams(const QMap<QString, QString> &params);

  QString getCondition();
  void setCondition(const QString &condition);

  QMap<QString, QString> getConditionParams();
  void setConditionParams(const QMap<QString, QString> &params);

  QString getConditionInterface();
  QMap<QString, QString> getConditonInterfaces();
  void addConditionInterface(const QString &uid, const QString &name, const QIcon &icon);
  void removeConditionInterface(const QString& name);

  QString getAction();
  void setAction(const QString &action);

  QMap<QString, QString> getActionParams();
  void setActionParams(const QMap<QString, QString> &params);

  QString getActionInterface();
  QMap<QString, QString> getActionInterfaces();
  void addActionInterface(const QString &uid, const QString &name, const QIcon &icon);
  void removeActionInterface(const QString& name);

  void setMode(const QString &connector = "",
               const QString &condition = "",
               const QString &action = "",
               Mode mode = CreateLink);

  void setBase(const QMap<QString, QVector<QString> > &conditions,
               const QMap<QString, QVector<QString> > &actions,
               const QMap<QString, QVector<QString> > &params);

protected slots:
  void changeContent(const QString &connector);

  void switchConnectorParam(int state);
  void switchConditionParam(int state);
  void switchActionParam(int state);

protected:
  virtual void moveEvent(QMoveEvent* event);
  virtual void resizeEvent(QResizeEvent* event);
  virtual bool event(QEvent* event);

private:
  QMap<QString, QString> getParams(QTableView* widget);
  void setParams(QTableView* widget, const QMap<QString, QString> &params);

  QString getInterface(const QComboBox* widget);
  QMap<QString, QString> getInterfaces(const QComboBox* widget);
  void addInterface(QComboBox* widget, const QString &uid, const QString &name, const QIcon &icon);
  void removeInterface(QComboBox* widget, const QString &name);

private:
  Mode _mode;
  bool _aligned;
  CompleteLineEdit* _search;

  QMap<QString, QVector<QString> > _conditions;
  QMap<QString, QVector<QString> > _actions;
  QMap<QString, QVector<QString> > _params;

  Ui::StructuralLinkForm _form;
};

#endif // STRUCTURALLINKDIALOG_H
