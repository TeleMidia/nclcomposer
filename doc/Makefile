# Makefile for NCL Composer Tutorial
#
# Dependencies:
#   * pandoc

PANDOC_BIN=pandoc
OUTPUT=PRE HTML PDF
# HTML_BOOTSTRAP CHUNKED_HTML
CSS=http://jasonm23.github.io/markdown-css-themes/markdown.css
#CSS=http://s3.jfh.me/css/john-full.css
#CSS=http://kevinburke.bitbucket.org/markdowncss/markdown.css

OUT=book
LANG=pt-br
INPUT=$(wildcard ${LANG}/*.md)
BUILD_DIR=.build

all: $(OUTPUT)

clean:
	rm -rf html pdf ${BUILD_DIR}

PRE:
	mkdir -p ${BUILD_DIR}/${LANG}
	$(foreach i,$(INPUT), lua ${LANG}/dict.lua $(i) >${BUILD_DIR}/$(i); )
HTML:
	mkdir html
	ls -d ${BUILD_DIR}/${LANG}/*.md | xargs pandoc --toc -o ./html/$(OUT)_${LANG}.html --css=$(CSS)

HTML_BOOTSTRAP:
	ls -d ${BUILD_DIR}/${LANG}/*.md | xargs pandoc --toc --toc-depth 2 --css html_templates/css/bootstrap.css --template html_templates/pandoc-template.html5 --self-contained -o ./html/$(OUT)_${LANG}_bootstrap.html --css=$(CSS)
  
CHUNKED_HTML:
	ls -d ${BUILD_DIR}/${LANG}/*.md | xargs pandoc -s -S -t docbook -o ${LANG}/$(OUT).db --css=$(CSS)
	xmlto xhtml -m config.xsl ${LANG}/$(OUT).db ${LANG}/html/

PDF:
	mkdir pdf && cd pdf
	ls -d ${BUILD_DIR}/${LANG}/*.md | xargs pandoc --toc -o ${LANG}/$(OUT).pdf -V documentclass=book
	cd ../

