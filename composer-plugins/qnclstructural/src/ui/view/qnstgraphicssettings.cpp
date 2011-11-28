#include "qnstgraphicssettings.h"

QnstGraphicsSettings::QnstGraphicsSettings(QnstGraphicsNode* parent)
    : QnstGraphicsMedia(parent)
{
    setnstType(Qnst::Settings);

    setIcon(":/icon/settings");
}

QnstGraphicsSettings::~QnstGraphicsSettings()
{

}
