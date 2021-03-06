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
    cmd = f"cd {clndir}; make"
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

    Input:
        impstm (str): stem of the input
      **expstm (str): stem of the output (default: '<impstm>_output')
      **impdir (str): import directory (default: '')
      **expdir (str): export directory (default: '')
      **impfmt (str): import format (default: 'dat')
      **expfmt (str): export format (default: 'dat')
      **clndir (str): program code clone directory (default: 'xrd')
      **rundir (str): directory where to export run data (default: 'runs')
      **exestm (str): name of the executable (default: 'lpa_xrd_simulator')
      **executer (Callable): function executing shell commands (default: os.)
      **hdw (int): hardware execution support (CPU:0 / GPU:1) (default: 1)
      **wgs (int): work-group size (default: 64)
      **nrp (int): number of random points (default: 200000)
      **nfv (int): number of Fourier variable values (default: 50)

    Output:
        cmd (str): executed sh command
        res (str): result of the sh command
    """
    # optional parameters
    expstm = getkwa('expstm', kwargs, str, impstm+'_output')
    impfmt = getkwa('impfmt', kwargs, str, 'dat')
    expfmt = getkwa('expfmt', kwargs, str, 'dat')
    impdir = os.path.abspath(getkwa('impdir', kwargs, str, ''))
    expdir = os.path.abspath(getkwa('expdir', kwargs, str, ''))
    clndir = os.path.abspath(getkwa('clndir', kwargs, str, clone_dir))
    rundir = os.path.abspath(getkwa('rundir', kwargs, str, 'runs'))
    exestm = getkwa('exestm', kwargs, str, 'lpa_xrd_simulator')
    executer = getkwa('executer', kwargs, Callable, os.system)
    hdw = getkwa('hdw', kwargs, int, 1)
    wgs = getkwa('wgs', kwargs, int, 64)
    nrp = getkwa('nrp', kwargs, int, 200000)
    nfv = getkwa('nfv', kwargs, int, 50)
    endkwa(kwargs)
    # make directory
    if not os.path.isdir(rundir):
        os.mkdir(rundir)
    if not os.path.isdir(clndir):
        raise ValueError("the simulator must be cloned with xrd.code.clone()")
    # run
    args = " ".join((
        str(hdw), # hardware
        str(wgs), # work-group size
        os.path.join(impdir, impstm+'.'+impfmt), # input path
        str(nrp), # number of random points
        str(nfv), # number of Fourier variable values
        os.path.join(expdir, expstm+'.'+expfmt), # output path
    ))
    cmd = (f"cd {clndir}; "
           f"./{exestm} {args} > {os.path.join(rundir, impstm)}.out 2>&1")
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
    expstm = getkwa('expstm', kwargs, str, impstm+'_output')
    impdir = getkwa('impdir', kwargs, str, '')
    expdir = getkwa('expdir', kwargs, str, '')
    rundir = getkwa('rundir', kwargs, str, 'runs')
    impfmt = getkwa('impfmt', kwargs, str, 'dat')
    executer = getkwa('executer', kwargs, Callable, os.system)
    # parameters
    impdir_stm = os.path.join(os.path.abspath(impdir), impstm)
    expdir_stm = os.path.join(os.path.abspath(expdir), expstm)
    rundir_stm = os.path.join(os.path.abspath(rundir), impstm)
    # make directories
    if not os.path.exists(rundir):
        os.mkdir(rundir)
    if not os.path.exists(rundir_stm):
        os.mkdir(rundir_stm)
    if not os.path.exists(expdir_stm):
        os.mkdir(expdir_stm)
    else:
        raise ValueError(f"delete existing output directory: {expdir_stm}")
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
