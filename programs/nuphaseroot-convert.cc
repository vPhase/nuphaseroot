/** 
 *
 *  Converter utility program. Basically a front-end to nuphase::convert::. 
 *
 */ 

#include <stdio.h> 
#include "nuphaseConverter.h" 
#include <sys/stat.h> 
#include <string.h> 

void printUsage()
{
  printf("tldr: nuphaseroot-convert  what=[hk status header event] input=[input file | input files | input dir ] output_file.root \n\n"); 
  printf("nuphaseroot-convert will convert raw nuphase files into a root tree. The second argument is the type of file to convert (hk, status, header or event). The last argument is the output ROOT file. Between the type and the output can be a single file, multiple files, or a single directory.\n"); 
}


int main (int nargs, char ** args) 
{

  if (nargs < 4)
  {
    printUsage(); 
    return 1; 
  }
 
  char * what = args[1]; 

  int ninputs = nargs - 3; 


  bool is_dir = false; 
  if (ninputs == 1) 
  {
    //check if we have a directory
    struct stat sb; 
    if (stat(args[2],&sb) && S_ISDIR(sb.st_mode))
    {
      is_dir = true; 
    }
  }


  if (!strcasecmp(what,"hk"))
  {
    if (is_dir) nuphase::convert::convertHkDir(args[2],args[3]);
    else nuphase::convert::convertHkFiles(ninputs, (const char **) args+2, args[nargs-1]); 
  }
  else if (!strcasecmp(what,"status"))
  {
    if (is_dir) nuphase::convert::convertStatusDir(args[2],args[3]);
    else nuphase::convert::convertStatusFiles(ninputs, (const char **) args+2, args[nargs-1]); 
  }
  else if (!strcasecmp(what,"header"))
  {
    if (is_dir) nuphase::convert::convertHeaderDir(args[2],args[3]);
    else nuphase::convert::convertHeaderFiles(ninputs, (const char **) args+2, args[nargs-1]); 
  }
  else if (!strcasecmp(what,"event"))
  {
    if (is_dir) nuphase::convert::convertEventDir(args[2],args[3]);
    else nuphase::convert::convertEventFiles(ninputs, (const char **) args+2, args[nargs-1]); 
  }
  else
  {
    printUsage(); 
    return 1; 
  }


  return 0; 
}



