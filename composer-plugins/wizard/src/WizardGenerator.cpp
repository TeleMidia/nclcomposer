#include "wizardgenerator.h"

#include "pdpwriter.h"
#include "wswriter.h"

void WizardGenerator::run(QString path)
{
  QVector <QString> auxPaths = TemplateParser::parse(path);
  foreach (QString path, auxPaths)
  {
    PDPWriter::writePDP(path);
    WSWriter::writeWizard(path);
  }
}
