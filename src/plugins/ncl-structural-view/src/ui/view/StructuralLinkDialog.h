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
  CompleteLineEdit(const QStringList &words, QWidget *parent = 0);
  void setStringList(const QStringList &words);

  void hideList();
  void updateListPos();
  void updateListSize();

public slots:
  void setCompleter(const QString &text);
  void completeText(const QModelIndex &index);

protected:
  virtual void keyPressEvent(QKeyEvent *e);
  virtual void focusInEvent(QFocusEvent *e);
  virtual void hideEvent ( QHideEvent * event );
//  virtual void moveEvent( QMoveEvent* event);
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

  enum LinkDialogMode
  {
    CreateLink,
    CreateAction,
    CreateCondition,
    EditLink,
    EditAction,
    EditCondition
  };

  void setData(const QMap<QString, QVector<QString> > &conditions,
               const QMap<QString, QVector<QString> > &actions,
               const QMap<QString, QVector<QString> > &StructuralLinkDialogparams);

  void init(const QString &connName = "", const QString &condName = "",
            const QString &actionName = "", LinkDialogMode mode = CreateLink);

  void updateCurrentLinkParam(const QMap<QString, QString> &params);
  void updateCurrentConditionParam(const QMap<QString, QString> &params);
  void updateCurrentActionParam(const QMap<QString, QString> &params);

  QMap<QString, QString> getLinkParams();
  QMap<QString, QString> getConditionParams();
  QMap<QString, QString> getActionParams();

protected slots:
  void updateForm(QString conn);
  void changeLinkParamState(int state);
  void changeConditionParamState(int state);
  void changeActionParamState(int state);


protected:
  virtual void showEvent(QShowEvent *evt);
  virtual void moveEvent(QMoveEvent *event);
  virtual void resizeEvent(QResizeEvent *event);

public:
  //TODO: This shouldn't be public
  Ui::StructuralLinkForm form;

  QMap<QString, QVector<QString> > _conditions;
  QMap<QString, QVector<QString> > _actions;
  QMap<QString, QVector<QString> > _params;

  QString getCurrentConnector();

private:
  QMap<QString, QString> getParams(QTableView* table);
  void updateCurrentParams(QTableView* table,
                           const QMap<QString, QString> &params);
  bool firstTime, changeModel;
  LinkDialogMode _currentMode;
  CompleteLineEdit *connLineEdit;
};


#endif // STRUCTURALLINKDIALOG_H
