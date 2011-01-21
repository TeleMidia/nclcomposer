#ifndef PREFERENCESPAGE_H
#define PREFERENCESPAGE_H

#include <QObject>
#include <QIcon>
#include <QtPlugin>

class IPreferencesPage
{
public:
    virtual ~IPreferencesPage() {}

    virtual QString getTitle() = 0;
    virtual QIcon getIcon() = 0;
    virtual QWidget* getPageWidget() = 0;
    virtual void setDefaultValues() = 0;
    virtual void applyValues() = 0;
};

Q_DECLARE_INTERFACE (IPreferencesPage, "br.puc-rio.telemidia.composer.IPreferencesPage");

#endif // IPREFERENCESPAGE_H
