#!/usr/bin/env python
# coding: utf-8

"""
Script to test the module run.
"""

from lpa.xrd import run

executer = lambda sh: '/!\ nothing executed, choose an executer'

cmd, res = run.make(
    executer=executer
)
print("$ "+cmd)
print(res)
print()

cmd, res = run.distribution(
    'rho5e13m-2_square_3200nm_RDD_d5e-5nm-2_edge_PBCR1_S0',
    executer=executer,
)
print("$ "+cmd)
print(res)
print()

cmd, res = run.sample(
    '10_rho5e13m-2_square_3200nm_RDD_d5e-5nm-2_edge_PBCR1_S0',
    executer=executer,
)
print("$ "+cmd)
print(res)
