<div align="center">
  <img width="250" src="https://dunstan.becht.network/views/signatures/mines.svg" alt="Mines Saint-Etienne">
</div>

# Line Profile Analysis - X-Ray Diffraction

This project is related to the analysis of crystals containing dislocations by X-ray diffraction. It was developed and used for a study conducted during a research internship at the laboratory of material and structural sciences of the *École Nationale Supérieure des Mines de Saint-Étienne*. This repository contains the distribution of one of the three published python packages that have been proposed to conduct line profile analyses based on simulation results:
* [`lpa-input`](https://github.com/DunstanBecht/lpa-input) (line profile analysis input generator)
* [`lpa-xrd`](https://github.com/DunstanBecht/lpa-xrd) (line profile analysis x-ray diffraction simulation program)
* [`lpa-output`](https://github.com/DunstanBecht/lpa-output) (line profile analysis output analyzer)

The repository [`lpa-workspace`](https://github.com/DunstanBecht/lpa-workspace) contains the parameters and the scripts for the generation of the data used in the study. You can then easily replicate the results obtained or use it as inspiration to take the code in hand and conduct your own calculations. The software is placed in the public domain and you can use it as you wish. However, users are encouraged to contribute to the development and report issues.

# Features

The package `lpa.xrd` can be used to:
* make a local copy of the X-ray diffraction simulator code
* automate the tasks of compilations and executions

# Installation

The package is indexed on [PyPI](https://pypi.org/project/lpa-xrd/) and installable directly via pip:
```bash
pip install -U lpa-xrd
```

# Examples

### Output data files
```
   0.9.1 # v: lpa-xrd version
5.00e+13 # d: dislocation density [m^-2]
 1  1  0 # z: direction of 'l' (line vector) [uvw]
 1  1  0 # b: Burgers vector direction [uvw] 
 2  0  0 # g: diffraction vector direction (hkl)
0.250000 # C: contrast coefficient [1]
0.404940 # a: cell parameter [nm]
    3200 # s: square side [nm]
0.345000 # nu: Poisson's number [1]
    4608 # number of dislocations in the input file
  200000 # number of random points
#    L       cos1   err_cos1       sin1   err_sin1       cos2   err_cos2       sin2   err_sin2       cos3   err_cos3       sin3   err_sin3       cos4   err_cos4       sin4   err_sin4       cos5   err_cos5       sin5   err_sin5    <eps^2> bad_points
  11.8  0.9612154  0.0000021  0.0933476  0.0000031  0.8701386  0.0000030  0.1714553  0.0000041  0.7468478  0.0008511  0.2258043  0.0013274  0.6086918  0.0011362  0.2543138  0.0015160  0.4702471  0.0013753  0.2584702  0.0016228  0.0000252      42638
  23.6  0.8665810  0.0000030  0.1707687  0.0000041  0.6052743  0.0000043  0.2540308  0.0000049  0.3411569  0.0015559  0.2425574  0.0016745  0.1456840  0.0017478  0.1720591  0.0017198  0.0373021  0.0017909  0.0859400  0.0017526  0.0000230      42140
  35.4  0.7379861  0.0000038  0.2238752  0.0000046  0.3382133  0.0000050  0.2410983  0.0000052  0.0802127  0.0017801  0.1269875  0.0017336 -0.0024581  0.0017699  0.0132620  0.0017840  0.0014496  0.0017637 -0.0397089  0.0017877  0.0000217      41685
  47.2  0.5948167  0.0000043  0.2502348  0.0000049  0.1419134  0.0000053  0.1685363  0.0000052 -0.0032791  0.0017689  0.0117668  0.0017799  0.0081841  0.0017682 -0.0435061  0.0017774  0.0147161  0.0017667 -0.0277220  0.0017806  0.0000208      41221
  59.0  0.4528781  0.0000047  0.2515366  0.0000051  0.0340490  0.0000053  0.0808046  0.0000052  0.0031487  0.0017597 -0.0394095  0.0017818  0.0146570  0.0017650 -0.0274590  0.0017776 -0.0127916  0.0017426 -0.0269929  0.0017997  0.0000202      40791
  70.8  0.3243543  0.0000050  0.2336830  0.0000051 -0.0026564  0.0000053  0.0090003  0.0000053  0.0158562  0.0017722 -0.0388728  0.0017641 -0.0100881  0.0017365 -0.0213947  0.0018013 -0.0126061  0.0017664 -0.0507593  0.0017681  0.0000196      40351
  82.6  0.2160115  0.0000051  0.2010122  0.0000051 -0.0011513  0.0000052 -0.0312326  0.0000053  0.0084157  0.0017491 -0.0207299  0.0017840 -0.0182648  0.0017596 -0.0431111  0.0017706  0.0020582  0.0017679 -0.0308282  0.0017645  0.0000192      39872
  94.4  0.1304829  0.0000052  0.1599180  0.0000052  0.0102773  0.0000052 -0.0439197  0.0000052 -0.0125465  0.0017316 -0.0206975  0.0017963 -0.0040411  0.0017634 -0.0470010  0.0017622 -0.0089315  0.0017692  0.0009732  0.0017602  0.0000187      39459
 106.2  0.0683771  0.0000052  0.1149025  0.0000052  0.0180239  0.0000052 -0.0372628  0.0000052 -0.0220605  0.0017483 -0.0379005  0.0017732  0.0038755  0.0017669 -0.0261968  0.0017568 -0.0337534  0.0017571  0.0051250  0.0017657  0.0000184      39038
 118.0  0.0272849  0.0000052  0.0722057  0.0000052  0.0149200  0.0000052 -0.0235532  0.0000052 -0.0139199  0.0017560 -0.0502954  0.0017593 -0.0074092  0.0017632  0.0013092  0.0017568 -0.0321244  0.0017564 -0.0164403  0.0017614  0.0000180      38593
 129.8  0.0038832  0.0000052  0.0346053  0.0000052  0.0015528  0.0000052 -0.0145444  0.0000052  0.0004925  0.0017567 -0.0449161  0.0017556 -0.0286177  0.0017548  0.0075796  0.0017596 -0.0093185  0.0017597 -0.0171924  0.0017556  0.0000177      38212
 141.6 -0.0061087  0.0000052  0.0037970  0.0000052 -0.0129179  0.0000051 -0.0190608  0.0000052  0.0044642  0.0017619 -0.0241372  0.0017484 -0.0388432  0.0017508 -0.0085551  0.0017578 -0.0009124  0.0017560 -0.0061874  0.0017553  0.0000175      37792
 153.4 -0.0065594  0.0000051 -0.0188136  0.0000052 -0.0197994  0.0000051 -0.0324252  0.0000052 -0.0051803  0.0017567 -0.0022034  0.0017500 -0.0248754  0.0017549 -0.0181516  0.0017503 -0.0005297  0.0017574 -0.0022869  0.0017494  0.0000172      37371
 165.2 -0.0010009  0.0000051 -0.0335534  0.0000052 -0.0167724  0.0000051 -0.0453826  0.0000052 -0.0226977  0.0017476  0.0089888  0.0017540 -0.0072835  0.0017488 -0.0119663  0.0017536  0.0028156  0.0017559 -0.0031082  0.0017468  0.0000170      36983
 177.0  0.0073620  0.0000051 -0.0402837  0.0000051 -0.0062724  0.0000051 -0.0515630  0.0000051 -0.0399376  0.0017464  0.0016746  0.0017492 -0.0037399  0.0017500 -0.0019716  0.0017483  0.0071393  0.0017511  0.0027499  0.0017472  0.0000168      36586
 188.8  0.0158582  0.0000051 -0.0409735  0.0000051  0.0036469  0.0000051 -0.0473049  0.0000051 -0.0425736  0.0017457 -0.0155699  0.0017448 -0.0018466  0.0017527  0.0026666  0.0017414  0.0020900  0.0017497  0.0124803  0.0017441  0.0000166      36182
 200.6  0.0227334  0.0000051 -0.0371576  0.0000051  0.0085334  0.0000051 -0.0337201  0.0000051 -0.0303306  0.0017474 -0.0240984  0.0017399 -0.0041470  0.0017514  0.0006563  0.0017385 -0.0052252  0.0017416  0.0134191  0.0017480  0.0000164      35793
 212.4  0.0265865  0.0000051 -0.0313617  0.0000051  0.0060522  0.0000051 -0.0172591  0.0000051 -0.0158474  0.0017447 -0.0203815  0.0017401 -0.0011870  0.0017470 -0.0008468  0.0017389 -0.0032276  0.0017402  0.0103123  0.0017455  0.0000162      35412
```

# User guide

It is necessary to install the [NVIDIA CUDA Toolkit](https://developer.nvidia.com/cuda-downloads) to compile the OpenCL code on NVIDIA GPUs.

The directory `tests/` contains several examples of package module usage. The docstrings are carefully written and it is recommended to refer to the documentation with the `help()` python command.

The installation from PyPI does not allow the modification of the code. To edit the package and contribute to the development use the following commands in your working directory.
```bash
pip uninstall lpa-xrd
git clone https://github.com/DunstanBecht/lpa-xrd.git
pip install -e lpa-xrd
cd lpa-xrd
git branch <name_of_your_new_branch>
```
