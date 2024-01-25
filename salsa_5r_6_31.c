//Code for verifying that the claimed biases of the 3 distinguishers given in ISPEC 2022 are incorrect
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <x86intrin.h>
#include<math.h>

#define ul unsigned int
#define ull unsigned long long 

#define rotateleft(x, n) (((x) << (n)) ^ ((x) >> (32-n)))
#define rotateright(x, n) (((x) >> (n)) | ((x) << (32-n)))
#define update(a, b, c, n) ((a)^(rotateleft(((b)+(c)),(n))))

#define myrand32 ((ul) (4294967296.0*drand48()))

static unsigned int x8,y88,z8,w8,v8;
    

unsigned long xorshift(void)
{
    unsigned long t8 = x8^(x8>>7);
    x8=y88; y88=z8; z8=w8; w8=v8;
    v8=(v8^(v8<<6))^(t8^(t8<<13)); 
    return v8 ;
    
     

}

void
seed()
{
    x8 =  myrand32;  
    v8 =  myrand32; 
    w8 =  myrand32; 
    z8 =  myrand32; 
    y88 = myrand32; 

} 


#define LOOP ((1024*1024*1024ULL)*1ULL)





void copystate(ul *x1, ul *x){
	int i;
	for (i = 0; i < 16; i++)
		x1[i] = x[i];
}



void qr(ul *x0, ul *x1, ul *x2, ul *x3){
	
	ul z0, z1, z2, z3;

	z1 = update(*x1, *x3, *x0, 7);
	z2 = update(*x2, *x0, z1, 9);
	z3 = update(*x3, z1, z2, 13);
	z0 = update(*x0, z2, z3, 18);

	*x0 = z0; *x1 = z1, *x2 = z2, *x3 = z3;
}



void inqr(ul *x0, ul *x1, ul *x2, ul *x3){
	
	ul z0, z1, z2, z3;

	z0 = update(*x0, *x3, *x2, 18);
	z3 = update(*x3, *x2, *x1, 13);
	z2 = update(*x2, *x1, z0, 9);
	z1 = update(*x1, z0, z3, 7);

	*x0 = z0; *x1 = z1, *x2 = z2, *x3 = z3;
}


void columnround(ul *x){
     qr(&(x[0]), &(x[4]), &(x[8]), &(x[12]));
     qr(&(x[5]), &(x[9]), &(x[13]), &(x[1]));
     qr(&(x[10]), &(x[14]), &(x[2]), &(x[6]));
     qr(&(x[15]), &(x[3]), &(x[7]), &(x[11]));
     
 }

void incolumnround(ul *x){
     inqr(&(x[0]), &(x[4]), &(x[8]), &(x[12]));
     inqr(&(x[5]), &(x[9]), &(x[13]), &(x[1]));
     inqr(&(x[10]), &(x[14]), &(x[2]), &(x[6]));
     inqr(&(x[15]), &(x[3]), &(x[7]), &(x[11]));
     
 }
 
void rowround(ul *x){
     qr(&(x[0]), &(x[1]), &(x[2]), &(x[3]));
     qr(&(x[5]), &(x[6]), &(x[7]), &(x[4]));
     qr(&(x[10]), &(x[11]), &(x[8]), &(x[9]));
     qr(&(x[15]), &(x[12]), &(x[13]), &(x[14]));
     
 }     

void inrowround(ul *x){
     inqr(&(x[0]), &(x[1]), &(x[2]), &(x[3]));
     inqr(&(x[5]), &(x[6]), &(x[7]), &(x[4]));
     inqr(&(x[10]), &(x[11]), &(x[8]), &(x[9]));
     inqr(&(x[15]), &(x[12]), &(x[13]), &(x[14]));
     
 }     
 



int main(){

        ul x[16],x0[16], x1[16],x01[16],z[8], pattern, pt,i1,i2,i3,y1,y11,aa,bb;
	int i, j, k, j1, m, n, o, p, a, b; 
	ull loop = 0, val=0;
	
	
        
         srand48(time(NULL) ^ getpid());

         seed();

         printf("%u   %u    %u   %u  %u\n", x8,y88,v8,w8,z8);
         
	
        
       
          while (loop < LOOP){
                 x[0] = 0x61707865;
	         x[5] = 0x3320646e;
	         x[10] = 0x79622d32;
	         x[15] = 0x6b206574;
                 
                 x[1] = xorshift(); 
                 x[2] = xorshift();
                 x[3] = xorshift();  
       
                x[4] = xorshift(); 
                x[6] = xorshift(); 
                x[7] = xorshift(); 
                
                x[8] = xorshift(); 
                x[9] = xorshift(); 
                x[11] =xorshift(); 
                
                x[12] =xorshift(); 
                x[13] =xorshift();
                x[14] =xorshift();
	
		 copystate(x1, x);
		 pt=0x00000001;
                
		x1[6] = x[6] ^ (pt<<31);


             
                


                
                columnround(x);  columnround(x1);
                rowround(x);     rowround(x1);
                columnround(x);  columnround(x1);
                rowround(x);     rowround(x1);
                columnround(x);  columnround(x1);
               
                
                
                

                       a=0;
                       b=18;
                
			        pattern = 0x00000001<<b;
				if (((x[a] ^ x1[a]) & pattern) == 0) {val = val + 1.0;}
				
				
		loop++;

               
        
             if (loop > 0 && loop%(1024*1024*32) == 0)
               {
            
			
	printf(" %llu   %0.10f         %0.10f  \n",  loop/(1024*1024*32), (double)val/loop, (double)(2*val)/loop-1.0);
			
                 


                 
                }  
          }




         

            

}







