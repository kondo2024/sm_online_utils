# utility package for SAMURAI online analysis
This package contains these utilities.
- converters and filters for the data merge
- (onlinemonitor) [http://github.com/kondo2024/onlinemonitor]
- (dceffplot) [http://github.com/kondo2024/dceffplot]

## Installation
Required
- ANAROOT, ROOT

```
git clone --recursive https://github.com/kondo2024/sm_online_utils.git
cd sm_online_utils
mkdir build
mkdir install
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install ../
```

## How to use
### converters
1. Add include path of lib/smconverterlib and load
   lib/smconverter/libsmconverter.so in rootlogon.C.
2. ROOT[0] .L OnlineMonitor.cc+g
3. ROOT[1] OnlineMonitor *mon = new OnlineMonitor() 
   // for online data, input ridf file name for offline analysis 
4. ROOT[2] mon->Run()

click the canvas and press any key, then analysis is paused and
several keyboard commands will be available. Ctrl+c stops 
OnlineMonitor.

## Filter
This extracts and saves detector data which coincide with the SAMURAI
timestamp. NPTOOL is required for complation for PFAD
filter. $NPTOOL_HOME have to be defined.

### Onlinemonitor
```
onlinemonitor
```
(for using THttpServer)

```
onlinemonitor xxxx.ridf
```
(for analysis of ridf file with TCanvas display, similar to old version)

## macros
sample root macros.

## prev/sXXX/
Old codes used in the previous experiments.

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
processors, Run should be called. Then, preparation of tree/histograms
and eventloop will happens. The class is called in
sXXX/converters/smts_converter.cc, etc.

### filter
Directory structure is like this. 

```
sm_online_utils
├── README.md
├── converter
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
├── filters
├── macros
│   ├── chkmerge.cc
│   ├── make_dctdcdists.cc
│   ├── make_dctdcdists_multi.cc
│   └── pfad_filter_run.cc
├── dceffplot
├── onlinemonitor
└── prev
    ├── s0XX (old packaged used in previous exps)
    └── ...
```

