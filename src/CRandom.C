// CRandom.C 
 
// The ranFl random number generator originally appeared in "Toward a Universal 
// Random Number Generator" by George Marsaglia and Arif Zaman. 
// Florida State University Report: FSU-SCRI-87-50 (1987) 
// 
// It was later modified by F. James and published in "A Review of Pseudo- 
// Random Number Generators" 
// 
// THIS IS THE BEST KNOWN RANDOM NUMBER GENERATOR AVAILABLE.  (in 1990, EW) 
//       (However, a newly discovered technique can yield 
//         a period of 10^600. But that is still in the development stage.) 
// 
// It passes ALL of the tests for random number generators and has a period 
//   of 2^144, is completely portable (gives bit identical results on all 
//   machines with at least 24-bit mantissas in the doubleing point 
//   representation). 
// 
// The algorithm is a combination of a Fibonacci sequence (with lags of 97 
//   and 33, and operation "subtraction plus one, modulo one") and an 
//   "arithmetic sequence" (using subtraction). 
// 
//======================================================================= 
// 
// This C language version was written by Jim Butler, and was based on a 
// FORTRAN program posted by David LaSalle of Florida State University. 
// 
//======================================================================= 
// 
// This is the initialization routine for the CRandom number generator RANMAR() 
// NOTE: The seed variables can have values between:    0 <= IJ <= 31328 
//                                                      0 <= KL <= 30081 
// The ranFl number sequences created by these two seeds are of sufficient 
// length to complete an entire calculation with. For example, if sveral 
// different groups are working on different parts of the same calculation, 
// each group could be assigned its own IJ seed. This would leave each group 
// with 30000 choices for the second seed. That is to say, this ranFl 
// number generator can create 900 million different subsequences -- with 
// each subsequence having a length of approximately 10^30. 
// 
// Use IJ = 1802 & KL = 9373 to test the ranFl number generator. The 
// main() that is #if 0'ed should be used to generate 20000 CRandom numbers. 
// It displays the next six CRandom numbers generated multiplied by 4096*4096 
// If the ranFl number generator is working properly, the pseudrandom numbers 
// should be: 
//           6533892.0  14220222.0  7275067.0 
//           6172232.0  8354498.0   10633180.0 
// 
//======================================================================= 
// Other functions are taken from Numerical Recipes (ran3 and bnldev) 
//======================================================================= 
 
#include <iostream.h> 
#include <stdlib.h> 
#include <math.h> 
#include <stdio.h> 
#include <time.h> 
#include "const.h" 
#include "CRandom.h" 
 
#define MBIG 1000000000 
#define MSEED 161803398 
#define MZ 0 
#define FAC (1.0/MBIG) 
 
double 
CRandom::ran3() 
{ 
	static long inext,inextp; 
	static long ma[56]; 
	static long iff=0; 
	long mj,mk; 
	long i,ii,k; 
 
	if (idum < 0 || iff == 0) { 
		iff=1; 
		mj=MSEED-(idum < 0 ? -idum : idum); 
		mj %= MBIG; 
		ma[55]=mj; 
		mk=1; 
		for (i=1;i<=54;i++) { 
			ii=(21*i) % 55; 
			ma[ii]=mk; 
			mk=mj-mk; 
			if (mk < MZ) mk += MBIG; 
			mj=ma[ii]; 
		} 
		for (k=1;k<=4;k++) 
			for (i=1;i<=55;i++) { 
				ma[i] -= ma[1+(i+30) % 55]; 
				if (ma[i] < MZ) ma[i] += MBIG; 
			} 
		inext=0; 
		inextp=31; 
		idum=1; 
	} 
	if (++inext == 56) inext=1; 
	if (++inextp == 56) inextp=1; 
	mj=ma[inext]-ma[inextp]; 
	if (mj < MZ) mj += MBIG; 
	ma[inext]=mj; 
	return mj*FAC; 
} 
#undef MBIG 
#undef MSEED 
#undef MZ 
#undef FAC 
// (C) Copr. 1986-92 Numerical Recipes Software '$&'3$.  
 
#if 0 
// This main() checks to see if ranFl() is working properly on the current 
// machine you are using 
long 
main() { 
  long I; 
  FILE * OUTFILE; 
	class CRandom rand_gen; 
 
  rand_gen.seed(1802, 9373); 
  for (I = 0; I < 20000; I++) 
    rand_gen.random(); 
  for(I = 0; I < 6; I++) 
    printf("%f\n", rand_gen.random()*4096*4096); 
  return 0; 
} 
#endif 
 
long  
CRandom::seed(long ij, long kl) 
  { 
  long i, j, k, l, ii, jj, m; 
  double s, t; 
 
  i97 = -1; 
  if (ij == -1) 
    { 
    ij = ((long) time(NULL) & 0x7FFF); 
    if (ij > 31327) ij = ij / 2; 
    } 
  initialSeed1 = ij; 
 
  if (kl == -1) 
    { 
    kl = ((long) time(NULL) & 0x7FFF); 
    if (kl > 30080) kl = kl / 2; 
    } 
  initialSeed2 = kl; 
 
  if (ij<0 || ij>31328 || kl<0 || kl>30081) 
    { 
// puts("The first CRandom number seed must have a value between 0 and 31328."); 
// puts("The second seed must have a value between 0 and 30081."); 
    return 0; 
    } 
 
  i = (ij/177)%177 + 2; 
  j = ij%177 + 2; 
  k = (kl/169)%178 + 1; 
  l = kl%169; 
 
  for (ii=1; ii<=97; ii++) 
    { 
    s = 0.0; 
    t = 0.5; 
    for (jj=1; jj<=24; jj++) 
      { 
      m = (((i*j)%179)*k) % 179; 
      i = j; 
      j = k; 
      k = m; 
      l = (53*l + 1) % 169; 
      if ((l*m)%64 >= 32) 
        s += t; 
      t *= 0.5; 
      } 
    u[ii] = s; 
    } 
 
  c  =   362436.0 / 16777216.0; 
  cd =  7654321.0 / 16777216.0; 
  cm = 16777213.0 / 16777216.0; 
 
  i97 = 97; 
  j97 = 33; 
 
  haveNextGaussian = 0; 
	idum = -1*initialSeed1; 
	if (idum > 0) { 
		cout << "Resetting the random seed to -15" << endl; 
    idum = -15; 
	} 
  return 1; 
  } 
 
double  
CRandom::ranFl() 
  { 
  double uni; 
 
  uni = u[i97] - u[j97]; 
  if (uni < 0.0) 
    uni += 1.0; 
  u[i97] = uni; 
  i97--; 
  if (i97 == 0) 
    i97 = 97; 
  j97--; 
  if (j97 == 0) 
    j97 = 97; 
  c -= cd; 
  if (c < 0.0) 
    c += cm; 
  uni -= c; 
  if (uni < 0.0) 
    uni += 1.0; 
  return uni; 
  } 
 
// This method will return an integer such that 0 <= rnd < range. 
int 
CRandom::randomIRange(int range) 
{ 
  return (int) (random()*range); 
} 
 
// This method will return a long integer such that 0 <= rnd < range. 
long int  
CRandom::randomLRange(long int range) 
{ 
  return (long int)(random()*range); 
} 
 
void  
CRandom::getInitialSeed(long *s1, long *s2) 
{ 
  *s1 = initialSeed1; 
  *s2 = initialSeed2; 
} 
 
void  
CRandom::reset() 
{ 
  seed(initialSeed1, initialSeed2); 
} 
 
 
double  
CRandom::exponential(double mean) 
{ 
  double x = random(); 
 
  while (x == 0.0) 
    x = random(); 
 
	return -(1.0 / mean) * log(x); 
} 
 
 
// Based upon the Polar method for normal deviates. 
// Reference Knuth, Seminumerical Algorithms, The Art of Computer Programming 
// Vol. 2, 1981, pp. 117. 
// Knuth references G.E.P. Box, M.E. Muller, and G. Marsaglia. 
// also see Numerical Recipes 2nd Ed. pg. 289 
 
double  
CRandom::gaussian() 
{ 
  double dist = -1.0, temp, uniform1, uniform2; 
 
  if (haveNextGaussian) { 
    haveNextGaussian = 0; 
    return nextGaussian; 
  } 
  else { 
    // find two points whose distance between them are less than one.  
    while ((dist >= 1.0) || (dist <= 0.0)) { 
      uniform1 = 2.0*random()-1.0; 
      uniform2 = 2.0*random()-1.0; 
      dist = uniform1*uniform1 + uniform2*uniform2; 
    } 
    temp = sqrt (-2.0*(log(dist)/dist)); 
    // save one for later, and return the other...     
    nextGaussian = uniform2*temp; 
    haveNextGaussian = 1; 
    return uniform1*temp; 
  } 
} 
 
// lifted from Numerical Recipes 
double 
gammln(double inX) 
{ 
  double x,y,tmp,ser; 
  static double cof[6]={76.18009172947146,-86.50532032941677, 
    24.01409824083091,-1.231739572450155, 
    0.1208650973866179e-2,-0.5395239384953e-5}; 
  long j; 
 
  y=x=inX; 
  tmp=x+5.5; 
  tmp -= (x+0.5)*log(tmp); 
  ser=1.000000000190015; 
  for (j=0;j<=5;j++) ser += cof[j]/++y; 
  return (double)-tmp+log(2.5066282746310005*ser/x); 
} 
 
// lifted from Numerical Recipes 
double 
CRandom::bnldev(double pp, long n) 
{ 
	long j; 
	static long nold=(-1); 
	double am,em,g,angle,p,bnl,sq,t,y; 
	static double pold=(-1.0),pc,plog,pclog,en,oldg; 
 
	p=(pp <= 0.5 ? pp : 1.0-pp); 
	am=n*p; 
	if (n < 25) { 
		bnl=0.0; 
		for (j=1;j<=n;j++) 
			if (random() < p) ++bnl; 
	} else if (am < 1.0) { 
		g=exp(-am); 
		t=1.0; 
		for (j=0;j<=n;j++) { 
			t *= random(); 
			if (t < g) break; 
		} 
		bnl=(j <= n ? j : n); 
	} else { 
		if (n != nold) { 
			en=n; 
			oldg=gammln(en+1.0); 
			nold=n; 
		} if (p != pold) { 
			pc=1.0-p; 
			plog=log(p); 
			pclog=log(pc); 
			pold=p; 
		} 
		sq=sqrt(2.0*am*pc); 
		do { 
			do { 
				angle=pi*random(); 
				y=tan(angle); 
				em=sq*y+am; 
			} while (em < 0.0 || em >= (en+1.0)); 
			em=floor(em); 
			t=1.2*sq*(1.0+y*y)*exp(oldg-gammln(em+1.0) 
				-gammln(en-em+1.0)+em*plog+(en-em)*pclog); 
		} while (random() > t); 
		bnl=em; 
	} 
	if (p != pp) bnl=n-bnl; 
	return bnl; 
} 
 
// returns a new array that generates nested binomial samples 
long * 
CRandom::multinomial(double *inP, long inN, long inCategories) 
{ 
  long *theAns = new long[inCategories]; 
  long I; 
  double theRProb; 
  long theRNum; 
  theRProb = 1.0; // the sum of probabilities of remaining categories 
  theRNum = inN;  // the remaining sample that hasn't been sampled yet 
  for (I = 0; I < (inCategories - 1); I++) { 
    theAns[I] = (long)bnldev(inP[I]/theRProb, theRNum); 
    theRProb -= inP[I]; 
    theRNum -= theAns[I]; 
  } 
  theAns[inCategories - 1] = theRNum; 
	// the last category is just the leftovers 
   
  return theAns; 
} 
 
// simulates one generation of genetic drift, inN individuals (alleles) 
// are sampled according to a multinomial distribution with the frequencies 
// of inP, then inP is set to the sample frequencies in this sample 
void 
CRandom::multiShift(double *inP, long inN, long inCategories) 
{ 
  long *theAns = new long[inCategories], theN; 
  long I; 
  double theRProb; 
  long theRNum; 
  theRProb = 1.0; // the sum of probabilities of remaining categories 
  theRNum = inN;  // the remaining sample that hasn't been sampled yet 
  for (I = 0; I < (inCategories - 1); I++) { 
    theAns[I] = (long)bnldev(inP[I]/theRProb, theRNum); 
    theRProb -= inP[I]; 
    theRNum -= theAns[I]; 
  } 
  theAns[inCategories - 1] = theRNum; // the last category is just the leftovers 
  for (I = 0; I < inCategories; I++) 
    inP[I] = (double)theAns[I]/inN; 
  delete [] theAns; 
} 
 
void 
CRandom::multiShift(long *inP, long inOrigN, long inN, long *inCategories) 
{ 
  long *theAns = new long[*inCategories]; 
	double *theFreq = new double[*inCategories]; 
  long theMin, theMax, i; 
  double theRProb; 
  long theRNum; 
 
  for (i = 0; i < *inCategories; i++)  
		theFreq[i] = (double)inP[i] / inOrigN; 
 
  theRProb = 1.0; // the sum of probabilities of remaining categories 
  theRNum = inN;  // the remaining sample that hasn't been sampled yet 
 
  for (i = 0; i < (*inCategories - 1); i++) { 
    theAns[i] = (long)bnldev(theFreq[i]/theRProb, theRNum); 
    theRProb -= theFreq[i]; 
    theRNum -= theAns[i]; 
  } 
 
  theAns[*inCategories-1] = theRNum; // the last category is just the leftovers 
 
	// update the number of categories and eliminate zeroes on the ends 
	// of the returning vector inP 
	theMin = 0;  
	while (theAns[theMin] == 0) theMin++; 
	theMax = *inCategories - 1;  
	while (theAns[theMax] == 0) theMax--; 
	*inCategories = theMax - theMin + 1; 
  for (i = theMin; i <= theMax; i++) 
    inP[i-theMin] = theAns[i]; 
 
  delete [] theAns; 
	delete [] theFreq; 
} 
 
// generates random (genetic) drift conditional on the fact that 
// the inNotLost alleles that are set to true are not lost 
void 
CRandom::cMultiShift(long *inNotLost, double *inP, long inN, long inCategories) 
{ 
  long I, theNumNotLost; 
  long *theAns = new long[inCategories]; 
	// put an allele of each category in your new population/sample if 
	// you know it is sampled (not lost) 
	theNumNotLost = 0; 
	for (I = 0; I < inCategories; I++) { 
		if (inNotLost[I] == E_TRUE) { 
			theAns[I] = 1; 
			theNumNotLost++; 
		} 
    else 
			theAns[I] = 0; 
	} 
  double theRProb; 
  long theRNum; 
	theRNum = inN-theNumNotLost;  // remainder that hasn't been sampled yet 
	theRProb = 1.0; // the sum of probabilities of remaining categories 
	for (I = 0; I < (inCategories - 1); I++) { 
			theAns[I] += (long)bnldev(inP[I]/theRProb, theRNum); 
			theRProb -= inP[I]; 
			theRNum -= (theAns[I]-inNotLost[I]); 
	} 
	// the last category is just the leftovers 
  theAns[inCategories - 1] += theRNum; 
  for (I = 0; I < inCategories; I++) 
    inP[I] = (double)theAns[I]/inN; 
  delete [] theAns; 
} 
 
double  
CRandom::rndgamma (double s) 
{ 
/* random standard gamma (Mean=Var=s,  with shape par=s, scale par=1) 
      r^(s-1)*exp(-r) 
   J. Dagpunar (1988) Principles of random variate generation, 
   Clarendon Press, Oxford 
   calling rndgamma1() if s<1 or 
           rndgamma2() if s>1 or 
           exponential if s=1 
*/ 
   double r=0; 
 
   if (s<=0)      puts ("err rndgamma()."); 
   else if (s<1)  r=rndgamma1 (s); 
   else if (s>1)  r=rndgamma2 (s); 
   else           r=-log(random()); 
   return (r); 
} 
 
 
double  
CRandom::rndgamma1 (double s) 
{ 
/* random standard gamma for s<1 
   switching method 
*/ 
   double r, x=0,small=1e-37,w; 
   static double a,p,uf,ss=10,d; 
 
   if (s!=ss) { 
      a=1-s; 
      p=a/(a+s*exp(-a)); 
      uf=p*pow(small/a,s); 
      d=a*log(a); 
      ss=s; 
   } 
   for (;;) { 
      r=random(); 
      if (r>p)        x=a-log((1-r)/(1-p)), w=a*log(x)-d; 
      else if (r>uf)  x=a*pow(r/p,1/s), w=x; 
      else            return (0); 
      r=random(); 
      if (1-r<=w && r>0) 
         if (r*(w+1)>=1 || -log(r)<=w)  continue; 
      break; 
   } 
   return (x); 
} 
 
double  
CRandom::rndgamma2 (double s) 
{ 
/* random standard gamma for s>1 
   Best's (1978) t distribution method 
*/ 
   double r,d,f,g,x; 
   static double b,h,ss=0; 
   if (s!=ss) { 
      b=s-1; 
      h=sqrt(3*s-0.75); 
      ss=s; 
   } 
   for (;;) { 
      r=random(); 
      g=r-r*r; 
      f=(r-0.5)*h/sqrt(g); 
      x=b+f; 
      if (x <= 0) continue; 
      r=random(); 
      d=64*r*r*g*g*g; 
      if (d*x < x-2*f*f || log(d) < 2*(b*log(x/b)-f))  break; 
   } 
   return (x); 
} 
 
 
long 
CRandom::geometric(double p) 
{ 
  int i; 
  double u; 
	i = 1; 
	u = random(); 
  while(u >= p) { 
		u = random(); 
		i++; 
	} 
	return i; 
} 


