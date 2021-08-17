#!/usr/bin/env python
# coding: utf-8

"""
Module to automate the execution of simulations.
"""

import os

PATH = os.path.join(os.getcwd(), 'xrd') # simulator program location

def make(
    executer=os.system,
    path=PATH,
) -> None:
    """
    Compile the simulation program.

    Input:
        executer:
        path:
    """
    b = executer("module load cuda/10.1; cd "+path+"; make")
    print("".join([p.decode('utf-8') for p in b]))

def run(
    imstm: str,
    imdir: str = "",
    exdir: str = "",
    h: int = 1,
    b: int = 200,
    r: int = 1000,
    f: int = 35,
    executer=os.system,
    path=PATH,
) -> None:
    """
    Run the simulation on a sample of distributions.

    Input:
        imstm: stem of the input
        imdir: import directory
        exdir: export directory
        h: hardware to use (1 for gpu / 0 for cpu)
        b: block size
        r: block repetitions (r*b gives the number of random points)
        f: number of Fourier coefficients
        executer:
    """
    n = r*b # number of random points
    imdir_stm = os.path.join(imdir, imstm)
    exdir_stm = os.path.join(exdir, imstm)
    if not os.path.exists(exdir_stm):
        os.mkdir(exdir_stm)
    ipath = "../"+imdir_stm
    opath = "../"+exdir_stm
    for e in os.listdir(imdir_stm):
        args = [
            h,
            b,
            os.path.join(ipath, e),
            n,
            f,
            os.path.join(opath, e),
        ]
        args = " ".join([str(a) for a in args])
        print("- "+e+" ("+args+")")
        executer("cd "+path+"; ./a.out "+args+" >& run-"+s+"-"+e)
