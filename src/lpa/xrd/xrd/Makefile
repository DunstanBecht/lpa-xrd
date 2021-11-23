#-- makefile for compiling OpenCL code
all: lpa_xrd_simulator
lpa_xrd_simulator: lpa_xrd_simulator.c
	@echo "compile OpenCL code"
	nvcc -O3 -w -DDUMP=0 lpa_xrd_simulator.c -o lpa_xrd_simulator -lOpenCL
clean:
	rm -f *.o lpa_xrd_simulator
