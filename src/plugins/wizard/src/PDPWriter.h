#ifndef PDPWRITER_H
#define PDPWRITER_H

#include <QDomDocument>

#include "Constants.h"
#include "SelectsParser.h"

class PDPWriter
{
public:
  static void writePDP (QString);

private:
  static QDomElement parseGapElement (const QDomElement&, QDomDocument &);

  static int _mediaCount;
  static int _contextCount;
  static int _switchCount;
};

#endif // PDPWRITER_H
