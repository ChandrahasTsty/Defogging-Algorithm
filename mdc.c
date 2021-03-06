/* This code is not compile ready and this is used to write one of functions required by removefog.c, compile that file which is linked to this file via make-file */

/*Include all the libraries that are needed */
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include "DataTypes.h"
/* In this program we assume that We have already read Image intensties and these were supplied to Dark Channel method along with patch size and these two were the inputs for the program */
float minarray(float* start,int size);/*forward declaration to avoid compiler warnings */
float* makeDarkChannel(ImageData i, int patch_size)
{
  long int r,c,h,k,f,h1,k1;/* Declaration of loop variables*/
  
  /* As per the MakeDarkChannel.m actual patch size was derived from patch_size input, One has to be careful with variables when using math functions */
  
  double e=(patch_size/2);
  int act_patch_size=(int) 2*floor(e);
  
  /*Buffer for storing all the intensity values of the elements in that patch*/
  
  float buffer[3*(act_patch_size+1)*(act_patch_size+1)];
  f=i.pixels;
  /* f is used as dummy purposes for this program */
  float* J=(float*)calloc(f,sizeof(float));
  
  /*Dynamic memory allocation is done for J, We should free this!!*/
  /* In order to avoid padding we dont loop to each and every variable we just loop to extent upto which covers all the elements in the image, here r is loop variable for rows and c is for columns */
  
  for(r=0;r<(i.height);r++)
  {
   for(c=0;c<(i.width);c++)
   {
    f=0;
    for(h=r;h<=(r+act_patch_size);h++)/*Loops for patch dimensions for row */
    {
     for(k=c;k<=(c+act_patch_size);k++)/* Loops for patch dimensions for column */
     {
      /* In Matlab Implementation Image Array was pre and post padded for correct Implementation, Such
      padding would be expensive, because it requires another chunk of memory, We avoid that by
      interpolating h,k into h1,k1 which are mathematically represented to give same result as padding
      symmetrically */
       
      if(h>(i.height-1))
        h1=(i.height-1)-((h%(i.height-1))-1);
      else if (h<(act_patch_size/2))
        h1=((act_patch_size/2)-h)-1;
      else 
        h1=(h-(act_patch_size/2))%(i.height-1);        
      
      if(k>(i.width-1))
        k1=(i.width-1)-((k%(i.width-1))-1);
      else if (k<(act_patch_size/2))
        k1=((act_patch_size/2)-k)-1;
      else 
        k1=(k-(act_patch_size/2))%(i.width-1); 
      
      buffer[3*f]=*(i.rdata+(h1*(i.width)+k1));/*Buffer filling for Red channel */
      buffer[(3*f)+1]=*(i.gdata+(h1*(i.width)+k1));/*Buffer filling for Green channel */
      buffer[(3*f)+2]=*(i.bdata+(h1*(i.width)+k1));/*Buffer filling for Blue channel */
      f++;
     }
    }
    /*Now we pass the buffer to minarray which returns the min value */
    J[r*(i.width)+c]=minarray(buffer,3*(act_patch_size+1)*(act_patch_size+1));
   }
  }
  return J;
 }
 /* Defintions of the minarray whose semantics are straight forward */
float minarray(float* start,int size)
   {
    float min_val=*(start);
    float ret;
    int i;
    for(i=0;i<size;i++)
     if(*(start+i)<min_val)
      min_val=*(start+i);
    return min_val;
    }
