// con.h 
 
#ifndef CON_H 
#define CON_H 
 
const long E_FALSE = 0; 
const long E_TRUE = 1; 
 
const long gMAX_NE = 800; 
 
const float pi = 3.141592654; 
 
// constants for Ne_Classes.C 
const char gINFILE[] = "infile"; 
// const char gINFILE[] = "pike.data"; 
// const char gINFILE[] = "pike.2"; 
 
const long COND_POLY = 0; 
 
// number of monte carlo iterations 
const long gMAX_REPS = 1000; 
// number of times to repeat the whole experiment 
const long gNUM_EXP = 200; 
const long gSIM_NUM_LOCI = 100; 
//long gSIM_NUM_LOCI;  
const long gSIM_NUM_ALLELES = 2; 
// long gSIM_NUM_ALLELES; 
const long gSIM_NUM_SAMPLES = 3; 
const long gSIM_SAMPLE_SIZE[] = { 100, 100, 100, 100, 100, 100, 100, 100 }; 
const long gSIM_GEN[] = { 0, 4, 8, 12}; 
const long gSIM_No = 50; // first offspring population size 
// long gSIM_No; // first offspring population size 
const double gSIM_W = 1.2; // growth rate 
const long gFIND_GROW = E_FALSE; 
const long gBIG_START_NE = gSIM_No; 
 
#endif


