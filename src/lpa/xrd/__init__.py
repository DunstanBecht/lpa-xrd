#!/usr/bin/env python
# coding: utf-8

"""
X-ray diffraction simulations program.
"""

__author__ = "Dunstan Becht, Asdin Aoufi"
__version__ = "0.8.12"

import os
import sys
from typing import Union, Optional, Any

if sys.version_info[0]>=3 and sys.version_info[1]>=9:
    from collections.abc import Callable
else:
    from typing import Callable

if sys.version_info[0]>=3 and sys.version_info[1]>=8:
    from typing import get_args, get_origin
else:
    get_args = lambda x: x.__args__
    get_origin = lambda x: x.__origin__ if hasattr(x, '__origin__') else x

from beartype import beartype

clone_dir = 'xrd' # default location of the local copy of the simulator code

def getkwa(
    key: str,
    kwa: dict,
    typ: type,
    dft: Any,
) -> Any:
    """
    Retrieve the keyword argument and check its type.

    Input:
        key (str): keyword argument name
        kwa (dict): keyword arguments dictionary
        typ (type): keyword argument type
        dft (Any): keyword argument default value

    Output:
        val (Any): value of the keyword argument
    """
    val = kwa.pop(key, dft)
    if get_origin(typ) == Union:
        typ = get_args(typ)
    if not isinstance(val, typ):
        msg = ("keyword argument '"+key+"' must be of type "+str(typ)
            + " but "+str(type(val))+" given")
        raise TypeError(msg)
    return val

def endkwa(
    kwa: dict,
) -> None:
    """
    Check that no bad keyword arguments have been passed.

    Input:
        kwa (dict): keyword arguments dictionary
    """
    if len(kwa) > 0:
        msg = ("unexpected keyword argument(s): "
            + ", ".join([repr(key) for key in kwa]))
        raise TypeError(msg)
