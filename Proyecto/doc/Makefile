all: doxygen pdf doc latex

pdf: Propuesta.pdf Usage.pdf Technical_Report.pdf

Technical_Report.pdf: Technical_Report.tex
	@echo "******************* TECHNICAL REPORT PDF **************************************;\
	pdflatex Technical_Report.tex
	pdflatex Technical_Report.tex
	make clean

Propuesta.pdf : Propuesta.tex
	@echo "******************* PROPUESTA PDF **************************************";\
	pdflatex Propuesta.tex
	pdflatex Propuesta.tex
	make clean
	
Usage.pdf : Usage.tex
	@echo "******************* PROPUESTA PDF **************************************";\
	pdflatex Usage.tex
	pdflatex Usage.tex
	

doxygen: Doxyfile
	@echo "******************* CREATING DOXYGEN OUTPUT ****************************";\
	doxygen Doxyfile
	make latex

doc: 
	make -C ./doxygen/ all

latex: latex/refman.tex
	@echo "******************* PRODUCING DOXYGEN LATEX OUTPUT *********************";\
	make -C latex/ all
	cp latex/refman.pdf ./

clean:
	@echo "******************* CLEANING *******************************************"
	rm -f *.ps *.dvi *.aux *.toc *.idx *.ind *.ilg *.log *.out *.brf *.blg *.bbl
	make -C latex/ clean

