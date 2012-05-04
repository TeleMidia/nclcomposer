#include "qnstgraphicsbinddialog.h"

QnstGraphicsBindDialog::QnstGraphicsBindDialog(QWidget* parent)
    : QDialog(parent)
{
    form.setupUi(this);
}

QnstGraphicsBindDialog::~QnstGraphicsBindDialog()
{

}


void QnstGraphicsBindDialog::init(QMap<QString, QString> params)
{
    int ncol = 2;
    int nrow = params.size();

    form.table->verticalHeader()->hide();
    form.table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    QStandardItemModel *model = new QStandardItemModel(nrow, ncol);

    model->setHorizontalHeaderItem(0, new QStandardItem("Name"));
    model->setHorizontalHeaderItem(1, new QStandardItem("Value"));

    int i = 0;

    foreach(QString name, params.keys()){
        QStandardItem* nitem = new QStandardItem(name);
        nitem->setEditable(false);

        QStandardItem* vitem = new QStandardItem(params[name]);

        model->setItem(i, 0, nitem);
        model->setItem(i, 1, vitem);

        ++i;
    }

    form.table->setModel(model);
}

QMap<QString, QString> QnstGraphicsBindDialog::getProperties()
{
    QMap<QString, QString> p;

    QAbstractItemModel* model = form.table->model();

    int nrow = model->rowCount();

    for (int i=0; i<nrow; ++i){
        p[model->data(model->index(i,0)).toString()] =
                model->data(model->index(i,1)).toString();

        qDebug() << model->data(model->index(i,0)).toString();
        qDebug() << model->data(model->index(i,1)).toString();
    }

    return p;
}
