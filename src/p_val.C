#include "buffer.h" 
#include "CRandom.h"                                                                                                                                                                                                                                                                                            
/* because I am mixing C and C++, I have to imbed this like so (ECA) */
extern "C" {
	#include "eca_opt3.h"
}
#define max(A, B) ((A) > (B) ? (A) : (B)) 
#define min(A, B) ((A) < (B) ? (A) : (B)) 
 
#define gMaxAlleles 2000 
#define gMaxLoci 500 
#define gMaxSampleSize 2000 
#define gNumTrials 10000 
#define square(A) ((A) * (A)) 
 
CRandom gRand; 
 
double g4NeMu, gMeanStepSize, gOneStep; 
char gInFileName[500]; 
 
class ListEntry { 
public: 
  ListEntry(); 
  struct ListEntry *nxt; 
  double val; 
}; 
typedef class ListEntry ListEntry; 
 
ListEntry::ListEntry() 
{ 
  nxt = NULL; 
} 
 
// smallest number will be inArray[0] 
void 
Sort(double *inArray, long inSize) 
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
 
// smallest number will be inArray[0] 
void 
Sort(long *inArray, long inSize) 
{ 
  ListEntry *theListElements = new ListEntry[inSize]; 
  ListEntry *theFirst, *theTemp, *theLastTemp; 
  long i; 
  theFirst = NULL; 
  for (i = 0; i < inSize; i++) { 
    theListElements[i].val = (double)inArray[i]; 
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
    inArray[i++] = (long)theTemp->val; 
    theTemp = theTemp->nxt; 
  } 
  delete [] theListElements; 
}      
 
 
// note if mAlleleSize is an int then we can't have partial increments in 
// repeat number, like half a repeat increase in allele size 
struct Locus 
{ 
  int mNumAlleles; 
	int mRange; 
	int mRepeatSize; 
	int mSampleSize; 
	int mAlleleSize[gMaxAlleles]; 
	int mAlleleCount[gMaxAlleles]; 
}; 
typedef struct Locus Locus; 
 
void 
Print(Locus *inLoc) 
{ 
	cout << "# alleles = "; 
  cout << inLoc->mNumAlleles; 
	cout << "  range = "; 
  cout << inLoc->mRange;  
	cout << "  ratio = " << (double)inLoc->mNumAlleles/inLoc->mRange; 
	cout << "  sample size = " << inLoc->mSampleSize << endl;; 
#if 1 
	int i; 
  for (i = 0; i < inLoc->mNumAlleles; i++) 
		cout << inLoc->mAlleleSize[i] << "\t"; 
	cout << endl; 
  for (i = 0; i < inLoc->mNumAlleles; i++) 
		cout << inLoc->mAlleleCount[i] << "\t"; 
	cout << endl; 
	cout << endl; 
#endif 
} 
 
void 
SetEqual(Locus *newLoc, Locus *oldLoc) 
{ 
	int i; 
  newLoc->mNumAlleles = oldLoc->mNumAlleles; 
  newLoc->mRange = oldLoc->mRange; 
	newLoc->mSampleSize = oldLoc->mSampleSize; 
	for (i = 0; i < newLoc->mNumAlleles; i++) { 
		newLoc->mAlleleSize[i] = oldLoc->mAlleleSize[i]; 
		newLoc->mAlleleCount[i] = oldLoc->mAlleleCount[i]; 
	} 
} 
 
void 
GetLocusList(Locus *inLocusList, int *inNumLoci) 
{ 
  CBuffer theFreqBuf; 
	Locus theLoc; 
	int i, j, lo, tempSize[gMaxAlleles], tempCount[gMaxAlleles], linkLo; 
	theFreqBuf.OpenBuffer(gInFileName); 
	lo = 0; 
	g4NeMu=theFreqBuf.GetFloat(); 
	gMeanStepSize=theFreqBuf.GetFloat(); 
	gOneStep=1.0-theFreqBuf.GetFloat(); 
	while (!theFreqBuf.IsAtEnd()) { 
		theFreqBuf.SkipSpace(); 
		theLoc.mRepeatSize = theFreqBuf.GetInt(); 
		theLoc.mNumAlleles = theFreqBuf.CountCols(); 
		for (i=0; i < theLoc.mNumAlleles; i++)  
			tempSize[i] = theFreqBuf.GetInt(); 
		for (i=0; i < theLoc.mNumAlleles; i++)  
			tempCount[i] = theFreqBuf.GetInt(); 
		j = 0; 
		theLoc.mSampleSize = 0; 
		if (tempSize[0] < tempSize[theLoc.mNumAlleles-1]) { 
			for (i = 0; i < theLoc.mNumAlleles; i++) { 
				if (tempCount[i] != 0) { 
					theLoc.mAlleleCount[j] = tempCount[i]; 
					theLoc.mAlleleSize[j] = tempSize[i]; 
					theLoc.mSampleSize += tempCount[i]; 
					j++; 
				} 
			} 
		} 
		else { 
			for (i = theLoc.mNumAlleles - 1; i >= 0; i--) { 
				if (tempCount[i] != 0) { 
					theLoc.mAlleleCount[j] = tempCount[i]; 
					theLoc.mAlleleSize[j] = tempSize[i]; 
					theLoc.mSampleSize += tempCount[i]; 
					j++; 
				} 
			} 
		} 
		theLoc.mNumAlleles = j; 
		// now rescale to start at 0 and count repeat units 
		// first re-order small to large 
		for (i=1; i < theLoc.mNumAlleles; i++)  
			theLoc.mAlleleSize[i] =  
			  (theLoc.mAlleleSize[i] - 
theLoc.mAlleleSize[0])/theLoc.mRepeatSize; 
		theLoc.mAlleleSize[0] = 0; 
		theLoc.mRange = theLoc.mAlleleSize[theLoc.mNumAlleles-1] + 1; 
		SetEqual(&(inLocusList[lo]), &theLoc); 
		theFreqBuf.SkipSpace(); 
		lo++; 
	} 
	*inNumLoci = lo; 
// (ECA) I commented this line out because it always led to malloc errors and it does not seem
// crucial to the running of the program.
//	theFreqBuf.CloseBuffer(); 
} 
 
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
 
// returns ratio 
// population is at equilibrium 
// only returns polymorphic loci 
double 
DoEquilibrium(long inSampleSize) 
{ 
	long theAlleleVec[gMaxAlleles]; 
	long theAlleleRange, theNumAlleles; 
	long i, j, k, t, theMin, theMax; 
	long theSample[gMaxSampleSize]; 
	double uni; 
	theNumAlleles = 1; 
	while (theNumAlleles < 2) { 
		GetSample(theSample, inSampleSize); 
		Sort(theSample, inSampleSize);  
		theMin = theSample[0]; 
		theMax = theSample[inSampleSize - 1]; 
		theAlleleRange = theMax - theMin + 1; 
		for (i = 0; i < gMaxAlleles; i++) 
			theAlleleVec[i] = 0; 
		j = 0; 
		for (i = 0; i < inSampleSize; i++) { 
			if (theSample[i] == j + theMin) 
				theAlleleVec[j]++; 
			else { 
				j++; 
				i--; 
			} 
		} 
		theNumAlleles = 0; 
		for (i = 0; i < theAlleleRange; i++) { 
			if (theAlleleVec[i] != 0)  
				(theNumAlleles)++; 
		} 
	} 
	return (double)theNumAlleles/theAlleleRange; 
} 
 

/*
Because all of the user-input variables here are global variables anyway, we will just make this
a void function.  Easy enough.  (ECA)
*/
void Get_Pval_Options(int argc, char *argv[])
{
	int FileName_f=0;
	
	DECLARE_ECA_OPT_VARS  
 
 
 
	/* This is a good place to set some default values for variables in your program */
 
	/* some information about the program, author, etc */
	SET_PROGRAM_NAME("P_val_M");  /* Note:  QUOTED string */
	SET_PROGRAM_SHORT_DESCRIPTION("Calculate M and assess significance for a microsatellite data set"); /* QUOTED string */
	SET_PROGRAM_LONG_DESCRIPTION(M_P_Val.exe calculates the value of M for a microsatellite data set.  The use of M to detect population
						bottlenecks is described in: J. C. Garza and E. G. Williamson (2001) Detection of reduction in population size using 
						data from microsatellite loci. Molecular Ecology 10\054 305–318 doi:10.1046/j.1365-294x.2001.01190.x.  The program then simulates 
						an equilibrium distribution of M\054 according to the method described in Garza and Williamson (2001)\054 
						and given assumed values for the three parameters of the two-phase mutation model\054 and rank the calculated 
						value relative to the equilibrium distribution. Using conventional criteria\054 there is evidence of a 
						significant reduction in population size if less than 5% of the replicates are below the observed value. 
						The input file consists of the relative allele sizes\054 simple allele counts and assumed mean values for the 
						three parameters: theta=4*effective population size*mutation rate\054 ps=mean percentage of mutations that add 
						or delete only one repeat unit (the program actually uses ps-1) and deltag=mean size of larger mutations. 
						The proper format for the input file is described below. In a survey of empirical mutation data from the literature\054 
						Garza and Williamson (2001) found ps=0.88 and deltag=2.8. Theta will be population specific. \n\n
						
						This program was written by Ellen Williamson.  In April of 2008\054 Eric C. Anderson made some additions 
						to clean up the command line interface a little bit and to host this program within the GuiLiner GUI
						written by Nicholas Manoukis.\n\n
						
						\tAn example input file looks like:\n\n
							\t\t# example input file\054 you can write comments on lines that start with #\n
							\t\t#entries should be separated by tabs or spaces\n
							\t\t# on the first row list\n
							\t\t# pre-bottleneck theta = 4*(effective population size)*(mutation rate)\n
							\t\t# then average size of mutations that are not one-step mutations\n
							\t\t# then the percent of the time mutations are larger (not one-step)\n
							\t\t4 3.5 0.2\n
							\t\t\n
							\t\t# now list each locus one at a time\054 first put the size of repeats\n
							\t\t# for this locus (ie 2 would mean this is a dinucleotide microsatellite)\n
							\t\t2\n
							\t\t#then put the allele sizes\n
							\t\t10	8	6	4	2\n
							\t\t# then the number of observations of each type of allele \n
							\t\t22	20	20	24 20      \n
							\t\t# This is the format for locus 2\n
							\t\t3			  \n
							\t\t0	3	6	12	\n
							\t\t10	1	10	10\n
							\t\t# thats it!\n
						);  /* UN-QUOTED string! */
  SET_PROGRAM_AUTHOR_STRING("Ellen Williamson (CLI interface by Eric C. Anderson)"); /* QUOTED string */
  SET_VERSION("Version ??");  /* QUOTED string */
  SET_VERSION_HISTORY("Version ??.  Basically this is the same code as it has always been, but packaged up to run better on the CLI and using GuiLiner."); /*QUOTED string */
 
 
  BEGIN_OPT_LOOP  
 
 
    /* use this to start a block of related options.  Close the block with the CLOSE_SUBSET macro (below)  */
    OPEN_SUBSET(Options the user can input,  /* Name of Subset for output in --help, --help-full, and --help-nroff */
		 Input Options, /* Name of subset for lumping them together in GuiLiner */
		 There is only one option\054 so it is sort of silly to lump it up in a subset /* Description of subset.  Not really used currently */
	);
	
		if(REQUIRED_OPTION(Input File,
			FileName_f,
			f,
			,
			F,
			pathname to input file,
			Use this option to give the program the path to the input file.  Proper file format is given in the information about the program.)){
				if(ARGS_EQ(1)) {
					GET_STR(gInFileName);
				}
		}
	CLOSE_SUBSET;
	
	END_OPT_LOOP
}
 
int main(int argc, char *argv[]) 
{ 
  Locus theLocusList[gMaxLoci]; 
	double theRatioList[gNumTrials]; 
	int theNumLoci, tr, i; 
	double theAvgRatio, theSimAR; 
	int theSampleSize[gMaxLoci]; 
//(ECA)	cout << "Please type the name of the input file" << endl; 
//(ECA)	cin >> gInFileName; 
// THEN ECA ADDED THE FOLLOWING:
	Get_Pval_Options(argc, argv);





	GetLocusList(theLocusList, &theNumLoci); 
	cout << "4*Ne*(mutation rate)= " << g4NeMu;  
	cout << "  %larger mutations = "  << 1.0-gOneStep; 
	cout << "  mean step size = " << gMeanStepSize << endl; 
	for (i = 0; i < theNumLoci; i++) { 
		cout << "Locus " << i << ": "; 
		Print(&(theLocusList[i])); 
		theAvgRatio += 
(double)theLocusList[i].mNumAlleles/theLocusList[i].mRange; 
		theSampleSize[i] = theLocusList[i].mSampleSize; 
  } 
	theAvgRatio /= theNumLoci; 
 
 
	for (tr = 0; tr < gNumTrials; tr++) { 
		theSimAR = 0.0; 
		for (i = 0; i < theNumLoci; i++)  
			theSimAR += DoEquilibrium(theSampleSize[i])/theNumLoci;  
		theRatioList[tr] = theSimAR; 
  } 
	cout << "Average M = " << theAvgRatio << endl; 
	cout << "Please wait..." << endl; 
	Sort(theRatioList, gNumTrials); 
	for (tr = 0; tr < gNumTrials; tr++) { 
		if (theRatioList[tr] >= theAvgRatio) 
			break; 
  } 
	cout << (double)tr/100; 
  cout  << "% of the time you expect a smaller ratio at equilibrium"; 
	cout << endl; 
} 
 


