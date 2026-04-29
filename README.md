# utility package for SAMURAI online analysis
This package contains these packages.
- smconverter: convert ridf file to root tree
- ANAROOT macros
- [onlinemonitor](http://github.com/kondo2024/onlinemonitor): Drawer of online data
- [dceffplot](http://github.com/kondo2024/dceffplot): Plotter of DC efficiency curve

## Installation
Requirements
- ANAROOT, ROOT (v6)

1. set TARTSYS environmental variable, using $TARTSYS/script/setup.sh
2. download and compile
```
git clone --recursive https://github.com/kondo2024/sm_online_utils.git
cd sm_online_utils
mkdir build
mkdir install
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install ../
```

If you forgot "--recursive" during clone, run:
`git submodule update --init --recursive`

## How to use
### smconverter
1. set PATH and LD_LIBRARY_PATH for smconverter.so
```
source install/bin/setup_smconverter.sh
```
2. run converter like this.
```
smts_converter xxxxx.ridf yyyyy.root
```
In case of drift chambers, a root file containing tdc spectra should be prepared for space-time conversion. It can be made by macros/make_dctdcdists_multi.cc. A converter should run like this.
```
bdc_converter xxxxx.ridf yyyyy.root tdcspectrum_runZZZZ.root
```

currently these converters are availble
- smts_converter: converter for SAMURAI Timestamp
- plastic_converter: contains coincidence reg., beamline plastics, HOD, NEBULA
- bdc_converter: contains BDC1 and BDC2
- fdc1_converter
- fdc2_converter
- ninja_converter

### macros
sample root macros. See top lines of the file to see how to use it.

### onlinemonitor
see [README](http://github.com/kondo2024/onlinemonitor) of onlinemonitor.

### dceffplot: DC Efficiency plotter
see [README](http://github.com/kondo2024/dceffplot) of dceffplot. 

### prev/sXXX/
Old codes used in previous experiments.

### Filter
Under development, old files are stored in prev/filters directory.


- - -
## Notes for developers

### smconverter
This converts the RIDF data to root tree and/or historgrams.

##### XXXXDataProcessor
Load parameter file XXXX.xml,call TArtCalibXXXX,prepare tree,
histograms. All the processors have to inherit
SAMURAIDataProcessor. In case of drift chambers TDC distributions are
also loaded for drift time -> drift length calibration.

##### SAMURAIDataConverter
main class of the converter. After registration of the data
processors, Run() should be called. Then, preparation of tree/histograms
and event loop will happens. The class is called in main functions, such as smconverters/apps/smts_converter.cc. 

### Directories
Directory structure is like this. 

```
sm_online_utils
├── README.md
├── smconverter
│   ├── apps
│   │   ├── plastics_converter.cc
│   │   ├── bdc_converter.cc
│   │   └── ...
│   ├── CMakeLists.txt
│   ├── include
│   └── src
│       ├── PlasticDataProcessor.cc
│       ├── BDCDataProcessor.cc
│       └── ...
├── macros
│   ├── make_dctdcdists.cc
│   └── make_dctdcdists_multi.cc
├── dceffplot
├── onlinemonitor
└── prev
    ├── s0XX (old packages used in previous exps)
    └── ...
```

