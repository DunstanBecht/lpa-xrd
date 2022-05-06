#!/usr/bin/env python
# coding: utf-8

"""
Script to test the module run.

Commands for a possible connection to a GPU host machine.
$ ssh compute-0-1-gpu
$ module load cuda/10.1
$ cd <tests-directory>

Commands for launching simulations:
$ python3 test_code.py
$ python3 test_run.py

This script can also be submitted with:
$ sbatch test_run.job
"""

from lpa.xrd import run

"""
The following line is used to compile the simulator source code.
"""
cmd, res = run.make()
print("$ "+cmd)
print(res)
print()


"""
The following lines run the simulation on each file of the sample.
150 values of the Fourier variable L are considered.
"""
cmd, res = run.sample(
    '10_rho5e13m-2_square_3200nm_RDD_d5e-5nm-2_screw_S0_PBC1',
    nfv=50,
    impdir='input',
    #expdir='output',
)
print("$ "+cmd)
print(res)
print()

"""
In the following simulations are launched on distribution files.
150 values of the Fourier variable L are considered.
"""

cmd, res = run.distribution(
    'rho5e13m-2_square_3200nm_RDD_d5e-5nm-2_screw_PBC1_S0',
    nfv=50,
    impdir='input',
    #expdir='output',
)
print("$ "+cmd)
print(res)
print()

cmd, res = run.distribution(
    'rho49985e9m-2_circle_1600nm_RDD_d5e-5nm-2_edge_S0',
    nfv=50,
    impdir='input',
    #expdir='output',
)
print("$ "+cmd)
print(res)
print()

cmd, res = run.distribution(
    'rho49985e9m-2_circle_1600nm_RDD_d5e-5nm-2_screw_ISD_S0',
    nfv=50,
    impdir='input',
    #expdir='output',
)
print("$ "+cmd)
print(res)
print()

input("OK")
