#ifndef PDPWRITER_H
#define PDPWRITER_H

#include <QDomDocument>

#include "constants.h"
#include "selectsparser.h"

class PDPWriter
{
private:
    static QDomElement parseGapElement (QDomElement&, QDomDocument &);
    static int mediaCount;
    static int contextCount;
    static int switchCount;

public:
    static void writePDP (QString);
};

#endif // PDPWRITER_H
