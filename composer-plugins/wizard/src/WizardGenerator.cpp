#include "WizardGenerator.h"

#include "PDPWriter.h"
#include "WSWriter.h"

void WizardGenerator::run(const QString &path)
{
  QVector <QString> auxPaths = TemplateParser::parse(path);
  foreach (QString path, auxPaths)
  {
    PDPWriter::writePDP(path);
    WSWriter::writeWizard(path);
  }
}
