#ifndef VALIDATORPLUGIN_H
#define VALIDATORPLUGIN_H

#include <core/extensions/IPlugin.h>
#include <QWidget>
#include <QVector>
#include <QHBoxLayout>
#include "validatortreewidgetitem.h"
#include "nclmodel/nclmodel.h"
#include "validation/Validator.h"
#include "composerncladapter.h"

#include <QSize>

using namespace composer::extension;

namespace composer{
    namespace plugin{
        namespace validator{

class ValidatorTreeWidget :
    public QTreeWidget
{
public:
  ValidatorTreeWidget(QWidget *parent=0) : QTreeWidget(parent)
  {
  }

  QSize sizeHint () const
  {
    return QSize(900, 200);
  }
};

class ValidatorPlugin : public IPlugin
{
  Q_OBJECT
public:
  ValidatorPlugin();

  QWidget* getWidget() { return table; }
  bool saveSubsession() {return true;}
  void init();

public slots:
  void onEntityAdded(QString ID, Entity *);
  void onEntityChanged(QString ID, Entity *);
  void onEntityRemoved(QString ID, QString entityID);

  void errorMessage(QString error);

  void itemSelected (QTreeWidgetItem *);
  void itemDoubleClickedSelected (QTreeWidgetItem *);

//Composer Messages
  void clearValidationMessages (QString, void *);
  void validationError (QString, void *);
  void validationErrorSelected (QString, void *);
  void askAllValidationMessages (QString, void *);


private:
  void updateModel(Entity *);
  void updateMessages (std::vector<pair<void *, string> >);


  QVector <pair<QString, QString> > pairsMessages;

  QWidget* window;
  ValidatorTreeWidget *table;
  ComposerNCLAdapter adapter;

};

        }
    }
}


#endif // VALIDATORPLUGIN_H
