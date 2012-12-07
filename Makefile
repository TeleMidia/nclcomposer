# Makefile for NCL Composer Tutorial
#
# Dependencies:
#   * asciidoc

SOURCES=nclcomposer-tutorial.asciidoc
OUTPUT=HTML CHUNKED_HTML EPUB PDF

ASCIIDOC_BIN=asciidoc
ASCIIDOC_PARAMS=-b html5 -a toc2 -a theme=flask -a icons

all: $(OUTPUT)

HTML:
	$(ASCIIDOC_BIN) $(ASCIIDOC_PARAMS) $(SOURCES)

CHUNKED_HTML:
	a2x --format=chunked --no-xmllint $(SOURCES)

PDF:
	a2x -fpdf -dbook $(SOURCES) --no-xmllint --fop

EPUB:
	a2x -fepub -dbook $(SOURCES) --no-xmllint

clean:
	rm *.pdf *.html *.epub
	rm -r *.chunked

