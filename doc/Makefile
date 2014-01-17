# Makefile for NCL Composer Tutorial
#
# Dependencies:
#   * pandoc

PANDOC_BIN=pandoc
OUTPUT=HTML 
# HTML_BOOTSTRAP CHUNKED_HTML
CSS=http://jasonm23.github.io/markdown-css-themes/markdown.css
#CSS=http://s3.jfh.me/css/john-full.css
#CSS=http://kevinburke.bitbucket.org/markdowncss/markdown.css

OUT=book

all: $(OUTPUT)

clean:
	rm -r html

HTML:
	mkdir html
	ls -d pt-br/*.md | xargs pandoc --toc -o ./html/$(OUT)_pt-br.html --css=$(CSS)

HTML_BOOTSTRAP:
	ls -d pt-br/*.md | xargs pandoc --toc --toc-depth 2 --css html_templates/css/bootstrap.css --template html_templates/pandoc-template.html5 --self-contained -o ./html/$(OUT)_pt-br_bootstrap.html --css=$(CSS)
  
CHUNKED_HTML:
	ls -d pt-br/* | xargs pandoc -s -S -t docbook -o pt-br/$(OUT).db --css=$(CSS)
	xmlto xhtml -m config.xsl pt-br/$(OUT).db pt-br/html/

