#ifndef NCLVALIDATOR_H
#define NCLVALIDATOR_H

#include <vector>
using namespace std;

#include <QString>
#include <QFile>

class NCLValidator
{
private:
    vector <QString> errorMessages;
    vector <QString> warningMessages;

public:
    NCLValidator();
    bool validate (const QFile file);
    bool validate (const QString &text);

    bool begin();
    QString getNextErrorMessage();
    QString getNextWarningMessage();
};

#endif // NCLVALIDATOR_H
