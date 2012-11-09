#/bin/bash
a2x -fpdf -dbook nclcomposer-tutorial.asciidoc

xsltproc /usr/share/xml/docbook/stylesheet/docbook-xsl-ns/fo/docbook.xsl \
nclcomposer-tutorial.xml > nclcomposer-tutorial.fo

fop -fo nclcomposer-tutorial.fo -pdf nclcomposer-tutorial.pdf
