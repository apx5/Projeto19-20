MD := \
    ranges-v3.md \

PDF := $(MD:.md=.pdf)

default: $(PDF)

watch:
	ls -1 $(MD) | entr make

%.pdf: %.md
	pandoc -f markdown -t latex $< -o $@

.PHONY: default watch
