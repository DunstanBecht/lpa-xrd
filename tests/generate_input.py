#!/usr/bin/env python
# coding: utf-8

"""
Script to generate input data in order to test the module run.
"""

from lpa.input import data, sets
from lpa.input.models import RDD

rdd = RDD, {'d': 5e13*1e-18}

data.export(
    sets.Sample(10, 'square', 3200, *rdd, S=0),
    pbc=1,
    expdir='input',
)

data.export(
    sets.Distribution('square', 3200, *rdd, S=0, c='PBC1'),
    expdir='input',
)

input("OK")
