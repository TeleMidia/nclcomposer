#ifndef WORKSPACESWITCH_H
#define WORKSPACESWITCH_H

#include <QDialog>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QFileDialog>
#include <QDir>

namespace composer {
    namespace ui {

class WorkspaceSwitch : public QDialog
{
    Q_OBJECT
public:
    explicit WorkspaceSwitch(QWidget *parent = 0);
    inline QString getWorspacePath() { return wsPath->text(); }
private:
    QLineEdit *wsPath;
    QPushButton *openDir;
    QPushButton *bOk;
    QPushButton *bCancel;

    void init();

signals:

private slots:
    void openDirectory();

};

}} //end namespace

#endif // WORKSPACESWITCH_H
