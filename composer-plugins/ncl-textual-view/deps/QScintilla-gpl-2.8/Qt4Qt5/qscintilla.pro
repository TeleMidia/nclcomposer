# The project file for the QScintilla library.
#
# Copyright (c) 2012 Riverbank Computing Limited <info@riverbankcomputing.com>
# 
# This file is part of QScintilla.
# 
# This file may be used under the terms of the GNU General Public
# License versions 2.0 or 3.0 as published by the Free Software
# Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
# included in the packaging of this file.  Alternatively you may (at
# your option) use any later version of the GNU General Public
# License if such license has been publicly approved by Riverbank
# Computing Limited (or its successors, if any) and the KDE Free Qt
# Foundation. In addition, as a special exception, Riverbank gives you
# certain additional rights. These rights are described in the Riverbank
# GPL Exception version 1.1, which can be found in the file
# GPL_EXCEPTION.txt in this package.
# 
# If you are unsure which license is appropriate for your use, please
# contact the sales department at sales@riverbankcomputing.com.
# 
# This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
# WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


# This must be kept in sync with Python/configure.py, Python/configure-old.py,
# example-Qt4Qt5/application.pro and designer-Qt4Qt5/designer.pro.
!win32:VERSION = 11.0.0

TEMPLATE = lib
TARGET = qscintilla2_telem
CONFIG += qt warn_off release dll thread silent

# Uses FORCERELEASE variable because CONFIG and SUBDIR force three executions
# if qmake and the last one does not preserves CONFIG from command line.
contains(FORCERELEASE, true) {
  CONFIG += qt warn_on release
  CONFIG -= debug
  DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_DEBUG_WARNING
  message ("qscintilla.pro RELEASE build!")
}
else {
  CONFIG += qt warn_on debug console
  CONFIG -= release
  message ("qscintilla.pro DEBUG build!")
}

INCLUDEPATH = . ../include ../lexlib ../src

# We use QMAKE_CXXFLAGS instead of INCLUDEPATH because our qscintilla is
# modified, and must be found before any other that is installed.
QMAKE_CXXFLAGS  += -I. -I../include -I../lexlib -I../src
DEFINES = QSCINTILLA_MAKE_DLL SCINTILLA_QT SCI_LEXER

greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets
	QT += printsupport

    greaterThan(QT_MINOR_VERSION, 1) {
	    macx:QT += macextras
    }
}

# Comment this in if you want the internal Scintilla classes to be placed in a
# Scintilla namespace rather than pollute the global namespace.
#DEFINES += SCI_NAMESPACE

mac {
  QSCI_INSTALL_LIBS = $$quote(/Library/Application Support/Composer/Extensions)
  QSCI_INSTALL_HEADERS = $$[QT_INSTALL_HEADERS]
  QSCI_INSTALL_TRANSLATIONS = $$[QT_INSTALL_HEADERS]
  QSCI_INSTALL_DATA = $$[QT_INSTALL_DATA]

  QMAKE_LFLAGS += -Wl,-install_name,'\'$$QSCI_INSTALL_LIBS/lib'$$TARGET'.dylib\''
}
else:unix {
  isEmpty(PREFIX) {
    PREFIX = /usr/local
  }

  QSCI_INSTALL_LIBS = $$PREFIX/lib/composer/extensions
  QSCI_INSTALL_HEADERS = $$PREFIX/include/composer
  QSCI_INSTALL_TRANSLATIONS = $$PREFIX/lib/composer/translations
  QSCI_INSTALL_DATA = $$PREFIX/lib/composer/
}
else:win32 {
  isEmpty(PREFIX) {
    PREFIX = "C:/Composer"
  }

  #TODO: This should be $$PREFIX/extensions, but on Windows we cannot link
  # against a lib inside the $$PREFIX/extensions path.
  QSCI_INSTALL_LIBS = $$PREFIX
  QSCI_INSTALL_HEADERS = $$PREFIX/include/composer
  #QSCI_INSTALL_TRANSLATIONS = $$PREFIX/translations
  QSCI_INSTALL_TRANSLATIONS = $$PREFIX/extensions
  QSCI_INSTALL_DATA = $$PREFIX/extensions
}

#QSCI_INSTALL_LIBS = $$[QT_INSTALL_LIBS]
#QSCI_INSTALL_HEADERS = $$[QT_INSTALL_HEADERS]
#QSCI_INSTALL_TRANSLATIONS = $$[QT_INSTALL_HEADERS]
#QSCI_INSTALL_DATA = $$[QT_INSTALL_DATA]

# Handle both Qt v4 and v3.
target.path = $$QSCI_INSTALL_LIBS
isEmpty(target.path) {
	target.path = $(QTDIR)/lib
}

header.path = $$QSCI_INSTALL_HEADERS
header.files = Qsci
isEmpty(header.path) {
   header.path = $(QTDIR)/include/Qsci
   header.files = Qsci/qsci*.h
}

trans.path = $$QSCI_INSTALL_TRANSLATIONS
trans.files = qscintilla_*.qm
isEmpty(trans.path) {
	trans.path = $(QTDIR)/translations
}

qsci.path = $$QSCI_INSTALL_DATA
qsci.files = ../qsci
isEmpty(qsci.path) {
	qsci.path = $(QTDIR)
}

INSTALLS += header trans qsci target

HEADERS = \
	./Qsci/qsciglobal.h \
	./Qsci/qsciscintilla.h \
	./Qsci/qsciscintillabase.h \
	./Qsci/qsciabstractapis.h \
	./Qsci/qsciapis.h \
	./Qsci/qscicommand.h \
	./Qsci/qscicommandset.h \
	./Qsci/qscidocument.h \
	./Qsci/qscilexer.h

contains(ALL_LEXERS, true) {
	HEADERS += \
	./Qsci/qscilexerbash.h \
	./Qsci/qscilexerbatch.h \
	./Qsci/qscilexercmake.h \
	./Qsci/qscilexercpp.h \
	./Qsci/qscilexercsharp.h \
	./Qsci/qscilexercss.h \
	./Qsci/qscilexercustom.h \
	./Qsci/qscilexerd.h \
	./Qsci/qscilexerdiff.h \
	./Qsci/qscilexerfortran.h \
	./Qsci/qscilexerfortran77.h \
	./Qsci/qscilexerhtml.h \
	./Qsci/qscilexeridl.h \
	./Qsci/qscilexerjava.h \
	./Qsci/qscilexerjavascript.h \
	./Qsci/qscilexerlua.h \
	./Qsci/qscilexermakefile.h \
	./Qsci/qscilexermatlab.h \
	./Qsci/qscilexeroctave.h \
	./Qsci/qscilexerpascal.h \
	./Qsci/qscilexerperl.h \
	./Qsci/qscilexerpostscript.h \
	./Qsci/qscilexerpov.h \
	./Qsci/qscilexerproperties.h \
	./Qsci/qscilexerpython.h \
	./Qsci/qscilexerruby.h \
	./Qsci/qscilexerspice.h \
	./Qsci/qscilexersql.h \
	./Qsci/qscilexertcl.h \
	./Qsci/qscilexertex.h \
	./Qsci/qscilexerverilog.h \
	./Qsci/qscilexervhdl.h \
	./Qsci/qscilexerxml.h \
	./Qsci/qscilexeryaml.h
}
else {
	HEADERS += \
		./Qsci/qscilexercustom.h \
		./Qsci/qscilexercpp.h \
		./Qsci/qscilexerpython.h \
		./Qsci/qscilexerjavascript.h \
		./Qsci/qscilexerhtml.h \
		./Qsci/qscilexerxml.h
}

HEADERS += \
	./Qsci/qscimacro.h \
	./Qsci/qsciprinter.h \
	./Qsci/qscistyle.h \
	./Qsci/qscistyledtext.h \
	ListBoxQt.h \
	SciClasses.h \
	SciNamespace.h \
	ScintillaQt.h \
	../include/ILexer.h \
	../include/Platform.h \
	../include/SciLexer.h \
	../include/Scintilla.h \
	../include/ScintillaWidget.h \
	../lexlib/Accessor.h \
	../lexlib/CharacterCategory.h \
	../lexlib/CharacterSet.h \
	../lexlib/LexAccessor.h \
	../lexlib/LexerBase.h \
	../lexlib/LexerModule.h \
	../lexlib/LexerNoExceptions.h \
	../lexlib/LexerSimple.h \
	../lexlib/OptionSet.h \
	../lexlib/PropSetSimple.h \
	../lexlib/StyleContext.h \
	../lexlib/SubStyles.h \
	../lexlib/WordList.h \
	../src/AutoComplete.h \
	../src/CallTip.h \
	../src/CaseConvert.h \
	../src/CaseFolder.h \
	../src/Catalogue.h \
	../src/CellBuffer.h \
	../src/CharClassify.h \
	../src/ContractionState.h \
	../src/Decoration.h \
	../src/Document.h \
	../src/Editor.h \
	../src/ExternalLexer.h \
	../src/FontQuality.h \
	../src/Indicator.h \
	../src/KeyMap.h \
	../src/LineMarker.h \
	../src/Partitioning.h \
	../src/PerLine.h \
	../src/PositionCache.h \
	../src/RESearch.h \
	../src/RunStyles.h \
	../src/ScintillaBase.h \
	../src/Selection.h \
	../src/SplitVector.h \
	../src/Style.h \
	../src/UnicodeFromUTF8.h \
	../src/UniConversion.h \
	../src/ViewStyle.h \
	../src/XPM.h

SOURCES = \
	qsciscintilla.cpp \
	qsciscintillabase.cpp \
	qsciabstractapis.cpp \
	qsciapis.cpp \
	qscicommand.cpp \
	qscicommandset.cpp \
	qscidocument.cpp \
	qscilexer.cpp

contains (ALL_LEXERS, true) {
	SOURCES += \
	qscilexerbash.cpp \
	qscilexerbatch.cpp \
	qscilexercmake.cpp \
	qscilexercpp.cpp \
	qscilexercsharp.cpp \
	qscilexercss.cpp \
	qscilexercustom.cpp \
	qscilexerd.cpp \
	qscilexerdiff.cpp \
	qscilexerfortran.cpp \
	qscilexerfortran77.cpp \
	qscilexerhtml.cpp \
	qscilexeridl.cpp \
	qscilexerjava.cpp \
	qscilexerjavascript.cpp \
	qscilexerlua.cpp \
	qscilexermakefile.cpp \
	qscilexermatlab.cpp \
	qscilexeroctave.cpp \
	qscilexerpascal.cpp \
	qscilexerperl.cpp \
	qscilexerpostscript.cpp \
	qscilexerpov.cpp \
	qscilexerproperties.cpp \
	qscilexerpython.cpp \
	qscilexerruby.cpp \
	qscilexerspice.cpp \
	qscilexersql.cpp \
	qscilexertcl.cpp \
	qscilexertex.cpp \
	qscilexerverilog.cpp \
	qscilexervhdl.cpp \
	qscilexerxml.cpp \
	qscilexeryaml.cpp
}
else {
	SOURCES += \
		qscilexercustom.cpp \
		qscilexercpp.cpp \
		qscilexerpython.cpp \
		qscilexerjavascript.cpp \
		qscilexerhtml.cpp \
		qscilexerxml.cpp
}

SOURCES += \
	qscimacro.cpp \
	qsciprinter.cpp \
	qscistyle.cpp \
	qscistyledtext.cpp \
    MacPasteboardMime.cpp \
    InputMethod.cpp \
	SciClasses.cpp \
	ListBoxQt.cpp \
	PlatQt.cpp \
	ScintillaQt.cpp

contains(ALL_LEXERS, true) {
	SOURCES += \
	../lexers/LexA68k.cpp \
	../lexers/LexAbaqus.cpp \
	../lexers/LexAda.cpp \
	../lexers/LexAPDL.cpp \
	../lexers/LexAsm.cpp \
	../lexers/LexAsn1.cpp \
	../lexers/LexASY.cpp \
	../lexers/LexAU3.cpp \
	../lexers/LexAVE.cpp \
	../lexers/LexAVS.cpp \
	../lexers/LexBaan.cpp \
	../lexers/LexBash.cpp \
	../lexers/LexBasic.cpp \
	../lexers/LexBullant.cpp \
	../lexers/LexCaml.cpp \
	../lexers/LexCLW.cpp \
	../lexers/LexCmake.cpp \
	../lexers/LexCOBOL.cpp \
	../lexers/LexCoffeeScript.cpp \
	../lexers/LexConf.cpp \
	../lexers/LexCPP.cpp \
	../lexers/LexCrontab.cpp \
	../lexers/LexCsound.cpp \
	../lexers/LexCSS.cpp \
	../lexers/LexD.cpp \
	../lexers/LexECL.cpp \
	../lexers/LexEiffel.cpp \
	../lexers/LexErlang.cpp \
	../lexers/LexEScript.cpp \
	../lexers/LexFlagship.cpp \
	../lexers/LexForth.cpp \
	../lexers/LexFortran.cpp \
	../lexers/LexGAP.cpp \
	../lexers/LexGui4Cli.cpp \
	../lexers/LexHaskell.cpp \
	../lexers/LexHTML.cpp \
	../lexers/LexInno.cpp \
	../lexers/LexKix.cpp \
	../lexers/LexKVIrc.cpp \
	../lexers/LexLaTex.cpp \
	../lexers/LexLisp.cpp \
	../lexers/LexLout.cpp \
	../lexers/LexLua.cpp \
	../lexers/LexMagik.cpp \
	../lexers/LexMarkdown.cpp \
	../lexers/LexMatlab.cpp \
	../lexers/LexMetapost.cpp \
	../lexers/LexMMIXAL.cpp \
	../lexers/LexModula.cpp \
	../lexers/LexMPT.cpp \
	../lexers/LexMSSQL.cpp \
	../lexers/LexMySQL.cpp \
	../lexers/LexNimrod.cpp \
	../lexers/LexNsis.cpp \
	../lexers/LexOpal.cpp \
	../lexers/LexOScript.cpp \
	../lexers/LexOthers.cpp \
	../lexers/LexPascal.cpp \
	../lexers/LexPB.cpp \
	../lexers/LexPerl.cpp \
	../lexers/LexPLM.cpp \
	../lexers/LexPO.cpp \
	../lexers/LexPOV.cpp \
	../lexers/LexPowerPro.cpp \
	../lexers/LexPowerShell.cpp \
	../lexers/LexProgress.cpp \
	../lexers/LexPS.cpp \
	../lexers/LexPython.cpp \
	../lexers/LexR.cpp \
	../lexers/LexRebol.cpp \
	../lexers/LexRuby.cpp \
	../lexers/LexRust.cpp \
	../lexers/LexScriptol.cpp \
	../lexers/LexSmalltalk.cpp \
	../lexers/LexSML.cpp \
	../lexers/LexSorcus.cpp \
	../lexers/LexSpecman.cpp \
	../lexers/LexSpice.cpp \
	../lexers/LexSQL.cpp \
	../lexers/LexSTTXT.cpp \
	../lexers/LexTACL.cpp \
	../lexers/LexTADS3.cpp \
	../lexers/LexTAL.cpp \
	../lexers/LexTCL.cpp \
	../lexers/LexTCMD.cpp \
	../lexers/LexTeX.cpp \
	../lexers/LexTxt2tags.cpp \
	../lexers/LexVB.cpp \
	../lexers/LexVerilog.cpp \
	../lexers/LexVHDL.cpp \
	../lexers/LexVisualProlog.cpp \
	../lexers/LexYAML.cpp
}
else {
	SOURCES += \
		../lexers/LexPython.cpp \
		../lexers/LexCPP.cpp \
		../lexers/LexHTML.cpp
}

SOURCES += \
	../lexlib/Accessor.cpp \
	../lexlib/CharacterCategory.cpp \
	../lexlib/CharacterSet.cpp \
	../lexlib/LexerBase.cpp \
	../lexlib/LexerModule.cpp \
	../lexlib/LexerNoExceptions.cpp \
	../lexlib/LexerSimple.cpp \
	../lexlib/PropSetSimple.cpp \
	../lexlib/StyleContext.cpp \
	../lexlib/WordList.cpp \
	../src/AutoComplete.cpp \
	../src/CallTip.cpp \
	../src/CaseConvert.cpp \
	../src/CaseFolder.cpp \
	../src/Catalogue.cpp \
	../src/CellBuffer.cpp \
	../src/CharClassify.cpp \
	../src/ContractionState.cpp \
	../src/Decoration.cpp \
	../src/Document.cpp \
	../src/Editor.cpp \
	../src/ExternalLexer.cpp \
	../src/Indicator.cpp \
    ../src/KeyMap.cpp \
	../src/LineMarker.cpp \
	../src/PerLine.cpp \
	../src/PositionCache.cpp \
    ../src/RESearch.cpp \
	../src/RunStyles.cpp \
    ../src/ScintillaBase.cpp \
    ../src/Selection.cpp \
	../src/Style.cpp \
	../src/UniConversion.cpp \
	../src/ViewStyle.cpp \
	../src/XPM.cpp

TRANSLATIONS = \
	qscintilla_cs.ts \
	qscintilla_de.ts \
	qscintilla_es.ts \
	qscintilla_fr.ts \
	qscintilla_pt_br.ts \
	qscintilla_ru.ts
