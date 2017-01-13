cmake_minimum_required (VERSION 3.2)
project(qscintilla2_telem)

set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

find_package (Qt5Core REQUIRED)
find_package (Qt5PrintSupport REQUIRED)
find_package (Qt5Widgets REQUIRED)

set (QSCINTILLA_SOURCES
  ./Qsci/qsciglobal.h
  ./Qsci/qsciscintilla.h
  ./Qsci/qsciscintillabase.h
  ./Qsci/qsciabstractapis.h
  ./Qsci/qsciapis.h
  ./Qsci/qscicommand.h
  ./Qsci/qscicommandset.h
  ./Qsci/qscidocument.h
  ./Qsci/qscilexer.h
  
  ./Qsci/qscilexercustom.h
  ./Qsci/qscilexercpp.h
  ./Qsci/qscilexerpython.h
  ./Qsci/qscilexerjavascript.h
  ./Qsci/qscilexerhtml.h
  ./Qsci/qscilexerxml.h
  ./Qsci/qscilexerlua.h
  
  ./Qsci/qscimacro.h
  ./Qsci/qsciprinter.h
  ./Qsci/qscistyle.h
  ./Qsci/qscistyledtext.h
  ListBoxQt.h
  SciClasses.h
  SciNamespace.h
  ScintillaQt.h
  ../include/ILexer.h
  ../include/Platform.h
  ../include/SciLexer.h
  ../include/Scintilla.h
  ../include/ScintillaWidget.h
  ../lexlib/Accessor.h
  ../lexlib/CharacterCategory.h
  ../lexlib/CharacterSet.h
  ../lexlib/LexAccessor.h
  ../lexlib/LexerBase.h
  ../lexlib/LexerModule.h
  ../lexlib/LexerNoExceptions.h
  ../lexlib/LexerSimple.h
  ../lexlib/OptionSet.h
  ../lexlib/PropSetSimple.h
  ../lexlib/StringCopy.h
  ../lexlib/StyleContext.h
  ../lexlib/SubStyles.h
  ../lexlib/WordList.h
  ../src/AutoComplete.h
  ../src/CallTip.h
  ../src/CaseConvert.h
  ../src/CaseFolder.h
  ../src/Catalogue.h
  ../src/CellBuffer.h
  ../src/CharClassify.h
  ../src/ContractionState.h
  ../src/Decoration.h
  ../src/Document.h
  ../src/EditModel.h
  ../src/Editor.h
  ../src/EditView.h
  ../src/ExternalLexer.h
  ../src/FontQuality.h
  ../src/Indicator.h
  ../src/KeyMap.h
  ../src/LineMarker.h
  ../src/MarginView.h
  ../src/Partitioning.h
  ../src/PerLine.h
  ../src/PositionCache.h
  ../src/RESearch.h
  ../src/RunStyles.h
  ../src/ScintillaBase.h
  ../src/Selection.h
  ../src/SplitVector.h
  ../src/Style.h
  ../src/UnicodeFromUTF8.h
  ../src/UniConversion.h
  ../src/ViewStyle.h
  ../src/XPM.h
  
  qsciscintilla.cpp
  qsciscintillabase.cpp
  qsciabstractapis.cpp
  qsciapis.cpp
  qscicommand.cpp
  qscicommandset.cpp
  qscidocument.cpp
  qscilexer.cpp
  
  qscilexercustom.cpp
  qscilexercpp.cpp
  qscilexerpython.cpp
  qscilexerjavascript.cpp
  qscilexerhtml.cpp
  qscilexerxml.cpp
  qscilexerlua.cpp
  
  qscimacro.cpp
  qsciprinter.cpp
  qscistyle.cpp
  qscistyledtext.cpp
  MacPasteboardMime.cpp
  InputMethod.cpp
  SciClasses.cpp
  ListBoxQt.cpp
  PlatQt.cpp
  ScintillaQt.cpp
  
  ../lexers/LexPython.cpp
  ../lexers/LexCPP.cpp
  ../lexers/LexHTML.cpp
  ../lexers/LexLua.cpp
  
  ../lexlib/Accessor.cpp
  ../lexlib/CharacterCategory.cpp
  ../lexlib/CharacterSet.cpp
  ../lexlib/LexerBase.cpp
  ../lexlib/LexerModule.cpp
  ../lexlib/LexerNoExceptions.cpp
  ../lexlib/LexerSimple.cpp
  ../lexlib/PropSetSimple.cpp
  ../lexlib/StyleContext.cpp
  ../lexlib/WordList.cpp
  ../src/AutoComplete.cpp
  ../src/CallTip.cpp
  ../src/CaseConvert.cpp
  ../src/CaseFolder.cpp
  ../src/Catalogue.cpp
  ../src/CellBuffer.cpp
  ../src/CharClassify.cpp
  ../src/ContractionState.cpp
  ../src/Decoration.cpp
  ../src/Document.cpp
  ../src/EditModel.cpp
  ../src/Editor.cpp
  ../src/EditView.cpp
  ../src/ExternalLexer.cpp
  ../src/Indicator.cpp
  ../src/KeyMap.cpp
  ../src/LineMarker.cpp
  ../src/MarginView.cpp
  ../src/PerLine.cpp
  ../src/PositionCache.cpp
  ../src/RESearch.cpp
  ../src/RunStyles.cpp
  ../src/ScintillaBase.cpp
  ../src/Selection.cpp
  ../src/Style.cpp
  ../src/UniConversion.cpp
  ../src/ViewStyle.cpp
  ../src/XPM.cpp
)

add_library (qscintilla2_telem
  SHARED
  ${QSCINTILLA_SOURCES}
)

target_compile_definitions(qscintilla2_telem PRIVATE
  QSCINTILLA_MAKE_DLL
  SCINTILLA_QT
  SCI_LEXER
)

target_include_directories(qscintilla2_telem
  PUBLIC . ../include ../lexlib ../src
)

target_link_libraries (qscintilla2_telem
  Qt5::Core
  Qt5::PrintSupport
  Qt5::Widgets
)

install (TARGETS qscintilla2_telem DESTINATION ${NCLCOMPOSER_BINARY_DIR_PLUGINS})
