MD := \
    relatorio.md \

PDF := $(MD:.md=.pdf)

default: $(PDF)

watch:
	ls -1 $(MD) | entr make

%.pdf: %.md
	pandoc --toc -N --template=template.latex -f markdown -t latex $< -o $@

.PHONY: default watch
