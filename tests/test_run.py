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

if False: # generate input data
    from lpa.input import data, sets
    from lpa.input.models import RDD
    rdd = RDD, {'d': 5e13*1e-18}
    data.export(sets.Sample(10, 'square', 3200, *rdd, S=0), pbc=1)
    data.export(sets.Distribution('square', 3200, *rdd, S=0, c='PBC1'))

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
