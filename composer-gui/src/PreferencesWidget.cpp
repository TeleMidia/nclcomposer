#include "include/PreferencesWidget.h"
#include "ui_PreferencesWidget.h"
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE

PreferencesWidget::PreferencesWidget(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::PreferencesWidget)
{
    ui->setupUi(this);
    this->setModal(true);
    loadPreferencesPages();
}

PreferencesWidget::~PreferencesWidget()
{
    delete ui;
}

bool PreferencesWidget::loadPreferencesPages(){
    QString pluginsDirStr = QCoreApplication::applicationDirPath() + "/plugins";
    QDir pluginsDir = QDir(pluginsDirStr);

    if(!pluginsDir.exists()) {
        qDebug() << tr("The Language Profile extension "
                       "directory (%1) does not exist!").arg(pluginsDirStr);
        return false;
    }

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {

        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *qObject = loader.instance();
        IPreferencesPage *prefPage;

        if (qObject) {
            prefPage = qobject_cast<IPreferencesPage*> (qObject);
            if (prefPage) {
                QString title = prefPage->getTitle();
                QIcon icon = prefPage->getIcon();

                if (pages.contains(title)) {
                    qDebug() << "Plugin already loaded";
                } else {
                    pages[title] = prefPage;
                    new QListWidgetItem(icon, title, ui->listWidget, 0);
                    QWidget *page = prefPage->getPageWidget();
                    ui->scrollAreaVerticalLayout->addWidget(page);
                }
            } else {
                qDebug() << tr("ERROR: Preference Page Extension (%1)").
                                 arg(fileName);
                return false;
            }
        } else {
            qDebug() << tr("ERROR: Language Extension (%1)").
                             arg(fileName);
            return false;
        }
    }
    return true;
}
QT_END_NAMESPACE
