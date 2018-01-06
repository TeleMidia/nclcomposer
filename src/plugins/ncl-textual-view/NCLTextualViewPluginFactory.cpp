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
#include "NCLTextualViewPluginFactory.h"
#include "ui_TextualPluginPreferencesWidget.h"

#include "util/Preferences.h"

NCLTextualViewPluginFactory::NCLTextualViewPluginFactory ()
{
  _prefPageWidget = nullptr;

  Preferences *prefs = Preferences::instance ();
  prefs->registerPreferences (
      { { "cpr.textual.bgCaretLine",
          new Preference ("bgCaretLine", QColor ("#B9D3EE"), "Textual") },
        { "cpr.textual.caretLineVisible",
          new Preference ("caretLineVisible", true, "Textual") },
        { "cpr.textual.bgMarginColor",
          new Preference ("bgMarginColor", QColor ("#ffffff"), "Textual") },
        { "cpr.textual.marginForeColor",
          new Preference ("marginForeColor", QColor ("#B9D3EE"), "Textual") },
        { "cpr.textual.marginBackColor",
          new Preference ("marginBackColor", QColor ("#FFFFFF"), "Textual") },
        { "cpr.textual.fontSize", new Preference ("fontSize", 10, "Textual") },
        { "cpr.textual.tabWidth", new Preference ("tabWidth", 2, "Textual") },
        { "cpr.textual.whitespaceVisibility",
          new Preference ("whitespaceVisibility", false, "Textual") },
        { "cpr.textual.edgeMode",
          new Preference ("edgeMode", false, "Textual") },
        { "cpr.textual.edgeColumn",
          new Preference ("edgeColumn", "80", "Textual") } });
}
