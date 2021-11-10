#!/usr/bin/env python
# coding: utf-8

"""
Script to test the module run.

Example of commands for testing:
ssh compute-0-1-gpu
cd tests
python3 test_code.py
python3 test_run.py
"""

from lpa.xrd import run

cmd, res = run.make()
print("$ "+cmd)
print(res)
print()

cmd, res = run.distribution(
    'rho5e13m-2_square_3200nm_RDD_d5e-5nm-2_screw_PBC1_S0',
    impdir='input',
    #expdir='output',
)
print("$ "+cmd)
print(res)
print()

cmd, res = run.sample(
    '10_rho5e13m-2_square_3200nm_RDD_d5e-5nm-2_screw_S0_PBC1',
    impdir='input',
    #expdir='output',
)
print("$ "+cmd)
print(res)
print()

input("OK")
