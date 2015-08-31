LATEX = pdflatex

all: presentation document

presentation: pres.tex
	$(LATEX) pres.tex

document: proposal.tex
	$(LATEX) proposal.tex

.PHONY: clean
clean:
	$(RM) pres.log pres.toc pres.aux pres.nav pres.snm pres.out proposal.out proposal.aux proposal.log
