#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// *** Global
int Power(int A,int B);

// *** Was basis.h
void LOAD_BASIS(void);
int (*GOPHER[100])(int I1,int I2);
int GOPHER_MAX=8; // 16 for mj
char *LOGIC_TYPE[8]={"ONE ","GATE","NOT ","AND ","OR  ","NOR ","XOR ","NAND"};
inline int ONE(int I1,int I2){return 1;}
inline int GATE(int I1,int I2){return I1;}
inline int NOT(int I1,int I2){return !I1;}
inline int AND(int I1,int I2){return I1&I2;}
inline int OR(int I1,int I2){return I1|I2;}
inline int NOR(int I1,int I2){return !(I1|I2);}
inline int XOR(int I1,int I2){return (I1!=I2);}
inline int NAND(int I1,int I2){return !(I1&I2);}
// *** End of basis.h

// *** Was time.h
int TPI;
// *** End of time.h

// *** Was memory.h
int File_Dimension(char *Filename);
int File_Insert(char *Filename,char *Data,int Start);
int File_Remove(char *Filename,int Start,int Length);
int File_Word_Beg(char *Filename,int Word_Number);
int File_Word_End(char *Filename,int Word_Number);
int File_Extract_Word(char *Filename,int Word_Number);
int File_Exists(char *Filename);
int File_Neural(char *Data,char *Filename);
char File_Word[32];
// *** End of memory.h

// *** Was kernel.h
int S_Cluster;                                                              // Public access SuperCluster size
int Super_Cluster[4][256];                                                  // Public access SuperCluster union of Subnets
unsigned long int Minds;                                                    // Publically accessible stats feature
int RUN_ENiX(int Local_Ex[256],int Local_N_Ex);                             // Publically accessible
int Execute2(int Local_Index,int Local_Neural[4][128],int Local_Cluster_S); // Complete
int Eval(int Local_Index);                                                  // Complete
int I_Band;                                                                 // Vital for remote execution
int N_Example;
int O_Band;   
int Bits[8];
int Examples[256];
int Found;
int Learn();
int Morph2(int Local_Neuron,int Local_Output,int Local_Neural[4][128],int Local_CS);
int Test(int Local_Output,int Local_Neural[4][128],int Local_CS);
int Bit_Distribution();
int Bandwidth(int Local_C);
// *** End of kernel.h

// *** Was detect.h
int Detected_Bandwidth;
int System_Bandwidth();
void Detect();
// *** End of detect.h

// *** Was input.h
int IExamples;
char *Example[1024];
int Example_Category[1024];
void Input();
int Command_Present(char *Local_Command,int Local_Length,int Local_TPI);
int Example_Type(int Local_TPI);
int Numeric[2][1024];
float Floats[2][1024];
int Numeric_Term;
char Data[1024][32];
// *** End of input.h

// *** Was output.h
int Output_Language;
char Voice_Synthesis_Command[32];
void Help(char Local_Str[32]);
void PrintF(float Local_F);
void PrintGap();
void PrintTPI(int Local_TPI);
void PrintL(char *Local_Str);
void PrintS(char *Local_Str);
void PrintN(int Local_Number);
void PrintNL(int Local_Data[2][1024],int Local_Size);
void PrintPoly(int Local_P[1024],int Local_M);
void PrintRec(float LF[1024],int LC,int LS);
void PrintLegacy(int _[4][1024],int __,int ___);
void Splash(int Local_Width);
void Margin();
void Out_Init();
void Out_Term();
void Rec_Filter(int Local_Success,int Local_Order,float Local_Coefficient,float Local_Constant);
void PrintDep(int L_Exam,int L_TPI,int L_SA[10][32]);
void PrintSA(int L_Exam,int L_TPI,int L_SA[10][32]);
void Version();
void Binary(int Nums[2][1024],int Dim);
void SayS(char *Spoken);
void SayI(int Spoken);
void SayF(float Spoken);
void Output();
// *** End of output.h

// *** Was rotax2.h
int Rotax_Examples[1024];
int Rotax_Unknowns[1024];
int Rotax_Max;
int Rotax_Polynomial[1024];
int Rotax_Poly_Dim;
int Start_Rotax(int Local_F);
void Rotax_List_Numbers(int Local_N[1024],int Local_Unknown[1024],int Local_M);
int Rotax_Exe_Polynomial(int Local_C);
int Rotax_Neural();
int Rotax_Factorial(int Local_N); 
// *** End of rotax2.h

// *** Was recursion.h
int R_Filter(int LData[2][1024],int LDim);
void R_Deltic(int LData[2][1024],int LDim);
void R_TPI_AA();
int R_TPI_RA(int L_TPI);
int R_Successful(int Local_Dim);
float R_Neural[1024];
int REC_Start;
int R_Filter_Success;
int R_Filter_Order;
float R_Filter_Coefficient;
float R_Filter_Constant;
int R_Neural_Order;
int R_Order();
float R_Solution_Set[1024];
void R_Push_FIFO(float L_Data);
float R_Eval_FIFO();
int R_Filter_Inp[2][1024];
int R_Filter_Shifted[2][1024];
int R_Filter_Dim;
void R_Filter_Shift(int L_Shift);
int R_Filter_Test();
void R_Filter_Create();
// *** End of recursion.h

// *** Was control.h
void Go();
void Classify();
void Parse();
void Poly();
int Rec();
int TPIs();
int Logic();                       // This is ENiX original mode but with behavioural inhibitor assembly
void Procreator(int Force);
void Seq(char *Filename);
void Par(char *Filename);
int Mtd(char *Filename);
void Control();
int Model(char _[32]); // sets problem mode
int Inside(int _[2][1024],int __);  // specifies the template of the problem
int By(int Override);
int Behavior();
int Defines(char *Name);
int Extract_Floats();
int Extract_Numbers();
int Success;
int Method;                                                     // Method  0 == POLY  1 == RECU  2 == ENiX II  3 == Legacy
int Polynomial[1024];
float Recursive[1024];
int ENiX_Neural[10][32];
int Neural_Borders[2];
int R_Start;
int Solution_Set[2][1024];
int Solution_Dim;
int Poly_Dim;
int Recurs_Test(int Local_A[2][1024],int Local_B[2][1024],int Local_Max);
int Rec_BI_Success;
int Rec_BI_Order;
float Rec_BI_Coefficient;
float Rec_BI_Constant;
int Legacy_Neural[4][1024];
int Legacy_Size;
int Legacy_Band;
int Complexity;
// *** End of control.h

// *** Was enix.h
// This class is the first attempt to implant old enix technologies into a new environmental control
// this new environment will be able to design enixs thoughts before they are guessed and  or tested.
  int Legacy[4][1024];                             // Stores Legacys converted neural
  int Legacy_Dim;                                  // Specifies the size of this neural
  int Legacy_I_Band;                               // Contains the input boundary specification
  int ENiX_Complexity;                                  // Notion of complexity
  int Execute_Legacy(int Local_TPI);               // Obtains Legacy s logical prediction
  int Setup(int Local_D[2][1024],int Local_M);
  int TPI_Band_Width;
  int Dimension_Vector[2];
  int Build_Self_Awareness();
  int Data_Super_Charger();                        // Optimises data into canonical form for Legacy
  int Neural_SA[10][32];                           // Self-awareness from the Procreator module.
  int Build_Neural();
  int Final_Test();
  int Calc_TPI(int LTPI);
  int Time_Taken;
  int Self_Awareness[32][32];                      // Self-Awareness Matrix [xx][] == bit. [][xx] == TPI.
  int Recursion_Wavelengths[32];                   // Contains supplimentary information on self-awareness.
  int Self_Aware_Res;
  int ENiX_In_Dat[2][1024];
  int ENiX_In_Max;
  int ENiX2_Neural[1024];
  int Morph1(int Max,int Current);
  int Neural_Feedback(int Max);
  int Neural_Input[1024];                          // Which input bits
  int Neural_Output;                               // Which output bit
  int Calc_Bit(int Max,int LTPI);
  int Twisted_Pear(int LS1,int LS2,int LW,int LP); // The Twisted Pear == 1 when LS1==LS2, else is 0
  int Factorise(int L_Wavelength);
  void TPI_ENiX_Bandwidth(int L);
  void Example_ENiX_Bandwidth();
  int ENiX_Bandwidth(int L);
  int Test_Category(int L);
  int Test_Phase_Shift(int L_Data[2][1024],int L_Lambda);
  int Diagnose();
  void Single_Digit(int Bit_Stream);
  void Single_Input(int Bit_Stream);
  void Compound(int Bit_Stream,int Stream_ENiX_Complexity);
// *** End of enix.h

// *** basis.h
void LOAD_BASIS(){
  GOPHER[0]=ONE,GOPHER[1]=GATE,GOPHER[2]=NOT,GOPHER[3]=AND,GOPHER[4]=OR,GOPHER[5]=NOR,GOPHER[6]=XOR,GOPHER[7]=NAND;
}
// *** memory.h
int File_Neural(char *Data,char *Filename){
  int A;
  char Path[1024];
  FILE *Read;
  FILE *Write;
  strcpy(Path,"./ENiX.DATA/");
  strcat(Path,Filename);
  strcat(Path,".METHOD");
  A=File_Dimension(Path);
  if(A==-1){
    if((Write=fopen(Path,"w"))==NULL)
      return -1;
    else{
      fprintf(Write,"%s",Data);
      fclose(Write);
      return 0;
    }
  }
  else{

  }
}

int File_Exists(char *Filename){
  FILE *Read;
  if((Read=fopen(Filename,"r"))==NULL)
    return 0;
  else{
    fclose(Read);
    return 1;
  }
}

int File_Extract_Word(char *Filename,int Word_Number){
  int Max,Size,Start,Finish;
  char C;
  FILE *Read;
  Start=File_Word_Beg(Filename,Word_Number);
  Finish=File_Word_End(Filename,Word_Number);
  Max=File_Dimension(Filename);
  if(((Start<0)|(Finish<0))|(Max<0))
    return -1;
  if((Read=fopen(Filename,"r"))==NULL)
    return -1;
  for(Size=0;Size<32;Size++)
    File_Word[Size]=0;
  for(Size=0;Size<Start;Size++)
    C=fgetc(Read);
  for(Size=Start;Size<Finish-1;Size++){
    C=fgetc(Read);
    File_Word[Size-Start]=C;
  }
  return 0;
}

int File_Word_End(char *Filename,int Word_Number){
  int Size,Max,Start;
  char C;
  FILE *Read;
  Max=File_Dimension(Filename);
  Start=File_Word_Beg(Filename,Word_Number);
  if(Start<0)
    return -1;
  if((Read=fopen(Filename,"r"))==NULL)
    return -1;
  for(Size=0;Size<Start;Size++)
    C=fgetc(Read);
  for(Size=Start;Size<Max;Size++){
    C=fgetc(Read);
    if(C==' '){
      fclose(Read);
      return Size+1;
    }
  }
  fclose(Read);
  return Max;
}

int File_Word_Beg(char *Filename,int Word_Number){
  int Size,Max,Word=0,Words=0;
  char C;
  FILE *Read;
  Max=File_Dimension(Filename);
  if((Read=fopen(Filename,"r"))==NULL)
    return -1;
  for(Size=0;Size<Max;Size++){
    C=fgetc(Read);
    if((C!=' ')&(Word==0)){
      Word=1;
      Words++;
    }
    if((C==' ')&(Word==1))
      Word=0;
    if(Word_Number==Words){
      fclose(Read);
      return Size;
    }
  }
  fclose(Read);
  return -1;
}

int File_Remove(char *Filename,int Start,int Length){
  int Size,Max;
  char C,OS[45];
  FILE *Read,*Reserved;
  Max=File_Dimension(Filename);
  if((strlen(Filename)>32)|(Max<Start+Length))
    return -1;
  if((Read=fopen(Filename,"r"))==NULL)
    return -1;
  if((Reserved=fopen("_TEMP_","w"))==NULL){
    fclose(Read);
    return -1;
  }
  for(Size=0;Size<Max;Size++){
    if((Size>Start-1)&(Size<Start+Length))
      C=fgetc(Read);
    else{
      C=fgetc(Read);
      fprintf(Reserved,"%c",C);
    }
  }
  fclose(Read);
  fclose(Reserved);
  strcpy(OS,"mv _TEMP_ ");
  strcat(OS,Filename);
  system(OS);
  return 0;
}

int File_Insert(char *Filename,char *Data,int Start){
  int Size,Max;
  char C,OS[45];
  FILE *Read,*Reserved;
  Max=File_Dimension(Filename);
  if((strlen(Filename)>32)|(Max<Start))
    return -1;
  if((Read=fopen(Filename,"r"))==NULL)
    return -1;
  if((Reserved=fopen("_TEMP_","w"))==NULL){
    fclose(Read);
    return -1;
  }
  for(Size=0;Size<Max;Size++){
    if(Size==Start)
      fprintf(Reserved,"%s",Data);
    else{
      C=fgetc(Read);
      fprintf(Reserved,"%c",C);
    }
  }
  fclose(Read);
  fclose(Reserved);
  strcpy(OS,"mv _TEMP_ ");
  strcat(OS,Filename);
  system(OS);
  return 0;
}

int File_Dimension(char *Filename){
  int Size;
  char C;
  FILE *Buffer;
  if((Buffer=fopen(Filename,"r"))==NULL)
    return -1;
  C=fgetc(Buffer);
  Size=1;
  while(C!=EOF){
    C=fgetc(Buffer);
    Size++;
  }
  fclose(Buffer);
  return Size;
}

// *** kernel.h
int RUN_ENiX(int Local_Ex[256],int Local_N_Ex){
  int Local_C;
  Minds=0;
  for(Local_C=0;Local_C<256;Local_C++)
    Examples[Local_C]=Local_Ex[Local_C];
  I_Band=Bandwidth(Local_N_Ex);
  N_Example=1<<I_Band;                                                                                              // 09.09.06
  for(Local_C=Local_N_Ex;Local_C<N_Example;Local_C++)
    Examples[Local_C]=-1;
  Bit_Distribution();
  if(Learn()==-1)
    return -1;                                                                  // Problem complexity exceeded
}

                                                                                      // multiple lamination upgrades.
int Execute2(int Local_Index,int Local_Neural[4][128],int Local_Cluster_S){
  int Local_Bits[12];                                                                 // increase input buffer range
  int Local_R;                                                                        // establish a return register
  for(Local_Bits[11]=0;Local_Bits[11]<11;Local_Bits[11]++)                            // use upper register
    Local_Bits[Local_Bits[11]]=0;                                                     // tidy upper register
  for(Local_Bits[11]=0;Local_Bits[11]<I_Band;Local_Bits[11]++)                        // for lower register range
    Local_Bits[Local_Bits[11]]=(Local_Index>>Local_Bits[11])&1;                       // fill lower registers
  Local_Bits[11]=0;                                                                   // tidy up
  for(Local_Bits[11]=0;Local_Bits[11]<Local_Cluster_S+1;Local_Bits[11]++){ 
    Local_R=GOPHER[Local_Neural[2][Local_Bits[11]]](Local_Bits[Local_Neural[0][Local_Bits[11]]],Local_Bits[Local_Neural[1][Local_Bits[11]]]);
    Local_Bits[I_Band+Local_Bits[11]]=Local_R;                                        // stash the previous output
  }                                                                                   // in extended registers
  return Local_R;                                                                     // should contain the output
}

int Eval(int Local_Index){
   int Local_Bits[12],Local_Out[8],Local_A,Local_B,Local_D;          // Extending registers
   int Local_Lambda,Local_Delta;                                     // Lambda=test, Delta=cluster spec
   int Local_R;                                                      // Adding hard register
   for(Local_Bits[11]=0;Local_Bits[11]<11;Local_Bits[11]++)          // All registers
     Local_Bits[Local_Bits[11]]=0;                                   // Zero inputs
   for(Local_Bits[11]=0;Local_Bits[11]<8;Local_Bits[11]++)           // All registers
     Local_Out[Local_Bits[11]]=0;                                    // Zero outputs
   for(Local_Bits[11]=0;Local_Bits[11]<I_Band;Local_Bits[11]++)      // All registers - I_Band needs specifying
     Local_Bits[Local_Bits[11]]=(Local_Index>>Local_Bits[11])&1;     // Load input regs
   Local_Bits[11]=0;                                                 // Tidy up
   Local_Delta=0;
   for(Local_Bits[11]=0;Local_Bits[11]<S_Cluster;Local_Bits[11]++){
      Local_Lambda=Super_Cluster[3][Local_Bits[11]]; 
      Local_Bits[I_Band+Local_Delta]=GOPHER[Super_Cluster[2][Local_Bits[11]]](Local_Bits[Super_Cluster[0][Local_Bits[11]]],Local_Bits[Super_Cluster[1][Local_Bits[11]]]);
      Local_Out[Super_Cluster[3][Local_Bits[11]]]=Local_Bits[I_Band+Local_Delta];
      Local_Delta++;
      if(Local_Lambda!=Super_Cluster[3][Local_Bits[11]+1])
	Local_Delta=0;
   }
   Local_A=1;
   Local_B=0;
   for(Local_D=0;Local_D<8;Local_D++){
      Local_B+=Local_A*Local_Out[Local_D];
      Local_A*=2;
   }
   return Local_B;
}
// multiple lamination upgrades.

int Learn(){                               // Uses variates: Super_Cluster[][], S_Cluster, O_Band, Cluster_Size,
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
      Morph2(Local_Neurons,Local_C,Local_Neural,Local_Neurons);
      Local_Neurons++;  
      if(Minds>250000)
	return -1;
    }
    Local_M+=Minds;
  }
  Minds=Local_M;
  return 0;
}

int Test(int Local_Output,int Local_Neural[4][128],int Local_CS){  // Variates used: Minds, Found, N_Example, S_Cluster
  int Local_C,Local_A,Local_B;                                                                   // Bits[], Super_Cluster[][].
  Found=1;                                            // Variates we can keep: Minds, Found, N_Example, Bits[], Super_Cluster,
  Minds++;                                                                                                       // S_Cluster.
  for(Local_C=0;Local_C<N_Example;Local_C++){
    Local_A=Execute2(Local_C,Local_Neural,Local_CS);
    Local_B=Examples[Local_C]<0?-1:(Examples[Local_C]>>Bits[Local_Output])&1;                                       // 09.09.06
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
int Morph2(int Local_Neuron,int Local_Output,int Local_Neural[4][128],int Local_CS){
  int Local_C=Local_Neuron;
  Local_Neural[3][Local_Neuron]=Local_Output;
  if(!Found){
    for(Local_Neural[2][Local_Neuron]=0;Local_Neural[2][Local_Neuron]<GOPHER_MAX;Local_Neural[2][Local_Neuron]++){
      if(Local_Neural[2][Local_Neuron]<1){
	if(Local_Neuron>0)
	  Morph2(Local_Neuron-1,Local_Output,Local_Neural,Local_CS);
	else{
	  Test(Local_Output,Local_Neural,Local_CS);
	  if(Found)
	    return 1;
	}
      }
      else{
	for(Local_Neural[0][Local_Neuron]=0;Local_Neural[0][Local_Neuron]<I_Band+Local_C;Local_Neural[0][Local_Neuron]++){
	  if(Local_Neural[2][Local_Neuron]<3){
	    if(Local_Neuron>0)
	      Morph2(Local_Neuron-1,Local_Output,Local_Neural,Local_CS);
	    else{
	      Test(Local_Output,Local_Neural,Local_CS);
	      if(Found)
		return 1;
	    }
	  }
	  else{
	    for(Local_Neural[1][Local_Neuron]=Local_Neural[0][Local_Neuron];Local_Neural[1][Local_Neuron]<I_Band+Local_C;Local_Neural[1][Local_Neuron]++){
	      if(Local_Neuron>0)
		Morph2(Local_Neuron-1,Local_Output,Local_Neural,Local_CS);
	      else{
		Test(Local_Output,Local_Neural,Local_CS);
		if(Found)
		  return 1;
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
  for(Local_C=0;Local_C<8;Local_C++)
    Bits[Local_C]=Local_C;
  O_Band=8;
  for(Local_C1=0;Local_C1<O_Band;Local_C1++){
    Local_F=0;
    for(Local_C=0;Local_C<N_Example;Local_C++)
      if(((Examples[Local_C]>>Bits[Local_C1])&1)&(Examples[Local_C]>-1)==1)                                       // 09.09.06
	Local_F=1;
    if(Local_F==0){
      O_Band--;
      for(Local_D=Local_C1;Local_D<7;Local_D++)
	Bits[Local_D]=Bits[Local_D+1];
      Local_C1--;
    }
  }
  return 0;
}

int Bandwidth(int Local_C){
  int Local_R=0,Local_A=1;
  for(Local_R=0;Local_A<Local_C;Local_R++)
    Local_A*=2;
  return Local_R;
}

// *** detect.h
void Detect(){
  Detected_Bandwidth=System_Bandwidth();
}

int System_Bandwidth(){
  int Local_A=1,Local_B;
  for(Local_B=0;Local_A>0;Local_B++)
    Local_A=Local_A<<1;
  return ++Local_B;
}

// *** input.h
int Example_Type(int Local_TPI){
  char Local_Buffer[32];
  int Local_A,Local_R;
  for(Local_A=0;Local_A<32;Local_A++)
    Local_Buffer[Local_A]=0;
  strcpy(Local_Buffer,Example[Local_TPI]);
  if(Local_Buffer[0]=='?')
    return 1;
  if(Local_Buffer[0]==':')
    return 2;
  if(Local_Buffer[0]=='-')
    return 0;
  if(Local_Buffer[0]>47)
    if(Local_Buffer[0]<58)
      return 0;
  return 3;
}

int Command_Present(char *Local_Command,int Local_Length,int Local_TPI){
  if(strncasecmp(Example[Local_TPI+1],Local_Command,Local_Length)==0)
    return 1;
  return 0;
}

void Input(){
  int Local_A,Local_B;
  for(Local_A=0;Local_A<1024;Local_A++)
    for(Local_B=0;Local_B<32;Local_B++)
      Data[Local_A][Local_B]=0;
  for(Local_A=0;Local_A<1024;Local_A++)
    Example[Local_A]=&Data[Local_A][0];
}

// *** output.h
void PrintLegacy(int _[4][1024],int __,int ___){
  int A,Symbol=0;
  int Bandwidth=0;
  for(A=__;A!=1;A=A>>1)
    Bandwidth++;
  if(Output_Language!=2){
    if(Output_Language==3)
      SayS("Please see screen for the logical connection between output over time.");
    for(A=0;A<__;A++){
      Margin();
      printf("%04d ",A);
      if(_[0][A]<Bandwidth)
	printf("INP%02d",_[0][A]);
      else
	printf("TOK%02d",_[0][A]-Bandwidth);
      printf(" %s ",LOGIC_TYPE[_[2][A]]);      
      if(_[2][A]<3)
	printf("      -> ");
      else{
	if(_[1][A]<Bandwidth)
	  printf("INP%02d -> ",_[1][A]);
	else
	  printf("TOK%02d -> ",_[1][A]-Bandwidth);
      }
      if(A==__-1)
	printf("SOL%02d SOLUTION",_[3][A]);
      else{
	if(_[3][A]!=_[3][A+1]){
	  printf("SOL%02d SOLUTION",_[3][A]);
	  Symbol=0;
	}
	else{
	  printf("TOK%02d THEN",Symbol);
	  Symbol++;
	}
      }
      PrintL("");
    }
    Margin();
    PrintS("Input Boundaries: ");
    PrintN(___);
    PrintL("");
  }
}

void SayF(float Spoken){
  char _[1024],__[32];
  int A;
  for(A=0;A<1024;A++)
    _[A]=0;
  for(A=0;A<32;A++)
    __[A]=0;
  strcpy(_,Voice_Synthesis_Command);
  snprintf(__,31,"%f",Spoken);
  strcat(_,__);
  system(_);
}

void SayI(int Spoken){
  char _[1024],__[32];
  int A;
  for(A=0;A<32;A++)
    __[A]=0;
  for(A=0;A<1024;A++)
    _[A]=0;
  strcpy(_,Voice_Synthesis_Command);
  snprintf(__,31,"%d",Spoken);
  strcat(_,__);
  system(_);
}

void SayS(char *Spoken){
  char _[1024];
  int A,OK=0;
  for(A=0;A<strlen(Spoken);A++){
    strcpy(_,Spoken);
    if(_[A]!=' ')
      OK=1;
  }
  if(OK){
    for(A=0;A<1024;A++)
      _[A]=0;
    strcpy(_,Voice_Synthesis_Command);
    strcat(_,Spoken);
    system(_);
  }
}

void Output(){strcpy(Voice_Synthesis_Command,"say -v Zarvox ");}

void Binary(int Nums[2][1024],int Dim){
  int A=1,B,Max=0,Bandwidth=0;
  if(Output_Language!=2){
    if(Output_Language==3)
      SayS("Please see the table which shows the examples in binary.");
    for(B=0;B<Dim;B++)
      if((Nums[1][B]==0)&(Nums[0][B]>Max))
	Max=Nums[0][B];
    for(A=1;A<Max+1;A*=2)
      Bandwidth++;
    for(B=0;B<Bandwidth;B++){
      Margin();
      if(B<10)
	printf("0");
      printf("%d|",B);
      for(A=0;A<Dim;A++)
	if(Nums[1][A]==0)
	  printf("%d",(Nums[0][A]>>B)&1);
	else
	  printf("?");
      PrintL("");
    }
  }
}

void PrintSA(int L_Exam,int L_TPI,int L_SA[10][32]){
  int L1,L2;
  char *Logic_Type[16]={" -----"," 0    "," 1    "," WIRE "," NOT  "," OR   "," NOR  "," AND  "," NAND "," XOR  "," NXOR "," N-AND"," AND-N"," N-NA "," NA-N "," ERROR"};
  char *Logic_Html[16]={"CONNECT","ZERO","ONE","WIRE","NOT","OR","NOR","AND","NAND","XOR","NXOR","N_AND","AND_N","N_NA","NA_N","ERROR"};
  if((Output_Language==0)||(Output_Language==3)){
    if(Output_Language==3)
      SayS("Please see the table which displays how the examples relate over time.");
    printf("       I\\O | ");
    for(L2=0;L2<L_Exam;L2++)
      printf("%05d ",L2);
    printf("\n       ____|");
    for(L2=0;L2<L_Exam;L2++)
      printf("______");
    puts("");
    for(L1=0;L1<L_TPI-1;L1++){
      printf("       %03d |",L1);
      for(L2=0;L2<L_Exam;L2++)
	printf("%s",Logic_Type[L_SA[L1][L2]+1]);
      puts("");
    }
  }
  if(Output_Language==1){
    Margin();
    printf("<TABLE><TR><TD>I\\O</TD>");
    for(L2=0;L2<L_Exam;L2++)
      printf("<TD>%d</TD>",L2);
    for(L1=0;L1<L_TPI-1;L1++){
      printf("</TR><TR><TD>%d</TD>",L1);
      for(L2=0;L2<L_Exam;L2++)
	printf("<TD><IMG SRC=\"./ENiX.HTML/%s\" WIDTH=\"50\" HEIGHT=\"50\"></TD>",Logic_Html[L_SA[L1][L2]+1]);
    }
    printf("</TABLE>");
    PrintL("");
  }
}

void PrintDep(int L_Exam,int L_TPI,int L_SA[10][32]){
  int L1,L2;
  if((Output_Language==0)||(Output_Language==3)){
    if(Output_Language==3)
      SayS("Please see the table which shows which bits of time influence which examples.");
    printf("       I\\O | ");
    for(L2=0;L2<L_Exam;L2++)
      printf("%05d ",L2);
    printf("\n       ____|");
    for(L2=0;L2<L_Exam;L2++)
      printf("______");
    puts("");
    for(L1=0;L1<L_TPI-1;L1++){
      printf("       %03d |",L1);
      for(L2=0;L2<L_Exam;L2++){
	if(L_SA[L1][L2]>0)
	  printf(" TRUE ");
	else
	  printf(" FALSE");
      }
      puts("");
    }
  }
  if(Output_Language==1){
    Margin();
    printf("<TABLE><TR><TD>I\\O</TD>");
    for(L2=0;L2<L_Exam;L2++)
      printf("<TD>%d</TD>",L2);
    for(L1=0;L1<L_TPI-1;L1++){
      printf("</TR><TR><TD>%d</TD>",L1);
      for(L2=0;L2<L_Exam;L2++){
	if(L_SA[L1][L2]>0)
	  printf("<TD><IMG SRC=\"./ENiX.HTML/TRUE\" WIDTH=\"50\" HEIGHT=\"50\"></TD>");
	else
	  printf("<TD><IMG SRC=\"./ENiX.HTML/FALSE\" WIDTH=\"50\" HEIGHT=\"50\"></TD>");
      }
    }
    printf("</TABLE>");
    PrintL("");
  }
}

void Rec_Filter(int Local_Success,int Local_Order,float Local_Coefficient,float Local_Constant){
  if(Local_Success)
    if(Output_Language<2)
      printf("Behavioural inhibitor used: f[n+%d]=%f*f[n]+[%f].",Local_Order,Local_Coefficient,Local_Constant);
}

void PrintRec(float LF[1024],int LC,int LS){
  int C,LTPI,First_Term=1;
  Margin();
  if(Output_Language!=2){
    if(Output_Language==3)
      SayS("Please see screen for the recursive equation.");
    printf("F[n] = ");
    for(LTPI=0;LTPI<1024;LTPI++){
      if(LF[LTPI]!=0){
	if(!First_Term)
	  if(LF[LTPI]>0)
	    printf("+");
	First_Term=0;
	C=LF[LTPI];
	if(LF[LTPI]!=C)
	  printf("%.3f*F[n-%d]",LF[LTPI],LTPI+1);
	else{
	  if(LF[LTPI]!=1)
	    printf("%d*F[n-%d]",C,LTPI+1);
	  else
	    printf("F[n-%d]",LTPI+1);
	}
      }
    }
    if(!LS)
      printf("+[...]");
    printf(" Where F[0] = %d.",LC);
  }
}

void PrintF(float Local_F){
  if(Output_Language!=2){
    printf("%f",Local_F);
    if(Output_Language==3)
      SayF(Local_F);
  }
}

void PrintGap(){
  if(Output_Language==0)
    puts("");
  if(Output_Language==1)
    puts("</TABLE><BR><TABLE><COL WIDTH=100><COL WIDTH=700>");
  if(Output_Language==3)
    puts("");
}

void Out_Init(){
  if(Output_Language==0)
    puts("");
  if(Output_Language==1)
    puts("<TABLE><COL WIDTH=100><COL WIDTH=700>");
}

void Out_Term(){
  if(Output_Language==0)
    puts("");
  if(Output_Language==1)
    puts("</TABLE>");
}

void Margin(){
  if(Output_Language==0)
    printf("       ");
  if(Output_Language==1)
    printf("<TR><TD></TD><TD>");
  if(Output_Language==3)
    printf("       ");
}

void Splash(int Local_Width){
  if(Output_Language==0){
    puts("\n ENiX has detected your output.");
    printf(" ENiX has decided to use %d-bit.\n",Local_Width);
  }
  if(Output_Language==1){
    puts("<CENTER><TABLE>");
    puts("<TR><TD>ENiX has detected your output.</TD></TR>");
    printf("<TR><TD>ENiX has decided it's best to use %d-bit.</TD></TR>\n",Local_Width);
    puts("</TABLE></CENTER>");
  }
}

void PrintPoly(int Local_P[1024],int Local_M){
   char _[64];
   int Local_E,Local_A;
   Margin();
   if(Output_Language!=2){
      printf("F[x] = ");
      if(Output_Language==3)
	SayS("Please see the screen for the polynomial.");
      Local_E=0;
      for(Local_A=0;Local_A<Local_M;Local_A++){
	 if((Local_P[Local_M-1-Local_A]>0)&&(Local_E==1))                                                              // 10.09.06
	   printf("+");
	 if(Local_P[Local_M-1-Local_A]<0)
	   printf("-");
	 if(Local_P[Local_M-1-Local_A]==1){
	    if(Local_M-1-Local_A==0)                                                                                    // 13.09.06
	      printf("1");
	 }
	 else{
	    if(Local_P[Local_M-1-Local_A]!=0){
	       if(Local_P[Local_M-1-Local_A]<0){                                                                        // 10.09.06
		  if(Local_P[Local_M-1-Local_A]!=-1)
		    printf("%d",-1*Local_P[Local_M-1-Local_A]);
		  else
		    if(Local_M-1-Local_A==0)
		      printf("1");
	       }
	       else
		 printf("%d",Local_P[Local_M-1-Local_A]);
	    }
	 }
	 if((Local_P[Local_M-1-Local_A]!=0)&&((Local_M-1-Local_A)!=0))                                                 // 10.09.06
	   printf("x");
	 if((Local_P[Local_M-1-Local_A]!=0)&&(Local_M-1-Local_A>1))                                                    // 10.09.06
	   printf("^%d",Local_M-1-Local_A);
	 if((Local_P[Local_M-1-Local_A]!=0)&&(Local_E==0))                                                             // 10.09.06
	   Local_E=1;
      }      
      puts("");
   }
}

void Help(char Local_Str[32]){
   int Local_Recognised=0;
   if(strncasecmp(Local_Str,":BEHAVIOR",9)==0){
      Margin();
      PrintL("Syntax: \":BEHAVIOR\" {numbers}. Sets behavioral inhibitor.");
      Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":BINARY",7)==0){
    Margin();
    PrintL("Syntax: \":BINARY\" {numbers}. Displays the binary composition of sequence.");
    Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":BY",3)==0){
    Margin();
    PrintL("Syntax: \":BY\" {numbers}. Numbers are a vector code of neural.");
    Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":CLOAK",6)==0){
    Margin();
    PrintL("Syntax: \":CLOAK\". Cloaks ENiX output.");
    Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":DEFINES",8)==0){
    Margin();
    PrintL("Syntax: \":DEFINES\" [name]. Remembers concept as name.");
    Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":DEPENDS",8)==0){
    Margin();
    PrintL("Syntax: \":DEPENDS\". Displays ENiX bit dependencies.");
    Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":FORCE",6)==0){
    Margin();
    PrintL("Syntax: \":FORCE\". Force, over-rides behavioural inhibitor; ENiX solves sequent pattern.");
    Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":HELP",5)==0){
    Margin();
    PrintL("Syntax: \":HELP [:command]\". Displays the help menu.");
    Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":HTML",5)==0){
    Margin();
    PrintL("Syntax: \":HTML\". Reconfigures ENiX to use HTML output.");
    Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":INSIDE",7)==0){
    Margin();
    PrintL("Syntax: \":INSIDE\" {numbers}. Specifies limits of the neural.");
    Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":LOGIC",6)==0){
    Margin();
    PrintL("Suntax: \":LOGIC\". Displays the logical relationships between symbols.");
    Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":METHOD",7)==0){
    Margin();
    PrintL("Syntax: \":METHOD\". Displays working of last successful learning stage.");
    Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":MODEL",6)==0){
    Margin();
    PrintL("Syntax: \":MODEL\" [:command]. Specifies which neural to use.");
    Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":MTD",4)==0){
    Margin();
    PrintL("Syntax: \":MTD\" [file]. Loads [file].METHOD file into RAM.");
    Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":NORM",5)==0){
    Margin();
    PrintL("Syntax: \":NORM\". Reconfigures ENiX to use console output.");
    Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":PAR",4)==0){
    Margin();
    PrintL("Syntax: \":PAR\" filename. Executes filename in parallel.");
    Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":RECURSION",10)==0){
    Margin();
    PrintL("Syntax: \":RECURSION\" {numbers}. Finds a recursion relation that generates these numbers.");
    Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":SAY",4)==0){
    Margin();
    PrintL("Syntax: \":SEQ\". Switches on voice synthesis.");
    Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":SEQ",4)==0){
    Margin();
    PrintL("Syntax: \":SEQ\" filename. Executes filename sequentially.");
    Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":TPI",4)==0){
    Margin();
    PrintL("Syntax: \":TPI\" {numbers}. Calculates the outcomes of those scenerios.");
    Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":TYPE",5)==0){
    Margin();
    PrintL("Syntax: \":TYPE\" {numbers}. Assesses the most suitable way of solving the problem.");
    Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":VERSION",8)==0){
    Margin();
    PrintL("Syntax: \":VERSION\". Displays a list of technologies in ENiX II.");
    Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":NULL",5)==0){
    Margin();
    PrintL("Welcome to ENiX 2!");
    Margin();
    PrintL("On this page is a list of currently available commands:");
    Margin();
    PrintL(":BEHAVIOR :BINARY :BY :CLOAK :DEFINES :DEPENDS :FORCE :HELP");
    Margin();
    PrintL(":HTML :INSIDE :LOGIC :METHOD :MODEL :MTD :NORM :NULL :PAR");
    Margin();
    PrintL(":POLY :RECURSION :SAY :SEQ :TPI :TYPE :VERSION");
    PrintGap();
    Margin();
    PrintL("Use context sensitive help for more detailed information for example:");
    Margin();
    PrintL("\":HELP :NORM\" gives syntax and information on the command \":NORM\".");
    PrintGap();
    Margin();
    PrintL("Note that :NULL is a void command, necessary to the function of ENiX.");
    Margin();
    PrintL("therefore :HELP :NULL details have been dissabled to prevent Bertrand");
    Margin();
    PrintL("Russels Paradox. Eg understand \":HELP :NULL ...\" to be equivalent to"); 
    Margin();
    PrintL("\"(:HELP) :NULL ...\" rather than \"(:HELP :NULL) ...\".");
    Local_Recognised=1;
  }
  if(strncasecmp(Local_Str,":POLY",5)==0){
    Margin();
    PrintL("Syntax: \":POLY {numbers}\". Finds polynomial model for \"{numbers}\".");
    Local_Recognised=1;
  }
  if(!Local_Recognised){
    Margin();
    PrintL("No help topic found for this command.");
  }
}

void PrintTPI(int Local_TPI){
  if(Output_Language==0)
    printf(" %04d: ",Local_TPI);
  if(Output_Language==1)
    printf("<TR><TD>%04d:</TD><TD>",Local_TPI);
  if(Output_Language==3){
    SayS(" T P I at time ");
    SayI(Local_TPI);
    printf(" %04d: ",Local_TPI);
  }
}

void PrintNL(int Local_Data[2][1024],int Local_Size){
  int Local_A;
  if(Output_Language!=2){
    printf("[");
    for(Local_A=0;Local_A<Local_Size;Local_A++){
      if(Local_Data[1][Local_A]==0){
	printf("%d",Local_Data[0][Local_A]);
	if(Output_Language==3)
	  SayI(Local_Data[0][Local_A]);
	if(Local_A<Local_Size-1)
	  printf(", ");
      }
      else{
	printf("?");
	if(Local_A<Local_Size-1)
	  printf(", ");
      }
    }
    printf("]");
  }
}

void PrintN(int Local_Number){
  if(Output_Language!=2){
    if(Output_Language==3)
      SayI(Local_Number);
    printf("%d",Local_Number);
  }
}

void PrintS(char *Local_Str){
  if(Output_Language==0)
    printf("%s",Local_Str);
  if(Output_Language==1)
    printf("%s",Local_Str);
  if(Output_Language==3){
    SayS(Local_Str);
    printf("%s",Local_Str);
  }
}

void PrintL(char *Local_Str){
  if(Output_Language==0)
    printf("%s\n",Local_Str);
  if(Output_Language==1)
    printf("%s</TD></TR>\n",Local_Str);
  if(Output_Language==3){
    SayS(Local_Str);
    printf("%s\n",Local_Str);
  }
}

void Version(){
   if(Output_Language!=2){
      PrintGap();
      Margin();
      PrintL("   ENiX Behavioural Inhibitor Collection 2.");
      Margin();
      PrintL("Edition: KRELL SUITE 2.");
      Margin();
      PrintL("   ENiX Kernel: Kernel Rewrite Attempt 5, Release 2.");
      Margin();
      PrintL("Implicit Technologies: Thought Procreator, Trial and Error Learning,");
      Margin();
      PrintL("Multiple-Lamination, 32-Bit, Evolutionary Learning, Fast Self-Assessment,");
      Margin();
      PrintL("Data Supercharger, ENiX Legacy Support.");
      Margin();
      PrintL("Edition: CIROC.");
      Margin();
      PrintL("   ENiX Equation Comprehension: Rewrite 2.");
      Margin();
      PrintL("Edition: POLY-32 Integer Edition.");
      Margin();
      PrintL("   ENiX Inductive Reasoning: Edition 1.");
      Margin();
      PrintL("Implicit Technologies: Order 1 Poly + Recursion Hybreeding, Simplifier,");
      Margin();
      PrintL("Automatic TPI Modelling.");
      Margin();
      PrintL("Edition: QUASAR 1.");
      Margin();
      PrintL("   ENiX Context Sensitive Instruction Set: Rewrite 2.");
      Margin();
      PrintL("Implicit technologies: Parallel + Sequential Processing, Scope");
      Margin();
      PrintL("Resolution Control, Memory Management, Thought Programming,");
      Margin();
      PrintL("Context Sensitive Method Switching.");
      Margin();
      PrintL("Edition: CHOCKY 2.");
      Margin();
      PrintL("Developed by M.S.Braithwaite. GPL 2006. [OvO]wl Technology.");
   }
}

// *** rotax2.h
int Rotax_Neural(){
  int Local_Examples[256][256],Local_Unknowns[256][256],LX[256],LU[256],Local_Max;
  int Local_A,Local_B,Local_C,Local_D,Local_Iteration,Local_Found_Sol;
  Local_Max=Rotax_Max-2;
  for(Local_A=0;Local_A<256;Local_A++){
    Local_Examples[0][Local_A]=Rotax_Examples[Local_A],
      Local_Unknowns[0][Local_A]=Rotax_Unknowns[Local_A];
  }
  for(Local_Iteration=0;Local_Iteration<Local_Max-1;Local_Iteration++){
    for(Local_A=0;Local_A<Local_Max-1-Local_Iteration;Local_A++){
      if(!Local_Unknowns[Local_Iteration][Local_A]){
	if(!Local_Unknowns[Local_Iteration][Local_A+1]){
	  Local_Examples[Local_Iteration+1][Local_A]=Local_Examples[Local_Iteration][Local_A+1]-Local_Examples[Local_Iteration][Local_A];
	  Local_Unknowns[Local_Iteration+1][Local_A]=Local_Unknowns[Local_Iteration][Local_A+1]-Local_Unknowns[Local_Iteration][Local_A];
	}
	else
	  Local_Unknowns[Local_Iteration+1][Local_A]=1;
      }
      else
	Local_Unknowns[Local_Iteration+1][Local_A]=1;
    }
    Local_B=0;
    for(Local_A=0;Local_A<Local_Max-Local_Iteration;Local_A++)
      if(!Local_Unknowns[Local_Iteration][Local_A])
        Local_B=1;
    if(Local_B==0)
      return 0;
    Local_B=0;
    for(Local_A=0;Local_A<Local_Max-Local_Iteration;Local_A++){
      if(Local_Unknowns[Local_Iteration][Local_A]){}
      else{
	Local_B=Local_Examples[Local_Iteration][Local_A];
	Local_A=Local_Max;
      }
    }
    Local_C=0;
    for(Local_A=0;Local_A<Local_Max-Local_Iteration;Local_A++)
      if(!Local_Unknowns[Local_Iteration][Local_A])
	if(Local_Examples[Local_Iteration][Local_A]!=Local_B){
	  Local_A=Local_Max;
	  Local_C=1;
	}
    if(Local_C==1){}
    else{
      Local_Found_Sol=1;
      for(Local_A=0;Local_A<Local_Max-Local_Iteration;Local_A++)
	Local_Examples[Local_Iteration][Local_A]=Local_B;
      for(Local_A=Local_Iteration-1;Local_A>-1;Local_A--){
	for(Local_B=0;Local_B<Local_Max-Local_A;Local_B++)
	  if(!Local_Unknowns[Local_A][Local_B]){
	    Local_D=Local_B;
	    Local_B=Local_Max;  
	  }
	for(Local_B=Local_D-1;Local_B>-1;Local_B--)
	  Local_Examples[Local_A][Local_B]=Local_Examples[Local_A][Local_B+1]-Local_Examples[Local_A+1][Local_B];
	for(Local_B=Local_D;Local_B<Local_Max-1-Local_A;Local_B++){
	  if(Local_Unknowns[Local_A][Local_B+1])
	    Local_Examples[Local_A][Local_B+1]=Local_Examples[Local_A][Local_B]+Local_Examples[Local_A+1][Local_B];
	  else
	    if(Local_Examples[Local_A][Local_B+1]!=Local_Examples[Local_A][Local_B]+Local_Examples[Local_A+1][Local_B])
	      Local_Found_Sol=0;
	}
      }
      if(Local_Found_Sol){
	if(Rotax_Poly_Dim<Local_Iteration+1)
	  Rotax_Poly_Dim=Local_Iteration+1;
	Rotax_Polynomial[Local_Iteration]=Local_Examples[Local_Iteration][0]/Rotax_Factorial(Local_Iteration);
	if(Rotax_Polynomial[Local_Iteration])
	  return 1;
	else
	  return 0;
      }
    }
  }
  return 0;
}

int Rotax_Factorial(int Local_N){
  int Local_A,Local_R=1;
  for(Local_A=1;Local_A<Local_N+1;Local_A++)
    Local_R*=Local_A;
  return Local_R;
}

void Rotax_List_Numbers(int Local_N[1024],int Local_Unknown[1024],int Local_M){
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

int Rotax_Exe_Polynomial(int Local_C){
  int Local_A,Local_R=0,C;
  for(Local_A=0;Local_A<Rotax_Poly_Dim;Local_A++){
    C=Power(Local_C,Local_A);
    Local_R+=Rotax_Polynomial[Local_A]*C;
  }
  return Local_R;
}

int Start_Rotax(int Local_F){
  int Local_A,Local_B,Local_C,Local_D,Local_Shadow[1024];
  for(Local_A=0;Local_A<1024;Local_A++)
    Rotax_Polynomial[Local_A]=0;
  Rotax_Poly_Dim=0;
  for(Local_A=0;Local_A<1024;Local_A++)
    Local_Shadow[Local_A]=Rotax_Examples[Local_A];
  while(Rotax_Neural()){
    for(Local_A=0;Local_A<1024;Local_A++)
      Rotax_Examples[Local_A]=Local_Shadow[Local_A];
    for(Local_A=0;Local_A<Rotax_Max-2;Local_A++)
      if(!Rotax_Unknowns[Local_A])
	    Rotax_Examples[Local_A]-=Rotax_Exe_Polynomial(Local_A);
    Local_B=0;
    for(Local_A=0;Local_A<Rotax_Max-2;Local_A++)
      if(!Rotax_Unknowns[Local_A])
	if(Rotax_Examples[Local_A]!=0)
          Local_B=1;
    if(Local_B==0){
      for(Local_A=0;Local_A<Rotax_Max-2;Local_A++){
	Rotax_Examples[Local_A]=Rotax_Exe_Polynomial(Local_A);
	Rotax_Unknowns[Local_A]=0;
      }
      if(Local_F==1){
	printf("\nSolution set = ");
	Rotax_List_Numbers(Rotax_Examples,Rotax_Unknowns,Rotax_Max-2);
	puts("");
      }
      return 1;
    }
  }
  return 0;
}

// *** recursion.h
void R_Filter_Create(){
  int A,Found=0,LRI_1,LRI_2,LRS_1,LRS_2;
  R_Filter_Coefficient=1,R_Filter_Constant=0;
  for(A=0;A<R_Filter_Dim;A++)
    if((R_Filter_Inp[1][A]==0)&(R_Filter_Shifted[1][A]==0)){
      if(Found==0)
	LRI_1=R_Filter_Inp[0][A],Found=1,LRS_1=R_Filter_Shifted[0][A];
      if((Found==1)&(LRI_1!=R_Filter_Inp[0][A])){
	LRI_2=R_Filter_Inp[0][A],Found=2,LRS_2=R_Filter_Shifted[0][A];
	A=R_Filter_Dim;
      }
    }
  if((Found==2)&(LRI_1!=LRI_2))
    R_Filter_Coefficient=(LRS_1-LRS_2)/(LRI_1-LRI_2),
      R_Filter_Constant=LRS_1-R_Filter_Coefficient*LRI_1;
}

int R_Filter_Test(){
  int A;
  for(A=0;A<R_Filter_Dim;A++)
    if(((R_Filter_Shifted[1][A]!=1)&(R_Filter_Inp[1][A]!=1))&((R_Filter_Coefficient*R_Filter_Inp[0][A]+R_Filter_Constant)!=R_Filter_Shifted[0][A]))
      return 0;
  return 1;
}

void R_Filter_Shift(int L_Shift){
  int A,B=R_Filter_Dim-L_Shift;
  for(A=0;A<B;A++){
    R_Filter_Shifted[0][A]=R_Filter_Inp[0][A+L_Shift];
    R_Filter_Shifted[1][A]=R_Filter_Inp[1][A+L_Shift];
  }
  for(;A<1024;A++){
    R_Filter_Shifted[0][A]=0;
    R_Filter_Shifted[1][A]=1;
  }
}

int R_Filter(int LData[2][1024],int LDim){
  int A;
  R_Filter_Success=0;
  for(R_Filter_Dim=0;R_Filter_Dim<LDim;R_Filter_Dim++)
    R_Filter_Inp[0][R_Filter_Dim]=LData[0][R_Filter_Dim],
      R_Filter_Inp[1][R_Filter_Dim]=LData[1][R_Filter_Dim];
  for(A=1;A<R_Filter_Dim;A++){
    R_Filter_Shift(A);
    R_Filter_Create();
    if(R_Filter_Test()==1){
      R_Filter_Order=A;
      if(R_Filter_Order<LDim)
	R_Filter_Success=!((R_Filter_Coefficient==0)&(R_Filter_Constant==0));
      if(R_Filter_Success==1){
	if(A<(R_Filter_Dim/2))
	  return 1;
	else
	  R_Filter_Success=0;
      }
    }
  }
  return 0;
}

void R_Push_FIFO(float L_Data){
  int LA;
  for(LA=0;LA<R_Neural_Order+1;LA++)
    R_Solution_Set[LA]=R_Solution_Set[LA+1];
  R_Solution_Set[R_Neural_Order]=L_Data;
}

float R_Eval_FIFO(){
  float L_R=0;
  int LA;
  for(LA=0;LA<R_Neural_Order+1;LA++)
    L_R+=R_Solution_Set[R_Neural_Order-LA]*R_Neural[LA];
  return L_R;
}

void R_TPI_AA(){
  int LA,LB;
  for(LA=0;LA<1024;LA++)
    R_Solution_Set[LA]=0;
  if(R_Filter_Success)
    R_Neural_Order=R_Filter_Order+1;
  else
    R_Neural_Order=R_Order();
  R_Solution_Set[0]=REC_Start;
  for(LA=1;LA<R_Neural_Order+1;LA++)
    for(LB=1;LB<LA+1;LB++)
      R_Solution_Set[LA]+=R_Neural[LB-1]*R_Solution_Set[LA-LB];
}

int R_TPI_RA(int L_TPI){
  int C,LQuanta,Local_TPI=R_Neural_Order+1,Local_Quanta_Freq;
  float Local_Result=0;
  if(L_TPI<Local_TPI-2){                             // Modded
    C=R_Solution_Set[L_TPI];
    return C;
  }
  else{
    if(R_Filter_Success){
      Local_Result=R_Solution_Set[L_TPI%R_Filter_Order],
	Local_Quanta_Freq=L_TPI/R_Filter_Order;
      for(LQuanta=0;LQuanta<Local_Quanta_Freq;LQuanta++){
	Local_Result*=R_Filter_Coefficient;
	Local_Result+=R_Filter_Constant;
      }
      C=(Local_Result+0.5);
      return C;
    }
    else{
      for(Local_TPI;Local_TPI<L_TPI+1;Local_TPI++)
	R_Push_FIFO(R_Eval_FIFO());
      C=(R_Solution_Set[R_Neural_Order]+0.5);
      return C;
    }
  }
}

int R_Order(){
  int Local_A,Local_R=0;
  for(Local_A=0;Local_A<1024;Local_A++)
    if(R_Neural[Local_A]!=0)
      Local_R=Local_A;
  return Local_R;
}

void R_Deltic(int LData[2][1024],int LDim){
  int i,N;
  R_Filter(LData,LDim);
  REC_Start=LData[0][0];
  for(N=0;N<1024;N++)
    R_Neural[N]=0;
  for(N=1;N<LDim;N++){
    R_Neural[N-1]=LData[0][N];
    for(i=0;i<N-1;i++)
      R_Neural[N-1]-=(R_Neural[i]*LData[0][N-1-i]);
    R_Neural[N-1]/=LData[0][0];
  }
}

int R_Successful(int Local_Dim){
  int L,L1=0;
  for(L=0;L<1024;L++)
    if(R_Neural[L]!=0)
      L1=L;
  if(Local_Dim-2>L1)
    return 1;
  return 0;
}
// *** control.h
int Defines(char *Name){
  int A,B,C;
  char Source_Code[1024],Number[100];
  Margin();
  if(Method==0){
    snprintf(Source_Code,1024,":MODEL :POLY :INSIDE %d :BY",Poly_Dim);
    for(A=0;A<Poly_Dim;A++){
      snprintf(Number,20," %d",Polynomial[A]);
      strcat(Source_Code,Number);
    }
  }
  if(Method==1){
    snprintf(Source_Code,1024,":MODEL :RECURSION :INSIDE %d :BY",R_Start);
    for(A=0;(A<1024)&(Recursive[A]!=0);A++){
      C=Recursive[A];
      if(Recursive[A]==C)
	snprintf(Number,20," %d",C);
      else
	snprintf(Number,20," %f",Recursive[A]);
      strcat(Source_Code,Number);
    }
    if(Rec_BI_Success){
      snprintf(Number,50," :BEHAVIOR %d",Rec_BI_Order);
      strcat(Source_Code,Number);
      C=Rec_BI_Coefficient;
      if(Rec_BI_Coefficient==C)
	snprintf(Number,20," %d",C);
      else
	snprintf(Number,20," %f",Rec_BI_Coefficient);
      strcat(Source_Code,Number);
      C=Rec_BI_Constant;
      if(Rec_BI_Constant==C)
	snprintf(Number,20," %d",C);
      else
	snprintf(Number,20," %f",Rec_BI_Constant);
      strcat(Source_Code,Number);
    }
  }
  if(Method==2){
    snprintf(Source_Code,1024,":MODEL :LOGIC :INSIDE %d %d :BY",Neural_Borders[0],Neural_Borders[1]-1);
    for(B=0;B<Neural_Borders[1]-1;B++)
      for(A=0;A<Neural_Borders[0];A++){
	snprintf(Number,20," %d",ENiX_Neural[A][B]);
	strcat(Source_Code,Number);
      }
  }
  if(Method==3){
    snprintf(Source_Code,1024,":MODEL :LEGACY :INSIDE %d :BY",Legacy_Size);
    for(A=0;A<Legacy_Size;A++){
      snprintf(Number,99," %d %d %d %d",Legacy_Neural[0][A],Legacy_Neural[1][A],Legacy_Neural[2][A],Legacy_Neural[3][A]);
      strcat(Source_Code,Number);
    }
    snprintf(Number,5," %d",Legacy_Band);
    strcat(Source_Code,Number);
  }
  strcat(Source_Code," :NULL");
  PrintL(Source_Code);
  File_Neural(Source_Code,Name);
  Margin();
  PrintS("Defined as:");
  PrintS(Name);
  PrintL("");
  return 0;
}

int Behavior(){
  int A;
  if(Method==1)    A=Extract_Floats();
  else             A=Extract_Numbers();
  if(Method==0){}
  if(Method==1){
    Rec_BI_Order=Floats[0][0];
    Rec_BI_Coefficient=Floats[0][1];
    Rec_BI_Constant=Floats[0][2];
    Rec_BI_Success=1;
  }
  if(Method==2){}
  return A;
}

int By(int Override){
  int B,C,A;
  if(!Override){
    if(Method==1)
      A=Extract_Floats();
    else
      A=Extract_Numbers();
  }
  else
    A=Numeric_Term;
  if(A>0){
    if(Method==0){
      for(B=0;B<1024;B++)
	if(B<Numeric_Term)
	  Polynomial[B]=Numeric[0][B];
	else
	  Polynomial[B]=0;
      Success=1;
    }
    if(Method==1){
      for(B=0;B<1024;B++)
	if(B<Numeric_Term)
	  Recursive[B]=Floats[0][B];
	else
	  Recursive[B]=0;
      Success=1;
    }
    if(Method==2){
      if(A==Neural_Borders[0]*(Neural_Borders[1]-1)){
	Margin();
	PrintS("Data seems ok; loading matrix...");
	for(B=0;B<Neural_Borders[0];B++)
	  for(C=0;C<Neural_Borders[1]-1;C++)
	    ENiX_Neural[C][B]=Numeric[0][B+Neural_Borders[0]*C];
	PrintL("Done.");
	Success=1;
      }
      else{
	Margin();
	PrintL("Error: incompatible dimensions.");
	return 0;
      }    
    }
    if(Method==3){
      for(B=0;B<4;B++)
	for(C=0;C<Legacy_Size;C++)
	  Legacy_Neural[B][C]=Numeric[0][B+(C<<2)];
      Legacy_Band=Numeric[0][C<<2];
      Success=1;
    }
    return A;
  }
  return 0;
}

int Inside(int _[2][1024],int __){
  int Vertical,Horizontal;
  if(__>2){
    Margin();
    PrintL("ENiX doesn't support N-Dimensional methods where N is 3 or more.");
    return 0;
  }
  if(__<1){
    Margin();
    PrintL("ENiX needs at least 1 Dimension parameter.");
    return 0;
  }
  if(Method<2){
    if(__>1){
      Margin();
      PrintL("Polynomial and recursive systems should be 1 dimensional.");
      return 0;
    }
    if(Method==0)
      Poly_Dim=_[0][0];
    if(Method==1)
      R_Start=_[0][0];
  }
  else{
    if((__!=2)&&(Method==2)){
      Margin();
      PrintL("You must specify 2D boundaries for this problem.");
      return 0;
    }
    if(Method==2){
      Neural_Borders[0]=_[0][0];
      Neural_Borders[1]=_[0][1]+1;
      for(Vertical=0;Vertical<10;Vertical++)
	for(Horizontal=0;Horizontal<32;Horizontal++)
	  if((Vertical>Neural_Borders[0]-1)|(Horizontal>Neural_Borders[1]-2))
	    ENiX_Neural[Vertical][Horizontal]=-1;
    }
    else{
      Legacy_Size=_[0][0];
      for(Vertical=0;Vertical<4;Vertical++)
	for(Horizontal=0;Horizontal<1024;Horizontal++)
	  Legacy_Neural[Vertical][Horizontal]=0;
    }
  }
  Margin();
  PrintL("Model boundaries set.");
  return 1;
}

int Model(char _[32]){
  Margin();
  if(strncasecmp(_,":POLY",5)==0){
    PrintL("Environmental Control set to polynomial mode.");
    return 0;
  }
  if(strncasecmp(_,":RECURSION",10)==0){
    PrintL("Environmental Control set to recursion mode.");
    return 1;
  }
  if(strncasecmp(_,":LOGIC",6)==0){
    PrintL("Environmental Control set to second generation ENiX kernel mode.");
    return 2;
  }
  if(strncasecmp(_,":LEGACY",7)==0){
    PrintL("Environmental Control set to first generation ENiX kernel [Legacy] mode.");
    return 3;
  }
  PrintL("No such mode of operation.");
  return -1;
}

int Mtd(char *Filename){
  int A,B,C,Size=0,Inp[2][1024];
  char Path[1024];
  Margin();
  PrintS("Loading method in current scope...");
  snprintf(Path,1024,"./ENiX.DATA/%s.METHOD",Filename);
  if(File_Exists(Path)){
    for(A=0;A<1024;A++)
      Inp[0][A]=0,
	Inp[1][A]=0,
	Floats[0][A]=0,
	Floats[1][A]=0;
    PrintS("File Exists...");
    Size=File_Dimension(Path);
    for(A=0;A<Size;A++){
      File_Extract_Word(Path,A);
      if(strncasecmp(":MODEL",File_Word,6)==0){
	PrintL("Loaded.");
	File_Extract_Word(Path,++A);
	Method=Model(File_Word);
      }
      if(strncasecmp(":INSIDE",File_Word,7)==0){
	B=0;
	File_Extract_Word(Path,++A);
	Inp[0][0]=atoi(File_Word);
	B++;
	if(Method>1){
	  File_Extract_Word(Path,++A);
	  Inp[0][1]=atoi(File_Word);
	  B++;
	}
	Inside(Inp,B);
      }
      if(strncasecmp(":BY",File_Word,3)==0){
	if(Method==0){
	  for(B=0;B<Poly_Dim;B++){
	    File_Extract_Word(Path,B+A+1);
	    Numeric[0][B]=atoi(File_Word);
	  }
	  C=Poly_Dim;
	  Numeric_Term=Poly_Dim;
	}
	if(Method==1){
	  C=0;
	  for(B=0;B<1024-A-1;B++){
	    File_Extract_Word(Path,B+A+1);
	    if(File_Word[0]==':')
	      B=1024;
	    else{
	      Floats[0][B]=atof(File_Word);
	      C++;
	    }
	  }
	}
	if(Method==2){
	  C=0;
	  for(B=0;B<1024-A-1;B++){
	    File_Extract_Word(Path,B+A+1);
	    if(File_Word[0]==':')
	      B=1024;
	    else{
	      Numeric[0][B]=atoi(File_Word);
	      C++;
	    }
	  }
	}
	if(Method==3){
	  C=0;
	  for(B=0;B<1024-A-1;B++){
	    File_Extract_Word(Path,B+A+1);
	    if(File_Word[0]==':')
	      B=1024;
	    else{
	      Numeric[0][B]=atoi(File_Word);
	      C++;
	    }
	  }
	}
	Numeric_Term=C;
	A+=Numeric_Term;
	By(1);
      }
    }
    return 1;
  }
  else{
    PrintS("What is ");
    PrintS(Filename);
    PrintL(" ?");
    return 0;
  }
}

void Par(char *Filename){
  int A,Size=1;
  char OS[1024],Path[1024];
  snprintf(Path,1024,"./ENiX.DATA/%s.METHOD",Filename);
  if(File_Exists(Path)){
    for(A=0;A<1024;A++)
      OS[A]=0;
    strcpy(OS,"./ENiX_II ");
    while(File_Extract_Word(Path,Size)!=-1){
      strcat(OS,File_Word);
      strcat(OS," ");
      Size++;
    }
    strcat(OS,"> /dev/null &");
    Margin();
    PrintS("ENiX II is now spawning: ");
    PrintL(OS);
    system(OS);
  }
  else{
    Margin();
    PrintL("I have been unable to access this file.");
  }
}

void Seq(char *Filename){
  int A,Size=1;
  char OS[1024],Path[1024];
  snprintf(Path,1024,"./ENiX.DATA/%s.METHOD",Filename);
  if(File_Exists(Path)){
    for(A=0;A<1024;A++)
      OS[A]=0;
     strcpy(OS,"./ENiX_II ");
    while(File_Extract_Word(Path,Size)!=-1){
      strcat(OS,File_Word);
      strcat(OS," ");
      Size++;
    }
    Margin();
    PrintS("ENiX II is now spawning: ");
    PrintL(OS);
    system(OS);
  }
  else{
    Margin();
    PrintL("I have been unable to access this file.");
  }
}

void Procreator(int Force){ // uses lower-level ai to build higher-level concepts
  int L_Lang,L_AX,REG1,REG2;
  L_Lang=Output_Language;
  Output_Language=2;
  L_AX=Logic();
  Output_Language=L_Lang;
  if(L_AX<1|(Force>0)){
    Margin();
    Setup(Numeric,Numeric_Term);
    Build_Self_Awareness();
    if(Force==2){
      Data_Super_Charger();
      for(REG1=0;REG1<1024;REG1++)
	for(REG2=0;REG2<4;REG2++)
	  Legacy_Neural[REG2][REG1]=Legacy[REG2][REG1];
      Legacy_Size=Legacy_Dim;
      Complexity=Complexity;
      Legacy_Band=Legacy_I_Band;
    }
    else
      Build_Neural();
    Success=0;
    Neural_Borders[0]=Dimension_Vector[1]-1;
    Neural_Borders[1]=TPI_Band_Width;
    for(REG1=0;REG1<10;REG1++)
      for(REG2=0;REG2<32;REG2++)
	ENiX_Neural[REG1][REG2]=Neural_SA[REG1][REG2];
    if(Final_Test()|(Force>0)){
      Success=1;
      Method=2;
      PrintS("Concept understood. Time taken: less than ");
      PrintN(Time_Taken+1);
      PrintL(" seconds.");
    }
    else
      PrintL("This technology [:ENiX] is not suitable for understanding this data.");
    if(Force==2)
      Method=3;
  }
  if((L_AX==1)&(Force==0))
    Rec();
  if((L_AX==2)&(Force==0))
    Poly();
  if((L_AX==3)&(Force==0))
    Procreator(2);
}


int Logic(){ // r=0 iff inconcl  r=-1 iff logical  r=1 iff recur  r=2 iff poly  r=3 iff legacy
  int L,L_Lang;
  L_Lang=Output_Language;
  L=Setup(Numeric,Numeric_Term);
  if(L==1){
    Margin();
    PrintL("This problem is logically modelled.");
    Method=2;
    return -1;
  }
  else{
    Output_Language=2;
    Poly();
    if(Success){
      Output_Language=L_Lang;
      Margin();
      PrintL("This problem is polynomial modelled.");
      Method=0;
      return 2;
    }
    Rec();
    Output_Language=L_Lang;
    if(Success){
      Margin();
      PrintL("This problem is recursively modelled.");
      Method=1;
      return 1;
    }
    else{
      if(L==4){
	Margin();
	PrintL("This problem might be logically modelled. No promises.");
	return 3;
      }
      else{
	Margin();
	PrintL("I can find no evidence to suggest this problem *ISN'T* logical.");
	return 0;
      }
    }
  }
  return 3;
}

int TPIs(){
  int Local_A,Local_B;
  if(Method==0){
    for(Local_B=0;Local_B<1024;Local_B++)
      Rotax_Polynomial[Local_B]=Polynomial[Local_B];
    Rotax_Poly_Dim=1024;
  }
  if(Method==1){
    R_Filter_Success=Rec_BI_Success;
    R_Filter_Order=Rec_BI_Order;
    R_Filter_Coefficient=Rec_BI_Coefficient;
    R_Filter_Constant=Rec_BI_Constant;
    REC_Start=R_Start;
    for(Local_B=0;Local_B<1024;Local_B++)
      R_Neural[Local_B]=Recursive[Local_B];
  }
  if(Method==2){
    for(Local_A=0;Local_A<10;Local_A++)
      for(Local_B=0;Local_B<32;Local_B++)
	Neural_SA[Local_A][Local_B]=ENiX_Neural[Local_A][Local_B];
    Dimension_Vector[1]=Neural_Borders[0]+1;
    TPI_Band_Width=Neural_Borders[1];
  }
  if(Method==3){
    for(Local_A=0;Local_A<1024;Local_A++)
      for(Local_B=0;Local_B<4;Local_B++)
	Legacy[Local_B][Local_A]=Legacy_Neural[Local_B][Local_A];
    Legacy_Dim=Legacy_Size;
    Legacy_I_Band=Legacy_Band;
  }
  if(Success==0){
    Margin();
    PrintL("Warning: the algorithm failed, so if these answers are accurate it is only by coincidence.");
  }
  Margin();
  PrintS("The respective solutions are: [");
  for(Local_A=0;Local_A<Numeric_Term;Local_A++){
    if(Method==0){
      PrintN(Rotax_Exe_Polynomial(Numeric[0][Local_A]));
    }
    if(Method==1){
      R_TPI_AA();
      PrintN(R_TPI_RA(Numeric[0][Local_A]));
    }
    if(Method==2)
      PrintN(Calc_TPI(Numeric[0][Local_A]));
    if(Method==3)
      PrintN(Execute_Legacy(Numeric[0][Local_A]));
    if(Local_A<Numeric_Term-1)
      PrintS(", ");
  }
  PrintL("]");
  return 0;
}

int Rec(){
  Success=0;
  Method=1;
  R_Deltic(Numeric,Numeric_Term);
  Margin();
  for(R_Start=0;R_Start<1024;R_Start++)
    Recursive[R_Start]=R_Neural[R_Start];
  R_Start=REC_Start;
  Rec_BI_Success=0;                             // fixed bug in self-awareness
  if(R_Filter_Success){
    Rec_BI_Success=R_Filter_Success;
    Rec_BI_Order=R_Filter_Order;
    Rec_BI_Coefficient=R_Filter_Coefficient;
    Rec_BI_Constant=R_Filter_Constant;
    Success=1;
    PrintL("Concept understood.");
    return 0;
  }
  if(R_Successful(Numeric_Term)){
    Success=1;
    PrintL("Concept understood.");
  }
  else
    PrintL("Insufficient data; adopted method memorization only.");
  return 0;
}

void Parse(){
  int Local_Recognised,Local_Time_Shift,Local_Loop;
  for(TPI=0;TPI<IExamples-1;TPI++){
    if(Example_Category[TPI+1]==2){
      Local_Recognised=0;
      Local_Time_Shift=0;
      PrintTPI(TPI);
      if(Command_Present(":BEHAVIOR",9,TPI)){
	PrintS(":BEHAVIOR ");
	Local_Recognised=1;
	Local_Time_Shift=Behavior();
      }
      if(Command_Present(":BINARY",7,TPI)){
	PrintS(":BINARY ");
	Local_Recognised=1;
	Local_Time_Shift=Extract_Numbers();
	Binary(Numeric,Numeric_Term);
      }
      if(Command_Present(":BY",3,TPI)){
	PrintS(":BY");
	Local_Recognised=1;
	Local_Time_Shift=By(0);
      }
      if(Command_Present(":DEPENDS",8,TPI)){
	PrintL(":DEPENDS ");
	Local_Recognised=1;
	if((Method==2)||(Method==3))
	  PrintDep(Neural_Borders[0],Neural_Borders[1],ENiX_Neural);
	else{
	  Margin();
	  PrintL("Inappropriate context.");
	}
      }
      if(Command_Present(":DEFINES",8,TPI)){
	PrintS(":DEFINES ");
	PrintS(Example[TPI+2]);
	PrintL("");
	Defines(Example[TPI+2]);
	Local_Recognised=1;
	Local_Time_Shift=1;
      }
      if(Command_Present(":HELP",5,TPI)){
	PrintS(":HELP ");
	Local_Recognised=1;
	if(TPI<IExamples-2){
	  PrintL(Example[TPI+2]);
	  Help(Example[TPI+2]);
	}
	else{
	  PrintL(":NULL");
	  Help(":NULL");
	}
	Local_Time_Shift=1;
      }
      if(Command_Present(":FORCE",6,TPI)){
	PrintS(":FORCE");
	Local_Recognised=1;
	Local_Time_Shift=Extract_Numbers();
	Procreator(1);
      }
      if(Command_Present(":HTML",5,TPI)){
	PrintL(":HTML");
	Local_Recognised=1;
	Margin();
	PrintL("ENiX is reconfigured to speak HTML.");
	Output_Language=1;
      }
      if(Command_Present(":INSIDE",7,TPI)){
	PrintS(":INSIDE");
	Local_Recognised=1;
	Local_Time_Shift=Extract_Numbers();
	Inside(Numeric,Numeric_Term);

      }
      if(Command_Present(":LEGACY",7,TPI)){
	PrintS(":LEGACY");
	Local_Recognised=1;
	Local_Time_Shift=Extract_Numbers();
	Procreator(2);
      }
      if(Command_Present(":LOGIC",6,TPI)){
	PrintS(":LOGIC");
	Local_Recognised=1;
	Local_Time_Shift=Extract_Numbers();
	Procreator(0);
      }
      if(Command_Present(":METHOD",7,TPI)){
	PrintL(":METHOD");
	Local_Recognised=1;
	if(Method==0)
	  PrintPoly(Polynomial,Poly_Dim);
	if(Method==1){
	  PrintRec(Recursive,R_Start,Success);
	  PrintL("");
	  if(Rec_BI_Success==1){
	    Margin();
	    Rec_Filter(Rec_BI_Success,Rec_BI_Order,Rec_BI_Coefficient,Rec_BI_Constant);
	    PrintL("");
	  }
	}
	if(Method==2)
	  PrintSA(Neural_Borders[0],Neural_Borders[1],ENiX_Neural);
	if(Method==3)
	  PrintLegacy(Legacy_Neural,Legacy_Size,Legacy_Band);
      }
      if(Command_Present(":MODEL",6,TPI)){
	PrintS(":MODEL ");
	PrintL(Example[TPI+2]);
	Method=Model(Example[TPI+2]);
	Local_Time_Shift=1;
	Local_Recognised=1;
      }
      if(Command_Present(":MTD",4,TPI)){
	PrintS(":MTD ");
	PrintL(Example[TPI+2]);
	Mtd(Example[TPI+2]);
	Local_Time_Shift=1;
	Local_Recognised=1;
      }
      if(Command_Present(":NORM",5,TPI)){
	PrintL(":NORM");
	Local_Recognised=1;
	Margin();
	if(Output_Language==2)
	  PrintS("ENiX is uncloaked; ");
	PrintL("ENiX is reconfigured to console mode.");
	Output_Language=0;
      }
      if(Command_Present(":NULL",5,TPI)){
	Local_Recognised=1;
	PrintS(":NULL");
	PrintL("");
      }
      if(Command_Present(":PAR",4,TPI)){
	PrintS(":PAR ");
	PrintL(Example[TPI+2]);
	Par(Example[TPI+2]);
	Local_Time_Shift=1;
	Local_Recognised=1;
      }
      if(Command_Present(":POLY",5,TPI)){
	PrintS(":POLY");
	Local_Recognised=1;
	Local_Time_Shift=Extract_Numbers();
	if(Numeric_Term>256){
	  Margin();
	  PrintL("It is not permitted to operate this algorithm on more than 256 numbers.");
	}
	else
	  Poly();
      }
      if(Command_Present(":RECURSION",10,TPI)){
	PrintS(":RECURSION");
	Local_Recognised=1;
	Local_Time_Shift=Extract_Numbers();
	Rec();
      }
      if(Command_Present(":CLOAK",6,TPI)){
	PrintL(":CLOAK");
	Local_Recognised=1;
	Margin();
	PrintL("ENiX has been cloaked.");
	Output_Language=2;
      }
      if(Command_Present(":SAY",4,TPI)){
	PrintS(":SAY ");
	PrintL("");
	Margin();
	Output_Language=3;
	PrintL("Voice synthesis online.");
	Local_Recognised=1;
      }
      if(Command_Present(":SEQ",4,TPI)){
	PrintS(":SEQ ");
	PrintL(Example[TPI+2]);
	Seq(Example[TPI+2]);
	Local_Time_Shift=1;
	Local_Recognised=1;
      }
      if(Command_Present(":SOLS",5,TPI)){
	PrintL(":SOLS");
	Margin();
	PrintNL(Solution_Set,Solution_Dim);
	PrintL("");
	Local_Recognised=1;
      }
      if(Command_Present(":TPI",4,TPI)){
	PrintS(":TPI");
	Local_Recognised=1;
	Local_Time_Shift=Extract_Numbers();
	TPIs();
      }
      if(Command_Present(":TYPE",5,TPI)){
	PrintS(":TYPE");
	Local_Recognised=1;
	Local_Time_Shift=Extract_Numbers();
	Logic();
      }
      if(Command_Present(":VERSION",8,TPI)){
	PrintS(":VERSION");
	Local_Recognised=1;
	Version();
      }
      if(!Local_Recognised){
	PrintL(Example[TPI+1]);
	Margin();
	PrintL("Unrecognised behavioral inhibitor.");
      }
      TPI+=Local_Time_Shift;
    }
  }
}

void Classify(){
  int Local_A;
  for(Local_A=0;Local_A<IExamples-1;Local_A++)
    Example_Category[Local_A+1]=Example_Type(Local_A+1);
}

void Go(){
  char Local_System[100];
  if(strncmp(Example[0],"./ENiX_II",9)==0){
  
    Classify();
    Parse();
  
  }
  else{
    system("clear");
    PrintL(" WARNING: You are not running enix2 from it's default name - autocorrecting.");
    strcpy(Local_System,"mv ");
    strcat(Local_System,Example[0]);
    strcat(Local_System," ./ENiX_II");
    system(Local_System);
    system("rm *~");
    system("cp ./ENiX_II /Library/Apache2/htdocs/Data/enix2");
    PrintL(" WARNING: Corrected; program renamed to ENiX_II.");
  }
}

void Control(){
  int Local_A;
  for(Local_A=0;Local_A<1024;Local_A++)
    Polynomial[Local_A]=0;
  Poly_Dim=0;
}

void Poly(){
  int Local_A;
  for(Local_A=0;Local_A<1024;Local_A++){
    Rotax_Unknowns[Local_A]=Numeric[1][Local_A];
    Rotax_Examples[Local_A]=Numeric[0][Local_A];
  }
  Rotax_Max=Numeric_Term+2;
  Success=Start_Rotax(0);
  if(Success){
    for(Local_A=0;Local_A<1024;Local_A++){
      Polynomial[Local_A]=Rotax_Polynomial[Local_A];
      Solution_Set[0][Local_A]=Rotax_Examples[Local_A];
      Solution_Set[1][Local_A]=Rotax_Unknowns[Local_A];
    }
    Solution_Dim=Rotax_Max-2;
    Poly_Dim=Rotax_Poly_Dim;
    Method=0;
    Margin();
    PrintL("Concept understood.");
  }
  else{
    Margin();
    PrintL("This technology (:POLY) is not suitable for understanding this data.");
  }
}

int Extract_Numbers(){
  int Local_Loop;
  Numeric_Term=0;
  for(Local_Loop=TPI+2;Local_Loop<IExamples;Local_Loop++){
    if(Example_Category[Local_Loop]==0){
      Numeric[0][Numeric_Term]=atoi(Example[Local_Loop]);
      Numeric[1][Numeric_Term]=0;
      Numeric_Term++;
    }
    else{
      if(Example_Category[Local_Loop]==1){
	Numeric[0][Numeric_Term]=atoi(Example[Local_Loop]);
	Numeric[1][Numeric_Term]=1;
	Numeric_Term++;
      }
      else
	Local_Loop=IExamples+1;
    }
  }
  PrintS(" ");
  PrintNL(Numeric,Numeric_Term);
  PrintL("");
  return Numeric_Term;
}

int Extract_Floats(){
  int Local_Loop;
  Numeric_Term=0;
  for(Local_Loop=TPI+2;Local_Loop<IExamples;Local_Loop++){
    if(Example_Category[Local_Loop]==0){
      Floats[0][Numeric_Term]=atof(Example[Local_Loop]);
      Floats[1][Numeric_Term]=0;
      Numeric_Term++;
    }
    else{
      if(Example_Category[Local_Loop]==1){
	Floats[0][Numeric_Term]=0;
	Floats[1][Numeric_Term]=1;
	Numeric_Term++;
      }
      else
	Local_Loop=IExamples+1;
    }
  }
  PrintL(" {...}");
  return Numeric_Term;
};

// *** enix.h
int Execute_Legacy(int Local_TPI){
   int Answer=0,LA,A,B;
   LOAD_BASIS();
   for(A=0;A<Legacy[3][Legacy_Dim-1]+1;A++){
      S_Cluster=0;
      for(B=0;B<Legacy_Dim;B++){
	if(Legacy[3][B]==A){
	   Super_Cluster[0][S_Cluster]=Legacy[0][B];
	   Super_Cluster[1][S_Cluster]=Legacy[1][B];
	   Super_Cluster[2][S_Cluster]=Legacy[2][B];
	   Super_Cluster[3][S_Cluster]=0;
	   S_Cluster++;
	}
      }
      I_Band=Legacy_I_Band;
      LA=Eval(Local_TPI);
      Answer+=LA<<A;
   }
   return Answer;
}

int Data_Super_Charger(){ // Enix data super charger type A. Experimental.
  int A,B,C,D,Known;
  int L_Examples[2][1024];
  int Converted[256];
  int L_Example_Dim;
  int Time=time(NULL);
  for(A=0;A<1024;A++)
    Legacy[0][A]=Legacy[1][A]=Legacy[2][A]=Legacy[3][A]=0;
  Legacy_Dim=0;
  ENiX_Complexity=0;
  for(A=0;A<Dimension_Vector[1]-1;A++){
    C=0;
    for(B=0;B<TPI_Band_Width-1;B++)
      if(Neural_SA[B][A]!=-1)
	C=B+1;
    L_Example_Dim=1<<C;
    for(B=0;B<1024;B++)
      L_Examples[0][B]=0,
	L_Examples[1][B]=0;
    for(B=0;B<L_Example_Dim;B++){
      Known=0;
      if(ENiX_In_Dat[1][B]==0)
	Known=1,
	  L_Examples[0][B]=(ENiX_In_Dat[0][B]>>A)&1;
      else
	for(D=0;D<ENiX_In_Max;D+=L_Example_Dim)
	  if(ENiX_In_Dat[1][D+B%L_Example_Dim]==0){
	    Known=1;
	    L_Examples[0][B]=(ENiX_In_Dat[0][D+B%L_Example_Dim]>>A)&1;
	    D=ENiX_In_Max;
	  }
      if(Known)
	L_Examples[1][B]=0;
      else
	L_Examples[1][B]=1,
	  L_Examples[0][B]=-1;
    }
    for(B=0;B<256;B++)
      Converted[B]=L_Examples[0][B];
    L_Example_Dim<256?RUN_ENiX(Converted,L_Example_Dim):RUN_ENiX(Converted,256);                              // 09.09.06
    for(B=0;(B<256)|(B<S_Cluster);B++)
      Legacy[0][B+Legacy_Dim]=Super_Cluster[0][B],
	Legacy[1][B+Legacy_Dim]=Super_Cluster[1][B],
	Legacy[2][B+Legacy_Dim]=Super_Cluster[2][B],
	Legacy[3][B+Legacy_Dim]=A;
    Legacy_Dim+=S_Cluster;
    ENiX_Complexity+=Minds;
  }
  Legacy_I_Band=I_Band;
  Time_Taken=time(NULL)-Time;
  return 0;
}

int Calc_TPI(int LTPI){
  int Horizontal,Vertical,Base=1,Gates,Answer=0,Register,Vertical_Start;
  for(Horizontal=0;Horizontal<32;Horizontal++){ // Add the numbers up.
    if(Neural_SA[0][Horizontal]==1)
      Answer+=Base;
    Base*=2;
  }
  Base=1;
  for(Horizontal=0;Horizontal<32;Horizontal++){
    Gates=0;
    for(Vertical=0;Vertical<TPI_Band_Width;Vertical++)
      if(Neural_SA[Vertical][Horizontal]>1){
	Neural_Input[Gates]=Vertical;
	Gates++;
      }
    if(Gates==1){
      if(Neural_SA[Neural_Input[0]][Horizontal]==2)
	Answer+=Base*((LTPI>>Neural_Input[0])&1);
      if(Neural_SA[Neural_Input[0]][Horizontal]==3)
	if(!((LTPI>>Neural_Input[0])&1))
	  Answer+=Base;
    }
    if(Gates>1){ // The bug is here
      for(Vertical=0;Vertical<1024;Vertical++)
	ENiX2_Neural[Vertical]=0;
      // First neuron is at position Neural_Input[1]
      ENiX2_Neural[0]=Neural_SA[Neural_Input[1]][Horizontal]-4;
      for(Vertical=1;Vertical<Gates-1;Vertical++)
	ENiX2_Neural[Vertical]=Neural_SA[Neural_Input[Vertical+1]][Horizontal]-4;
      Answer+=Base*Calc_Bit(Gates-1,LTPI);
    }
    Base*=2;
  }
  return Answer;
}

int Calc_Bit(int Max,int LTPI){
  int Vertical,Top_Side_Feed,New_Register,New_TPI_Port;
  Top_Side_Feed=(LTPI>>Neural_Input[0])&1;
  for(Vertical=0;Vertical<Max;Vertical++){
    New_TPI_Port=(LTPI>>Neural_Input[Vertical+1])&1;
    if(ENiX2_Neural[Vertical]==0)                                // OR
      New_Register=(Top_Side_Feed|New_TPI_Port);
    if(ENiX2_Neural[Vertical]==1)                                // NOR
      New_Register=!(Top_Side_Feed|New_TPI_Port);
    if(ENiX2_Neural[Vertical]==2)                                // AND
      New_Register=(Top_Side_Feed&New_TPI_Port);
    if(ENiX2_Neural[Vertical]==3)                                // NAND
      New_Register=!(Top_Side_Feed&New_TPI_Port);
    if(ENiX2_Neural[Vertical]==4)                                // XOR
      New_Register=(Top_Side_Feed^New_TPI_Port);
    if(ENiX2_Neural[Vertical]==5)                                // NXOR
      New_Register=!(Top_Side_Feed^New_TPI_Port);
    if(ENiX2_Neural[Vertical]==6)                                // N-A
      New_Register=(!Top_Side_Feed)&New_TPI_Port;
    if(ENiX2_Neural[Vertical]==7)                                // A-N
      New_Register=Top_Side_Feed&(!New_TPI_Port);
    if(ENiX2_Neural[Vertical]==8)                                // N-NA
      New_Register=!((!Top_Side_Feed)&New_TPI_Port);
    if(ENiX2_Neural[Vertical]==9)                                // NA-N
      New_Register=!(Top_Side_Feed&(!New_TPI_Port));
    Top_Side_Feed=New_Register;
  }
  return New_Register;
}

int Final_Test(){
  int Horizontal,Vertical;
  for(Horizontal=0;Horizontal<Dimension_Vector[1]-1;Horizontal++)
    for(Vertical=0;Vertical<TPI_Band_Width-1;Vertical++)
      if(Neural_SA[Vertical][Horizontal]>13)
	return 0;
  return 1;
}

void Compound(int Bit_Stream,int Stream_ENiX_Complexity){
  int Logic_Gate=0,Vertical;
  Neural_Output=Bit_Stream;
  Morph1(Stream_ENiX_Complexity-1,0);
  for(Vertical=0;Vertical<TPI_Band_Width-1;Vertical++)
    if(Neural_SA[Vertical][Bit_Stream]==1){
      Neural_SA[Vertical][Bit_Stream]=(Logic_Gate>0)?ENiX2_Neural[Logic_Gate-1]+4:2;                                // 10.09.06
      Logic_Gate++;
    }
}

int Build_Neural(){
  int Clear,ENiX_Complexity,Vertical,Stream;
  for(Stream=0;Stream<Dimension_Vector[1]-1;Stream++){
    for(Clear=0;Clear<1024;Clear++)
      Neural_Input[Clear]=0;
    ENiX_Complexity=0;
    for(Vertical=0;Vertical<TPI_Band_Width-1;Vertical++)
      if(Neural_SA[Vertical][Stream]==1){
	Neural_Input[ENiX_Complexity]=Vertical;
	ENiX_Complexity++;
      }
    if(ENiX_Complexity==0)
      Single_Digit(Stream);
    if(ENiX_Complexity==1)
      Single_Input(Stream);
    if(ENiX_Complexity>1)
      Compound(Stream,ENiX_Complexity);
  }
  return 0;
}

int Morph1(int Max,int Current){
  for(ENiX2_Neural[Current]=0;ENiX2_Neural[Current]<10;ENiX2_Neural[Current]++)   
    if(Current<Max){
      if(Morph1(Max,Current+1))
	return 1;
    }
    else
      if(Neural_Feedback(Max))
	return 1;
  return 0;
}

int Neural_Feedback(int Max){
  int LTPI,Vertical,Top_Side_Feed,New_Register,New_TPI_Port;
  for(LTPI=0;LTPI<ENiX_In_Max;LTPI++){
    if(ENiX_In_Dat[1][LTPI]==0){
      Top_Side_Feed=(LTPI>>Neural_Input[0])&1;
      for(Vertical=0;Vertical<Max;Vertical++){
	New_TPI_Port=(LTPI>>Neural_Input[Vertical+1])&1;
	if(ENiX2_Neural[Vertical]==0)                                // OR
	  New_Register=(Top_Side_Feed|New_TPI_Port);
	if(ENiX2_Neural[Vertical]==1)                                // NOR
	  New_Register=!(Top_Side_Feed|New_TPI_Port);
	if(ENiX2_Neural[Vertical]==2)                                // AND
	  New_Register=(Top_Side_Feed&New_TPI_Port);
	if(ENiX2_Neural[Vertical]==3)                                // NAND
	  New_Register=!(Top_Side_Feed&New_TPI_Port);
	if(ENiX2_Neural[Vertical]==4)                                // XOR
	  New_Register=(Top_Side_Feed^New_TPI_Port);
	if(ENiX2_Neural[Vertical]==5)                                // NXOR
	  New_Register=!(Top_Side_Feed^New_TPI_Port);
	if(ENiX2_Neural[Vertical]==6)                                // N-A
	  New_Register=(!Top_Side_Feed)&New_TPI_Port;
	if(ENiX2_Neural[Vertical]==7)                                // A-N
	  New_Register=Top_Side_Feed&(!New_TPI_Port);
	if(ENiX2_Neural[Vertical]==8)                                // N-NA
	  New_Register=!((!Top_Side_Feed)&New_TPI_Port);
	if(ENiX2_Neural[Vertical]==9)                                // NA-N
	  New_Register=!(Top_Side_Feed&(!New_TPI_Port));
	Top_Side_Feed=New_Register;
      }
      if((New_Register)!=((ENiX_In_Dat[0][LTPI]>>Neural_Output)&1))
	return 0;
    }    
  }
  return 1;
}

int Twisted_Pear(int LS1,int LS2,int LW,int LP){ // refer to Test_Category for comp
  int LA;
  for(LA=0;LA<LW;LA++)
    if(((ENiX_In_Dat[1][LA+LS1]==0)&&(ENiX_In_Dat[1][LA+LS2]==0))&&(((ENiX_In_Dat[0][LA+LS1]>>LP)&1)!=((ENiX_In_Dat[0][LA+LS2]>>LP)&1)))                                                                                                             // 10.09.06
      return 0;
  return 1;
}

int Build_Self_Awareness(){
  int LZ1,LZ2,LR1,LR2,LR3;
  for(LZ1=0;LZ1<10;LZ1++)
    for(LZ2=0;LZ2<32;LZ2++)
      Neural_SA[LZ1][LZ2]=-1;
  for(LZ1=0;LZ1<Dimension_Vector[1]-1;LZ1++){
    LR1=0;
    for(LZ2=Dimension_Vector[0]>>1;LZ2>0;LZ2=LZ2>>1){
      LR1++;
      LR2=Dimension_Vector[0]/LZ2;
      for(LR3=0;LR3<LR2;LR3+=2)
	if(Twisted_Pear(LR3*LZ2,(LR3+1)*LZ2,LZ2,LZ1)==0)
	  LR3=LR2+1,
	    Neural_SA[TPI_Band_Width-LR1-1][LZ1]=1;
    }
  }
  return 0;
}

int Factorise(int L_Wavelength){ // looking for evidence that wavelength contains factors not 2
  int L1=L_Wavelength,L2=L_Wavelength;
  for(;L1>0;L1/=2){
    if(L1*2!=L2)
      return 0;
    L2=L1;
  }
  return 1;
}

int Diagnose(){
  int LT1,LT2,L;
  for(L=0;L<Dimension_Vector[1]-1;L++)
    if(Recursion_Wavelengths[L]<(ENiX_In_Max/2))
      if(!Factorise(Recursion_Wavelengths[L]))
	return 4;
  LT1=1;
  for(L=0;L<Dimension_Vector[1]-1;L++)
    if(Self_Awareness[TPI_Band_Width-3][L]==0)
      LT1=0;
  if(LT1)
    return 1;
  LT1=0;
  for(L=0;L<Dimension_Vector[1]-1;L++)
    if(Self_Awareness[TPI_Band_Width-3][L]==0){
      if(L==Dimension_Vector[1]-2)
	LT1=1;
      else{
	LT1=2;
	L=Dimension_Vector[1];
      }
    }
  if(LT1==1)
    return 2; 
  return 3;
}

int Test_Phase_Shift(int L_Data[2][1024],int L_Lambda){
  int LA,L_Data_Shadowed[2][1024];
  for(LA=0;LA<1024;LA++)
    L_Data_Shadowed[0][LA]=0,
      L_Data_Shadowed[1][LA]=1;
  for(LA=0;LA<1024-L_Lambda;LA++)
    L_Data_Shadowed[0][LA]=L_Data[0][LA+L_Lambda],
      L_Data_Shadowed[1][LA]=L_Data[1][LA+L_Lambda];
  for(LA=0;LA<1024;LA++)
    if(((L_Data_Shadowed[1][LA]==0)&(L_Data[1][LA]==0))&(L_Data_Shadowed[0][LA]!=L_Data[0][LA]))                   // 10.09.06
      return 0;
  return 1;
}

int Test_Category(int L){
  int LA,LB=1,Local_Data[2][1024];
  for(LA=0;LA<1024;LA++){
    Local_Data[1][LA]=ENiX_In_Dat[1][LA];
    Local_Data[0][LA]=(ENiX_In_Dat[0][LA]>>L)&1;
  }
  for(LA=0;LA<TPI_Band_Width-2;LA++){
    LB*=2;
    Self_Awareness[LA][L]=Test_Phase_Shift(Local_Data,LB);
  }
  for(LA=0;LA<1024;LA++){
    LB=Test_Phase_Shift(Local_Data,LB);
    if(LB==1){
      Recursion_Wavelengths[L]=LA;
      return 0;
    }
  }
  return 0;
}

int Setup(int Local_D[2][1024],int Local_M){
  int LA,LB;
  for(LB=0;LB<32;LB++)
    for(LA=0;LA<32;LA++)
      Self_Awareness[LA][LB]=0;
  for(LB=0;LB<1024;LB++)
    Recursion_Wavelengths[LB]=0;
  for(ENiX_In_Max=0;ENiX_In_Max<Local_M;ENiX_In_Max++){
    ENiX_In_Dat[0][ENiX_In_Max]=Local_D[0][ENiX_In_Max];
    ENiX_In_Dat[1][ENiX_In_Max]=Local_D[1][ENiX_In_Max];
  }               // and the rest of this control sequence below for when the example space is extended...
  for(LA=Local_M;LA<1024;LA++){
    ENiX_In_Dat[0][LA]=0;
    ENiX_In_Dat[1][LA]=1;
  }
  Dimension_Vector[0]=0;
  Dimension_Vector[1]=0;
  TPI_Band_Width=0;
  TPI_ENiX_Bandwidth(ENiX_In_Max);
  Example_ENiX_Bandwidth();
  TPI_Band_Width=ENiX_Bandwidth(Dimension_Vector[0]);
  for(LB=0;LB<Dimension_Vector[1]-1;LB++)
    Test_Category(LB);
  return Diagnose();
}

int ENiX_Bandwidth(int L){
  int LA,LC=1;
  for(LA=1;LC<L+1;LA++)
    LC*=2;
  return LA-1;
}

void TPI_ENiX_Bandwidth(int L){
  int LA;
  for(LA=1;LA<L;LA=LA<<1);
  Dimension_Vector[0]=LA;
}

void Example_ENiX_Bandwidth(){
  int LA,LC=1,LB=0;
  for(LA=0;LA<1024;LA++)
    if((ENiX_In_Dat[1][LA]==0)&(ENiX_In_Dat[0][LA]>LB))
      LB=ENiX_In_Dat[0][LA];
  Dimension_Vector[1]=ENiX_Bandwidth(LB)+1;
}

void Single_Digit(int Bit_Stream){
  int _TPI;
  for(_TPI=0;_TPI<1024;_TPI++)
    if(ENiX_In_Dat[1][_TPI]==0){
      Neural_SA[0][Bit_Stream]=(ENiX_In_Dat[0][_TPI]>>Bit_Stream)&1;
      _TPI=1024;
    }
}

void Single_Input(int Bit_Stream){
  int Gate_Position,_TPI,Vertical;
  for(Vertical=0;Vertical<TPI_Band_Width-1;Vertical++)
    if(Neural_SA[Vertical][Bit_Stream]!=-1){
      Gate_Position=Vertical;
      Vertical=TPI_Band_Width;
    }
  for(_TPI=0;_TPI<1024;_TPI++)
    if(ENiX_In_Dat[1][_TPI]==0){
      Neural_SA[Gate_Position][Bit_Stream]=((ENiX_In_Dat[0][_TPI]>>Bit_Stream)&1)==((_TPI>>Gate_Position)&1)?2:3; // 10.09.06
      _TPI=1024;
    }
}

int Power(int A,int B){
   int C=1,D;
   for(D=0;D<B;D++) C*=A;
   return C;
}



int main(int argc,char *argv[]){
  int Local_A,Local_B=0;
  TPI=0;
  Input();
  Output();
  Detected_Bandwidth=System_Bandwidth();
  IExamples=argc;
  for(Local_A=0;Local_A<argc;Local_A++)
    strncpy(Example[Local_A],argv[Local_A],32);
  Output_Language=Command_Present(":HTML",5,TPI);
  //  ENiX.Splash(ENiX.Detected_Bandwidth);
  Out_Init();
  if(argc<1025){
    for(Local_A=0;Local_A<IExamples;Local_A++)
      if(strlen(argv[Local_A])>32){
	Local_B=1;
	Local_A=IExamples;
      }
    if(Local_B){
      Margin();
      PrintL("WARNING: Words longer than 32 characters have been reduced to 32 characters.");
    }
    Go();
  }
  else{
    Margin();
    PrintL("ERROR: More than 1024 entries (including program name) is not permitted.");
  }
  Out_Term();
  return 0;
}
