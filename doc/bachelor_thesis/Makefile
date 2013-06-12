PROJ=projekt

all: $(PROJ).pdf

pdf: $(PROJ).pdf

$(PROJ).pdf: $(PROJ).tex
	pdflatex $(PROJ)
	pdflatex $(PROJ)

clean:
	rm -f *.dvi *.log  $(PROJ).toc *.aux $(PROJ).out $(PROJ).lof
	rm -f $(PROJ).pdf
	rm -f *~

