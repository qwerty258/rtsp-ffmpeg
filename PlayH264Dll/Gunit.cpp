#include "stdafx.h"
#include <fstream>

 void WriteLog(const char* filename,const char * str)
{
 
 FILE *logf=fopen(filename,"a+");
 if (logf==NULL) return;
 //int t=GetTickCount();
  fprintf(logf,"%d,%s \n",1,str);
  fclose(logf);
  return;
}