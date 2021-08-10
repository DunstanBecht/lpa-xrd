#!/usr/bin/env python
# coding: utf-8

"""
Module to automate the execution of simulations.
"""

import os

PATH = 'workspace/xrd' # simulator location

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
    s: str,
    i: str = "",
    o: str = "",
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
        s: directory name of the sample of distributions
        i: input directory
        o: output directory
        h: hardware to use (1 for gpu / 0 for cpu)
        b: block size
        r: block repetitions (r*b gives the number of random points)
        f: number of Fourier coefficients
        executer:
    """
    if i!="" and i[-1]!="/":
        i += "/"
    if o!="" and o[-1]!="/":
        o += "/"
    n = r*b # number of random points
    if not os.path.exists(o+s):
        os.mkdir(o+s)
    ipath = "../"+i+s+"/"
    opath = "../"+o+s+"/"
    for e in os.listdir(i+s):
        args = " ".join(str(a) for a in [h, b, ipath+e, n, f, opath+e])
        print("- "+e+" ("+args+")")
        executer("cd "+path+"; ./a.out "+args+" >& run-"+s+"-"+e)
