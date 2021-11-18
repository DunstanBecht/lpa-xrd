#-- makefile for compiling OpenCL code
all: a.out
a.out: square-cyl-ocl-mt-double-alldislo-modular.c
	@echo "compile OpenCL code"
	nvcc -O3 -w -DDUMP=0 square-cyl-ocl-mt-double-alldislo-modular.c -lOpenCL
clean:
	rm -f *.o a.out
