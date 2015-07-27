LATEX = pdflatex

all: report double-ring

report: report.tex
	$(LATEX) report.tex

double-ring: double_ring.tex
	$(LATEX) double_ring.tex

.PHONY: clean
clean:
	$(RM) *.aux *.log *.out
