/* buffer.C: the latest version... */ 
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h> 
#include <iostream> 
#include "buffer.h" 
#include "buf_con.h" 
 
/*********************************************/ 
 
CBuffer::CBuffer() 
{ 
  mBuffer = new char[MAX_BUF]; 
  mIsEndInBuffer = E_FALSE; 
  mIsAtEnd = E_FALSE;  
} 
 
CBuffer::CBuffer(char *inFileName) 
{ 
  mBuffer = new char[MAX_BUF]; 
  mIsEndInBuffer = E_FALSE; 
  mIsAtEnd = E_FALSE;  
  mInFile.open(inFileName); 
  if ( mInFile == NULL) { 
    cout << "ERROR Not a valid file name!" << endl; 
    exit(1); 
  } 
  FillBuffer(); 
  mInFile.close(); 
}	 
 
void 
CBuffer::OpenBuffer(char *inFileName) 
{ 
  mBuffer = new char[MAX_BUF]; 
  mIsEndInBuffer = E_FALSE; 
  mIsAtEnd = E_FALSE;  
  mInFile.open(inFileName); 
  if ( mInFile == NULL) { 
    cout << "ERROR Not a valid file name!" << endl; 
    exit(1); 
  } 
  FillBuffer(); 
  mInFile.close(); 
}	 
 
void 
CBuffer::CloseBuffer() 
{ 
  delete [] mBuffer; 
} 
 
CBuffer::~CBuffer() 
{ 
  delete [] mBuffer; 
} 
 
/* getNextByte() returns the next byte from Buffer, if this is the */ 
/* last byte in the Buffer it calls  fillBuffer() */ 
char 
CBuffer::GetNextByte(void) 
{ 
  char theNextByte; 
 
  theNextByte = mBuffer[mBufPos++]; 
	while (theNextByte == '#') { 
		while ((theNextByte != '\n') && (theNextByte != '\r') &&  
								 (mBufPos != mBufSize))  
			theNextByte = mBuffer[mBufPos++]; 
		if (mBufPos != mBufSize) 
			theNextByte = mBuffer[mBufPos++]; 
	} 
  if (mBufPos == mBufSize) {  
		if(mIsEndInBuffer) 
			mIsAtEnd = E_TRUE; 
	} 
	// definately messed up! 
	if (mBufPos > mBufSize) { 
		cout << " Buffer ERROR Went too far!" << endl; 
	  exit(1); 
	} 
  return theNextByte; 
} 
   
void 
CBuffer::FillBuffer() 
{ 
  mBufPos = 0L; 
  mBufSize = 0L; 
  // just make it grab the whole file every time... 
  while (mInFile.peek() != EOF) { 
    mInFile.get(mBuffer[mBufSize++]); 
    if (mBufSize == MAX_BUF) { 
			cout << "ERROR File too big for buffer.C!" << endl; 
      exit(1); 
		} 
  } 
  mIsEndInBuffer = E_TRUE;         
  mBuffer[mBufSize] = '\r';     
} 
#if 0 // the old version... 
void 
CBuffer::fillBuffer(void) 
{ 
  FILE *InFile; 
  char ch; 
  char file_name[MAX_NAMELEN]; 
 
  mBufPos = 0; /* so the way it is now, you can't start Jan 1 */ 
  InFile = buf_file; 
  if (InFile == NULL) { 
    cout << "ERROR Bad file name in file_name[]!" << endl; 
    exit(1); 
  } 
  if (file_pos == 0) { 
    fseek(InFile, file_pos, 0); 
    mBufSize = fread(mBuffer, 1, myBufSize, InFile); 
  } 
  else { 
    fseek(InFile, file_pos, 0); 
    mBufSize = fread(mBuffer, 1, MIN_BUF, InFile); 
  } 
  if (mBufSize == myBufSize) { 
    while ((ch = (char)getc(InFile)) != '\n') { 
      if (mBufSize > myBufSize + EXTRA_BUF) { 
        cout << "ERROR in fillBuffer()" << endl; 
        exit(1); 
      } 
      mBuffer[mBufSize] = ch; 
      if (ch == EOF) 
        break; 
      else 
				mBufSize++; 
    } 
    mBuffer[mBufSize] = ch; 
  } 
  else 
    mBuffer[mBufSize] = EOF; 
  if (mBuffer[mBufSize] != EOF) { 
    mBuffer[++mBufSize] = (char)NULL; 
    file_pos = ftell(InFile); 
  } 
  fclose(InFile); 
} 
#endif 
 
void 
CBuffer::SkipSpace(void) 
{ 
  while (((isspace(InspectByte(0)) != E_FALSE) || InspectByte(0) == '#') 
								 && (!mIsAtEnd)) { 
		if (InspectByte(0) == '#') { 
			GetNextByte(); 
			mBufPos--; 
    } 
		else 
			GetNextByte(); 
  } 
} 
 
/* find the next non-space and then skip to the next space */ 
void 
CBuffer::SkipOneCol(void) 
{ 
  char theCh; 
 
  if (!mIsAtEnd) 
    theCh = GetNextByte(); 
  while ((theCh != '\n') && (theCh != '\t') && 
         (theCh !=  '\r') && (theCh != ' ') && !mIsAtEnd)  
    theCh = GetNextByte(); 
  SkipSpace(); 
} 
 
void 
CBuffer::SkipCols(int num_cols) 
{ 
  int I = 0; 
 
  for (I = 0; I < num_cols; I++) 
    SkipOneCol(); 
} 
 
void 
CBuffer::OutputCols(int inNumCols) 
{ 
  int I; 
 
  for (I = 0; I < inNumCols; I++)  
		OutputCol(); 
} 
 
void 
CBuffer::OutputCol(void) 
{ 
  char theCh[MAX_ROW_LEN]; 
	int i,j; 
	i = j = 0; 
  if (!mIsAtEnd) { 
    theCh[i++] = GetNextByte(); 
		if (!isspace(theCh[i-1])) 
			j = i; 
	} 
  while ((theCh[i-1] != '\n') && (theCh[i-1] != '\r') && !mIsAtEnd && 
					(theCh[i-1] != '\t') && (theCh[i-1] != ' ')) { 
    theCh[i++] = GetNextByte(); 
		if (!isspace(theCh[i-1])) 
			j = i; 
	} 
  for (i = 0; i < j; i++) 
		cout.put(theCh[i]); 
  cout << "\t"; 
} 
 
void 
CBuffer::OutputRow(void) 
{ 
  char theCh[MAX_ROW_LEN]; 
	int i,j; 
	i = j = 0; 
  if (!mIsAtEnd) { 
    theCh[i++] = GetNextByte(); 
		if (!isspace(theCh[i-1])) 
			j = i; 
	} 
  while ((theCh[i-1] != '\n') && (theCh[i-1] != '\r') && !mIsAtEnd) { 
    theCh[i++] = GetNextByte(); 
		if (!isspace(theCh[i-1])) 
			j = i; 
	} 
  for (i = 0; i < j; i++) 
		cout.put(theCh[i]); 
  cout << endl; 
} 
 
void 
CBuffer::MoveToNextRow(void) 
{ 
  char theCh; 
  if (!mIsAtEnd) 
    theCh = GetNextByte(); 
  while ((theCh != '\n') && (theCh != '\r') && !mIsAtEnd)  
    theCh = GetNextByte(); 
} 
 
void 
CBuffer::OutputRows(int num_rows) 
{ 
  int I; 
 
  for (I = 0; I < num_rows; I++)  
		OutputRow(); 
} 
 
void 
CBuffer::SkipRows(int num_rows) 
{ 
  int I; 
 
  for (I = 0; I < num_rows; I++)  
		MoveToNextRow(); 
} 
 
 
double 
CBuffer::GetFloat(void) 
{ 
  char *theFltPtr; 
  float theFlt; 
 char theNextByte; 
   
	SkipSpace(); 
	theNextByte = mBuffer[mBufPos]; 
	while (theNextByte == '#') { 
		while ((theNextByte != '\n') && (theNextByte != '\r') && !mIsAtEnd)  
			theNextByte = mBuffer[mBufPos++]; 
		theNextByte = mBuffer[mBufPos]; 
	} 
	theFltPtr = &(mBuffer[mBufPos]); 
  theFlt = strtod(theFltPtr,(char **)0); 
  SkipOneCol(); 
  return (double)theFlt; 
} 
float 
CBuffer::InspectFloatPast(int inBytesToSkip) 
{ 
  char *flt_ptr; 
  float flt; 
  int space_bytes; 
 
  space_bytes = 0; 
  while (isspace(mBuffer[mBufPos + inBytesToSkip + space_bytes])) 
    space_bytes++; 
  flt_ptr = &mBuffer[mBufPos + inBytesToSkip + space_bytes]; 
  flt = atof(flt_ptr); 
  return flt; 
} 
 
 
/* retreives the byte incr increments beyond the current position */ 
/* in the mBuffer but does not advance mBufPos */ 
/* or retreives the byte -incr increments before the current position */ 
/* in the mBuffer but does not change mBufPos */ 
char 
CBuffer::InspectByte(int incr) 
{ 
  if (((mBufPos + incr) > mBufSize) || ((mBufPos + incr) < 0)) { 
    cout << "ERROR in Buffer::InspectByte(),"; 
		cout << " can't look beyond end of mBuffer!" << endl; 
    exit(1); 
  } 
  return mBuffer[mBufPos + incr]; 
} 
 
 
int 
CBuffer::BytesToNextCol(int inBytesToSkip) 
{ 
  int more_bytes; 
 
  more_bytes = 0; 
  while (isspace(InspectByte(inBytesToSkip + more_bytes))) 
    more_bytes++; 
  while (!isspace(InspectByte(inBytesToSkip + more_bytes))) 
    more_bytes++; 
  while (isspace(InspectByte(inBytesToSkip + more_bytes))) 
    more_bytes++; 
  return more_bytes; 
} 
 
/* note: each tide entry = 1 row */ 
int 
CBuffer::BytesToRow(int row_num) 
{ 
  char ch; 
  int I; 
  int K; 
 
  I = 0; 
  if (row_num > 0) { 
    for (K = 1; K < row_num; K++) { 
      ch = InspectByte(I++); 
      while (ch != '\n') 
        ch = InspectByte(I++); 
    } 
  } 
  else { 
    for (K = 2; K > row_num; K--) { 
      ch = InspectByte( I--); 
      while (ch != '\n') 
        ch = InspectByte( I--); 
    } 
    I+=2; 
  } 
  return I; 
} 
 
 
#if 0 
// counts the rest of the columns in the row without advancing mBufPos 
int 
CBuffer::CountCols(void) 
{ 
  int theColCount = 0; 
  int theByteInspected = 0; 
  char theCh = InspectByte(theByteInspected); 
   
  while ((theCh != '\n') && (theCh != '\r')) { 
    while ((theCh != '\t') && (theCh != '\n') && (theCh != '\r')) 
      theCh = InspectByte(theByteInspected++); 
    while (theCh == '\t')  
			theCh = InspectByte(theByteInspected++);     
    theColCount++;     
  } 
 
  return theColCount; 
} 
#else 
int 
CBuffer::CountCols(void) 
{ 
  int theColCount = 0; 
  int theByteInspected = 0; 
  char theCh = InspectByte(theByteInspected); 
  
  while ((theCh != '\n') && (theCh != '\r')) { 
    while ((theCh != '\t') && (theCh != '\n') && (theCh != '\r') && 
           (theCh != ' ')) 
      theCh = InspectByte(theByteInspected++); 
    while (theCh == '\t' || theCh == ' ') 
      theCh = InspectByte(theByteInspected++);    
    theColCount++; 
  } 
  return theColCount; 
} 
#endif 
 
char * 
CBuffer::GetString(void) 
{ 
  int theMaxStrLen = 300; // change both of these if you need to change the 300 
  char theString[300];  
  int I = 0; 
  SkipSpace(); 
 
  theString[I++] = GetNextByte(); 
  while ((theString[I - 1] != '\n') && 
         (theString[I - 1] != '\r') && 
         (theString[I - 1] != '\t') && 
         (I < theMaxStrLen))  
         theString[I++] = GetNextByte(); 
  theString[I - 1] = '\0';        
  char *theNewString = new char[I];  
  for (int J = 0; J < I; J++) 
    theNewString[J] = theString[J]; 
  return theNewString;          
} 
 
 
void 
CBuffer::GetString(char *inStr) 
{ 
  int theMaxStrLen = 300; // change both of these if you need to change the 300 
  char theString[300];  
  SkipSpace(); 
 
  int I = 0; 
  theString[I++] = GetNextByte(); 
  while ((theString[I - 1] != '\n') && 
         (theString[I - 1] != '\r') && 
         (theString[I - 1] != '\t') && 
         (I < theMaxStrLen))  
         theString[I++] = GetNextByte(); 
  theString[I - 1] = '\0';        
  for (int J = 0; J < I; J++) 
    inStr[J] = theString[J]; 
} 
 
unsigned char * 
CBuffer::GetPascalString() 
{ 
  int theMaxStrLen = 300; // change both of these if you need to change the 300 
  unsigned char theString[300];  
  int I = 0; 
  SkipSpace(); 
 
  theString[++I] = (unsigned char)GetNextByte(); 
  while ((theString[I] != '\t') && 
         (theString[I] != '\n') && 
         (theString[I] != '\r') && 
         (theString[I] != '\0') && 
         (I < theMaxStrLen)) 
         theString[++I] = (unsigned char)GetNextByte(); 
  theString[I] = '\0'; 
  theString[0] = I - 1; // don't include the terminal null 
	// make room for first byte + null 
  unsigned char *theNewString = new unsigned char[I + 1];
  for (int J = 0; J <= I; J++) 
    theNewString[J] = theString[J]; 
  return theNewString;          
} 
 
int 
CBuffer::GetInt(void) 
{ 
  char *theIntPtr; 
  int theInt; 
  char theNextByte; 
   
	SkipSpace(); 
	theNextByte = mBuffer[mBufPos]; 
	while (theNextByte == '#') { 
		while ((theNextByte != '\n') && (theNextByte != '\r') && !mIsAtEnd)  
			theNextByte = mBuffer[mBufPos++]; 
	  theNextByte = mBuffer[mBufPos];		 
	} 
  theIntPtr = &(mBuffer[mBufPos]); 
  theInt = atoi(theIntPtr); 
  SkipOneCol(); 
  return theInt; 
} 


