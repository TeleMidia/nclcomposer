# Makefile for NCL Composer Tutorial
#
# Dependencies:
#   * pandoc

PANDOC_BIN=pandoc
OUTPUT=HTML CHUNKED_HTML
CSS=http://jasonm23.github.io/markdown-css-themes/markdown.css
#CSS=http://s3.jfh.me/css/john-full.css
#CSS=http://kevinburke.bitbucket.org/markdowncss/markdown.css

OUT=book

all: $(OUTPUT)

clean:
	rm pt-br/*.html

HTML:
	ls -d pt-br/*.md | xargs pandoc --toc -o pt-br/$(OUT).html --css=$(CSS)
  
CHUNKED_HTML:
	ls -d pt-br/* | xargs pandoc -s -S -t docbook -o pt-br/$(OUT).db --css=$(CSS)
	xmlto xhtml -m config.xsl pt-br/$(OUT).db pt-br/html/ 
