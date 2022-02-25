#pragma once
#include <iostream> 
#include <fstream>
using namespace std;

class ICompress
{
public:
	virtual bool init() = 0;
	virtual bool fini() = 0;

public:
	virtual bool compress(const int count, const bool value[], unsigned char** data, int& length) = 0;
	virtual bool decompress(unsigned char* data, int length, bool** value, int& count) = 0;

	virtual bool compress(const int count, const float value[], unsigned char** data, int& length) = 0;
	virtual bool decompress(unsigned char* data, int length, float** value, int& count) = 0;

	virtual bool compress(const int count, const double value[], unsigned char** data, int& length) = 0;
	virtual bool decompress(unsigned char* data, int length, double** value, int& count) = 0;

	virtual bool compress(const int count, const int value[], unsigned char** data, int& length) = 0;
	virtual bool decompress(unsigned char* data, int length, int** value, int& count) = 0;

	virtual bool timecompress(const int count, const int value[], unsigned char** data, int& length) = 0;
	virtual bool timedecompress(unsigned char* data, int length, int** value, int& count) = 0;
};

class Compress : public ICompress
{
public:
	Compress();
	~Compress();

public:
	virtual bool init();
	virtual bool fini();

public:

	virtual bool compress(const int count, const bool value[], unsigned char** data, int& length);
	virtual bool decompress(unsigned char* data, int length, bool** value, int& count);

	virtual bool compress(const int count, const float value[], unsigned char** data, int& length);
	virtual bool decompress(unsigned char* data, int length, float** value, int& count);

	virtual bool compress(const int count, const double value[], unsigned char** data, int& length);
	virtual bool decompress(unsigned char* data, int length, double** value, int& count);

	virtual bool compress(const int count, const int value[], unsigned char** data, int& length);
	virtual bool decompress(unsigned char* data, int length, int** value, int& count);

	virtual bool timecompress(const int count, const int value[], unsigned char** data, int& length);
	virtual bool timedecompress(unsigned char* data, int length, int** value, int& count);
private:
	unsigned long long Gxor(unsigned long long* num1, unsigned long long* num2);
	void count0(unsigned long long* deltaLong, unsigned long long* front0, unsigned long long* back0);
	unsigned int fxor(unsigned int* num1, unsigned int* num2);
	void fcount0(unsigned int* deltaLong, unsigned int* front0, unsigned int* back0);
};


