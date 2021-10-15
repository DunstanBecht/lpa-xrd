#!/usr/bin/env python
# coding: utf-8

"""
Module to manage the simulation program code.
"""

from . import *

import shutil
import pkg_resources

@beartype
def clone(
    clndir: str = clone_dir,
) -> None:
    """
    Clone the simulation program files.

    Input:
        clndir (str): export path of the cloned program
    """
    if os.path.isdir(clndir):
        raise ValueError(f"existing clone directory: {clndir}")
    path = pkg_resources.resource_filename('lpa.xrd', 'xrd/') # path to code
    shutil.copytree(path, clndir) # copy program files
