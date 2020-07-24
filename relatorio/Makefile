PANDOC_OPTS := \
	--toc                    \
	-N                       \
	-V documentclass=article \
	-V lang=pt-PT            \
	-V papersize=a4          \
	-s                       \

MD := \
    relatorio.md \

DEPS := $(MD) bibliografia.bib template.latex Makefile

PDF := $(MD:.md=.pdf)

default: $(PDF)

watch:
	ls -1 $(DEPS) | entr -c make

relatorio.pdf: relatorio.md bibliografia.bib template.latex Makefile
	pandoc $(PANDOC_OPTS) --bibliography bibliografia.bib --template template.latex -f markdown -t latex+citations $< -o $@

.PHONY: default watch
