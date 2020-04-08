LATEX = pdflatex
BIBTEX = bibtex
TARGET = thesis
PDF = $(TARGET).pdf
PS = $(TARGET).ps
DVI = $(TARGET).dvi
TEX = $(TARGET).tex

thesis.pdf: thesis.bib thesis.tex abstract.tex changes.tex\
    acknowledgements.tex chapter1.tex chapter2.tex appendix1.tex \
    appendix2.tex glossary.tex ecte45x.sty
	$(LATEX) $(TARGET)
	$(BIBTEX) $(TARGET)
	$(LATEX) $(TARGET)
	$(LATEX) $(TARGET)

clean:
	rm -f *~ *.out *.log *.aux *.bbl *.dvi *.lof *.lot *.toc *.blg $(PDF)
