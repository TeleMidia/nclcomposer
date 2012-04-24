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

#include <QtHelp/QHelpEngine>
#include <QtHelp/QHelpIndexWidget>
#include <QtHelp/QHelpContentWidget>

#include <QTimer>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QWebView>

namespace composer {
namespace gui {

/*!
 * The HelpBrowser will shows the HTML content of the current selected Help.
 */
class HelpBrowser : public QWebView
{
  Q_OBJECT

public:
  explicit HelpBrowser(QHelpEngine &helpEngine, QWidget *parent = 0);

public Q_SLOTS:
  virtual void setSource(const QUrl &name);

private:
  QHelpEngine &helpEngine;

};

/*!
 * A widget to handle NCL Composer Help.
 */
class ComposerHelpWidget : public QWidget
{
  Q_OBJECT

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
