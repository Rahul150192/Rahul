//*****************C code for checking backward bias of 8 rounds Salsa************************//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include<math.h>

#define ul unsigned int
#define ull unsigned long long 

#define rotateleft(x, n) (((x) << (n)) ^ ((x) >> (32-n)))
#define rotateright(x, n) (((x) >> (n)) | ((x) << (32-n)))
#define update(a, b, c, n) ((a)^(rotateleft(((b)+(c)),(n))))


#define LOOP ((1024*1024*1024ULL)*1024ULL)


ull MOD = 4294967296;

int   POS[50], BIT[50];
ull Bias[256];
ull onex[16];
        
int l=0;
int i2, i3;
int C[160];


ul myrand32(){
    ull temp;
    int i;
    if (drand48() < 0.5)
      temp = 0;
    else
      temp = 1;
   
    for (i = 1; i < 32; i++){
      if (drand48() < 0.5)
	      temp = temp << 1;
      else
	      temp = (temp << 1) + 1;
    }
    return temp;
}


//******************Function for Initialization**********************************//
void initializeR(ul *x){
	int i;
        ul pt, i2,i3;
	for (i = 0; i < 16; i++){
              if(i==0 || i==5 || i==10 || i==15)
                continue;

              
              else
                x[i]=myrand32();

                               }

       
    
	x[0] = 0x61707865;
	x[5] = 0x3320646e;
	x[10] = 0x79622d32;
	x[15] = 0x6b206574;
	
	

}



void copystate(ul *x1, ul *x){
	int i;
	for (i = 0; i < 16; i++)
		x1[i] = x[i];
}


//***********************Quarter Round Function******************************//
void qr(ul *x0, ul *x1, ul *x2, ul *x3){
	
	ul z0, z1, z2, z3;

	z1 = update(*x1, *x3, *x0, 7);
	z2 = update(*x2, *x0, z1, 9);
	z3 = update(*x3, z1, z2, 13);
	z0 = update(*x0, z2, z3, 18);

	*x0 = z0; *x1 = z1, *x2 = z2, *x3 = z3;
}



//***********************Inverse Quarter Round Function******************************//
void inqr(ul *x0, ul *x1, ul *x2, ul *x3){
	
	ul z0, z1, z2, z3;

	z0 = update(*x0, *x3, *x2, 18);
	z3 = update(*x3, *x2, *x1, 13);
	z2 = update(*x2, *x1, z0, 9);
	z1 = update(*x1, z0, z3, 7);

	*x0 = z0; *x1 = z1, *x2 = z2, *x3 = z3;
}


//***************Column Round Function**************************//
void columnround(ul *x){
     qr(&(x[0]), &(x[4]), &(x[8]), &(x[12]));
     qr(&(x[5]), &(x[9]), &(x[13]), &(x[1]));
     qr(&(x[10]), &(x[14]), &(x[2]), &(x[6]));
     qr(&(x[15]), &(x[3]), &(x[7]), &(x[11]));
     
 }

//***************Inverse Column Round Function**************************//
void incolumnround(ul *x){
     inqr(&(x[0]), &(x[4]), &(x[8]), &(x[12]));
     inqr(&(x[5]), &(x[9]), &(x[13]), &(x[1]));
     inqr(&(x[10]), &(x[14]), &(x[2]), &(x[6]));
     inqr(&(x[15]), &(x[3]), &(x[7]), &(x[11]));
     
 }

//***************Row Round Function**************************// 
void rowround(ul *x){
     qr(&(x[0]), &(x[1]), &(x[2]), &(x[3]));
     qr(&(x[5]), &(x[6]), &(x[7]), &(x[4]));
     qr(&(x[10]), &(x[11]), &(x[8]), &(x[9]));
     qr(&(x[15]), &(x[12]), &(x[13]), &(x[14]));
     
 } 
     
//***************Inverse Row Round Function**************************//
void inrowround(ul *x){
     inqr(&(x[0]), &(x[1]), &(x[2]), &(x[3]));
     inqr(&(x[5]), &(x[6]), &(x[7]), &(x[4]));
     inqr(&(x[10]), &(x[11]), &(x[8]), &(x[9]));
     inqr(&(x[15]), &(x[12]), &(x[13]), &(x[14]));
     
 }     
 


int main(){
	ul x[16], x1[16],x0[16], x01[16], y[16], y1[16],y0[16], y01[16], z1[16], z2[16], FORWARD_DIFF, BACK_DIFF, pattern;
	int i, j, k, l,MISS_MATCH, WORD, BIT, A[]={1,2,3,4,11,12,13,14}; 
  

      
        //*************************************************PNBs SET*******************************************************//
        int PNB_SET[163]={4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                          21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 36, 37, 38, 39, 50, 51,
                          52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68,
                          69, 70, 71, 72, 73, 74, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92,
                          93, 94, 95, 103, 104, 105, 106, 107, 108, 115, 116, 117, 118,
                          119, 120, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148,
                          149, 150, 151, 152, 159, 160, 161, 162, 163, 164, 165, 166,
                          167, 174, 175, 176, 177, 178, 179, 180, 192, 193, 194, 199,
                          204, 205, 206, 207, 224, 225, 237, 238, 239, 241, 249, 255,
                          128, 129, 153, 154, 40, 168, 47, 181, 182, 186, 187, 188, 195,
                          200, 201, 75, 76, 208, 209, 213, 214, 218, 97, 226, 100, 231,
                          232, 250, 109, 110, 240, 245, 121, 122};
       
       //******************************************************************************************************************//
       
        int LL = 163;


	ull loop = 0, keycounter=0, perkeyloop = 0 ;
	double cnt = 0;
        
	
	srand48(time(NULL));
	
       

       while (keycounter < 1024*128){
        perkeyloop=0;
        k = 0;
        while(1){
		   initializeR(x);	copystate(x1, x);
		   x1[7] = x[7] ^ (0x00000001<<31);//Input difference
                   copystate(x0,x);
                   copystate(x01,x1);
		   columnround(x); columnround(x1);
                   MISS_MATCH = 0;
                   for(j = 3; j<16; j = j+4){

                     for(k = 0;k<32;k++){

                                   pattern = 0x00000001<<k;
				if (((x[j] ^ x1[j]) & pattern)!= 0) MISS_MATCH++;

                                    }
                 
                                }
                            

                     if(MISS_MATCH==4 )
                   
                     break;


                     }
	while (perkeyloop < 1024*1024){
	          
               copystate(x,x0);
               copystate(x1,x01);
               x[6] = myrand32(); x[8] = myrand32(); x[9] = myrand32();
               x1[6] = x[6]; x1[8] = x[8]; x1[9] = x[9];
               columnround(x);  columnround(x1);
               rowround(x);     rowround(x1);
               columnround(x);  columnround(x1);
                rowround(x);     rowround(x1);
               columnround(x);  columnround(x1);

               FORWARD_DIFF = (x[4]>>7)^(x1[4]>>7); //Forward difference
               
                rowround(x);     rowround(x1);
              columnround(x);  columnround(x1);
                rowround(x);     rowround(x1);
               
                 
              

               for(i = 0; i<16; i++){
		                   z1[i] = (x[i]+x0[i]);
                                   z2[i] = (x1[i]+x01[i]);

                                  
                                   
		                  }
             


                for(j = 0;j<LL;j++){

                        WORD = A[PNB_SET[j]/32];
      			BIT =  PNB_SET[j]%32;

             		if(drand48()<0.5){
                		x0[WORD] = x0[WORD] ^ (0x00000001<<BIT );
               			x01[WORD] = x01[WORD] ^ (0x00000001<<BIT );
                               		}    


                               }
               

              for(i = 0;i<16;i++){
                  
                                x[i] = z1[i]+(MOD-(x0[i]));
	                        x1[i] =z2[i]+(MOD-(x01[i]));
	                        

                                }
              inrowround(x);  inrowround(x1);
              incolumnround(x);  incolumnround(x1);
              inrowround(x);  inrowround(x1);
              
              BACK_DIFF = (x[4]>>7)^(x1[4]>>7);//Backward difference
                      
              if (((FORWARD_DIFF^BACK_DIFF) & 0x00000001) == 0)  
		     cnt = cnt+ 1.0;

               loop++; 
               perkeyloop++;            


                 
				
		
               if (loop > 0 && loop % (1024 * 1024) == 0)
                   printf("Number of PNBs: %d  Number of Samples: 2^20*%llu      Bias: %0.10f   \n", LL, loop/(1024*1024), 2 * ((cnt / loop) - 0.5));

                }
         keycounter++;           
}
                
              
			

}

