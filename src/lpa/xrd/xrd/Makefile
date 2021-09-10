#-- makefile for compiling OpenCL code
all: a.out
a.out: square-cyl-ocl-mt-double-alldislo-modular.c
	@echo "version may 2021"
	@echo "compile OpenCL code square-cyl-...-modular"
	@echo ""
	nvcc -O3 -w -DDUMP=0 -DUNIFORM=0 -DRandMT=1 square-cyl-ocl-mt-double-alldislo-modular.c -lOpenCL 
	@echo ""
clean: 
	rm -f *.o a.out

