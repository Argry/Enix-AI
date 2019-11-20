#include<ctype.h>
#include<math.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include <time.h>

int main(int argc, char *argv[]);

int Power(int A, int B);

int LOAD_BASIS(void);
int GATE(int I1,int I2);
int AND( int I1,int I2);
int OR(  int I1,int I2);
int NOR( int I1,int I2);
int XOR( int I1,int I2);
int NOT( int I1,int I2);
int NAND(int I1,int I2);
int RAND(int I1,int I2);
int ONE( int I1,int I2);
int (*GOPHER[100])(int I1,int I2);
int GOPHER_MAX;
char *LOGIC_TYPE[8]={"ONE ","GATE","NOT ","AND ","OR  ","NOR ","XOR ","NAND"};

int S_Cluster;
int Super_Cluster[4][256];
unsigned long int Minds;
int ENiX(int Local_Ex[256],int Local_N_Ex);
int Execute2(int Local_Index,int Local_Neural[4][128],int Local_Cluster_S);
int Eval(int Local_Index);
int Bit(int Local_Bit,int Local_Num);
int N_Example;
int O_Band;
int Problem_Time; 
int Bits[8];
int I_Band;
int Examples[256];
int Found;
int Learn();
int Morph(int Local_Neuron,int Local_Output,int Local_Neural[4][128],int Local_CS);
int Test(int Local_Output,int Local_Neural[4][128],int Local_CS);
int Bit_Distribution();
int Bandwidth(int Local_C);

int Rotax_Examples[256];
int Rotax_Unknowns[256];
int Rotax_Max;
int Start_Rotax(int Local_F);
void Display_Polynomial(int Local_P[256],int Local_M);
void List_Numbers1(int Local_N[256],int Local_Unknown[256],int Local_M);
int Neural();
int Factorial(int Local_N); 
int Polynomial[256];
int Poly_Dim;
int Execute_Polynomial(int Local_C);

int Number;
int Numbers[256];
int Unknowns[256];
char *Program_Name;
char *Option;
char *ASCII_Examples[256];
int Maximum(int Local_Numbers[256],int Local_Dim);
int Minimum(int Local_Numbers[256],int Local_Dim);
int ILEARN();                  // NO FRILLS DEFAULT NUMBER PATTERN ANALYSER
int IL_CO1();                  // CODED FOR MONOTONICS INCREASING PATTERNS
int IL_CO2();                  // TRANSFORM-CODED FOR MONOTONIC DECREASING PATTERNS
int IL_SC();                   // MULTIPLE-UNIT SCALING
int AUTO();                    // ENVIRONMENT DECIDES WHATS BEST
int Oddman();                  // INTELLIGENT ERROR DETECTION
int DualScan();                // ORTHOGNAL AWARENESS TRIAL
int Cyber_Talk();              // APPLY PATTERN ANALYSER TO BASIC LANGUAGE
int Analyzer();                //
void Equalizer();              //
int Evaluate();
int Kindred();   
int Prodigy();
int Translate();
int Chocky();
int IRC_Chocky();
int Linguistics();
char IRC_2_Chocky[1024];
void Init_Rotax(int Local_C);
int Fullauto();
int Coding_F;
int Coding_M;
int Scaling_F;
int Reflective_Trans;
int LCD();
void Make_Solution_Set();
int Solution_Set[256];
int Solution_Max;
void Set_Transform(int Local_Data[256],int Local_M);
int Transformed[256];
int Transformed_Max;
void Error_Correct(int Local_Data[256],int Local_M);
int Error_Complexity[256];
int Error_Index_List[256];
int Error_Max;
int Equalized(int Local_D[256],int Local_M);
int P_Equalized(int Local_D[256],int Local_M);
void Save_RWEs();
void Restore_RWEs();
int RWEs[256];
int RWE_Size;
int Siblings(int Local_M,int Local_Iteration);
int Kindred_Cardinality;
int Kindred_Data[256];
void List_Numbers(int Local_N[256],int Local_M);
void Chocky_Word();
void Chocky_Reset();
void Chocky_Run();
void Chocky_Example();
void Chocky_Execute();
char Chocky_Input_Data[1024];
char Chocky_Word_Data[1024];
char Chocky_Input[8][32];
char Chocky_Output[8][32];
int Byte[8],Example_Dim,Abstract[2][8][32],Chocky_IM,Chocky_OM,Chocky_IT,Chocky_OT;
void Save(char *LD);
void Load(char *LD);
void Delete(char *LD);
void Stop();

int Summary();
int Print_Out();	
int Help();

int Power(int A,int B){
   int _,R=1;
   for(_=0;_<B;_++)           R*=A;
   return R;
}

int ONE( int I1,int I2){      return 1;              }
int GATE(int I1,int I2){      return I1;             }
int NOT( int I1,int I2){      return !I1;            }
int AND( int I1,int I2){      return I1&&I2;         }
int OR(  int I1,int I2){      return I1||I2;         }
int NOR( int I1,int I2){      return !(I1||I2);      }
int XOR( int I1,int I2){      return I1!=I2;         }
int NAND(int I1,int I2){      return !(I1&&I2);      }

int LOAD_BASIS(){
   GOPHER[0]=ONE,GOPHER[1]=GATE,GOPHER[2]=NOT,GOPHER[3]=AND,GOPHER[4]=OR,GOPHER[5]=NOR,GOPHER[6]=XOR,GOPHER[7]=NAND,GOPHER_MAX=8;
}

int ENiX(int Local_Ex[256],int Local_N_Ex){
   int Local_C;
   time_t Local_Start,Local_Finish;
   Local_Start=time(NULL);
   Minds=0;
   for(Local_C=0;Local_C<256;Local_C++) Examples[Local_C]=Local_Ex[Local_C];
   I_Band=Bandwidth(Local_N_Ex);
   N_Example=Power(2,I_Band);
   for(Local_C=Local_N_Ex;Local_C<N_Example;Local_C++) Examples[Local_C]=-1;
   Bit_Distribution();
   if(Learn()==-1){
      puts(" ! WARNING: ENVIRONMENTAL CONTROL SHUTDOWN - PROBLEM COMPLEXITY EXCEEDED; IGNORE ALL DATA BEYOND THIS POINT; NEURAL STALLED - KERNEL SHUTDOWN IMMINENT.");
      exit(-1);
   }
   Local_Finish=time(NULL);
   Problem_Time=Local_Finish-Local_Start;
}


int Bit(int Local_Bit,int Local_Num){                // Extracts the Nth (Local_Bit) 
   int Local_C,Local_R=0,Local_D;                    // from byte Local_Num
   if(Local_Num<0) return -1;                        // 
   for(Local_C=0;Local_C<Local_Bit+1;Local_C++){     // 
      Local_D=Local_Num;                             //
      Local_Num/=2;                                  //
      Local_R=Local_D-2*(Local_Num);                 //
   }                                                 //
   return Local_R;                                   // Code is completely self-sufficient
}                                                    //

                                                                                                     // multiple lamination upgrades.
int Execute2(int Local_Index,int Local_Neural[4][128],int Local_Cluster_S){            
   int Local_Bits[12];                                           // increase input buffer range
   int Local_R;                                                  // establish a return register
   for(Local_Bits[11]=0;Local_Bits[11]<11;Local_Bits[11]++)      // use upper register
     Local_Bits[Local_Bits[11]]=0;                               // tidy upper register
   for(Local_Bits[11]=0;Local_Bits[11]<I_Band;Local_Bits[11]++)  // for lower register range                                          
     Local_Bits[Local_Bits[11]]=Bit(Local_Bits[11],Local_Index); // fill lower registers
   for(Local_Bits[11]=0;Local_Bits[11]<Local_Cluster_S+1;Local_Bits[11]++){ 
      Local_R=GOPHER[Local_Neural[2][Local_Bits[11]]](Local_Bits[Local_Neural[0][Local_Bits[11]]],Local_Bits[Local_Neural[1][Local_Bits[11]]]);
      Local_Bits[I_Band+Local_Bits[11]]=Local_R;                 // stash the previous output
   }                                                             // in extended registers
   return Local_R;                                               // should contain the output
}

int Eval(int Local_Index){            
   int Local_Bits[12],Local_Out[8],Local_A,Local_B,Local_D;          // Extending registers
   int Local_Lambda,Local_Delta;                                     // Lambda=test, Delta=cluster spec
   int Local_R;                                                      // Adding hard register
   for(Local_Bits[11]=0;Local_Bits[11]<11;Local_Bits[11]++)          // All registers
     Local_Bits[Local_Bits[11]]=0;                                   // Zero inputs
   for(Local_Bits[11]=0;Local_Bits[11]<8;Local_Bits[11]++)           // All registers
     Local_Out[Local_Bits[11]]=0;                                    // Zero outputs
   for(Local_Bits[11]=0;Local_Bits[11]<I_Band;Local_Bits[11]++)      // All registers
     Local_Bits[Local_Bits[11]]=Bit(Local_Bits[11],Local_Index);     // Load input regs
   Local_Delta=0;
   for(Local_Bits[11]=0;Local_Bits[11]<S_Cluster;Local_Bits[11]++){
      Local_Lambda=Super_Cluster[3][Local_Bits[11]]; 
      Local_Bits[I_Band+Local_Delta]=GOPHER[Super_Cluster[2][Local_Bits[11]]](Local_Bits[Super_Cluster[0][Local_Bits[11]]],Local_Bits[Super_Cluster[1][Local_Bits[11]]]);
      Local_Out[Super_Cluster[3][Local_Bits[11]]]=Local_Bits[I_Band+Local_Delta];
      Local_Delta++;
      if(Local_Lambda!=Super_Cluster[3][Local_Bits[11]+1]) Local_Delta=0;
   }
   Local_A=1;
   Local_B=0;
   for(Local_D=0;Local_D<8;Local_D++){
      Local_B=Local_B+Local_A*Local_Out[Local_D];
      Local_A*=2;
   }
   return Local_B;
}

int Learn(){                                       // Uses variates: Super_Cluster[][], S_Cluster, O_Band, Cluster_Size,
   int Local_C,Local_Neurons,Local_Neural[4][128]; // Found, Neural[][]
   int Local_C1,Local_C2;                          // 
   unsigned long int Local_M;
   for(Local_C1=0;Local_C1<4;Local_C1++)
     for(Local_C2=0;Local_C2<256;Local_C2++)
       Super_Cluster[Local_C1][Local_C2]=0;
   S_Cluster=0;
   Local_M=0;
   for(Local_C=0;Local_C<O_Band;Local_C++){
      LOAD_BASIS();
      Found=0,Local_Neurons=0;
      for(Local_C1=0;Local_C1<4;Local_C1++)
	for(Local_C2=0;Local_C2<128;Local_C2++)
	  Local_Neural[Local_C1][Local_C2]=0;
      Minds=0;
      while(!Found){
	 Morph(Local_Neurons,Local_C,Local_Neural,Local_Neurons);
	 Local_Neurons++;  
	 if(Minds>250000)                  return -1;
      }
      Local_M+=Minds;
   }
   Minds=Local_M;
   return 0;
}

int Test(int Local_Output,int Local_Neural[4][128],int Local_CS){  // Variates used: Minds, Found, N_Example, S_Cluster
   int Local_C,Local_A,Local_B;                                            // Bits[], Super_Cluster[][].
   Found=1;                                                                // Variates we can keep: Minds, Found, N_Example, Bits[], Super_Cluster,
   Minds++;                                                                // S_Cluster.
   for(Local_C=0;Local_C<N_Example;Local_C++){
      Local_A=Execute2(Local_C,Local_Neural,Local_CS);
      Local_B=Bit(Bits[Local_Output],Examples[Local_C]);
      if(Examples[Local_C]>-1)
	if(Local_A!=Local_B){
	   Found=0;
	   return 1;
	}
   }
   if(Found){
      for(Local_C=0;Local_C<Local_CS+1;Local_C++){
	 Super_Cluster[0][S_Cluster+Local_C]=Local_Neural[0][Local_C];
	 Super_Cluster[1][S_Cluster+Local_C]=Local_Neural[1][Local_C];
	 Super_Cluster[2][S_Cluster+Local_C]=Local_Neural[2][Local_C];
	 Super_Cluster[3][S_Cluster+Local_C]=Bits[Local_Neural[3][Local_C]];
      }
      S_Cluster+=Local_CS+1;
   }
   return 1;
}

// Full multiple lamination upgrades go into Kernel::Morph (below).
int Morph(int Local_Neuron,int Local_Output,int Local_Neural[4][128],int Local_CS){
   int Local_C=Local_Neuron;
   Local_Neural[3][Local_Neuron]=Local_Output;
   if(!Found){
      for(Local_Neural[2][Local_Neuron]=0;Local_Neural[2][Local_Neuron]<GOPHER_MAX;Local_Neural[2][Local_Neuron]++){
	 if(Local_Neural[2][Local_Neuron]<1){
	    if(Local_Neuron>0)
	      Morph(Local_Neuron-1,Local_Output,Local_Neural,Local_CS);
	    else{
	       Test(Local_Output,Local_Neural,Local_CS);
	       if(Found)       return 1;
	    }
	 }
	 else{
	    for(Local_Neural[0][Local_Neuron]=0;Local_Neural[0][Local_Neuron]<I_Band+Local_C;Local_Neural[0][Local_Neuron]++){
	       if(Local_Neural[2][Local_Neuron]<3){
		  if(Local_Neuron>0)
		    Morph(Local_Neuron-1,Local_Output,Local_Neural,Local_CS);
		  else{
		     Test(Local_Output,Local_Neural,Local_CS);
		     if(Found) return 1;
		  }
	       }
	       else{
		  for(Local_Neural[1][Local_Neuron]=Local_Neural[0][Local_Neuron];Local_Neural[1][Local_Neuron]<I_Band+Local_C;Local_Neural[1][Local_Neuron]++){
		     if(Local_Neuron>0)
		       Morph(Local_Neuron-1,Local_Output,Local_Neural,Local_CS);
		     else{
			Test(Local_Output,Local_Neural,Local_CS);
			if(Found) return 1;
		     }
	          }
	       }
	    }
	 }
      }
   }
   return 1;
}

int Bit_Distribution(){ // !!!!!!!!!!!!!!!!!  DO NOT MODIFY !!!!!!!!!!!!!!!!!!!!!!
   int Local_C,Local_C1,Local_F,Local_D;
   for(Local_C=0;Local_C<8;Local_C++) Bits[Local_C]=Local_C;
   O_Band=8;
   for(Local_C1=0;Local_C1<O_Band;Local_C1++){
      Local_F=0;
      for(Local_C=0;Local_C<N_Example;Local_C++) if(Bit(Bits[Local_C1],Examples[Local_C])==1) Local_F=1;
      if(Local_F==0){
	 O_Band--;
	 for(Local_D=Local_C1;Local_D<7;Local_D++) Bits[Local_D]=Bits[Local_D+1];
	 Local_C1--;
      }
   }
   return 0;
}

int Bandwidth(int Local_C){
   int Local_R=0,Local_A=1;
   for(Local_R=0;Local_A<Local_C;Local_R++) Local_A*=2;
   return Local_R;
}

int Neural(){
   int Local_Examples[255][256],Local_Unknowns[255][256],LX[256],LU[256],Local_Max;
   int Local_A,Local_B,Local_C,Local_D,Local_Iteration,Local_Found_Sol;
   Local_Max=Rotax_Max-2;
   for(Local_A=0;Local_A<256;Local_A++){
      Local_Examples[0][Local_A]=Rotax_Examples[Local_A];
      Local_Unknowns[0][Local_A]=Rotax_Unknowns[Local_A];
   }
   for(Local_Iteration=0;Local_Iteration<Local_Max-1;Local_Iteration++){
      for(Local_A=0;Local_A<Local_Max-1-Local_Iteration;Local_A++){
	 if(!Local_Unknowns[Local_Iteration][Local_A]){
	    if(!Local_Unknowns[Local_Iteration][Local_A+1]){
	       Local_Examples[Local_Iteration+1][Local_A]=Local_Examples[Local_Iteration][Local_A+1]-Local_Examples[Local_Iteration][Local_A];
	       Local_Unknowns[Local_Iteration+1][Local_A]=Local_Unknowns[Local_Iteration][Local_A+1]-Local_Unknowns[Local_Iteration][Local_A];
	    }
	    else Local_Unknowns[Local_Iteration+1][Local_A]=1;
	 }
	 else    Local_Unknowns[Local_Iteration+1][Local_A]=1;
      }
      Local_B=0;
      for(Local_A=0;Local_A<Local_Max-Local_Iteration;Local_A++) if(!Local_Unknowns[Local_Iteration][Local_A]) Local_B=1;
      if(Local_B==0) return 0;
      Local_B=0;
      for(Local_A=0;Local_A<Local_Max-Local_Iteration;Local_A++)
	if(Local_Unknowns[Local_Iteration][Local_A]==0){
	   Local_B=Local_Examples[Local_Iteration][Local_A];
	   Local_A=Local_Max;
	}
      Local_C=0;
      for(Local_A=0;Local_A<Local_Max-Local_Iteration;Local_A++)
	if(!Local_Unknowns[Local_Iteration][Local_A])
	  if(Local_Examples[Local_Iteration][Local_A]!=Local_B){
	     Local_A=Local_Max;
	     Local_C=1;
	  }
      if(Local_C!=1){
	 Local_Found_Sol=1;
	 for(Local_A=0;Local_A<Local_Max-Local_Iteration;Local_A++) Local_Examples[Local_Iteration][Local_A]=Local_B;
	 for(Local_A=Local_Iteration-1;Local_A>-1;Local_A--){
	    for(Local_B=0;Local_B<Local_Max-Local_A;Local_B++)
	      if(!Local_Unknowns[Local_A][Local_B]){
		 Local_D=Local_B;
		 Local_B=Local_Max;  
	      }
	    for(Local_B=Local_D-1;Local_B>-1;Local_B--) Local_Examples[Local_A][Local_B]=Local_Examples[Local_A][Local_B+1]-Local_Examples[Local_A+1][Local_B];
	    for(Local_B=Local_D;Local_B<Local_Max-1-Local_A;Local_B++){
	       if(Local_Unknowns[Local_A][Local_B+1]) Local_Examples[Local_A][Local_B+1]=Local_Examples[Local_A][Local_B]+Local_Examples[Local_A+1][Local_B];
	       else if(Local_Examples[Local_A][Local_B+1]!=Local_Examples[Local_A][Local_B]+Local_Examples[Local_A+1][Local_B]) Local_Found_Sol=0;
	    }
	 }
	 if(Local_Found_Sol){
	    if(Poly_Dim<Local_Iteration+1) Poly_Dim=Local_Iteration+1;
	    Polynomial[Local_Iteration]=Local_Examples[Local_Iteration][0]/Factorial(Local_Iteration);
	    return 1;
	 }
      }
   }
   return 0;
}


void Display_Polynomial(int Local_P[256],int Local_M){
   int Local_E,Local_A;
   printf("y(x) = ");
   Local_E=0;
   for(Local_A=0;Local_A<Local_M;Local_A++){
      if(Local_P[Local_M-1-Local_A]>0){
	 if(Local_E==1)
	   printf("+");
      }
      if(Local_P[Local_M-1-Local_A]<0)
	printf("-");
      if(Local_P[Local_M-1-Local_A]==1){
	 if(Local_M-1-Local_A>0){}
	 else
	   printf("1");
      }
      else{
	 if(Local_P[Local_M-1-Local_A]!=0){
	    if(Local_P[Local_M-1-Local_A]<0){
	       if(Local_P[Local_M-1-Local_A]!=-1)
		 printf("%d",-1*Local_P[Local_M-1-Local_A]);
	       else
		 if(Local_M-1-Local_A==0)
		   printf("1");
	    }
	    else{
	       printf("%d",Local_P[Local_M-1-Local_A]);
	    }
	 }
      }
      if(Local_P[Local_M-1-Local_A]>0)
	if((Local_M-1-Local_A)!=0)
	  printf("x");
      if(Local_P[Local_M-1-Local_A]<0)
	if((Local_M-1-Local_A)!=0)
	  printf("x");
      if(Local_P[Local_M-1-Local_A]!=0)
	if(Local_M-1-Local_A>1)
	  printf("^%d",Local_M-1-Local_A);
      if(Local_P[Local_M-1-Local_A]!=0)
	if(Local_E==0)
	  Local_E=1;
   }
   puts(" Where x denotes the time-parameterization index.");
}

int Factorial(int Local_N){
  int Local_A,Local_R=1;
  for(Local_A=1;Local_A<Local_N+1;Local_A++)
    Local_R*=Local_A;
  return Local_R;
}

void List_Numbers1(int Local_N[256],int Local_Unknown[256],int Local_M){
  int Local_A;
  printf("{");
  for(Local_A=0;Local_A<Local_M;Local_A++){
    if(Local_Unknown[Local_A])
	  printf("?");
    else
	  printf("%d",Local_N[Local_A]);
	if(Local_A<Local_M-1)
	  printf(",");
  }
  printf("}");
}

int Execute_Polynomial(int Local_C){
  int Local_A,Local_R=0;
  for(Local_A=0;Local_A<Poly_Dim;Local_A++)
    Local_R+=Polynomial[Local_A]*Power(Local_C,Local_A);
  return Local_R;
}

int Start_Rotax(int Local_F){
  int Local_A,Local_B,Local_C,Local_D,Local_Shadow[256];
  for(Local_A=0;Local_A<256;Local_A++)
    Polynomial[Local_A]=0;
  Poly_Dim=0;
  for(Local_A=0;Local_A<256;Local_A++)
    Local_Shadow[Local_A]=Rotax_Examples[Local_A];
  while(Neural()){
    for(Local_A=0;Local_A<256;Local_A++)
      Rotax_Examples[Local_A]=Local_Shadow[Local_A];
    for(Local_A=0;Local_A<Rotax_Max-2;Local_A++)
	  if(!Rotax_Unknowns[Local_A])
	    Rotax_Examples[Local_A]-=Execute_Polynomial(Local_A);
	Local_B=0;
    for(Local_A=0;Local_A<Rotax_Max-2;Local_A++)
	  if(!Rotax_Unknowns[Local_A])
	    if(Rotax_Examples[Local_A]!=0)
          Local_B=1;
	if(Local_B==0){
	  for(Local_A=0;Local_A<Rotax_Max-2;Local_A++){
		Rotax_Examples[Local_A]=Execute_Polynomial(Local_A);
		Rotax_Unknowns[Local_A]=0;
	  }
	  if(Local_F==1){
	    printf("\nSolution set = ");
	    List_Numbers1(Rotax_Examples,Rotax_Unknowns,Rotax_Max-2);
	    puts("");
	  }
	  else{
	    puts("");
	    Display_Polynomial(Polynomial,Poly_Dim);
	  }
	  return 1;
	}
  }
  return 0;
}

void Stop(){
  char Local_Line[1024];
  FILE *LOCAL_SOURCE;
  FILE *LOCAL_DEST;
  
  system("cp ./Memory.ENiX ./Backup.ENiX");
  system("rm ./Memory.ENiX");
  
  LOCAL_DEST=fopen("./Memory.ENiX","a");
  LOCAL_SOURCE=fopen("./Backup.ENiX","r");
  
  
  
  while(fgets(Local_Line,1024,LOCAL_SOURCE)!=NULL){
    if(strncmp(Local_Line,":RUN",4)!=0)
	  fprintf(LOCAL_DEST,"%s",Local_Line);
  }
  
  fclose(LOCAL_SOURCE);
  fclose(LOCAL_DEST);
  
  system("rm ./Backup.ENiX");
}

int Linguistics(){
  int Local_C;
  FILE *Memory_File;
  puts("");
  if(strncmp(Option,":STOP",5)==0){
   Stop();
   puts("Running process killed off.");
   return 0;
  }
  if(strncmp(Option,":SAVE",5)==0){
   Save(ASCII_Examples[0]);
   printf("Data stored as ./%s.ENiX.\n",ASCII_Examples[0]);
   return 0;
  }
  if(strncmp(Option,":LOAD",5)==0){
    Load(ASCII_Examples[0]);
	printf("Data loaded from ./%s.ENiX.\n",ASCII_Examples[0]);
	return 0;
  }
  if(strncmp(Option,":DELETE",5)==0){
    Delete(ASCII_Examples[0]);
	printf("File ./%s.ENiX deleted.\n",ASCII_Examples[0]);
	return 0;
  }  
  if(strncmp(Option,":RESET",6)==0){
    system("rm ./Memory.ENiX");
	puts("Data destroyed.");
    return 0;
  }
  if(strncmp(Option,":CASE",5)==0){
    strcpy(IRC_2_Chocky,Option);
	for(Local_C=0;Local_C<Number-2;Local_C++){
	  strcat(IRC_2_Chocky," ");
	  strcat(IRC_2_Chocky,ASCII_Examples[Local_C]);
	}
	system("cp Memory.ENiX Backup.ENiX");
    Memory_File=fopen("./Memory.ENiX","a");
    fprintf(Memory_File,"%s",Option);
    for(Local_C=0;Local_C<Number-2;Local_C++)
      fprintf(Memory_File," %s",ASCII_Examples[Local_C]);
    fprintf(Memory_File,"\n");
    fclose(Memory_File);
	
	IRC_Chocky();
	
	system("rm Memory.ENiX");
	system("mv Backup.ENiX Memory.ENiX");
    return 0;
  }
  Memory_File=fopen("./Memory.ENiX","a");
  fprintf(Memory_File,"%s",Option);
  for(Local_C=0;Local_C<Number-2;Local_C++)
    fprintf(Memory_File," %s",ASCII_Examples[Local_C]);
  fprintf(Memory_File,"\n");
  puts("Information saved.");
  fclose(Memory_File);
  return 0;
}

int Fullauto(){
   int Local_A;
   Rotax_Max=Number;
  for(Local_A=0;Local_A<256;Local_A++){
    Rotax_Examples[Local_A]=Numbers[Local_A];
	Rotax_Unknowns[Local_A]=Unknowns[Local_A];
  }
  if(Start_Rotax(1)){}
  else{
    AUTO();
	Evaluate();
  }
  return 0;
}

void Init_Rotax(int Local_C){
   int Local_A=0;
   Rotax_Max=Number;
   for(Local_A=0;Local_A<256;Local_A++){
      Rotax_Examples[Local_A]=Numbers[Local_A];
      Rotax_Unknowns[Local_A]=Unknowns[Local_A];
   }
   Start_Rotax(Local_C);
}

int IRC_Chocky(){
   int Local_A,Local_B,Local_C,Local_D,Local_F,Local_Q=0,Local_CTRL_SEQ=0;
   int Word_Length=32;
   char *Out_Ptr[8],*Inp_Ptr[8];
   FILE *Local_MEM;
   for(Local_A=0;Local_A<8;Local_A++){
      for(Local_F=0;Local_F<32;Local_F++){
	 Abstract[0][Local_A][Local_F]=-1;
	 Abstract[1][Local_A][Local_F]=-1;	    
      }
    Inp_Ptr[Local_A]=&Chocky_Input[Local_A][0];
      Out_Ptr[Local_A]=&Chocky_Output[Local_A][0];
   }
   Chocky_Reset();
   //  puts("");
   Local_MEM=fopen("./Memory.ENiX","r");
   
   
   while(!Local_Q){
      
      
      //    printf("<CTRL> ");
      //    gets(Chocky_Input_Data);
      fscanf(Local_MEM,"%s",Chocky_Input_Data);

      while(strlen(Chocky_Input_Data)>0){
	 Chocky_Word();
	 if(strlen(Chocky_Word_Data)>Word_Length-1)
	   printf("<ENiX> This word is too long. %d characters max for this version of Chocky.\n",Word_Length-1);
	 else{
	    if(Chocky_Word_Data[0]!=':'){
	       if(Local_CTRL_SEQ>0){
		  if(Local_CTRL_SEQ==1){
		     Local_F=0;
		     for(Local_A=0;Local_A<Chocky_IM;Local_A++)
		       if(strcmp(Inp_Ptr[Local_A],Chocky_Word_Data)==0)
			 Local_F=1;
		     if(Chocky_IM<8){
		        if(!Local_F){
			   strncpy(Inp_Ptr[Chocky_IM],Chocky_Word_Data,Word_Length);
			   Chocky_IM++;
		        }
		     }
		     else
		       puts("<ENiX> Disregarding last parameter; only 8 usable addresses in this version of Chocky.");
		     for(Local_A=0;Local_A<Chocky_IM;Local_A++)
		       if(strcmp(Inp_Ptr[Local_A],Chocky_Word_Data)==0)
			 if(Chocky_IT<8)
			   Abstract[0][Chocky_IT][Example_Dim]=Local_A;
		     if(Chocky_IT<8)	  
		       Chocky_IT++;
		  }
		  if(Local_CTRL_SEQ==2){
		     Local_F=0;
		     for(Local_A=0;Local_A<Chocky_OM;Local_A++)
		       if(strcmp(Out_Ptr[Local_A],Chocky_Word_Data)==0)
			 Local_F=1;
		     if(Chocky_OM<8){
		        if(!Local_F){
			   strncpy(Out_Ptr[Chocky_OM],Chocky_Word_Data,Word_Length);  
			   Chocky_OM++;
			}
		     }
		     else
		       puts("<ENiX> Disregarding last parameter; only 8 usable addresses in this version of Chocky.");
		     for(Local_A=0;Local_A<Chocky_OM;Local_A++)
		       if(strcmp(Out_Ptr[Local_A],Chocky_Word_Data)==0)
			 Abstract[1][Chocky_OT][Example_Dim]=Local_A;
		     if(Chocky_OT<8)
		       Chocky_OT++;
		  }
	       }
	    }
	 }
	 if(strncmp(Chocky_Word_Data,":BEGIN",6)==0){
	    if(Local_CTRL_SEQ==0){
	       Local_CTRL_SEQ=1;
	    }
	    else
	      puts("<ENiX> Control sequence already active. Disregarding last command.");
	 }
	 if(strncmp(Chocky_Word_Data,":CASE",5)==0){
	    if(Local_CTRL_SEQ==0){
	       Local_CTRL_SEQ=1;
	       for(Local_A=0;Local_A<8;Local_A++)
		 for(Local_F=0;Local_F<32;Local_F++)
		   Abstract[0][Local_A][Local_F]=-1;
	       Chocky_IT=0;
	    }
	    else
	      puts("<ENiX> Control sequence already active. Disregarding last command.");
	 }
	 if(strncmp(Chocky_Word_Data,":DO",3)==0){
	    if(Local_CTRL_SEQ==1){
	       Local_CTRL_SEQ=0;
	       //	      puts("<ENiX> ok");
	       Chocky_Execute();
	       //		  printf("<ENiX> ");
	       for(Local_A=0;Local_A<8;Local_A++)
		 if(Byte[Local_A]>0)
		   printf("%s ",Out_Ptr[Local_A]);
	       //		  puts("");
	       fclose(Local_MEM);
	       return 0;
	    }
	    else
	      puts("<ENiX> No control sequence activated. Disregarding last command.");
	 }	
	 if(strncmp(Chocky_Word_Data,":END",4)==0){
	    if(Local_CTRL_SEQ==2){
	       Local_CTRL_SEQ=0;
	       //		  puts("<ENiX> ok");
	       Chocky_Example();
	    }
	    else
	      puts("<ENiX> Control sequence already terminated. Disregarding last command.");
	 }
	 if(strncmp(Chocky_Word_Data,":EXIT",5)==0){
	    Local_Q=1;
	    puts("<ENiX> Chocky has finished.");
	 }
	 if(strncmp(Chocky_Word_Data,":FORGET",7)==0){
	    if(Local_CTRL_SEQ==0)
	      Local_CTRL_SEQ=4;
	    else
	      puts("<ENiX> Control sequence already active. Disregarding last command.");
	 }
	 if(strncmp(Chocky_Word_Data,":RESET",6)==0){
	    Local_CTRL_SEQ=0;
	    puts("<ENiX> Data reset.");
	 }
	 if(strncmp(Chocky_Word_Data,":RUN",4)==0){
	    Chocky_Run();
	    //	    puts("<ENiX> Scenerio loaded into memory.");
	 }
	 if(strncmp(Chocky_Word_Data,":THEN",5)==0){
	    if(Local_CTRL_SEQ==1){
	       Local_CTRL_SEQ=2;
	    }
	    else
	      puts("<ENiX> Control sequence already active or primary control sequence failed. Disregarding last command.");
	 }
      }	
   }
   fclose(Local_MEM);
   return 0;
}

int Chocky(){
   int Local_A,Local_B,Local_C,Local_D,Local_F,Local_Q=0,Local_CTRL_SEQ=0;
   int Word_Length=32;
   char *Out_Ptr[8],*Inp_Ptr[8];
   for(Local_A=0;Local_A<8;Local_A++){
      for(Local_F=0;Local_F<32;Local_F++){
	 Abstract[0][Local_A][Local_F]=-1;
	 Abstract[1][Local_A][Local_F]=-1;	    
      }
      Inp_Ptr[Local_A]=&Chocky_Input[Local_A][0];
      Out_Ptr[Local_A]=&Chocky_Output[Local_A][0];
   }
   Chocky_Reset();
   puts("\n-----------------------------------------------------------------------------------------------");
   puts("Chocky Prototype. Advanced ENiX algorithms and heuristics. [OvO]wl Technology. Powered by ENiX.");
   puts("-----------------------------------------------------------------------------------------------");
   puts(":BEGIN - Start listing circumstances,          :CASE  - Specify scenerio,");
   puts(":DO    - Consequences of scenerio,             :END   - End listing circumstances,");
   puts(":EXIT  - Exit Chocky,                          :RESET - Clear Data,");
   puts(":RUN   - Understand circumstances,             :THEN  - List of consequences,");
   puts("-----------------------------------------------------------------------------------------------");
   while(!Local_Q){
      printf("<CTRL> ");
      gets(Chocky_Input_Data);
      while(strlen(Chocky_Input_Data)>0){
	 Chocky_Word();
	 if(strlen(Chocky_Word_Data)>Word_Length-1)
	   printf("<ENiX> This word is too long. %d characters max for this version of Chocky.\n",Word_Length-1);
	 else{
	    if(Chocky_Word_Data[0]!=':'){
	       if(Local_CTRL_SEQ>0){
		  if(Local_CTRL_SEQ==1){
		     Local_F=0;
		     for(Local_A=0;Local_A<Chocky_IM;Local_A++)
		       if(strcmp(Inp_Ptr[Local_A],Chocky_Word_Data)==0)
			 Local_F=1;
		     if(Chocky_IM<8){
		        if(!Local_F){
			   strncpy(Inp_Ptr[Chocky_IM],Chocky_Word_Data,Word_Length);
			   Chocky_IM++;
		        }
		     }
		     else
		       puts("<ENiX> Disregarding last parameter; only 8 usable addresses in this version of Chocky.");
		     for(Local_A=0;Local_A<Chocky_IM;Local_A++)
		       if(strcmp(Inp_Ptr[Local_A],Chocky_Word_Data)==0)
			 if(Chocky_IT<8)
			   Abstract[0][Chocky_IT][Example_Dim]=Local_A;
		     if(Chocky_IT<8)	  
		       Chocky_IT++;
		  }
		  if(Local_CTRL_SEQ==2){
		     Local_F=0;
		     for(Local_A=0;Local_A<Chocky_OM;Local_A++)
		       if(strcmp(Out_Ptr[Local_A],Chocky_Word_Data)==0)
			 Local_F=1;
		     if(Chocky_OM<8){
		        if(!Local_F){
			   strncpy(Out_Ptr[Chocky_OM],Chocky_Word_Data,Word_Length);  
			   Chocky_OM++;
			}
		     }
		     else
		       puts("<ENiX> Disregarding last parameter; only 8 usable addresses in this version of Chocky.");
		     for(Local_A=0;Local_A<Chocky_OM;Local_A++)
		       if(strcmp(Out_Ptr[Local_A],Chocky_Word_Data)==0)
			 Abstract[1][Chocky_OT][Example_Dim]=Local_A;
		     if(Chocky_OT<8)
		       Chocky_OT++;
		  }
	       }
	    }
	 }
	 if(strncmp(Chocky_Word_Data,":BEGIN",6)==0){
	    if(Local_CTRL_SEQ==0){
	       Local_CTRL_SEQ=1;
	    }
	    else
	      puts("<ENiX> Control sequence already active. Disregarding last command.");
	 }
	 if(strncmp(Chocky_Word_Data,":CASE",5)==0){
	    if(Local_CTRL_SEQ==0){
	       Local_CTRL_SEQ=1;
	       for(Local_A=0;Local_A<8;Local_A++)
		 for(Local_F=0;Local_F<32;Local_F++)
		   Abstract[0][Local_A][Local_F]=-1;
	       Chocky_IT=0;
	    }
	    else
	      puts("<ENiX> Control sequence already active. Disregarding last command.");
	 }
	 if(strncmp(Chocky_Word_Data,":DO",3)==0){
	    if(Local_CTRL_SEQ==1){
	       Local_CTRL_SEQ=0;
	       puts("<ENiX> ok");
	       Chocky_Execute();
	       printf("<ENiX> ");
	       for(Local_A=0;Local_A<8;Local_A++)
		 if(Byte[Local_A]>0)
		   printf("%s ",Out_Ptr[Local_A]);
	       puts("");
	    }
	    else
	      puts("<ENiX> No control sequence activated. Disregarding last command.");
	 }	
	 if(strncmp(Chocky_Word_Data,":END",4)==0){
	    if(Local_CTRL_SEQ==2){
	       Local_CTRL_SEQ=0;
	       puts("<ENiX> ok");
	       Chocky_Example();
	    }
	    else
	      puts("<ENiX> Control sequence already terminated. Disregarding last command.");
	 }
	 if(strncmp(Chocky_Word_Data,":EXIT",5)==0){
	    Local_Q=1;
	    puts("<ENiX> Chocky has finished.");
	 }
	 if(strncmp(Chocky_Word_Data,":FORGET",7)==0){
	    if(Local_CTRL_SEQ==0)
	      Local_CTRL_SEQ=4;
	    else
	      puts("<ENiX> Control sequence already active. Disregarding last command.");
	 }
	 if(strncmp(Chocky_Word_Data,":RESET",6)==0){
	    Local_CTRL_SEQ=0;
	    puts("<ENiX> Data reset.");
	 }
	 if(strncmp(Chocky_Word_Data,":RUN",4)==0){
	    Chocky_Run();
	    puts("<ENiX> Scenerio loaded into memory.");
	 }
	 if(strncmp(Chocky_Word_Data,":THEN",5)==0){
	    if(Local_CTRL_SEQ==1){
	       Local_CTRL_SEQ=2;
	    }
	    else
	      puts("<ENiX> Control sequence already active or primary control sequence failed. Disregarding last command.");
	 }
      }	
   }
   return 0;
}


void List_Numbers(int Local_N[256],int Local_M){
   int Local_A;
   printf("{");
   for(Local_A=0;Local_A<Local_M;Local_A++){
      if(Local_N[Local_A]<0)
	printf("?");
      else
	printf("%d",Local_N[Local_A]);
      if(Local_A<Local_M-1)
	printf(",");
   }
   printf("}");
}

int Siblings(int Local_M,int Local_Iteration){
   int Local_A,Local_B;
   if(Local_Iteration>0){
      for(Local_A=0;Local_A<Kindred_Cardinality;Local_A++){
	 Kindred_Data[Local_M-Local_Iteration]=Local_A;
	 if(Local_Iteration<Local_M){
	    if(Kindred_Data[Local_M-Local_Iteration]>Kindred_Data[Local_M-Local_Iteration-1])
	      Siblings(Local_M,Local_Iteration-1);
	 }
	 else
          Siblings(Local_M,Local_Iteration-1);
      }
   }
   else{
      printf(" - ");
      List_Numbers(Kindred_Data,Local_M);
      printf(" => ");
      Save_RWEs();
      for(Local_A=0;Local_A<Local_M;Local_A++)
	Numbers[Error_Index_List[Kindred_Data[Local_A]]]=-1;
      List_Numbers(Numbers,Number-2);
      printf(" => ");
      ENiX(Numbers,Number-2);
      Make_Solution_Set();
      List_Numbers(Solution_Set,Number-2);
      puts(".");	
      Restore_RWEs();
   }
   return 0;
}

int Kindred(){
   int Local_A,Local_Equal,Local_B;
   Coding_F=0;
   Scaling_F=1;
   Reflective_Trans=0;
   puts("\nKindred Technology (Pattern Associativity); [OvO]wl Technology; Powered by ENiX.");
   puts("--------------------------------------------------------------------------------.");
   Error_Correct(Numbers,Number-2);
   printf(" + Testing for total equalization ");
   if(Equalized(Error_Complexity,Error_Max)){
      puts("method is equalized; can't continue this proceedure.");
      return 0;
   }
   else
     puts("method isn't equalized; continuing.");
   printf(" + Testing for partial equalization; ");
   Local_Equal=P_Equalized(Error_Complexity,Error_Max);
   printf("{%d of %d} ",Local_Equal,Number-2);
   if(Local_Equal<2){
      puts("Not partially equalized - Finishing.");
      return 0;
   }
   else
     puts("equalized; continuing.");
   printf(" + Calculating new RWEs: { ");
   for(Local_A=0;Local_A<Number-2-Local_Equal;Local_A++)
     printf("%d|%d|%d ",Error_Index_List[Local_A],Error_Complexity[Local_A],Numbers[Error_Index_List[Local_A]]);
   puts("}.");
   puts(" + Utilizing ENiX self-awareness, using Set Theory (combinatorics) behavioral inhibitor.");
   Kindred_Cardinality=Number-2-Local_Equal;
   for(Local_A=0;Local_A<Kindred_Cardinality+1;Local_A++){
      printf(" + Solution sets with %d errors:.\n",Local_A);
      Siblings(Local_A,Local_A);
   }
   return 0;
}

int Prodigy(){
   int Local_A,Local_B,Local_Equal,Local_Buffer[256];
   Coding_F=0;
   Scaling_F=1;
   Reflective_Trans=0;
   puts("\nProdigy Technology 3 (formerly Analyzer), [OvO]wl Technology, Powered by ENiX.");
   puts("------------------------------------------------------------------------------.");
   Error_Correct(Numbers,Number-2);
   printf(" + Testing for total equalization, ");
   if(Equalized(Error_Complexity,Error_Max)){
      puts("method is equalized; can't continue this proceedure.");
      return 0;
   }
   else
     puts("method isn't equalized; continuing.");
   printf(" + Testing for partial equalization, ");
   Local_Equal=P_Equalized(Error_Complexity,Error_Max);
   printf("{%d of %d} ",Local_Equal,Number-2);
   if(Local_Equal<2){
      puts("Not partially equalized. Finishing.");
      return 0;
   }
   else
     puts("equalized; continuing.");
   printf(" + Calculating new RWEs: { ");
   for(Local_A=0;Local_A<Number-2-Local_Equal;Local_A++)
     printf("%d|%d|%d ",Error_Index_List[Local_A],Error_Complexity[Local_A],Numbers[Error_Index_List[Local_A]]);
   puts("}.");
   for(Local_A=0;Local_A<Number-2-Local_Equal;Local_A++){
      printf(" + Adapted RWEs: ");
      Numbers[Error_Index_List[Local_A]]=-1;
      List_Numbers(Numbers,Number-2);
      printf(" => ");
      ENiX(Numbers,Number-2);
      Make_Solution_Set();
      List_Numbers(Solution_Set,Number-2);
      for(Local_B=0;Local_B<256;Local_B++)
	Local_Buffer[Local_B]=Error_Index_List[Local_B];
      Error_Correct(Numbers,Number-2);
      if(Equalized(Error_Complexity,Error_Max))
	printf(" [EQ].\n");
      else
	printf(" [PE].\n");
      for(Local_B=0;Local_B<256;Local_B++)
	Error_Index_List[Local_B]=Local_Buffer[Local_B];
   }
   return 0;
}

int Analyzer(){
   int _,Local_A,Local_Equal;
   float Local_P;
   puts("\nAnalyzer Technology 2 (formerly Equalizer), [OvO]wl Technology, Powered by ENiX.");
   puts("--------------------------------------------------------------------------------.");
   Error_Correct(Numbers,Number-2);
   printf(" + Testing for total equalization, ");
   if(Equalized(Error_Complexity,Error_Max)){
      puts("method is equalized; can't continue this proceedure.");
      return 0;
   }
   else
     puts("method isn't equalized; continuing.");
   printf(" + Testing for partial equalization, ");
   Local_Equal=P_Equalized(Error_Complexity,Error_Max);
   printf("{%d of %d} ",Local_Equal,Number-2);
   if(Local_Equal<2){
      puts("Not partially equalized; Finishing.");
      return 0;
   }
   else
     puts("equalized; continuing.");
   printf(" + Calculating new RWEs: { ");
   for(Local_A=0;Local_A<Number-2-Local_Equal;Local_A++){
      printf("%d|%d|%d ",Error_Index_List[Local_A],Error_Complexity[Local_A],Numbers[Error_Index_List[Local_A]]);
      Numbers[Error_Index_List[Local_A]]=-1;
   }
   puts("}.");
   puts(" + Calculating substitute RWEs.");
   Coding_F=0;
   Scaling_F=1;
   Reflective_Trans=0;
   ENiX(Numbers,Number-2);
   Make_Solution_Set();
   printf(" + The new solution set is now: ");
   List_Numbers(Solution_Set,Solution_Max);
   puts(".");
   Local_P=100*Local_Equal;
   Local_P/=Number-2;
   _=Local_P;
   printf(" + Solution set accuracy depends on %3d %% of examples.\n",_);
   return 1;
}

void Equalizer(){
  int Local_A,Local_D[256];
  puts("\nEqualizer Technology (Experimental) Version 1, [OvO]wl Technology, Powered by ENiX.");
  puts("-----------------------------------------------------------------------------------.");
  Coding_F=0;
  Scaling_F=1;
  Reflective_Trans=0;
  Error_Correct(Numbers,Number-2);
  while(!Equalized(Error_Complexity,Error_Max)){
    printf(" + Equalizing series: ");
    Numbers[Error_Index_List[0]]=-1;
    ENiX(Numbers,Number-2);
    Make_Solution_Set();
	for(Local_A=0;Local_A<256;Local_A++)
	  Numbers[Local_A]=Solution_Set[Local_A];
    Error_Correct(Numbers,Number-2);
    List_Numbers(Numbers,Number-2);
    puts(".");
  }
  puts(" + Series Equalized.\n");
}

int DualScan(){
   int _,Local_A,Local_B,Local_C,Local_D,Local_E,Local_Numbers[256],L_Dual_Numbers[256],L_Dual_Number,Local_Matrix[8][256],Local_Dual_Errors[2][8],Local_Dual_Max;
   float Local_Sigma=0,Local_Prob=1;  
   if(Number>10){
      puts("\nError: more than 8 numeric parameters; DualScan uses linear algebra to transform data into ENiX 8-bit compatible problems; Unfortunately, because of the nature of the linear-algebra involved, no more than 8 examples can be accurately entered with this 8-bit core.");
      return 0;
   }
   for(Local_A=0;Local_A<256;Local_A++)
     Local_Numbers[Local_A]=Numbers[Local_A];
   Local_D=Number-2;
   puts("\nDualScan Technology (Experimental) Version 1, [OvO]wl Technology, Powered by ENiX.");
   puts("----------------------------------------------------------------------------------.");  
   printf(" + Acquiring solution set, ");
   Coding_F=0;
   Scaling_F=1;
   Reflective_Trans=0;
   ENiX(Numbers,Number-2);
   Make_Solution_Set();  
   List_Numbers(Solution_Set,Solution_Max);  
   Set_Transform(Solution_Set,Solution_Max);
   printf(".\n + Assimilating Dual problem solution set, ");  
   List_Numbers(Transformed,Transformed_Max);
   printf(".\n + Applying ENiX intelligent correction heuristics (Oddman-Out) to dual problem,");
   Error_Correct(Transformed,Transformed_Max);
   printf(".\n + Likely errors: ");
   for(Local_A=0;Local_A<Error_Max;Local_A++){
      printf("[%d,%d]",Error_Index_List[Local_A],Error_Complexity[Local_A]);
      Local_Sigma+=Error_Complexity[Local_A];
      Local_Dual_Errors[0][Local_A]=Error_Index_List[Local_A];
      Local_Dual_Errors[1][Local_A]=Error_Complexity[Local_A];
   }
   printf(".\n + Applying ENiX intelligent correction heuristics (Oddman-Out) to original problem,");
   Error_Correct(Local_Numbers,Local_D);
   printf(".\n + Likely errors: ");
   for(Local_A=0;Local_A<Error_Max;Local_A++)
     printf("[%d,%d]",Error_Index_List[Local_A],Error_Complexity[Local_A]);
   printf(".\n + Acquiring solution set, ");  
   Local_Numbers[Error_Index_List[0]]=-1;
   Number=Local_D+2;
   ENiX(Local_Numbers,Number-2);
   Make_Solution_Set();  
   List_Numbers(Solution_Set,Solution_Max);
   printf(".\n + Computing dichotomy: [%d->%d]; Error bits: {",Numbers[Error_Index_List[0]],Solution_Set[Error_Index_List[0]]);
   Local_B=Numbers[Error_Index_List[0]];
   Local_C=Solution_Set[Error_Index_List[0]];
   for(Local_A=0;Local_A<8;Local_A++){
      if(Local_B-2*(Local_B/2)!=(Local_C-2*(Local_C/2))){
	 printf(" %d",Local_A);
	 for(Local_E=0;Local_E<Local_Dual_Max;Local_E++){
	    if(Local_Dual_Errors[0][Local_E]==Local_A){
	       Local_Prob*=(1-(Local_Dual_Errors[1][Local_E]/Local_Sigma));
	    }
	 }
      }
      Local_B/=2;
      Local_C/=2;
   }
   puts(" }.");
   _=100*Local_Prob;
   printf(" + DualScan agrees with this answer %d %%.\n",_);
   puts("");
   return 0;
}



int Cyber_Talk(){
   int Local_C,Local_D=0,Local_Reg=0,Local_E=0;
   int Local_Delta,Local_Gamma,Local_Lambda;
   puts("First of all, the byte, N, is broken down into a 8bit sequence and assigned positions Input[0]-[8]; ENiX");
   puts("successfully passed the examples into the kernel, which derived the following logical explanation:.\n");
   Local_D=0;
   Local_Delta=1;
   Local_Gamma=1;
   Local_Lambda=0;
   Local_D=Number-2;  //
   for(Local_C=0;Local_C<8;Local_C++){
      if(Local_Gamma<Local_D+1)
         if(Local_Lambda<Local_C)
            Local_Lambda=Local_C;
      Local_Gamma*=2;
   }
   Local_Lambda--; //
   for(Local_C=0;Local_C<S_Cluster;Local_C++){
      if(Super_Cluster[3][Local_C]==Super_Cluster[3][Local_C+1]){
	 Local_Reg=1;
	 if(Super_Cluster[1][Local_C]>Local_Lambda){
	   printf("                             :IF [%d] :%s <%d>       :THEN [%d] ->  %d  :ENDIF.\n",Super_Cluster[0][Local_C],LOGIC_TYPE[Super_Cluster[2][Local_C]],Super_Cluster[1][Local_C],Super_Cluster[3][Local_C],Local_E+Local_Lambda+1);
		Local_E++;
		}
	 else{ 
	   printf("                             :IF [%d] :%s [%d]       :THEN [%d] ->  %d  :ENDIF.\n",Super_Cluster[0][Local_C],LOGIC_TYPE[Super_Cluster[2][Local_C]],Super_Cluster[1][Local_C],Super_Cluster[3][Local_C],Local_E+Local_Lambda+1);
		Local_E++;
		}
      }
      else{
	 if(Super_Cluster[1][Local_C]>Local_Lambda){
	    if(Local_Reg==1){
	      printf("                             :IF [%d] :%s <%d>       :THEN     -> [%d] :ENDIF.\n",Super_Cluster[0][Local_C],LOGIC_TYPE[Super_Cluster[2][Local_C]],Super_Cluster[1][Local_C],Super_Cluster[3][Local_C]);
		Local_E=0;
		}
	    else{
	      printf("                             :IF [%d] :%s [%d]       :THEN     -> [%d] :ENDIF.\n",Super_Cluster[0][Local_C],LOGIC_TYPE[Super_Cluster[2][Local_C]],Super_Cluster[1][Local_C],Super_Cluster[3][Local_C]);
		Local_E=0;
		}
	 }
	 else{
	   printf("                             :IF [%d] :%s [%d]       :THEN     -> [%d] :ENDIF.\n",Super_Cluster[0][Local_C],LOGIC_TYPE[Super_Cluster[2][Local_C]],Super_Cluster[1][Local_C],Super_Cluster[3][Local_C]);
		Local_E=0;
		}
      }
   }
   printf(".\nBehavioural enhancers: ");
   if(Reflective_Trans==1)
     printf("reflection %d-N,",Coding_M);
   printf(" sequence starts at: %d, sequence uses multiples of: %d.\n",Coding_F,Scaling_F);
   puts(".\nBy specifying N to a particular value, executing it through the logic above, the binary output array");
   puts("expressed in denary is the Nth solution.");
   return 0;
}

int Evaluate(){                                            // Uses: N_Example,
  int Local_A;
  Make_Solution_Set();  
  printf("\nSolution set = ");
  List_Numbers(Solution_Set,Solution_Max);
  printf(", Complexity = %d neurals..\n\n",Minds);
  Cyber_Talk();
  return 1;
}

int AUTO(){
   int Local_C;
   LCD();
   Minimum(Numbers,Number-2);
   Maximum(Numbers,Number-2);
   Reflective_Trans=0;
   for(Local_C=0;Local_C<Number-2;Local_C++)
     if(Numbers[Local_C]!=-1)
       Numbers[Local_C]-=Coding_F;
   LCD();
   if(Scaling_F>1){
      for(Local_C=0;Local_C<Number-2;Local_C++)
	if(Numbers[Local_C]!=-1)
	  Numbers[Local_C]=Numbers[Local_C]/Scaling_F;
   }
   int Local_TMP1=256,Local_TMP2=0,Local_Increasing=0,Local_Decreasing=0;
   for(Local_C=0;Local_C<Number-2;Local_C++){
      if(Numbers[Local_C]!=-1){
	 if(Numbers[Local_C]>Local_TMP1)
	   Local_Increasing++;
	 if(Numbers[Local_C]<Local_TMP2)
	   Local_Decreasing++;
	 Local_TMP1=Local_TMP2=Numbers[Local_C];
      }		
   }
   if(!Local_Increasing){
      Maximum(Numbers,Number-2);
      for(Local_C=0;Local_C<Number-2;Local_C++)
	if(Numbers[Local_C]!=-1)
	  Numbers[Local_C]=Coding_M-Numbers[Local_C];
      Reflective_Trans=1;
   }
   if(Scaling_F<1)
     Scaling_F=1;
   ENiX(Numbers,Number-2);
   return 0;
}

int IL_SC(){ // *** STILL OFFLINE ***

   int Local_C;
   Scaling_F=1;
   Reflective_Trans=0;
   for(Local_C=0;Local_C<Number-2;Local_C++)
     if(Numbers[Local_C]!=-1)
       Numbers[Local_C]-=Coding_F;
   LCD();
   for(Local_C=0;Local_C<Number-2;Local_C++)
     if(Numbers[Local_C]!=-1)
       Numbers[Local_C]=Numbers[Local_C]/Scaling_F;
   ENiX(Numbers,Number-2);
   
   return 0;
}

int Translate(){
  int Local_C,Local_C1,Local_D;
  for(Local_C=0;Local_C<256;Local_C++){
	Numbers[256]=0;
    Unknowns[256]=0;
  }
  for(Local_C=0;Local_C<Number-2;Local_C++){
    for(Local_C1=0;Local_C1<strlen(ASCII_Examples[Local_C]);Local_C1++){
	  if((ASCII_Examples[Local_C][Local_C1]<47)||(ASCII_Examples[Local_C][Local_C1]>58)){
		if(strlen(ASCII_Examples[Local_C])==1){
		  if(ASCII_Examples[Local_C][Local_C1]!='?'){
		    printf("\n>ENiX Example number %d has to be in range 0 to 255 or ?.\n\n",Local_C+1);
		    return -5; 
		  }
		  else{
			Numbers[Local_C]=-1;
			Unknowns[Local_C]=1;
		  }
	    }
//	   else{
//		 printf("\n>ENiX Example number %d has to be in range 0 to 255 or ?.\n\n",Local_C+1);
//		 return -5;
//	   }
	  }
	}
    if(ASCII_Examples[Local_C][0]!='?'){
	  Local_D=atoi(ASCII_Examples[Local_C]);
	  if((Local_D<-255)||(Local_D>255)){
	    printf("\n>ENiX Example number %d has to be in range 0 to 255 or ?.\n\n", Local_C+1);
	    return -5;
	  }
	  Numbers[Local_C]=Local_D;
    }
  }
  return 0;
}

int ILEARN(){ // *** DEFAULT LEARNING TECHNIQUE - Handles all patterns. ***
   Coding_F=0;
   Scaling_F=1;
   Reflective_Trans=0;
   ENiX(Numbers,Number-2);
   return 0;
}

int IL_CO1(){ // *** Designed for monotonic increasing patterns only. ***
   int Local_C;
   Scaling_F=1;
   Reflective_Trans=0;
   for(Local_C=0;Local_C<Number-2;Local_C++)                                            // Code the examples
     if(Numbers[Local_C]!=-1)                                                           // if not unknown
       Numbers[Local_C]-=Coding_F;                                                      // code it!
   ENiX(Numbers,Number-2);
   return 0;
}

int IL_CO2(){ // *** Designed for monotonic decreasing patterns only. ***
   int Local_C;
   Coding_F=0;
   Scaling_F=1;
   Reflective_Trans=1;
   for(Local_C=0;Local_C<Number;Local_C++)
     if(Numbers[Local_C]!=-1)
       Numbers[Local_C]=Coding_M-Numbers[Local_C];
   ENiX(Numbers,Number-2);
   return 0;
}

int LCD(){
   int Local_C,Local_C2,Switch_Found;
   for(Local_C2=2;Local_C2<Coding_M+1;Local_C2++){
      Switch_Found=1;
      for(Local_C=0;Local_C<Number-2;Local_C++)
	if(Numbers[Local_C]!=-1)
	  if((Numbers[Local_C]%Local_C2)!=0)
	    Switch_Found=0;
      if(Switch_Found==1)
	Scaling_F=Local_C2;
   }
   return 0;
}

int Maximum(int Local_Numbers[256],int Local_Dim){ // Local_Dim == Number-2
   int Local_C;
   Coding_M=0;
   for(Local_C=0;Local_C<Local_Dim;Local_C++)
     if(Local_Numbers[Local_C]!=-1)
       if(Coding_M<Local_Numbers[Local_C])
	 Coding_M=Local_Numbers[Local_C];
   return Coding_M;
}

int Minimum(int Local_Numbers[256],int Local_Dim){ // Local_Dim == Number-2
   int Local_C;
   Coding_F=256;
   for(Local_C=0;Local_C<Local_Dim;Local_C++)
     if(Local_Numbers[Local_C]!=-1)
       if(Coding_F>Local_Numbers[Local_C])
	 Coding_F=Local_Numbers[Local_C];
   return Coding_F; 
}

void Make_Solution_Set(){
  int Local_A,Local_B;
  Solution_Max=Number-2;
  for(Local_A=0;Local_A<256;Local_A++)
    Solution_Set[Local_A]=0;
  for(Local_A=0;Local_A<Solution_Max;Local_A++){
    Local_B=Eval(Local_A);
    if(Reflective_Trans==1)
      Local_B=Coding_M-Local_B;
    Local_B*=Scaling_F;
    Local_B+=Coding_F;
    Solution_Set[Local_A]=Local_B;
  }
}

void Set_Transform(int Local_Data[256],int Local_M){
  int Local_A,Local_B,Local_C,Local_D,Local_Matrix[8][256];
  for(Local_A=0;Local_A<256;Local_A++)
    Transformed[Local_A]=0;
  Transformed_Max=0;
  for(Local_A=0;Local_A<Local_M;Local_A++){
    for(Local_B=0;Local_B<8;Local_B++)
      Local_Matrix[Local_B][Local_A]=0;
    Local_D=Local_Data[Local_A];
    for(Local_B=0;Local_B<8;Local_B++){    
      Local_C=Local_D/2;
      Local_Matrix[7-Local_B][Local_A]=Local_D-2*Local_C;
      Local_D/=2;
    }
  }
  for(Local_A=0;Local_A<8;Local_A++){
    Local_C=0;
    Local_D=1; 
    for(Local_B=0;Local_B<Local_M;Local_B++){
      Local_C+=Local_D*Local_Matrix[7-Local_A][Number-3-Local_B];
      Local_D*=2;
    }
    Transformed[Local_A]=Local_C;
    if(Local_C>0)
      Transformed_Max++;
  }
}

void Error_Correct(int Local_Data[256],int Local_M){
  int Local_D[256],Local_A,Local_B,Local_C;
  Save_RWEs();
  for(Local_A=0;Local_A<256;Local_A++)
    Local_D[Local_A]=Local_Data[Local_A];
  Error_Max=Local_M;
  for(Local_A=0;Local_A<Local_M;Local_A++){
    for(Local_B=0;Local_B<256;Local_B++)
      Local_Data[Local_B]=Local_D[Local_B];
    Local_Data[Local_A]=-1;
    ENiX(Local_Data,Local_M);
	Restore_RWEs();
    Error_Complexity[Local_A]=Minds;
    Error_Index_List[Local_A]=Local_A;
  }
  for(Local_A=0;Local_A<Local_M;Local_A++){
    for(Local_B=0;Local_B<Local_M-1;Local_B++){
      if(Error_Complexity[Local_B]>Error_Complexity[Local_B+1]){
        Local_C=Error_Complexity[Local_B];
        Error_Complexity[Local_B]=Error_Complexity[Local_B+1];
        Error_Complexity[Local_B+1]=Local_C;
        Local_C=Error_Index_List[Local_B];
        Error_Index_List[Local_B]=Error_Index_List[Local_B+1];
        Error_Index_List[Local_B+1]=Local_C;
      }
    }
  }
}

int Oddman(){
   int _,Local_D[256],Local_A,Local_B,Local_F,Local_Reg;
   float Local_P;
   Coding_F=0;
   Scaling_F=1;
   Reflective_Trans=0;
   Error_Correct(Numbers,Number-2);
   for(Local_A=0;Local_A<Error_Max;Local_A++)
     Local_F+=Error_Complexity[Local_A];
   puts("\n                 --[Oddman Out]------------------------------------------------------.");
   puts("                 Solution #    Position #    Value    Complexity    Error Probability.");
   puts("                 --------------------------------------------------------------------.");
   for(Local_A=0;Local_A<Error_Max;Local_A++){
      Local_P=100*Error_Complexity[Local_A];
      Local_P/=Local_F;
      _=Local_P;
      if(Numbers[Error_Index_List[Local_A]]<0)
	 printf("                        %3d           %3d        ?    %10d    %d %%.\n",Local_A+1,Error_Index_List[Local_A]+1,Error_Complexity[Local_A],_);
      else
	printf("                        %3d           %3d      %3d    %10d    %d %%.\n",Local_A+1,Error_Index_List[Local_A]+1,Numbers[Error_Index_List[Local_A]],Error_Complexity[Local_A],_);
   }
   puts("                 -------------------------------------------------------[Version 2]--.");
   printf("         Solution found; 'Oddman Out' is %3d numbers from the start of the sequence and is %3d.\n",Error_Index_List[0]+1,Numbers[Error_Index_List[0]]);
   Numbers[Error_Index_List[0]]=-1;
   ENiX(Numbers,Number-2);
   Make_Solution_Set();
   printf("Sequence should probably be: ");
   List_Numbers(Solution_Set,Solution_Max);
   puts(".");
   return 0;
}

int Equalized(int Local_D[256],int Local_M){
  int Local_A,Local_B=Local_D[0],Local_C=1;
  for(Local_A=0;Local_A<Local_M;Local_A++){
    if(Local_C==1)
      if(Local_B!=Local_D[Local_A])
        Local_C=0;
  }
  return Local_C;
}

void Save_RWEs(){
   int Local_A;
   RWE_Size=Number;
   for(Local_A=0;Local_A<256;Local_A++)
     RWEs[Local_A]=Numbers[Local_A];
}

void Restore_RWEs(){
   int Local_A;
   Number=RWE_Size;
   for(Local_A=0;Local_A<256;Local_A++)
     Numbers[Local_A]=RWEs[Local_A];
}

int P_Equalized(int Local_D[256],int Local_M){
  int Local_A,Local_B=0,Local_C=0;
  for(Local_A=0;Local_A<Local_M;Local_A++){
    if(Local_D[Local_A]>Local_B)
      Local_B=Local_D[Local_A];
  }
  for(Local_A=0;Local_A<Local_M;Local_A++){
    if(Local_D[Local_A]==Local_B)
	  Local_C++;
  }
  return Local_C;
}

void Chocky_Word(){
  int Local_Term,Local_Start,Local_A,Local_B,Local_String_Length;
  for(Local_A=0;Local_A<1024;Local_A++)
	Chocky_Word_Data[Local_A]=toupper(Chocky_Input_Data[Local_A]);
  Local_String_Length=strlen(Chocky_Input_Data);
  Local_Term=0;
  Local_Start=0;
  for(Local_A=0;Local_A<Local_String_Length;Local_A++){
	if(Chocky_Word_Data[Local_A]==' '){
	  if(Local_Term==0)
		Local_Term=Local_A;
	}
	else
	  if(Local_Term>0){  
		Local_Start=Local_A;
		Local_A=Local_String_Length;
	  }
  }
  if(Local_Term==0){
	for(Local_A=0;Local_A<1024;Local_A++)
	  Chocky_Word_Data[Local_A]=Chocky_Input_Data[Local_A];
	for(Local_A=0;Local_A<1024;Local_A++)
	  Chocky_Input_Data[Local_A]=0;
  }
  else{
	for(Local_A=0;Local_A<1024;Local_A++)
	  Chocky_Word_Data[Local_A]=0;
	for(Local_A=0;Local_A<Local_Term;Local_A++)
	  Chocky_Word_Data[Local_A]=Chocky_Input_Data[Local_A];
	Local_B=strlen(Chocky_Input_Data);
	for(Local_A=0;Local_A<Local_B+1;Local_A++)
	  Chocky_Input_Data[Local_A]=Chocky_Input_Data[Local_A+Local_Start];
  }
}

void Chocky_Reset(){
  int Local_A,Local_B;
  Example_Dim=0;
  Number=258;
  for(Local_A=0;Local_A<256;Local_A++)
	Numbers[Local_A]=-1;
  for(Local_A=0;Local_A<8;Local_A++)
	for(Local_B=0;Local_B<32;Local_B++){
	  Abstract[0][Local_A][Local_B]=-1;
	  Abstract[1][Local_A][Local_B]=-1;
	  Chocky_Input[Local_A][Local_B]=0;
	  Chocky_Output[Local_A][Local_B]=0;	    
	}
  Chocky_IT=0,
  Chocky_OT=0,
  Chocky_IM=0,
  Chocky_OM=0;
}

void Chocky_Run(){
  int Local_A,Local_B;
  for(Local_A=0;Local_A<256;Local_A++){
    if(Numbers[Local_A]>-1)
	  Local_B=Local_A+3;
  }
  Number=1;
  for(Local_A=0;Local_A<Chocky_IM;Local_A++)
	Number*=2;	
  Number+=2;
  Coding_F=0;
  Scaling_F=1;
  Reflective_Trans=0;
  ENiX(Numbers,Number-2);
  Make_Solution_Set();
}

void Chocky_Example(){
  int Local_A,Local_B,Local_C,Local_D;
  Local_B=0;
  Local_C=1;
  for(Local_A=0;Local_A<8;Local_A++)
	Byte[Local_A]=0;
  for(Local_A=0;Local_A<8;Local_A++)
	if(Abstract[0][Local_A][Example_Dim]>-1)
	  Byte[Abstract[0][Local_A][Example_Dim]]=1;
  for(Local_A=0;Local_A<8;Local_A++){
	Local_B+=Local_C*Byte[Local_A];
	Local_C*=2;
  }
  Local_D=Local_B;
  Local_B=0;
  Local_C=1;
  for(Local_A=0;Local_A<8;Local_A++)
	Byte[Local_A]=0;
  for(Local_A=0;Local_A<8;Local_A++)
	if(Abstract[1][Local_A][Example_Dim]>-1)
	  Byte[Abstract[1][Local_A][Example_Dim]]=1;
  for(Local_A=0;Local_A<8;Local_A++){
	Local_B+=Local_C*Byte[Local_A];
	Local_C*=2;
  }
  Numbers[Local_D]=Local_B;
  Chocky_IT=0;
  Chocky_OT=0;
  Example_Dim++;
}

void Chocky_Execute(){
  int Local_A,Local_B,Local_C;
  Local_B=0;
  Local_C=1;
  for(Local_A=0;Local_A<8;Local_A++)
	Byte[Local_A]=0;
  for(Local_A=0;Local_A<8;Local_A++)
	if(Abstract[0][Local_A][Example_Dim]>-1)
	  Byte[Abstract[0][Local_A][Example_Dim]]=1;
  for(Local_A=0;Local_A<8;Local_A++){
	Local_B+=Local_C*Byte[Local_A];
	Local_C*=2;
  }
  for(Local_A=0;Local_A<8;Local_A++)
	Byte[Local_A]=0;
  Local_B=Solution_Set[Local_B];
  for(Local_A=0;Local_A<8;Local_A++){
	Local_C=Local_B/2;
	if((2*Local_C)!=Local_B)
	  Byte[Local_A]=1;
	  Local_B=Local_C;
	}
}

void Save(char *LD){
    char Local_Data[256];
    strcpy(Local_Data,"cp ./Memory.ENiX ");
	strcat(Local_Data,LD);
	strcat(Local_Data,".ENiX");
	system(Local_Data);
}

void Load(char *LD){
    char Local_Data[256];
    strcpy(Local_Data,"cp ./");
	strcat(Local_Data,LD);
	strcat(Local_Data,".ENiX ./Memory.ENiX");
	system(Local_Data);
}

void Delete(char *LD){
    char Local_Data[256];
    strcpy(Local_Data,"rm ./");
	strcat(Local_Data,LD);
	strcat(Local_Data,".ENiX");
	system(Local_Data);
}

// USER INTERFACE

int Print_Out(){
   int Local_C,Local_A=-1,Local_B=-1;
   for(Local_C=0;Local_C<S_Cluster;Local_C++){
      printf("    => %03d: Neuron Type %s(%d,%d) -> %d Found. ",Local_C,LOGIC_TYPE[Super_Cluster[2][Local_C]],Super_Cluster[0][Local_C]+1,Super_Cluster[1][Local_C]+1,Super_Cluster[3][Local_C]+1);
      Local_A=Super_Cluster[3][Local_C];
      if(Local_A==Local_B)
	puts("[ML].");
      else
	puts("[PL].");
      Local_B=Local_A;
   }
   Evaluate();
   return 1;
}

int Summary(){
   int Recognised=0;
      Minimum(Numbers,Number-2);
   Maximum(Numbers,Number-2);
   if(strncmp(Option,":",1)==0)
     Linguistics();
   if(strncmp(Option,"ANALYZER",8)==0){
      Translate();
      Analyzer();
   }
   if(strncmp(Option,"AUTO",4)==0){
      Translate();
      AUTO();
      Evaluate();
   }
   if(strncmp(Option,"CHOCKY",5)==0)
     Chocky();
   if(strncmp(Option,"DUALSCAN",8)==0){
      Translate();   
      DualScan();
   }
   if(strncmp(Option,"EQUALIZER",9)==0){
      Translate();
      Equalizer();
   }
   if(strncmp(Option,"EQUATION",8)==0){
      Translate();
      Init_Rotax(2);
   }
   if(strncmp(Option,"FULLAUTO",8)==0){
      Translate();
      Fullauto();
   }
   if(strncmp(Option,"HELP",4)==0)
     Help();
   if(strncmp(Option,"IL-CO1",6)==0){
      Translate();
      IL_CO1();
      Evaluate();
   }
   if(strncmp(Option,"IL-CO2",6)==0){
      Translate();
      IL_CO2();
      Evaluate();
   }
   if(strncmp(Option,"ILEARN",6)==0){
      Translate();
      ILEARN();
      Evaluate();
   }
   if(strncmp(Option,"IL-SC",5)==0){                                             
      Translate();
      IL_SC();
      Evaluate();
   }
   if(strncmp(Option,"KINDRED",7)==0){
      Translate();
      Kindred();
   }
   if(strncmp(Option,"ODDMAN",6)==0){
      Translate();
      Oddman();
   }
   if(strncmp(Option,"PRODIGY",7)==0){
      Translate();
      Prodigy();
   }
   if(strncmp(Option,"ROTAX",5)==0){
      Translate();
      Init_Rotax(1);
   }
   puts(".\nENiX 8-bit IRC Edition; [OvO]wl Technology, AtlasNET; Powered by PA-RISC.");
   puts("                            GPL Bristol 9/2/2005 - Martyn S Braithwaite.");
   puts("                    [OvO]wl's System, AtlasNET; Powered by 2x SMP PA-RISC 785 CPUs.\n");
   return 0;
}

int Help(){
  puts("\nENiX [ AUTO || EQUATION || FULLAUTO || ILEARN || ODDMAN || ROTAX ] { [integer] || ? }.");
  return 0;
}



int main(int argc, char *argv[]){
   int Local_C;
   if(argc<2){
    puts(".\n>Too few parameters, must be: <ENiX Name> <Command> [Example001] [Example002] - [Example256].");
    puts(">Where ExampleXXX is a number from 0 to 255 or ?.\n");
    return -2;
  }
  if(argc>258){
    puts(".\n>Too many parameters, must be: <ENiX Name> <Command> [Example001] [Example002] - [Example256].");
    puts(">Where ExampleXXX is a number from 0 to 255 or ?.\n");
    return -1;
  }
  Number=argc;
  if(strlen(argv[0])>16){
    printf(".\n>ENiX CLI Name, %s, too long; Should be 16 chars Max.\n\n\r",argv[0]);
    return -3;
  } 
  Program_Name=argv[0];
  if(strlen(argv[1])>16){
    printf(".\n>ENiX CLI Option, %s, too long; Should be 16 chars Max.\n\n\r",argv[1]);
    return -4;
  }
  Option=argv[1];
  for(Local_C=2;Local_C<Number;Local_C++){
    if(strlen(argv[Local_C])>8){
      printf(".\n>ENiX Example number %d has to have at most 8 symbols.\n\n",Local_C-1);
      return -5;
    }
    ASCII_Examples[Local_C-2]=argv[Local_C];
  }
  Summary();
  return 1;

}

// return values
// return 1 - systems fine
// return -1 - too many cli parameters
// return -2 - too few cli parameters
// return -3 - ENiX name too long
// return -4 - ENiX Option too long
// return -5 - ENiX Example wrong format
// return -6 - No ENiX option selected

// programmed by ms braithwaite


