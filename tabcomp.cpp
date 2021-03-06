//---------------------------------------------------------------------
//   Thomas Ortlepp  15.02.2001
//   Funktionstest f�r Schaltungen
//   Vergleicht von $1.ext und $1.soll
//---------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "common.hpp"

#define ja   1
#define nein 0

int fertig=0;

int main (int argc, const char *argv[])
  {
    char s1[100],s2[100];
    double w1,w2;
    char fa[250],fb[250],f3[50]=".conf";
    FILE *ex,*ey;
    if ((argc==3)&&(argv[2][0]=='v'))  output=0;
    
    if (output==1) printf("Table Compare V 1.2  Thomas Ortlepp 16.10.2001 \n");
    if (output==1) printf("-----------------------------------------------\n");
    if (argc>=2)
      {
        strcpy(fconf,argv[1]); 	
	strcat(fconf,f3);
        readfilename(fa,"extout");

	readfilename(fb,"compin");
	//t1=atof(argv[2]);
	//t2=atof(argv[3]);	
      }	
    else    
      {
        printf("Programmaufruf: \n");      
        printf("tabcomp config-filename \n");      	
        printf("Vergleicht zwischen extout und compin\n");   
	printf("Diese Dateiname muessen im .conf File stehen.\n");   	
	exit(1);
      }	

    int error=0;
    if (output==1) printf("Vergleich der Dateien %s mit %s\n",fa,fb);
    ex=fopen(fa,"r+");
    ey=fopen(fb,"r+");
    
       while (!(feof(ex)))    
        {
          fscanf(ex,"%s",s1);   w1=atof(s1);
          fscanf(ey,"%s",s2);   if (strcmp(s2,"x")==0) w2=w1;  //beliebig
	                          else w2=atof(s2);
	   	  
	  if (fabs(w1-w2)>0.1)
	    {
              printf("-1\n");
	      printf("Fehler!  %s != %s \n",s1,s2);
	      error=1;
	    }
	}        
	
    fclose(ex); 
    fclose(ey);
    if (error==0) printf("0\n");            
    return 0;
  }
