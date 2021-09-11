#!/usr/bin/env python
# coding: utf-8

"""
Module to automate the execution of simulations.
"""

from . import *

@beartype
def make(
    executer: Callable = os.system,
    clndir: str = clone_dir,
) -> tuple:
    """
    Compile the simulation program.

    Input:
        executer (Callable): function executing shell commands
        clndir (str): program code clone directory
    """
    clndir = os.path.abspath(clndir)
    cmd = "module load cuda/10.1; cd "+clndir+"; make"
    res = executer(cmd)
    if isinstance(res, bytes):
        res = "".join([p.decode('utf-8') for p in res])
    return cmd, res

@beartype
def distribution(
    impstm: str,
    **kwargs,
) -> tuple:
    """
    Run the simulation on a distribution file input.

    r*b gives the number of random points.

    Input:
        impstm (str): stem of the input
      **expstm (str): stem of the output (default: 'output_<impstm>')
      **impdir (str): import directory (default: '')
      **expdir (str): export directory (default: '')
      **impfmt (str): import format (default: 'dat')
      **expfmt (str): export format (default: 'dat')
      **clndir (str): program code clone directory (default: 'xrd')
      **rundir (str): directory where to export run data (default: 'runs')
      **executer (Callable): function executing shell commands (default: os.)
      **h (int): hardware to use (1 for gpu / 0 for cpu) (default: 1)
      **b (int): block size (default: 200)
      **r (int): block repetitions (default: 1000)
      **f (int): number of Fourier coefficients (default: 50)

    Output:
        cmd (str): executed sh command
        res (str): result of the sh command
    """
    # optional parameters
    expstm = getkwa('expstm', kwargs, str, 'output_'+impstm)
    impfmt = getkwa('impfmt', kwargs, str, 'dat')
    expfmt = getkwa('expfmt', kwargs, str, 'dat')
    impdir = os.path.abspath(getkwa('impdir', kwargs, str, ''))
    expdir = os.path.abspath(getkwa('expdir', kwargs, str, ''))
    clndir = os.path.abspath(getkwa('clndir', kwargs, str, clone_dir))
    rundir = os.path.abspath(getkwa('rundir', kwargs, str, 'runs'))
    executer = getkwa('executer', kwargs, Callable, os.system)
    h = getkwa('h', kwargs, int, 1)
    b = getkwa('b', kwargs, int, 200)
    r = getkwa('r', kwargs, int, 1000)
    f = getkwa('f', kwargs, int, 50)
    endkwa(kwargs)
    # run
    n = r*b # number of random points
    if not os.path.exists(rundir):
        executer("mkdir "+rundir)
    args = (
        h,
        b,
        os.path.join(impdir, impstm+'.'+impfmt),
        n,
        f,
        os.path.join(expdir, expstm+'.'+expfmt),
    )
    args = " ".join([str(a) for a in args])
    cmd = ("cd "+clndir+"; "
        + "./a.out "+args+" >& "+os.path.join(rundir, impstm)+".out")
    res = executer(cmd)
    return cmd, str(res)

@beartype
def sample(
    impstm: str,
    **kwargs,
) -> tuple:
    """
    Run the simulation on a sample directory input.

    Input:
        impstm (str): stem of the input
        <see distribution for keyword arguments>

    Output:
        cmd (str): executed sh command
        res (str): result of the sh command
    """
    # optional arguments
    expstm = getkwa('expstm', kwargs, str, 'output_'+impstm)
    impdir = getkwa('impdir', kwargs, str, '')
    expdir = getkwa('expdir', kwargs, str, '')
    rundir = getkwa('rundir', kwargs, str, 'runs')
    impfmt = getkwa('impfmt', kwargs, str, 'dat')
    executer = getkwa('executer', kwargs, Callable, os.system)
    # parameters
    impdir_stm = os.path.join(os.path.abspath(impdir), impstm)
    expdir_stm = os.path.join(os.path.abspath(expdir), expstm)
    rundir_stm = os.path.join(os.path.abspath(rundir), impstm)
    print(rundir_stm)
    if not os.path.exists(rundir_stm):
        executer("mkdir "+rundir_stm)
    if not os.path.exists(expdir_stm):
        executer("mkdir "+expdir_stm)
    else:
        raise ValueError("existing output directory: "+expdir_stm)
    # run
    cmdl, resl = [], []
    stm_fmt = [os.path.splitext(e) for e in os.listdir(impdir_stm)]
    for stm, fmt in stm_fmt:
        fmt = fmt[1:]
        if fmt == impfmt:
            cmd, res = distribution(
                stm,
                impdir=impdir_stm,
                expdir=expdir_stm,
                expstm=stm,
                rundir=rundir_stm,
                impfmt=impfmt,
                executer=executer,
                **kwargs,
            )
            cmdl.append(cmd)
            resl.append(str(res))
    return "\n".join(cmdl), "\n".join(resl)
