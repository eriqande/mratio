// CRandom.h 
 
#ifndef CRandom_H 
#define CRandom_H 
 
class CRandom { 
 
  private: 
	// local variables for CRandom.c  
	long initialSeed1; 
	long initialSeed2; 
	long idum; // for ran3 
	double u[98]; 
	double c; 
	double cd; 
	double cm; 
	long i97; 
	long j97; 
	double nextGaussian; 
	long haveNextGaussian; 
 
  public: 
 
  CRandom() { seed(-1, -1); } 
 
// reset the CRandom number generator to the beginning of the two  
// seeds previously specified. 
	void reset(); 
	void reset(long inS1, long inS2) { seed(inS1, inS2); } 
 
// get the seeds that you set, in case you forgot... 
	void getInitialSeed(long* initialSeed1, long* initialSeed2); 
 
// Get an long between 0 and range 
	int randomIRange(int range); 
 
// * Get a long between 0 and range 
	long randomLRange(long range); 
 
// * Set the two seeds... 
	long seed(long ij, long kl); // was rmargin(ij, kl) 
 
// CHOOSE THE RANDOM NUMBER GENERATOR (ran3 or ranFl) HERE 
// random() returns a number between 0.0 and 1.0 uniformly distributed. 
// can't return a zero or a 1 exactly, this prevents GetPo from 
// ever giving you a starting set of frequencies with no copies 
// of one allele 
	double random() { double rand;  
									rand = ran3(); 
									while ((rand == 0) || 
(rand == 1.0)) 
									  rand = ran3(); 
									return rand; } 
 
// Florida state method 
	double ranFl(); 
 
// numerical recipes method 
	double ran3(); 
 
//* returns a number between 0.0 and 1.0 exponetially distributed towards 1.0 
  double exponential(double mean); 
 
// returns numbers whose mean is 0.0 and statndard deviation of 1.0 
  double gaussian(); 
 
// binomial deviate, returns the outcome of inN Bernoulli trials 
  double bnldev(double inP, long inN); 
  long *multinomial(double *inP, long inN, long inCategories); 
// (genetic) drift for one generation 
  void multiShift(long *inP, long inN, long inCategories); 
// (genetic) drift for one generation 
  void multiShift(double *inP, long inN, long inCategories); 
	void multiShift(long *inP, long inOrigN, long inN, long *inCategories); 
// (genetic) drift conditional on not losing alleles (one generation) 
  void cMultiShift(long *inNotLost, double *inP, long inN, long inCategories); 
	double rndgamma (double s); 
	double rndgamma1 (double s); 
	double rndgamma2 (double s); 
	long geometric (double p); 
}; // end of the class CCRandom  
typedef class CRandom CRandom; 
 
#endif // CRandom_H   


