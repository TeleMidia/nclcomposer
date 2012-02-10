/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef COMPOSERHELPWIDGET_H
#define COMPOSERHELPWIDGET_H

#include <QWidget>
#include <QTextBrowser>
#include <QtHelp/QHelpEngine>
#include <QtHelp/QHelpIndexWidget>
#include <QtHelp/QHelpContentWidget>

namespace composer {
namespace gui {

class HelpBrowser : public QTextBrowser
{
public:
  explicit HelpBrowser(QHelpEngine &helpEngine, QWidget *parent = 0);

protected:
  virtual QVariant loadResource(int type, const QUrl &url);

private:
  QHelpEngine &helpEngine;

};

class ComposerHelpWidget : public QWidget
{
public:
  explicit ComposerHelpWidget(QWidget *parent = 0);

protected:
  void retranslateUi();

private:
  QHelpEngine helpEngine;
  HelpBrowser helpBrowser;
};

} }  // end namespace
#endif // COMPOSERHELPWIDGET_H
