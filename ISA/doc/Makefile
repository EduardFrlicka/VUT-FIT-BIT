#AUTHOR: Eduard Frlicka
ZIP_NAME=package

PDF_FILES=$(patsubst %.tex,%.pdf, $(wildcard *.tex))

ZIP_FILES=*.eps *.sty


.PRECIOUS: %.ps %.dvi

all: $(PDF_FILES)

%.pdf:%.ps 
	@echo 'Compiling $<'
	ps2pdf $<

%.ps:%.dvi 
	@echo 'Compiling $<'
	dvips -t a4 -o $@ $<

%.dvi:%.tex 
	@echo 'Compiling $<'
	latex  $< 
	latex  $< 

recompile: clean all

zip: 
	zip $(ZIP_NAME) *.tex Makefile $(ZIP_FILES)

clean:
	rm -f *.pdf *.log *.out *.dvi *.aux *.ps
	rm -f $(ZIP_NAME).zip