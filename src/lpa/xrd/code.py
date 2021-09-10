#!/usr/bin/env python
# coding: utf-8

"""
Module to manage the simulation program code.
"""

from . import *

import shutil
import pkg_resources

def clone(
    expdir: str = clone_dir,
) -> None:
    """
    Clone the simulation program files.

    Input:
        expdir (str): export directory
    """
    path = pkg_resources.resource_filename('lpa.xrd', 'xrd/') # path to code
    shutil.copytree(path, expdir) # copy program files
