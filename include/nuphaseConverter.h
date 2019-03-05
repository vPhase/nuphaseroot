#ifndef NUPHASE_CONVERTER_H
#define NUPHASE_CONVERTER_H

/** This file implements converters from raw  files to a ROOT tree
 * Obviously, it requires libnuphsae
 */

namespace nuphase
{

  namespace convert
  {


 
  
  /** Are we allowed to overwrite files? (default is no) */ 
  void setAllowOverwrite(bool allow);  

  /** Convert a set of Hk files to a ROOT tree */ 
  int convertHkFiles(int nfiles, const char ** infiles, const char * outfile); 

  /** Convert a directory of Hk files to a ROOT tree. Files will be sorted by mtime */ 
  int convertHkDir(const char * dir, const char * outfile); 

  /** Convert a single Hk file into a ROOT file */ 
  inline int convertHkFile(const char * infile, const char * outfile) { return convertHkFiles(1,&infile,outfile); } 

  /** Convert a set of Header files to a ROOT tree */ 
  int convertHeaderFiles(int nfiles, const char ** infiles, const char * outfile); 

  /** Convert a directory of Header files to a ROOT tree. Files will be sorted by mtime */ 
  int convertHeaderDir(const char * dir, const char * outfile); 

  /** Convert a single Header file into a ROOT file */ 
  inline int convertHeaderFile(const char * infile, const char * outfile) {return  convertHeaderFiles(1,&infile,outfile); } 

  /** Correct Trig_time in a single Header ROOT file */
  int correctHeaderFileTime(const char * infileHeader, const char * infileStatus, const char * outfile);

  /** Convert a set of Status files to a ROOT tree */ 
  int convertStatusFiles(int nfiles, const char ** infiles, const char * outfile); 

  /** Convert a directory of Status files to a ROOT tree. Files will be sorted by mtime */ 
  int convertStatusDir(const char * dir, const char * outfile); 

  /** Convert a single Status file into a ROOT file */ 
  inline int convertStatusFile(const char * infile, const char * outfile) { return convertStatusFiles(1,&infile,outfile); } 

  /** Convert a set of Event files to a ROOT tree */ 
  int convertEventFiles(int nfiles, const char ** infiles, const char * outfile); 

  /** Convert a directory of Event files to a ROOT tree. Files will be sorted by mtime */ 
  int convertEventDir(const char * dir, const char * outfile); 

  /** Convert a single Event file into a ROOT file */ 
  inline int convertEventFile(const char * infile, const char * outfile) { return convertEventFiles(1,&infile,outfile); } 

  int makeFilteredHeadTree(const char * filtered_event_file, const char * full_head_file, const char * filtered_head_file); 

  int convertRun(const char * basedir_raw, const char * basedir_root, int run); 

  }
}



#endif
