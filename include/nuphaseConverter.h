#ifndef NUPHASE_CONVERTER_H
#define NUPHASE_CONVERTER_H

/** This file implements converters from raw  files to a ROOT tree
 * Obviously, it requires libnuphsae
 */

namespace nuphase
{

  namespace convert
  {


#ifdef HAVE_LIBNUPHASE
 
  
  /** Are we allowed to overwrite files? (default is no) */ 
  void setAllowOverwrite(bool allow);  

  /** Convert a set of Hk files to a ROOT tree */ 
  int convertHkFiles(int nfiles, const char ** infiles, const char * outfile, const char * treeName = "hk"); 

  /** Convert a directory of Hk files to a ROOT tree. Files will be sorted by mtime */ 
  int convertHkDir(const char * dir, const char * outfile, const char * treeName = "hk"); 

  /** Convert a single Hk file into a ROOT file */ 
  int convertHkFile(const char * infile, const char * outfile, const char * treeName = "hk") { convertHkFiles(1,&infile,outfile, treeName); } 

  /** Convert a set of Header files to a ROOT tree */ 
  int convertHeaderFiles(int nfiles, const char ** infiles, const char * outfile, const char * treeName = "header"); 

  /** Convert a directory of Header files to a ROOT tree. Files will be sorted by mtime */ 
  int convertHeaderDir(const char * dir, const char * outfile, const char * treeName = "header"); 

  /** Convert a single Header file into a ROOT file */ 
  int convertHeaderFile(const char * infile, const char * outfile, const char * treeName = "header") { convertHeaderFiles(1,&infile,outfile, treeName); } 

  /** Convert a set of Status files to a ROOT tree */ 
  int convertStatusFiles(int nfiles, const char ** infiles, const char * outfile, const char * treeName = "status"); 

  /** Convert a directory of Status files to a ROOT tree. Files will be sorted by mtime */ 
  int convertStatusDir(const char * dir, const char * outfile, const char * treeName = "status"); 

  /** Convert a single Status file into a ROOT file */ 
  int convertStatusFile(const char * infile, const char * outfile, const char * treeName = "status") { convertStatusFiles(1,&infile,outfile, treeName); } 

  /** Convert a set of Event files to a ROOT tree */ 
  int convertEventFiles(int nfiles, const char ** infiles, const char * outfile, const char * treeName = "event"); 

  /** Convert a directory of Event files to a ROOT tree. Files will be sorted by mtime */ 
  int convertEventDir(const char * dir, const char * outfile, const char * treeName = "event"); 

  /** Convert a single Event file into a ROOT file */ 
  int convertEventFile(const char * infile, const char * outfile, const char * treeName = "event") { convertEventFiles(1,&infile,outfile, treeName); } 


  int makeFilteredHeadTree(const char * filtered_event_file, const char * full_head_file, const char  filtered_head_file = 0); 

  int convertRun(const char * basedir_raw, const char * basedir_root, int run); 

#endif
  }
}



#endif
