//---------------------------------------------------------------------
//   Thomas Ortlepp  11.09.2001 / 15.04.2002 / 17.05.2004 / 22.06.2004
//   Version 2.1
//   Phasendaten aus dat File ausschneiden
//   
//---------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "common.hpp"

#define ja   1
#define nein 0

int ende=0;
char fa[250],fb[250];
double maxzerovoltage=0.0, minonevoltage=80.0,voltagesw;

int main (int argc, char *argv[])
  {
    output=nein;
      
    printf("---------------------------------------------------\n");
    printf("ext v 2.11  Thomas Ortlepp 11.09.2001 - 01.07.2004 \n");  
    printf("---------------------------------------------------\n");
    char s[100],f3[50]=".conf";

    int c,n,q=0,zd=0,Q=1,h;
    double tx=40.0e-12,eps=1e-15;
    printf("processing of simulation data for comparision to *.soll file.\n");
    FILE *ex,*ey;
    double w;
    if (argc==2)
      {
	strcpy(fconf,argv[1]);   // config-File   .conf
	strcat(fconf,f3);          
	readfilename(fa,"simout");
	readfilename(fb,"extout");
	
	voltagesw=readvalue("voltageswitch");
	if (voltagesw!=0.0)
	  {
	    maxzerovoltage=readvalue("maxzerovoltage");
	    minonevoltage=readvalue("minonevoltage");	    	    
	    printf("Last coloumn is a voltage (0:%lf,1:%lf)\n",
         	      maxzerovoltage,minonevoltage);
	  }
	
 //   printf("Einlesen der Datei :%s  %s\n",fa,fb);	
//	c=1-n;
        n=100;
        c=-99;
      }	
    else    
      {
        printf("How to call ext: \n");      
        printf("ext config-filename \n");      	
	printf("simout, extout, timestep has to be defined in *.conf file !\n");
        printf("input file: simout  and output file: extout \n"); 
	exit(1);
      }	
    tx=1e-12*readvalue("timestep");
  // printf("Einlesen der Datei :%s  %s\n",fa,fb);	
      /*
    fprintf(stderr, "Reading data:%s\n",fa);
    fflush(stdout);
      */
      
    ex=fopen(fa,"r+");
    if (ex==NULL) {
        fprintf(stderr, "Unable to open: %s\n", fa);
        exit(-4);
    }
    ey=fopen(fb,"w+");
      
    if (ey==NULL) {
        fprintf(stderr, "Unable to open: %s\n", fa);
        exit(-5);
    }
     
       fscanf(ex,"%s",s);
//     printf("%s\n",s);
      while (strcasecmp(s,".END")!=0) {    // Vergleich ".end"
           fscanf(ex,"%s",s);
          if (feof(ex)) {
              fprintf(stderr, "End of file found before finding '.END'\n");
              break; // Should this be an error?
          }
      }
 
       int outx=0;
       
       if (output==ja) printf("%s\n",s);
       fscanf(ex,"%s",s);

       ende=0;       
       while ((ende!=-1)&&(strcmp(s,"loop")!=0))    // Kopie bis "loop" oder eof
        {
          if (output==ja)  printf("#%s ",s);
	  w=atof(s);
	  if( (c<0)&&(fabs(w)>eps) )
	    {
	      n=99+c;
              c=1;
              if (output==ja)  printf("Z�hler: n=%d ",n);
              q=0;
              if (outx==1) fprintf(ey,"\n");	    
	      zd++;	      
	    }  
//	  fprintf(ey,"%s ",s);
          if (c%n==1)
	    {
	      if (w>Q*tx-eps) { outx=1; Q++; }
	       else outx=0;
	    }   
      //  if ((c%n==1)) printf("%le  %le\n",w,Q*tx);      
//	  printf("%d ",c);
	  if (outx==1) 
	   {
             if ((c%n)==1) 
               {
	         w=w*1e12;
  	         fprintf(ey,"%lf ",w); 
	       }	 
	       else 
	         {
		  if ( ((c%n)==0)  && (voltagesw!=0.0) )
		   {
		     if (w>minonevoltage*1e-6) w=1.0; 
		       else if (fabs(w)< maxzerovoltage*1e-6) w=0.0;
		         else w=-5.0;
		    }      // Spannung am sfqdc - Erweiterung am 17.05.04
		    else
		       w=w/6.28;
		   h=(int)(w+1000.5)-1000;
	//	   r=w-h;
	//	   if ((r>0.3)&&(r<-0.3)) h=-99; // Anpassung ->22.10.01
	           fprintf(ey,"%d ",h);		   
		 }  
	   }
	  ende=fscanf(ex,"%s ",s);  // bei neuem Compiler =1 anstatt =0
          if (output==ja)  printf("(#%s )%d",s,ende);	  
	  if (c++%n==0) 
	    {
              if (output==ja) printf("\n");
              q=0;
              if (outx==1) fprintf(ey,"\n");	    
	      zd++;
	    }   
	}        
	
    printf("%d lines processed!\n",c+4);        
    fclose(ey);  
    fclose(ex); 
    printf("Ready!\n");            

    return 0;
  }
