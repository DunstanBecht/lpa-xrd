#-- makefile for compiling OpenCL code
all: a.out
a.out: lpa_xrd_simulator.c
	@echo "compile OpenCL code"
	nvcc -O3 -w -DDUMP=0 lpa_xrd_simulator.c -lOpenCL
clean:
	rm -f *.o a.out
