# utility package for SAMURAI online analysis
This package contains these packages.
- smconverter: convert ridf file to root tree
- ANAROOT macros
- [onlinemonitor](http://github.com/kondo2024/onlinemonitor): Drawer of online data
- [dceffplot](http://github.com/kondo2024/dceffplot): Plotter of DC efficiency curve

## Installation
Required
- ANAROOT, ROOT (v6)

```
git clone --recursive https://github.com/kondo2024/sm_online_utils.git
cd sm_online_utils
mkdir build
mkdir install
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install ../
```

## How to use
### smconverter
1. Add include path of lib/smconverterlib and load
   lib/smconverter/libsmconverter.so in rootlogon.C.

click the canvas and press any key, then analysis is paused and
several keyboard commands will be available. Ctrl+c stops 
OnlineMonitor.

### macros
sample root macros.

### Onlinemonitor
see [README](http://github.com/kondo2024/onlinemonitor) of onlinemonitor.

### DC Efficiency plotter
see [README](http://github.com/kondo2024/dceffplot) of dceffplot. 

### prev/sXXX/
Old codes used in the previous experiments.

### Filter
Under development, old files are stored in prev/filters directory.


- - -
## Notes for developers

### converter
This converts the RIDF data to root tree and/or historgrams.
 

##### XXXXDataProcessor
Load parameter file XXXX.xml,call TArtCalibXXXX,prepare tree,
histograms. All the processors have to inherit
SAMURAIDataProcessor. In case of drift chambers TDC distributions are
also loaded for drift time -> drift length calibration.

##### SAMURAIDataConveter
main class of the converter. After registration of the data
processors, Run() should be called. Then, preparation of tree/histograms
and event loop will happens. The class is called in main functions, such as smconverters/apps/smts_converter.cc. 

### filter
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
    ├── s0XX (old packaged used in previous exps)
    └── ...
```

