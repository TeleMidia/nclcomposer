/*
 * Copyright (C) 2011-2018 TeleMidia/PUC-Rio.
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
#include "SearchLineEdit.h"
#include <QStyle>
#include <QToolButton>

#include <QDebug>

SearchLineEdit::SearchLineEdit (QWidget *parent) : QLineEdit (parent)
{
  // Create the search button and set its icon, cursor, and stylesheet
  this->_mSearchButton = new QToolButton (this);
  this->_mSearchButton->setFixedSize (18, 18);
  this->_mSearchButton->setCursor (Qt::ArrowCursor);
  this->_mSearchButton->setStyleSheet (
      this->buttonStyleSheetForCurrentState ());

  // Update the search button when the text changes
  QObject::connect (this, SIGNAL (textChanged (const QString &)),
                    SLOT (updateSearchButton (const QString &)));

  // Some stylesheet and size corrections for the text box
  this->setPlaceholderText (tr ("Search"));

  /* this->setMinimumSize(
        qMax(minSizeHint.width(),
             this->_mSearchButton->sizeHint().width() + frameWidth * 2 + 2),
        qMax(minSizeHint.height(),
             this->_mSearchButton->sizeHint().height() + frameWidth * 2 + 2));
     */

  this->_mSearchButton->setVisible (false);
}

void
SearchLineEdit::resizeEvent (QResizeEvent *event)
{
  Q_UNUSED (event);
  QSize size = this->_mSearchButton->sizeHint ();
  int frameWidth = this->style ()->pixelMetric (QStyle::PM_DefaultFrameWidth);
  this->_mSearchButton->move (
      this->rect ().right () - frameWidth - size.width () - 2,
      (this->rect ().bottom () + 2 - size.height ()) / 2);
}

void
SearchLineEdit::updateSearchButton (const QString &text)
{
  if (!text.isEmpty ())
  {
    // We have some text in the box - set the button to clear the text
    QObject::connect (this->_mSearchButton, SIGNAL (clicked ()),
                      SLOT (clear ()));
  }
  else
  {
    // The text box is empty - make the icon do nothing when clicked
    QObject::disconnect (this->_mSearchButton, SIGNAL (clicked ()), this,
                         SLOT (clear ()));
  }

  this->_mSearchButton->setStyleSheet (
      this->buttonStyleSheetForCurrentState ());
}

QString
SearchLineEdit::buttonStyleSheetForCurrentState () const
{
  QString style;
  style += "QToolButton {";
  style += "border: none; margin: 0; padding: 0;";
  style += QString ("background-image: url(:/images/esf-%1.png);")
               .arg (this->text ().isEmpty () ? "search" : "clear");

  style += "}";

  if (!this->text ().isEmpty ())
  {
    style += "QToolButton:hover { "
             "background-image: url(:/images/esf-clear-hover.png); "
             "}";
    style += "QToolButton:pressed { "
             "background-image: url(:/images/esf-clear-active.png); "
             "}";
  }

  return style;
}

void
SearchLineEdit::keyPressEvent (QKeyEvent *event)
{
  if (event->key () == Qt::Key_Escape)
  {
    emit escPressed ();
  }
  else if (event->key () == Qt::Key_Return)
  {
    if (event->modifiers () & Qt::ShiftModifier)
      emit shiftReturnPressed ();
    else
      emit returnPressed ();
  }
  else
  {
    QLineEdit::keyPressEvent (event);
  }
}
