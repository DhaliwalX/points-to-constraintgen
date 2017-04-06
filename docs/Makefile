MAIN = Title
NAME = constraintgen
LATEX = latex
#LATEX = xelatex
#LATEX = pdflatex -shell-escape

TARNAME = $(NAME).tgz
TARFOLDER = $(NAME).d
# TARTHESEFILES = *.tex references.bib *.pdf images *.sty Makefile doc-structure *.md
TARFILES = $(wildcard *.tex)
TARFILES += $(wildcard *.sty)
TARFILES += $(wildcard *.pdf)
TARFILES += $(wildcard *.md)
TARFILES += references.bib
TARFILES += Makefile
TARFILES += images
TARFILES += doc-structure

maindependencies = $(wildcard *.tex)
maindependencies += $(wildcard *.bib)
maindependencies += $(wildcard *.sty)

$(MAIN).pdf: $(maindependencies) 
	$(LATEX) $(MAIN) && bibtex8 $(MAIN) && $(LATEX) $(MAIN) && $(LATEX) $(MAIN) && dvips $(MAIN).dvi && ps2pdf $(MAIN).ps

clean:
	$(RM) *.aux *.bbl *.dvi *.log *.out *.toc *.blg *.lof *.lot *.ps *.nav *.snm *.vrb *.pdf *.bcf *.tgz main-blx.bib main.run.xml

tgz: $(MAIN).pdf
	if [ ! -d $(TARFOLDER) ]; then mkdir $(TARFOLDER); fi
	echo "Creating tar file $(TARNAME)"
	if [ -f $(TARNAME) ]; then rm -f $(TARNAME); fi
	rm -fr $(TARFOLDER)/*
	cp -r $(TARFILES) $(TARFOLDER)
	tar -zcvf $(TARNAME) $(TARFOLDER)
	rm -fr $(TARFOLDER)
	echo "$(TARNAME) Created."

show: $(MAIN).pdf
	evince $(MAIN).pdf &

.DEFAULT: $(MAIN).pdf
.PHONY: show tar clean


