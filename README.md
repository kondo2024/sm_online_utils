# utility package for SAMURAI online analysis
This package contains Converters, Filters for the data merge as well
as online monitor. They uses converterlib and filterlib. Directory is
like this. 

smutils
 |- converterlib
 |- filterlib
 |- sXXX    // for a specific experiment
     |- converters
     |- filters
     |- onlinemonitor

## Converter
This converts the RIDF data to root tree.

## Filter
This extracts and saves detector data which coincide with the SAMURAI
timestamp.

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
1. Add includ path of smconverterlib and load libsmconverter.so in root.
2. ROOT[0] .L OnlineMonitor.cc+g
3. ROOT[1] OnlineMonitor *mon = new OnlineMonitor() 
   // for online data, input ridf file name for offline analysis 
4. ROOT[2] mon->Run()

click the canvas and press any key, then analysis is paused and
several commands will be available.
