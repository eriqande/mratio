/* buffer.h */ 
 
#ifndef BUFFER_H 
#define BUFFER_H 
//#include "types.h" 
#include "buf_con.h" 
#include <stream.h> 
#include <fstream.h> 
 
class CBuffer { 
  public: 
	CBuffer(void); 
	CBuffer(char *inFileName); 
  ~CBuffer(); 
  ifstream mInFile;     
  char *mBuffer; 
	int mBufPos; // current position in the buffer 
	int mBufSize; // number of bytes in the buffer 
	long mIsEndInBuffer; // is the End Of File (EOF) currently in the buffer 
	long mIsAtEnd; // are we there, at the very end of the file, now   
	long IsAtEnd(void) { return mIsAtEnd; } 
	void OpenBuffer(char *inFileName); 
	void CloseBuffer(void); 
	void ResetBuffer(void) { mBufPos = 0; mIsAtEnd = E_FALSE;} 
	char GetNextByte(); // updates buf_pos, returns next byte of data 
	void FillBuffer(); 
	void SkipSpace(); 
	void SkipOneCol(); 
	void SkipCols(int); 
	int CountCols(void); 
  void MoveToNextRow(); 
	void OutputCol(); 
	void OutputCols(int); 
	void OutputRow(); 
	void OutputRows(int); 
	void SkipRows(int); 
  double GetFloat(); 
  int GetInt(); 
  char *GetString(); 
  void GetString(char *inStr); 
  unsigned char *GetPascalString();   
  float InspectFloatPast(int); 
  char InspectByte(int); 
  int BytesToNextCol(int); 
  int BytesToRow(int); 
}; 
 
#endif // BUFFER_H 


