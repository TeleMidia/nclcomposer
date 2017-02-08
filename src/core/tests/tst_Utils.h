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

#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include <QStringList>

class TestUtil
{
public:
  /* This function doesn't seem correct, using timeval_subtract insteads */
  static timeval diff(timeval start, timeval end)
  {
    timeval temp;

    if ((end.tv_usec-start.tv_usec)<0) {
      temp.tv_sec = end.tv_sec-start.tv_sec-1;
      temp.tv_usec = 1000000+end.tv_usec-start.tv_usec;
    } else {
      temp.tv_sec = end.tv_sec-start.tv_sec;
      temp.tv_usec = end.tv_usec-start.tv_usec;
    }
    return temp;
  }

  static long long int timeval_subtract_micro (struct timeval start,
                                               struct timeval stop)
  {
    long long int result = ((long long int)(stop.tv_sec - start.tv_sec))*1000000;
    result += stop.tv_usec - start.tv_usec;

    return result;
  }

  static QStringList getPluginList()
  {
    QString pluginDir;
    QStringList pluginList;

#ifdef Q_WS_MAC
    pluginDir = "/Library/Application Support/Composer/";
    pluginList << (pluginDir + "libncl_textual_plugin.dylib");
    pluginList << (pluginDir + "libdebug_console.dylib");
    pluginList << (pluginDir + "liboutline_view.dylib");
    pluginList << (pluginDir + "libproperties_view.dylib");
    pluginList << (pluginDir + "libQnly.dylib");
    pluginList << (pluginDir + "libQnst.dylib");
#elif WIN32
    //TODO:
#else
    pluginDir = "/usr/local/lib/composer/extensions/";
//    pluginList << ("");
//    pluginList << (pluginDir + "libdebug_console.so");
//    pluginList << (pluginDir + "libproperties_view.so");
//    pluginList << (pluginDir + "libQnly.so");
    pluginList << (pluginDir + "libQnst.so");
//    pluginList << (pluginDir + "libncl_textual_plugin.so");
//    pluginList << (pluginDir + "liboutline_view.so");
#endif

    return pluginList;
  }
};

#endif // TIMEUTILS_H
