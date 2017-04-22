#ifndef SEARCHBOX_H
#define SEARCHBOX_H

#include <QFrame>
#include <QString>

namespace Ui {
class SearchBox;
}

class SearchBox : public QFrame
{
  Q_OBJECT

public:
  explicit SearchBox(QWidget *parent = 0);
  ~SearchBox();
  const QString text() const;
  const QString replaceText() const;

public slots:
  void setFocusToFindLineEdit();

private:
  Ui::SearchBox *_ui;

signals:
  void findNext(const QString &);
  void findPrevious(const QString &);
  void replace(const QString &, const QString &, bool);
  void replaceAll(const QString &, const QString &);
  void hideButtonClicked();

private slots:
  void on_pushButton_hide_clicked();

  void on_pushButton_FindPrevious_clicked();
  void on_pushButton_FindNext_clicked();
  void on_pushButton_ReplaceAndFind_clicked();
  void on_pushButton_Replace_clicked();
  void on_pushButton_ReplaceAll_clicked();
  void on_lineEdit_Find_textChanged(const QString &arg1);
};

#endif // SEARCHBOX_H
