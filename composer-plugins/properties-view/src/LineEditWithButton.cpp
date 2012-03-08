/*
 * Copyright 2011-2012 TeleMidia/PUC-Rio.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include "LineEditWithButton.h"
#include <QToolButton>
#include <QStyle>

LineEditWithButton::LineEditWithButton(QWidget *parent,
                                       const QString &iconPath) :
  QLineEdit(parent), iconPath(iconPath)
{
  // Create the search button and set its icon, cursor, and stylesheet
  this->mButton = new QToolButton(this);
  this->mButton->setIcon(QIcon(iconPath));
//  this->mButton->setFixedSize(18, 18);
  this->mButton->setCursor(Qt::ArrowCursor);
  this->mButton->setStyleSheet(this->buttonStyleSheetForCurrentState());

  // Some stylesheet and size corrections for the text box
  this->setStyleSheet(this->styleSheetForCurrentState());

//  int frameWidth = this->style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
//  QSize minSizeHint = this->minimumSizeHint();
//  this->setMinimumSize(
//        qMax(minSizeHint.width(),
//             this->mButton->sizeHint().width() + frameWidth * 2 + 2),
//        qMax(minSizeHint.height(),
//             this->mButton->sizeHint().height() + frameWidth * 2 + 2));

  QObject::connect(this->mButton, SIGNAL(clicked()), SIGNAL(buttonPressed()));
}

void LineEditWithButton::resizeEvent(QResizeEvent *event)
{
  Q_UNUSED(event);
  QSize size = this->mButton->sizeHint();
  int frameWidth = this->style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
  this->mButton->move(
        this->rect().right() - frameWidth - size.width() - 2,
        (this->rect().bottom() + 2 - size.height()) / 2);
}

QString LineEditWithButton::styleSheetForCurrentState() const
{
  int frameWidth = this->style()->pixelMetric(QStyle::PM_DefaultFrameWidth);

  QString style;
  style += "QLineEdit {";
  /* if (this->text().isEmpty())
  { */
//    style += "font-family: 'MS Sans Serif';";
//    style += "font-style: italic;";
    style += "font-size: 11px;";
  // }

//  style += "padding-left: 3px;";
  style += QString("padding-right: %1px;").arg(
        this->mButton->sizeHint().width() + frameWidth + 1);
  /*
  style += "border-width: 3px;";
  style += "border-image: url( " + iconPath + ") 3 3 3 3 stretch;";
  style += "background-color: rgba(255, 255, 255, 204);";
  style += "}";
  style += "QLineEdit:hover, QLineEdit:focus {";
  style += "background-color: rgba(255, 255, 255, 255);"; */
  style += "}";
  return style;
}

QString LineEditWithButton::buttonStyleSheetForCurrentState() const
{
  QString style;
  /*style += "QToolButton {";
  style += "border: none; margin: 0; padding: 0;";
  style += QString("background-image: " + iconPath + ";");
      arg(this->text().isEmpty() ? "search" : "clear");

  style += "}";

  if (!this->text().isEmpty())
  {
    style += "QToolButton:hover { "
        "background-image: url(:/images/esf-clear-hover.png); "
        "}";
    style += "QToolButton:pressed { "
        "background-image: url(:/images/esf-clear-active.png); "
        "}";
  }*/

  return style;
}
