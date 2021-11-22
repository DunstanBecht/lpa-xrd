[![DOI](https://zenodo.org/badge/394667513.svg)](https://zenodo.org/badge/latestdoi/394667513)
[![License: CC0-1.0](https://img.shields.io/badge/License-CC0_1.0-lightgrey.svg)](http://creativecommons.org/publicdomain/zero/1.0/)

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
   0.9.6 # v: lpa-xrd version
5.00e+13 # d: dislocation density [m^-2]
 1 -1 -2 # z: direction of 'l' (line vector) [uvw]
 1  1  0 # b: Burgers vector direction [uvw]
 2  0  0 # g: diffraction vector direction (hkl)
0.266310 # C: contrast coefficient [1]
0.404940 # a: cell parameter [nm]
    1600 # s: radius of the region of interest [nm]
0.345000 # nu: Poisson's number [1]
     402 # nd: number of dislocations in the input file
  200000 # np: number of random points
#    L       cos1   err_cos1       sin1   err_sin1       cos2   err_cos2       sin2   err_sin2       cos3   err_cos3       sin3   err_sin3       cos4   err_cos4       sin4   err_sin4       cos5   err_cos5       sin5   err_sin5    <eps^2> bad_points
  11.8  0.9691774  0.0002213  0.0354857  0.0004982  0.8981967  0.0004647  0.0644053  0.0008541  0.8039566  0.0007153  0.0843319  0.0011050  0.6998830  0.0009444  0.0939833  0.0012708  0.5951457  0.0011361  0.0939301  0.0013763  0.0052798          0
  23.6  0.8966182  0.0004707  0.0643027  0.0008591  0.6964875  0.0009543  0.0927513  0.0012731  0.4935653  0.0012892  0.0856287  0.0014434  0.3344515  0.0014581  0.0564801  0.0015162  0.2237613  0.0015302  0.0220328  0.0015510  0.0025296          0
  35.4  0.7995268  0.0007296  0.0829412  0.0011122  0.4914096  0.0012917  0.0839892  0.0014453  0.2707633  0.0015042  0.0379384  0.0015374  0.1503394  0.0015619 -0.0079713  0.0015643  0.0868508  0.0015814 -0.0313676  0.0015673  0.0016052          0
  47.2  0.6921182  0.0009628  0.0907089  0.0012794  0.3288427  0.0014636  0.0537124  0.0015175  0.1490961  0.0015620 -0.0091325  0.0015648  0.0730938  0.0015835 -0.0365165  0.0015682  0.0367541  0.0015870 -0.0396468  0.0015706  0.0011040          0
  59.0  0.5840879  0.0011574  0.0890284  0.0013839  0.2181087  0.0015337  0.0179659  0.0015518  0.0876970  0.0015807 -0.0339458  0.0015675  0.0360840  0.0015877 -0.0410103  0.0015698  0.0191976  0.0015826 -0.0309166  0.0015776  0.0008361          2
  70.8  0.4832729  0.0013059  0.0796319  0.0014475  0.1492776  0.0015611 -0.0119999  0.0015655  0.0503282  0.0015903 -0.0420069  0.0015651  0.0193820  0.0015824 -0.0344922  0.0015774  0.0165531  0.0015823 -0.0236688  0.0015787  0.0006719          3
  82.6  0.3945230  0.0014104  0.0652190  0.0014871  0.1069602  0.0015752 -0.0301631  0.0015675  0.0297516  0.0015884 -0.0400960  0.0015699  0.0153246  0.0015808 -0.0290794  0.0015798  0.0159336  0.0015788 -0.0173461  0.0015827  0.0005566          4
  94.4  0.3201728  0.0014756  0.0475958  0.0015161  0.0760114  0.0015836 -0.0380712  0.0015672  0.0212624  0.0015825 -0.0351937  0.0015771  0.0146257  0.0015811 -0.0221551  0.0015801  0.0161533  0.0015780 -0.0156962  0.0015835  0.0004742          5
 106.2  0.2601319  0.0015134  0.0293876  0.0015385  0.0514778  0.0015901 -0.0411058  0.0015653  0.0179082  0.0015828 -0.0303686  0.0015775  0.0166580  0.0015784 -0.0155839  0.0015831  0.0111672  0.0015819 -0.0151724  0.0015799  0.0004103          9
 118.0  0.2126806  0.0015352  0.0129292  0.0015544  0.0332013  0.0015926 -0.0414957  0.0015652  0.0157721  0.0015825 -0.0264891  0.0015784  0.0166581  0.0015794 -0.0145700  0.0015822  0.0054224  0.0015825 -0.0104530  0.0015797  0.0003576         11
 129.8  0.1755840  0.0015508 -0.0003920  0.0015624  0.0236180  0.0015916 -0.0394686  0.0015673  0.0152945  0.0015801 -0.0235095  0.0015811  0.0143418  0.0015811 -0.0146568  0.0015807  0.0022709  0.0015846 -0.0087315  0.0015777  0.0003154         15
 141.6  0.1467438  0.0015632 -0.0101635  0.0015648  0.0204200  0.0015883 -0.0346822  0.0015715  0.0122803  0.0015802 -0.0170805  0.0015815  0.0098817  0.0015845 -0.0102362  0.0015776 -0.0004490  0.0015850 -0.0078732  0.0015773  0.0002816         17
 153.4  0.1237572  0.0015723 -0.0163748  0.0015654  0.0194010  0.0015841 -0.0297306  0.0015764  0.0088389  0.0015810 -0.0130033  0.0015810  0.0043625  0.0015876 -0.0059489  0.0015748 -0.0023964  0.0015849 -0.0049677  0.0015775  0.0002541         24
 165.2  0.1047742  0.0015759 -0.0197170  0.0015686  0.0152115  0.0015790 -0.0258545  0.0015821  0.0064488  0.0015826 -0.0093539  0.0015797 -0.0024127  0.0015870 -0.0065361  0.0015754  0.0004586  0.0015821 -0.0024844  0.0015804  0.0002320         30
 177.0  0.0875418  0.0015761 -0.0220423  0.0015735  0.0088049  0.0015779 -0.0253654  0.0015835  0.0049849  0.0015798 -0.0070509  0.0015826 -0.0040824  0.0015835 -0.0070113  0.0015789 -0.0010836  0.0015804  0.0064444  0.0015821  0.0002127         36
 188.8  0.0716121  0.0015754 -0.0247094  0.0015782  0.0027748  0.0015790 -0.0261702  0.0015826  0.0044388  0.0015808 -0.0030478  0.0015817 -0.0029691  0.0015852 -0.0007737  0.0015774  0.0000571  0.0015776  0.0117098  0.0015848  0.0001948         46
 200.6  0.0571829  0.0015770 -0.0283287  0.0015793  0.0009892  0.0015790 -0.0243663  0.0015827  0.0026351  0.0015877  0.0005714  0.0015750 -0.0029458  0.0015836  0.0045666  0.0015791 -0.0008007  0.0015725  0.0113606  0.0015899  0.0001792         53
 212.4  0.0446177  0.0015808 -0.0332234  0.0015770  0.0032781  0.0015777 -0.0188957  0.0015845 -0.0015135  0.0015908  0.0000217  0.0015719 -0.0046089  0.0015792  0.0074308  0.0015835 -0.0013925  0.0015730  0.0093487  0.0015896  0.0001658         63
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
