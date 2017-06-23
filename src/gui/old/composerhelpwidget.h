/* Copyright (C) 2011-2017 PUC-Rio/TeleMídia Lab.

 NCL Composer is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 NCL Composer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU General Lesser Public License
 along with NCL Composer.  If not, see <http://www.gnu.org/licenses/>. */

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
