#!/usr/bin/env python
# coding: utf-8

"""
Module to manage the simulation program code.
"""

from . import *
from . import __version__

import pkg_resources

replace = (
    ("!VERSION", __version__),
)

@beartype
def copy(
    cop: str,
    pst: str,
) -> None:
    """
    Clone recursively a file or a directory.

    Input:
        cop (str): file or directory to copy
        pst (str): file of directory name to past
    """
    if os.path.isdir(cop):
        os.mkdir(pst)
        for nod in os.listdir(cop):
            copy(os.path.join(cop, nod), os.path.join(pst, nod))
    else:
        with open(cop, "r") as f:
            ctt = f.read()
        for tgt, txt in replace:
            ctt = ctt.replace(tgt, txt)
        with open(pst, "w") as f:
            f.write(ctt)

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
    pth = pkg_resources.resource_filename('lpa.xrd', 'xrd/') # path to code
    copy(pth, clndir) # copy program files
