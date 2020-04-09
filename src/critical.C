// critical.C 
// a program for computing the critical values given a specific 
// sample size, number of loci, theta, % single steps, mean of larger steps 
 
#include <stdio.h> 
#include <iostream.h> 
#include <stdlib.h> 
#include <math.h> 
#include "CRandom.h" 
 
#define gMaxAlleles 500 
#define gNumDiv 25  
#define gTrials 10000 
#define gMaxSampleSize 2000 
int gSampleSize; 
int gNumLoci; 
double gOneStep; // fraction of the time a mutation is one step 
double gMeanStepSize; // mean of the geometric distribution 
double g4NeMu; // 4*Ne*mu : mu = 10^-4 , Ne = 1000 to 10000 
 
CRandom gRand; 
 
class ListEntry { 
public: 
  ListEntry(); 
  struct ListEntry *nxt; 
  long val; 
}; 
typedef class ListEntry ListEntry;          
 
ListEntry::ListEntry() 
{ 
  nxt = NULL; 
} 
 
void 
Sort(long *inArray, long inSize) 
{ 
  ListEntry *theListElements = new ListEntry[inSize]; 
  ListEntry *theFirst, *theTemp, *theLastTemp; 
  long i; 
  theFirst = NULL; 
  for (i = 0; i < inSize; i++) { 
    theListElements[i].val = inArray[i]; 
    theLastTemp = NULL; 
    theTemp = theFirst; 
    while ((theTemp != NULL) && (theTemp->val < theListElements[i].val))  { 
      theLastTemp = theTemp; 
      theTemp = theTemp->nxt; 
    } 
    theListElements[i].nxt  = theTemp; 
    if (theLastTemp == NULL) 
      theFirst =  &(theListElements[i]); 
    else 
      theLastTemp->nxt = &(theListElements[i]); 
  } 
  theTemp = theFirst; 
  i = 0;  
  while (theTemp != NULL) { 
    inArray[i++] = theTemp->val; 
    theTemp = theTemp->nxt; 
  } 
  delete [] theListElements; 
}      
 
void 
shell(int inN, double a[]) 
{ 
  int i,j,inc; 
  double v; 
  inc=1; 
  do { 
    inc *= 3; 
    inc++; 
  } while (inc <= inN); 
  do { 
    inc /= 3; 
    for (i=inc+1;i<=inN;i++) { 
      v=a[i]; 
      j=i; 
      while (a[j-inc] > v) { 
        a[j]=a[j-inc]; 
        j -= inc; 
        if (j <= inc) break; 
      } 
      a[j]=v; 
    } 
  } while (inc > 1); 
} 
/* (C) Copr. 1986-92 Numerical Recipes Software '$&'3$. */ 
 
void 
GetSample(long *inSample, long inSampleSize) 
{ 
	long theSize, i, j , k, done; 
	double uni; 
	long thePick; 
	theSize = 2; 
	for (i = 0; i < theSize; i++) 
		inSample[i] = 0; 
  done = 0; 
	while (1) { 
		// first pick an individual at random 
	  thePick = gRand.randomLRange(theSize); 
		// then pick if it duplicates or mutates 
		uni = gRand.random() - ((theSize - 1)/(g4NeMu + theSize - 1)); 
		if (uni < 0) { // it duplicates 
			// stop right at the last duplication 
			if (theSize == inSampleSize)  
				break; 
			else { 
				inSample[theSize] = inSample[thePick]; 
				theSize++; 
				continue; 
			} 
		} 
		// it mutates, if it gets this far 
		uni -= gOneStep*(g4NeMu/(2.0*(g4NeMu + theSize - 1))); 
		if (uni < 0) { // it gets one larger 
			inSample[thePick]++; 
			continue; 
    } 
		uni -= gOneStep*(g4NeMu/(2.0*(g4NeMu + theSize - 1))); 
		if (uni < 0) { // it gets one shorter 
			inSample[thePick]--; 
			continue; 
    } 
		uni -= (1.0-gOneStep)*g4NeMu/(2.0*(g4NeMu + theSize - 1));  
		if (uni < 0) { // it gets geometrically bigger 
			inSample[thePick] += gRand.geometric(1.0/(gMeanStepSize-1)) + 
1; 
			continue; 
    } 
		else { // it gets geometrically shorter 
			inSample[thePick] -= gRand.geometric(1.0/(gMeanStepSize-1)) + 
1; 
		} 
  }  
} 
 
 
double 
GetMc() 
{ 
	long tr, lo, i, j, k, t, theMin, theMax, numAlleles; 
	long theAlleleVec[gMaxAlleles], theAlleleRange; 
	long theSample[gMaxSampleSize]; 
	double theRatio[gTrials], theLocusRatio, theSampleRatio; 
	double uni; 
	double mean; 
	mean = 0.0; 
	for (tr = 0; tr < gTrials; tr++) { 
		theSampleRatio = 0.0; 
		for (lo = 0; lo < gNumLoci; lo++) { 
			GetSample(theSample, gSampleSize); 
			Sort(theSample, gSampleSize);  
			theMin = theSample[0]; 
			theMax = theSample[gSampleSize- 1]; 
			theAlleleRange = theMax - theMin + 1; 
			for (i = 0; i < theAlleleRange; i++) 
				theAlleleVec[i] = 0; 
			j = 0; 
			for (i = 0; i < gSampleSize; i++) { 
				if (theSample[i] == j + theMin) 
					theAlleleVec[j]++; 
				else { 
					j++; 
					i--; 
				} 
			} 
			numAlleles = 0; 
			for (i = 0; i < theAlleleRange; i++) { 
				if (theAlleleVec[i] != 0)  
					numAlleles++; 
			} 
			theLocusRatio = (double)numAlleles/theAlleleRange; 
			if (numAlleles == 1) 
				lo--; 
			else  // a polymorphic locus, keep it 
				theSampleRatio += theLocusRatio/gNumLoci; 
		} 
		theRatio[tr] = theSampleRatio; 
		mean += theSampleRatio/gTrials; 
	} 
	cout << "Mean M = " <<  mean << endl; 
	shell(gTrials-1, theRatio); 
	return theRatio[(long)(floor(0.05*gTrials))]; 
} 
 
 
main() 
{ 
  int i; 
	double theTempLargerStep, theTempMc; 
	long s1, s2; 
  gRand.reset(-1, 1188); 
  gRand.getInitialSeed(&s1,&s2); 
	cout << "Please enter the sample size (twice the number of diploid "; 
  cout << "individuals)" << endl; 
  cin >> gSampleSize; 
	cout << "Please enter the number of loci sampled" << endl; 
	cin >> gNumLoci; 
	cout << "Please enter the pre-bottleneck value of theta" << endl; 
	cout << "(note theta = 4*Ne*mu = 4 times effective population size times"; 
	cout << " mutation rate, " << endl;  
	cout << "so theta=10 if Ne = 5000 and mutation rate is 0.0005 )" << endl; 
  cin >> g4NeMu; 
	cout << "Please enter the mean size for larger mutations (example: 3.5)"; 
	cout << endl; 
  cin >> gMeanStepSize; 
	cout << "Please enter the fraction of mutations that are larger than single "; 
	cout << "steps" << endl; 
	cout << "(0 for the strict one-step model, 0.1 for 90% single steps)" << endl; 
	cin >> theTempLargerStep; 
  gOneStep = 1.0 - theTempLargerStep; 
	if (gMeanStepSize <= 1 || theTempLargerStep > 1.0) 
		gOneStep = 1.0; 
  cout << endl; 
	cout << gNumLoci << " loci" << endl; 
	cout << "sample size = " << gSampleSize << endl; 
	cout << "4*Ne*(mutation rate) = " << g4NeMu << endl; 
	cout << "% larger mutations = " << 1.0 - gOneStep << endl; 
	cout << "mean larger mutation size = " << gMeanStepSize << endl; 
	cout << "Please wait..." << endl; 
	theTempMc = GetMc(); 
	cout << "95% of equilibrium values of M should be above" << endl; 
	cout << "Mc = " << theTempMc << endl; 
	cout << "where M = number of alleles / range in allele size" << endl; 
	cout << "and range = (size of largest allele - "; 
	cout << "size of smallest allele + 1)" << endl; 
} 


