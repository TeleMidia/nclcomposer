import string
from markdown.preprocessors import Preprocessor
from markdown.extensions import Extension
import mydict

class MyExtension(Extension):
  def extendMarkdown(self, md, md_globals):
    md.preprocessors.add( 'replace_strings', MyPreprocessor(md), '_begin')
 
class MyPreprocessor(Preprocessor):
  def run(self, lines):
    new_lines = []
    for line in lines:
      new_line = line
      for key in mydict.mydict:
        new_line = new_line.replace ('{' + key +'}', mydict.mydict[key])
      new_lines.append (new_line)
    return new_lines

def makeExtension(configs={}):
    return MyExtension(configs=configs)

