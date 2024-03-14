# utility package for SAMURAI online analysis
This package contains Converters and Filters for the data merge as
well as online monitor. Directory structure is like this. 

```
sm_online_utils
├── README.md
├── lib
│   ├── smconverter
│   └── smfilter
└─sXXX          (for a specific experiment)
    ├── bin
    ├── converters
    │   ├── Makefile
    │   └── smts_converter.cc
    ├── filters
    │   ├── Makefile
    │   └── pfad_data_filter.cc
    ├── macros
    │   ├── chkmerge.cc
    │   └── pfad_filter_run.cc
    └── onlinemonitor
        ├── OnlineMonitor.cc
        └── OnlineMonitor.hh
```

Required

- ANAROOT
- NPTOOL

## Converter
This converts the RIDF data to root tree.

## Filter
This extracts and saves detector data which coincide with the SAMURAI
timestamp. NPTOOL is required for complation for PFAD
filter. $NPTOOL_HOME have to be defined.

### DataMergeFilter
Sample base class. You can inherit it for your application.
DataMergeFilter_PFAD class is an example. Most of cases can be covered
by the methods in the base class, but you have to write some part
(indicated by *****) in DataMergeFilter_XXX::Filter().

### sXXX/filters/pfad_data_filter.cc
Sample main program for PFAD using DataMergeFilter_PFAD. "cd src;
make; make install" generates executable file. NPTOOL is required and
$NPTOOL_HOME is needed to be defined (see Makefile).

### sXXX/macros/pfad_run.cc
sample root macro to use DataMergeFilter_PFAD. They are useful to see what
is going on in the procedure shown on Canvas. 

### sXXX/macros/chkmerge.cc
sample root macro to make plots from merged tree data.


## OnlineMonitor
Online data monitor for checking rawdata during beam time.

### usage
1. Add include path of lib/smconverterlib and load
   lib/smconverter/libsmconverter.so in rootlogon.C.
2. ROOT[0] .L OnlineMonitor.cc+g
3. ROOT[1] OnlineMonitor *mon = new OnlineMonitor() 
   // for online data, input ridf file name for offline analysis 
4. ROOT[2] mon->Run()

click the canvas and press any key, then analysis is paused and
several keyboard commands will be available. Ctrl+c stops 
OnlineMonitor.

- - -

## Notes for developers

### converter

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

##### DataMergeFilter
Base class for filter. This class has several useful methods to
automatically search time stamp offset, correction factor of the time
stamp period, etc. 

##### DataMergeFilter_XXXX
This class inherits the base class (DataMergeFilter) for a specific
detector system. Filter() have to be implemented for the definition of
the tree branch and initialization. It is recommended to implement
Run() for easy use. The class is called in sXXX/filters/XXXX_filter.cc 
