#!/usr/bin/env python
# coding: utf-8

"""
Script to test the module run.
"""

from lpa.xrd import run


run.make()

run.run(
    '10_rho5e13m-2_square_3200nm_RDD_d5e-5nm-2_edge_PBCR1_S0',
    expstm='output',
)
