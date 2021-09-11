#!/usr/bin/env python
# coding: utf-8

"""
Module to automate the execution of simulations.
"""

from . import *

def make(
    executer = os.system,
    clndir = clone_dir,
) -> None:
    """
    Compile the simulation program.

    Input:
        executer: function executing shell commands
        clndir: program code clone directory
    """
    clndir = os.path.abspath(clndir)
    b = executer("module load cuda/10.1; cd "+clndir+"; make")
    print("".join([p.decode('utf-8') for p in b]))

def run(
    impstm: str,
    expstm: str = "output",
    impdir: str = "",
    expdir: str = "",
    clndir = clone_dir,
    executer = os.system,
    h: int = 1,
    b: int = 200,
    r: int = 1000,
    f: int = 35,

) -> None:
    """
    Run the simulation on a sample of distributions.

    Input:
        impstm: stem of the input
        expstm: stem of the output
        impdir: import directory
        expdir: export directory
        clndir: program code clone directory
        executer: function executing shell commands
        h: hardware to use (1 for gpu / 0 for cpu)
        b: block size
        r: block repetitions (r*b gives the number of random points)
        f: number of Fourier coefficients
    """
    n = r*b # number of random points
    clndir = os.path.abspath(clndir)
    impdir_stm = os.path.join(os.path.abspath(impdir), impstm)
    expdir_stm = os.path.join(os.path.abspath(expdir), expstm)
    if not os.path.exists(expdir_stm):
        os.mkdir(expdir_stm)
    else:
        raise ValueError("existing output directory: "+expdir_stm)
    for e in os.listdir(impdir_stm):
        args = [
            h,
            b,
            os.path.join(impdir_stm, e),
            n,
            f,
            os.path.join(expdir_stm, e),
        ]
        args = " ".join([str(a) for a in args])
        print("- "+e+" ("+args+")")
        print("cd "+clndir+"; ./a.out "+args+" >& run-"+impstm+"-"+e)
        executer("cd "+clndir+"; ./a.out "+args+" >& run-"+impstm+"-"+e)
