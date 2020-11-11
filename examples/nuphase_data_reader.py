#! /usr/bin/env python 

# Example for how to read nuphase data with python This mostly defines a
# nuphase_data_reader class, but if you run it as a script it  will try to plot
# some waveforms from run 360. Normally, you would do something like
#    
#   import nuphase_data_reader 
#   d = nuphase_data_reader.Reader("/path/to/data",runno)  
#   d.setEntry(someInterestingEntry) 
#   d.wf(0) # get a numpy array of channel 0 


from __future__ import division, print_function

import ROOT
import numpy 
import sys
import os 
## You need to load the libnuphaseroot library
## For the following line to work, the shared library must be in 
## your dynamic loader path (LD_LIBRARY_PATH or DYLD_LIBRARY_PATH, accordingly) 

ROOT.gSystem.Load("libnuphaseroot.so"); 


## A class to read in data from a run
##  
##   e.g. d = nuphase_data_reader(runno)  # if $NUPHASE_ROOT_DATA isn't defined, you can pass base_dir as second argument
##
##   To select the 100th entry in the run
##     d.setEntry(65079); 
##   Or, you can select by event number instead 
##     d.setEvent(360000065079); 
##  
##   You can then obtain the corresponding header, event or status objects
##     d.header()
##     d.event() 
##     d.status()  (this gets the status with the closest readout time to the event) 
##    
##   For now, you can look at the c++ doc to see what is in each
##  
##  You can also numpy arrays of the waveform values and time using 
##  d.wf(channel)  
##  Channels 0-7 are on the master board, channels 8-11 are on the slave. 
##   
##  For convenience, d.t() makes a time array with the right number of samples (the sample rate is always 1.5 GSPS) 

class Reader:

  def __init__(self, run, base_dir=None):
    
    if base_dir == None: 
        base_dir = os.environ["NUPHASE_ROOT_DATA"] 
    self.run = int(run); 
    self.base_dir = base_dir

    self.event_file = ROOT.TFile.Open("%s/run%d/event.root" % (base_dir, run))
    self.event_tree = self.event_file.Get("event") 
    self.event_tree.BuildIndex("event.event_number")
    self.evt = None
    self.event_entry = -1; 

    self.head_file = ROOT.TFile.Open("%s/run%d/header.filtered.root" % (base_dir, run))
    self.head_tree = self.head_file.Get("header") 
    self.head = None
    self.head_entry = -1
    self.head_tree.BuildIndex("header.event_number") 

    self.status_file = ROOT.TFile.Open("%s/run%d/status.root" % (base_dir, run))
    self.status_tree = self.status_file.Get("status") 
    self.stat= None
    self.status_tree.BuildIndex("status.readout_time","status.readout_time_ns"); 
    self.status_entry =-1; 

    self.current_entry = 0; 
    
  def setEntry(self,i): 
    if (i < 0 or i >= self.head_tree.GetEntries()):
      sys.stderr.write("Entry out of bounds!") 
    else: 
      self.current_entry = i; 

  def setEvent(self,i):
    setEntry(self.head_tree.GetEntryNumberWithIndex(i)) 


  def event(self,force_reload = False): 
    if (self.event_entry != self.current_entry or force_reload):
      ev_number = self.header(force_reload).event_number
      i = self.event_tree.GetEntryNumberWithIndex(ev_number,0)
      self.event_tree.GetEntry(i) 
      self.event_entry = self.current_entry 
      self.evt = self.event_tree.event
    return self.evt 


  def wf(self,ch = 0):  

    bd = ROOT.nuphase.BOARD_MASTER;
    if (ch > 8) :
        ch-=8
        bd = ROOT.nuphase.BOARD_SLAVE
    ## stupid hack because for some reason it doesn't always report the right buffer length 
    return numpy.frombuffer(self.event().getData(ch,bd), numpy.dtype('float64'), self.event().getBufferLength()) - 64 

  def t(self):
    return numpy.linspace(0, self.event().getBufferLength() /1.5, self.event().getBufferLength()) 

  def header(self,force_reload = False): 
    if (self.head_entry != self.current_entry or force_reload): 
      self.head_tree.GetEntry(self.current_entry); 
      self.head_entry = self.current_entry 
      self.head = self.head_tree.header
    return self.head 

  def status(self,force_reload = False): 
    if (self.status_entry != self.current_entry or force_reload): 
      self.status_tree.GetEntry(self.status_tree.GetEntryNumberWithBestIndex(self.header().getReadoutTime(), self.header().getReadoutTimeNs()))
      self.status_entry = self.current_entry
      self.stat = self.status_tree.status

    return self.stat


  def N(self): 
    return self.head_tree.GetEntries() 


# Plot some waveforms is run as a binary 
if __name__=="__main__": 

## pyplot for plotting
  import matplotlib.pyplot as plt

# If your data is elsewhere, pass it as an argument
  datapath = sys.argv[1] if len(sys.argv) > 1 else os.environ["NUPHASE_ROOT_DATA"]

# look at run 360
  d = Reader(5562, datapath) 
# this is a SPIceCore event
  d.setEntry(0) 

## dump the headers and status, just to show they're there
  d.header().Dump(); 
  d.status().Dump(); 
  print(d.N())

# plot all waveforms
  for i in range(16): 
    plt.subplot(4,4,i+1); 
    plt.plot(d.t(), d.wf(i))
  
  plt.show() 




  








