#!/usr/bin/env python
# coding: utf-8

"""
X-ray diffraction simulations program.
"""

__author__ = "Dunstan Becht, Asdin Aoufi"
__version__ = "0.8.11"

import os
import sys

if sys.version_info[0]>=3 and sys.version_info[1]>=9:
    from collections.abc import Callable
else:
    from typing import Callable

clone_dir = 'xrd' # default location of the local copy of the simulator code
