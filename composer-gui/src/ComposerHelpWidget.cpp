/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */

#include "ComposerHelpWidget.h"

#include <QBoxLayout>

namespace composer {
namespace gui {

HelpBrowser::HelpBrowser(QHelpEngine &helpEngine, QWidget *parent)
  : QTextBrowser(parent), helpEngine(helpEngine)
{

}

QVariant HelpBrowser::loadResource(int type, const QUrl &url)
{
  if (url.scheme() == "qthelp")
    return QVariant(helpEngine.fileData(url));
  else
    return QTextBrowser::loadResource(type, url);
}


ComposerHelpWidget::ComposerHelpWidget(QWidget *parent)
  : helpEngine("../user_manual/html/nclcomposer.qhc", parent),
    helpBrowser(helpEngine, parent)
{
  QBoxLayout *hbox = new QHBoxLayout(this);
  helpEngine.contentWidget()->setMaximumWidth(200);
  hbox->addWidget(helpEngine.contentWidget());
//  hbox->addWidget(helpEngine.indexWidget());
  hbox->addWidget(&helpBrowser);
  setLayout(hbox);

  helpEngine.setupData();

  connect(helpEngine.contentWidget(), SIGNAL(linkActivated(const QUrl &)),
          &helpBrowser, SLOT(setSource(const QUrl &)));

  retranslateUi();
}

void ComposerHelpWidget::retranslateUi()
{
  setWindowTitle(tr("NCL Composer Help"));
}

} } // end namespace
