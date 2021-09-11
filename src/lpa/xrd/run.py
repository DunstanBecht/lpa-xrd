#!/usr/bin/env python
# coding: utf-8

"""
Module to automate the execution of simulations.
"""

from . import *

def make(
    executer: Callable = os.system,
    clndir: str = clone_dir,
) -> None:
    """
    Compile the simulation program.

    Input:
        executer: function executing shell commands
        clndir: program code clone directory
    """
    clndir = os.path.abspath(clndir)
    cmd = "module load cuda/10.1; cd "+clndir+"; make"
    res = executer(cmd)
    if isinstance(res, bytes):
        res = "".join([p.decode('utf-8') for p in res])
    return cmd, res

def run(
    impstm: str,
    expstm: str = "output",
    impdir: str = "",
    expdir: str = "",
    clndir: str = clone_dir,
    rundir: str = "runs",
    executer: Callable = os.system,
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
        rundir: directory where to export run data
        executer: function executing shell commands
        h: hardware to use (1 for gpu / 0 for cpu)
        b: block size
        r: block repetitions (r*b gives the number of random points)
        f: number of Fourier coefficients
    """
    n = r*b # number of random points
    clndir = os.path.abspath(clndir)
    rundir = os.path.abspath(rundir)
    impdir_stm = os.path.join(os.path.abspath(impdir), impstm)
    expdir_stm = os.path.join(os.path.abspath(expdir), expstm)
    if not os.path.exists(expdir_stm):
        executer("mkdir "+expdir_stm)
    else:
        raise ValueError("existing output directory: "+expdir_stm)
    if not os.path.exists(rundir):
        executer("mkdir "+rundir)
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
        cmd = ("cd "+clndir+"; "
            + "./a.out "+args+" >& "+os.path.join(rundir, impstm+"-"+e))
        res = executer(cmd)
        return cmd, res
