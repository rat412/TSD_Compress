// Compress.cpp: 定义应用程序的入口点。
//

#include "Compress.h"

using namespace std;


Compress::Compress()
{
}

Compress::~Compress()
{
}

bool Compress::init()
{
	return false;
}

bool Compress::fini()
{
	return false;
}

bool Compress::compress(const int count, const bool value[], unsigned char** data, int& length)
{
	unsigned int output = 0, ii = 0, temp = 0;
	unsigned int* uintarr = new unsigned int[100000];
	for (int i = 0; i < (count - count % 58); i++)
	{
		output += value[i];
		if (i % 58 == 57) {
			temp = 0x3F;
			temp  = temp << 58;
			uintarr[ii] = temp;
			uintarr[ii] += output;
			ii += 1;
			output = 0;
		}
		else {
			output = output << 1;
		}
	}
	output = 0;
	if (count % 58 != 0) {
		for (int i = (count - count % 58); i < count; i++) {
			output += value[i];
			output = output << 1;
		}
		output = output >> 1;
		output = output << (64 - 6 - count % 58);
		temp = (count % 58);
		temp = temp << 58;

		uintarr[ii] = temp;
		uintarr[ii] += output;
		ii += 1;
	}
	length = ii * 8;
	unsigned char* uchararr = new unsigned char[length];
	for (int i = 0; i < ii; i++) {
		for (int iiii = 0; iiii < 8; iiii++) {
			uchararr[i * 8 + iiii] = ((uintarr[i] << (8*iiii)) >> 56);
		}
	}
	*data = uchararr;
	delete[] uintarr;
	return false;
}

bool Compress::decompress(unsigned char* data, int length, bool** value, int& count)
{
	count = 0;
	unsigned long long* de_input = new unsigned long long[100000];

	unsigned long long temp = 0;
	int i = 0, ii = 0, cur = 0;
	for (int i = 0; i < length;i+=8) {
		for (int iii = 0; iii < 7; iii++) {
			temp += (unsigned long long)data[i + iii];
			temp = temp << 8;
		}
		temp += (unsigned long long)data[i + 7];
		de_input[i / 8] = temp;
		temp = 0;
	}

	for (int cur = 0; cur < (length / 8); cur++) {
		unsigned long long num = (de_input[cur] >> 58);
		count += num;
	}
	bool* de_output = new bool[count];
	for (int cur = 0; cur < (length /8);cur++) {
		unsigned long long num = (de_input[cur] >> 58);
		de_input[cur] = de_input[cur] << 6;

		for (int i = 64 - 1; i >= (64 - num); --i)
		{
			//cout << ((de_input[cur] >> i) & 0x01);
			de_output[ii] = (bool)((de_input[cur] >> i) & 0x01);
			ii += 1;
		}
	}

	*value = de_output;

	delete[] de_input;
	//delete[] de_output;
	return false;
}

bool Compress::compress(const int count, const float value[], unsigned char** data, int& length)
{

	double* doublearr = new double[count],tmpvalue=0;
	for (int i = 0; i < count; i++) {
		if ((int)((value[i] - value[i - 1])*1000000000000)==0) {
			if (tmpvalue == 0) {
				tmpvalue = value[i - 1];
			}
			doublearr[i] = tmpvalue;
		}
		else {
			tmpvalue = 0;
			doublearr[i] = value[i];
		}
	}
	double frontNum = 0, backNum = 0;
	unsigned long long LastFront0 = 0, LastBack0 = 0;
	unsigned long long front0 = 0, back0 = 0;
	unsigned long long bitSize = 0, numSize = 0;
	unsigned long long GencodeOutput = 0;
	unsigned long long newbitSize = 0;
	int ullnum = 0;

	unsigned long long* ullarr = new unsigned long long[100000];
	ullarr[ullnum] = (unsigned long long)count;
	ullnum += 1;
	unsigned long long* ullarr1 = (unsigned long long*) & doublearr[0];
	ullarr[ullnum] = *ullarr1;
	ullnum += 1;

	for (int i = 1; i < count + 1; i++) {
		//cout << "\n";
		frontNum = doublearr[i - 1];
		backNum = doublearr[i];

		unsigned long long* frontLong = (unsigned long long*) & frontNum;
		unsigned long long* backLong = (unsigned long long*) & backNum;
		unsigned long long delta = Gxor(frontLong, backLong);
		unsigned long long* deltaLong = (unsigned long long*) & delta; 
		//cout <<"\n"<< *frontLong <<"\n" << *backLong << "\n" << "delta\t" << delta << "\n";
		if (delta == 0) {
			unsigned long long output = 0;
			numSize = 1;

			if (numSize + bitSize < 64)
			{
				output = output >> bitSize;
				GencodeOutput += output;
				bitSize += numSize;
			}
			else {
				unsigned long long outputH = output >> bitSize;
				GencodeOutput += outputH;
				unsigned long long* ullarrtemp = (unsigned long long*) & GencodeOutput;
				ullarr[ullnum] = *ullarrtemp;
				ullnum += 1;
				//ofile.write((char*)& GencodeOutput, sizeof(double));
				GencodeOutput = output << (64 - bitSize);
				newbitSize = bitSize + numSize - 64;
				bitSize = newbitSize;
			}
		}
		else {
			count0(deltaLong, &front0, &back0);
			if ((front0 > LastFront0 && back0 > LastBack0) && (LastFront0 > 0 && LastBack0 > 0))
			{
				LastFront0 = LastFront0;
				LastBack0 = LastBack0;
				//次位置0，写有效位（减去上次的前后导0）
				unsigned long long output = 0x08;
				output = output << (64 - 4);
				numSize = 64 - LastFront0 - LastBack0;
				delta = delta << LastFront0;
				delta = delta >> 2;
				output += delta;
				numSize += 2;

				if (numSize + bitSize < 64)
				{
					output = output >> bitSize;
					GencodeOutput += output;
					bitSize += numSize;
				}
				else {
					unsigned long long outputH = output >> bitSize;
					GencodeOutput += outputH;
					unsigned long long* ullarrtemp = (unsigned long long*) & GencodeOutput;
					ullarr[ullnum] = *ullarrtemp;
					ullnum += 1;
					//ofile.write((char*)& GencodeOutput, sizeof(double));
					GencodeOutput = output << (64 - bitSize);
					newbitSize = bitSize + numSize - 64;
					bitSize = newbitSize;
				}
			}
			else {
				LastFront0 = front0;
				LastBack0 = back0;
				//次位置1，前导5位，后导6位，写有效位
				unsigned long long output = 0x0C;
				output = output << (64 - 4);
				unsigned long long tmpfront = front0 << (64 - 2 - 5);
				unsigned long long tmpback = (64 - front0 - back0) << (64 - 2 - 5 - 6);
				output = output + tmpfront + tmpback;
				unsigned long long tmpdelta = delta << front0;
				tmpdelta = tmpdelta >> (2 + 5 + 6);
				output = output + tmpdelta;
				numSize = 2 + 5 + 6 + 64 - front0 - back0;
				if (numSize + bitSize < 64)
				{
					output = output >> bitSize;
					GencodeOutput += output;
					bitSize += numSize;
				}
				else {
					unsigned long long outputH = output >> bitSize;
					GencodeOutput += outputH;
					unsigned long long* ullarrtemp = (unsigned long long*) & GencodeOutput;
					ullarr[ullnum] = *ullarrtemp;
					ullnum += 1;
					//ofile.write((char*)& GencodeOutput, sizeof(double));
					GencodeOutput = output << (64 - bitSize);
					newbitSize = bitSize + numSize - 64;
					bitSize = newbitSize;
				}
			}
		}
	}
	if (bitSize < 64) {
		unsigned long long* ullarrtemp = (unsigned long long*) & GencodeOutput;
		ullarr[ullnum] = *ullarrtemp;
		ullnum += 1;
		//ofile.write((char*)& GencodeOutput, sizeof(double));
	}
	int aaaa = 0;
	length = ullnum * 8;
	unsigned char* uchararr = new unsigned char[length];
	for (int i = 0; i < ullnum; i++) {
		for (int iiii = 0; iiii < 8; iiii++) {
			uchararr[i * 8 + iiii] = ((ullarr[i] << (8 * iiii)) >> (64-8));
		}
	}
	*data = uchararr;
	delete[] ullarr;
	delete[] doublearr;
	return false;
}

bool Compress::decompress(unsigned char* data, int length, float** value, int& count)
{
	unsigned long long* decodep_input = new unsigned long long[100000];

	unsigned long long temp = 0, inputnum = 0;

	for (int i = 0; i < length; i += 8) {
		for (int iii = 0; iii < (8-1); iii++) {
			temp += (unsigned long long)data[i + iii];
			temp = temp << 8;
		}
		temp += (unsigned long long)data[i + 8 - 1];
		decodep_input[i / 8] = temp;
		temp = 0;
	}

	inputnum = (length / 8);
	decodep_input[inputnum] = 0;
	decodep_input[inputnum + 1] = 0;

	unsigned long long LastdecodeFront = 0, Lastdecodeback = 0;
	int i = 0;

	count = (int)decodep_input[0];
	float* de_p_float = new float[count];

	double* decode0 = (double*)& decodep_input[1];
	unsigned long long decode_front = decodep_input[1];
	de_p_float[i] = (float)* decode0;
	i += 1;

	unsigned long long decodeI = 0;
	unsigned long long decodeII = 0;
	unsigned long long decodeIII = 0;

	unsigned long long cur = 0, decodeCur = 0;

	while (i < count)
	{
		cur = (decodeCur / 64) + 2;
		unsigned long long decodeI = decodep_input[cur];

		decodeI = decodeI << (decodeCur % 64);

		unsigned long long decodeII = decodep_input[cur + 1];
		unsigned long long decodeIII = decodep_input[cur + 2];

		if (decodeCur % 64 != 0)
		{
			unsigned long long tmpdecodeII = decodeII >> (64 - decodeCur % 64);
			decodeI += tmpdecodeII;
			decodeII = decodeII << (decodeCur % 64);
			unsigned long long tmpdecodeIII = decodeIII >> (64 - decodeCur % 64);
			decodeII += tmpdecodeIII;
		}

		unsigned long long decodeOutput = 0;

		int head0 = (decodeI >> (64-1)) & 0x01;
		int head1 = (decodeI >> (64-2)) & 0x01;

		//首位是0，次位是0
		if (head0 == 0)
		{
			//cout << "00输出：" << decode_front << "\n";
			double* decode_front_double = (double*)& decode_front;
			de_p_float[i] = (float)* decode_front_double;
			i += 1;
			decodeCur += 1;
		}
		else {
			if (head1 == 0) {
				unsigned long long decodeNumSize = 64 - LastdecodeFront - Lastdecodeback;

				if (decodeNumSize + 2 > 64)
				{
					decodeI = decodeI << 2;
					decodeII = decodeII >> (64 - 2);
					decodeI += decodeII;
					decodeI = decodeI >> LastdecodeFront;
					decodeI = decodeI >> (64 - LastdecodeFront - decodeNumSize);
					decodeI = decodeI << (64 - LastdecodeFront - decodeNumSize);
					decodeOutput = decodeI;
				}
				else
				{
					decodeI = decodeI >> (64 - 2 - decodeNumSize);
					decodeI = decodeI << (64 - 2 - decodeNumSize);
					decodeI = decodeI << 2;
					decodeI = decodeI >> LastdecodeFront;
					decodeOutput = decodeI;
				}


				unsigned long long* decodeOutputLL = (unsigned long long*) & decodeOutput;
				unsigned long long* decode_frontLL = (unsigned long long*) & decode_front;
				unsigned long long GxorOutput = Gxor(decode_frontLL, decodeOutputLL);
				double* finalOutput = (double*)& GxorOutput;

				//cout << "10输出：" << *finalOutput << "\n";
				de_p_float[i] = (float)* finalOutput;
				i += 1;
				decode_front = GxorOutput;
				decodeCur += (2 + decodeNumSize);
			}
			else {

				unsigned long long newFront0 = (decodeI >> (64 - 2 - 5)) & 0x1F;//5
				unsigned long long newBack0 = (decodeI >> (64 - 2 - 5 - 6)) & 0x3F;//6
				LastdecodeFront = newFront0;
				Lastdecodeback = 64 - newFront0 - newBack0;
				unsigned long long decodeNumSize = newBack0;
				if (decodeNumSize + 5 + 6 + 2 > 64)
				{
					decodeI = decodeI << (5 + 6 + 2);
					decodeII = decodeII >> (64 - 5 - 6 - 2);
					decodeI += decodeII;
					decodeI = decodeI >> newFront0;
					decodeI = decodeI >> (64 - newFront0 - decodeNumSize);
					decodeI = decodeI << (64 - newFront0 - decodeNumSize);
					decodeOutput = decodeI;
				}
				else
				{
					decodeI = decodeI >> (64 - 5 - 6 - 2 - decodeNumSize);
					decodeI = decodeI << (64 - 5 - 6 - 2 - decodeNumSize);
					decodeI = decodeI << (5 + 6 + 2);
					decodeI = decodeI >> newFront0;
					decodeOutput = decodeI;
				}

				unsigned long long* decodeOutputLL = (unsigned long long*) & decodeOutput;

				unsigned long long* decode_frontLL = (unsigned long long*) & decode_front;
				unsigned long long GxorOutput = Gxor(decode_frontLL, decodeOutputLL);
				double* finalOutput = (double*)& GxorOutput;

				//cout << "11输出：" << *finalOutput << "\n";
				de_p_float[i] = (float)* finalOutput;
				i += 1;
				decode_front = GxorOutput;
				decodeCur += (2 + 5 + 6 + decodeNumSize);
			}
		}

	}
	*value = de_p_float;
	//cout << "最后一个输出：" << de_p_float[fnum - 1] << "\n";
	delete[] decodep_input;
	return false;
}

bool Compress::compress(const int count, const double value[], unsigned char** data, int& length)
{
	double* doublearr = new double[count], tmpvalue = 0;
	for (int i = 0; i < count; i++) {
		if ((int)((value[i] - value[i - 1]) * 10000000000000) == 0) {
			if (tmpvalue == 0) {
				tmpvalue = value[i - 1];
			}
			doublearr[i] = tmpvalue;
		}
		else {
			tmpvalue = 0;
			doublearr[i] = value[i];
		}
	}
	double frontNum = 0, backNum = 0;
	unsigned long long LastFront0 = 0, LastBack0 = 0;
	unsigned long long front0 = 0, back0 = 0;
	unsigned long long bitSize = 0, numSize = 0;
	unsigned long long GencodeOutput = 0;
	unsigned long long newbitSize = 0;
	int ullnum = 0;

	unsigned long long* ullarr = new unsigned long long[100000];
	ullarr[ullnum] = count;
	ullnum += 1;
	unsigned long long* ullarr1 = (unsigned long long*) & value[0];
	ullarr[ullnum] = *ullarr1;
	ullnum += 1;

	for (int i = 1; i < count+1; i++) {
		//cout << "\n";
		frontNum = doublearr[i - 1];
		backNum = doublearr[i];

		unsigned long long* frontLong = (unsigned long long*)& frontNum;
		unsigned long long* backLong = (unsigned long long*)& backNum;
		unsigned long long delta = Gxor(frontLong, backLong);
		unsigned long long* deltaLong = (unsigned long long*)& delta;

		if (delta == 0) {
			unsigned long long output = 0;
			numSize = 1;

			if (numSize + bitSize < 64)
			{
				output = output >> bitSize;
				GencodeOutput += output;
				bitSize += numSize;
			}
			else {
				unsigned long long outputH = output >> bitSize;
				GencodeOutput += outputH;
				unsigned long long* ullarrtemp = (unsigned long long*) & GencodeOutput;
				ullarr[ullnum] = *ullarrtemp;
				ullnum += 1;
				//ofile.write((char*)& GencodeOutput, sizeof(double));
				GencodeOutput = output << (64 - bitSize);
				newbitSize = bitSize + numSize - 64;
				bitSize = newbitSize;
			}
		}
		else {
			count0(deltaLong, &front0, &back0);
			if ((front0 > LastFront0 && back0 > LastBack0) && (LastFront0 > 0 && LastBack0 > 0))
			{
				//次位置0，写有效位（减去上次的前后导0）
				unsigned long long output = 0x08;
				output = output << 60;
				numSize = 64 - LastFront0 - LastBack0;
				delta = delta << LastFront0;
				delta = delta >> 2;
				output += delta;
				numSize += 2;

				if (numSize + bitSize < 64)
				{
					output = output >> bitSize;
					GencodeOutput += output;
					bitSize += numSize;
				}
				else {
					unsigned long long outputH = output >> bitSize;
					GencodeOutput += outputH;
					unsigned long long* ullarrtemp = (unsigned long long*) & GencodeOutput;
					ullarr[ullnum] = *ullarrtemp;
					ullnum += 1;
					//ofile.write((char*)& GencodeOutput, sizeof(double));
					GencodeOutput = output << (64 - bitSize);
					newbitSize = bitSize + numSize - 64;
					bitSize = newbitSize;
				}
			}
			else {
				LastFront0 = front0;
				LastBack0 = back0;
				//次位置1，前导5位，后导6位，写有效位
				unsigned long long output = 0x0C;
				output = output << 60;
				unsigned long long tmpfront = front0 << 57;
				unsigned long long tmpback = (64- front0- back0) << 51;
				output = output + tmpfront + tmpback;
				unsigned long long tmpdelta = delta << front0;
				tmpdelta = tmpdelta >> 13;
				output = output + tmpdelta;
				numSize = 13 + 64 - front0 - back0;

				if (numSize + bitSize < 64)
				{
					output = output >> bitSize;
					GencodeOutput += output;
					bitSize += numSize;
				}
				else {
					unsigned long long outputH = output >> bitSize;
					GencodeOutput += outputH;
					unsigned long long* ullarrtemp = (unsigned long long*) & GencodeOutput;
					ullarr[ullnum] = *ullarrtemp;
					ullnum += 1;
					//ofile.write((char*)& GencodeOutput, sizeof(double));
					GencodeOutput = output << (64 - bitSize);
					newbitSize = bitSize + numSize - 64;
					bitSize = newbitSize;
				}
			}
		}
	}
	if (bitSize < 64) {
		unsigned long long* ullarrtemp = (unsigned long long*) & GencodeOutput;
		ullarr[ullnum] = *ullarrtemp;
		ullnum += 1;
		//ofile.write((char*)& GencodeOutput, sizeof(double));
	}
	ullnum -= 1;
	length = ullnum * 8;
	unsigned char* uchararr = new unsigned char[length];
	for (int i = 0; i < ullnum; i++) {
		for (int iiii = 0; iiii < 8; iiii++) {
			uchararr[i * 8 + iiii] = ((ullarr[i] << (8 * iiii)) >> 56);
		}
	}
	*data = uchararr;
	delete[] ullarr;
	delete[] doublearr;
	return false;
}

bool Compress::decompress(unsigned char* data, int length, double** value, int& count)
{
	unsigned long long* decodep_input = new unsigned long long[100000];

	unsigned long long temp = 0;

	for (int i = 0; i < length; i += 8) {
		for (int iii = 0; iii < 7; iii++) {
			temp += (unsigned long long)data[i + iii];
			temp = temp << 8;
		}
		temp += (unsigned long long)data[i + 7];
		decodep_input[i / 8] = temp;
		temp = 0;
	}

	decodep_input[length / 8] = 0;
	decodep_input[(length / 8) + 1] = 0;

	unsigned long long LastdecodeFront = 0, Lastdecodeback = 0;
	int i = 0;

	count = (int)decodep_input[0];
	double* de_p_float = new double[count];

	double* decode0 = (double*)& decodep_input[1];
	unsigned long long decode_front = decodep_input[1];
	de_p_float[i] = *decode0;
	i += 1;

	unsigned long long decodeI = 0;
	unsigned long long decodeII = 0;
	unsigned long long decodeIII = 0;

	unsigned long long cur=0, decodeCur = 0;

	while (i < count)
	{
		cur = (decodeCur / 64) + 2;
		unsigned long long decodeI = decodep_input[cur];

		decodeI = decodeI << (decodeCur % 64);

		unsigned long long decodeII = decodep_input[cur + 1];
		unsigned long long decodeIII = decodep_input[cur + 2];

		if (decodeCur % 64 != 0)
		{
			unsigned long long tmpdecodeII = decodeII >> (64 - decodeCur % 64);
			decodeI += tmpdecodeII;
			decodeII = decodeII << (decodeCur % 64);
			unsigned long long tmpdecodeIII = decodeIII >> (64 - decodeCur % 64);
			decodeII += tmpdecodeIII;
		}

		unsigned long long decodeOutput = 0;

		int head0 = (decodeI >> 63) & 0x01;
		int head1 = (decodeI >> 62) & 0x01;

		//首位是0，次位是0
		if (head0 == 0)
		{
			//cout << "00输出：" << decode_front << "\n";
			double* decode_front_double = (double*)& decode_front;
			de_p_float[i] = (float)* decode_front_double;
			i += 1;
			decodeCur += 1;
		}
		else {
			if (head1 == 0) {
				unsigned long long decodeNumSize = 64 - LastdecodeFront - Lastdecodeback;

				if (decodeNumSize + 2 > 64)
				{
					decodeI = decodeI << 2;
					decodeII = decodeII >> (64 - 2);
					decodeI += decodeII;
					decodeI = decodeI >> LastdecodeFront;
					decodeI = decodeI >> (64 - LastdecodeFront - decodeNumSize);
					decodeI = decodeI << (64 - LastdecodeFront - decodeNumSize);
					decodeOutput = decodeI;
				}
				else
				{
					decodeI = decodeI >> (64 - 2 - decodeNumSize);
					decodeI = decodeI << (64 - 2 - decodeNumSize);
					decodeI = decodeI << 2;
					decodeI = decodeI >> LastdecodeFront;
					decodeOutput = decodeI;
				}


				unsigned long long* decodeOutputLL = (unsigned long long*)& decodeOutput;
				unsigned long long* decode_frontLL = (unsigned long long*)& decode_front;
				unsigned long long GxorOutput = Gxor(decode_frontLL, decodeOutputLL);
				double* finalOutput = (double*)& GxorOutput;

				//cout << "10输出：" << *finalOutput << "\n";
				de_p_float[i] = *finalOutput;
				i += 1;
				decode_front = GxorOutput;
				decodeCur += (2 + decodeNumSize);
			}
			else {

				unsigned long long newFront0 = (decodeI >> 57) & 0x1F;
				unsigned long long newBack0 = (decodeI >> 51) & 0x3F;
				LastdecodeFront = newFront0;
				Lastdecodeback = 64 - newFront0 - newBack0;
				unsigned long long decodeNumSize = newBack0;
				if (decodeNumSize + 5 + 6 + 2 > 64)
				{
					decodeI = decodeI << 13;
					decodeII = decodeII >> (64 - 13);
					decodeI += decodeII;
					decodeI = decodeI >> newFront0;
					decodeI = decodeI >> (64 - newFront0 - decodeNumSize);
					decodeI = decodeI << (64 - newFront0 - decodeNumSize);
					decodeOutput = decodeI;
				}
				else
				{
					decodeI = decodeI >> (64 - 13 - decodeNumSize);
					decodeI = decodeI << (64 - 13 - decodeNumSize);
					decodeI = decodeI << 13;
					decodeI = decodeI >> newFront0;
					decodeOutput = decodeI;
				}

				unsigned long long* decodeOutputLL = (unsigned long long*)& decodeOutput;

				unsigned long long* decode_frontLL = (unsigned long long*)& decode_front;
				unsigned long long GxorOutput = Gxor(decode_frontLL, decodeOutputLL);
				double* finalOutput = (double*)& GxorOutput;

				//cout << "11输出：" << *finalOutput << "\n";
				de_p_float[i] = *finalOutput;
				i += 1;
				decode_front = GxorOutput;
				decodeCur += (2 + 5 + 6 + decodeNumSize);
			}
		}

	}
	*value = de_p_float;
	//cout << "最后一个输出：" << de_p_float[fnum - 1] << "\n";
	delete[] decodep_input;
	return false;
}

bool Compress::compress(const int count, const int value[], unsigned char** data, int& length)
{
	unsigned long long* ullarr = new unsigned long long[100000];
	unsigned long long num = 0, cnum = 0, curlen = 0, mode = 0, numbit = 0;
	unsigned long long intlen = 0, steplen = 1, maxintlen = 0;
	unsigned long long ull, encodeOutput;
	int ii = 0;
	ullarr[ii] = count;
	ii += 1;
	while (count - num > 0) {
		cnum = 0;
		steplen = 1;
		curlen = 0;
		for (int iii = num; iii < count; iii++) {
			ull = (value[iii] >= 0) ? (value[iii] << 1) : ~(value[iii] << 1);//zigzag编码
			intlen = 0;
			for (int j = 0; j < 64; j++) {
				if (((ull >> (63 - j)) & 0x01) == 1) { break; }
				else { intlen += 1; }
			}
			intlen = 64 - intlen;



			maxintlen = steplen;
			if (maxintlen < intlen) { maxintlen = intlen; }

			if (maxintlen > 28) {
				maxintlen = 59;
			}
			else {
				if (maxintlen > 19) {
					maxintlen = 28;
				}
				else {
					if (maxintlen > 14) {
						maxintlen = 19;
					}
					else {
						if (maxintlen > 11) {
							maxintlen = 14;
						}
						else {
							if (maxintlen > 9) {
								maxintlen = 11;
							}
						}
					}
				}
			}
			if ((maxintlen < 4) && (maxintlen * (cnum + 1) > 54)) { break; }
			if (maxintlen * (cnum + 1) > 57) { break; }

			steplen = maxintlen;
			cnum += 1;
			curlen = steplen * cnum;
		}
		switch (steplen) {
		case 1:
			mode = 1;
			numbit = 6;
			break;
		case 2:
			mode = 2;
			numbit = 5;
			break;
		case 3:
			mode = 3;
			numbit = 5;
			break;
		case 4:
			mode = 4;
			numbit = 4;
			break;
		case 5:
			mode = 5;
			numbit = 4;
			break;
		case 6:
			mode = 6;
			numbit = 4;
			break;
		case 7:
			mode = 7;
			numbit = 4;
			break;
		case 8:
			mode = 8;
			numbit = 4;
			break;
		case 9:
			mode = 9;
			numbit = 4;
			break;
		case 11:
			mode = 10;
			numbit = 4;
			break;
		case 14:
			mode = 11;
			numbit = 4;
			break;
		case 19:
			mode = 12;
			numbit = 3;
			break;
		case 28:
			mode = 13;
			numbit = 3;
			break;
		case 59:
			mode = 14;
			numbit = 1;
			break;
		default:
			mode = 0;
		}
		encodeOutput = (mode << 60);
		encodeOutput += (cnum << (64 - 4 - numbit));
		for (int iiii = 0; iiii < cnum; iiii++) {
			ull = (value[num + iiii] >= 0) ? (value[num + iiii] << 1) : ~(value[num + iiii] << 1);//zigzag编码
			encodeOutput += (ull << (64 - 4 - numbit - steplen - steplen * iiii));
		}

		ullarr[ii] = encodeOutput;
		ii += 1;
		//ofile2.write((char*)& encodeOutput, sizeof(unsigned long long));

		num += cnum;
	}

	length = ii * 8;
	unsigned char* uchararr = new unsigned char[length];
	for (int i = 0; i < ii; i++) {
		for (int iiii = 0; iiii < 8; iiii++) {
			uchararr[i * 8 + iiii] = ((ullarr[i] << (8 * iiii)) >> 56);
		}
	}
	*data = uchararr;
	delete[] ullarr;

	return false;
}

bool Compress::decompress(unsigned char* data, int length, int** value, int& count)
{
	unsigned long long* decodep_input = new unsigned long long[100000];

	unsigned long long temp = 0;

	for (int i = 0; i < length; i += 8) {
		for (int iii = 0; iii < 7; iii++) {
			temp += (unsigned long long)data[i + iii];
			temp = temp << 8;
		}
		temp += (unsigned long long)data[i + 7];
		decodep_input[i / 8] = temp;
		temp = 0;
	}

	unsigned long long numbit;
	int i = 0;
	int ii = 1, alreadnum = 0;
	int ff = 0;

	count = decodep_input[0];
	int* deoutput = new int[count];

	while (alreadnum < count) {
		int head = (decodep_input[ii] >> 60);
		int head1 = 0;
		unsigned long long  desteplen = 0;
		switch (head)
		{
		case 1:
			desteplen = 1;
			numbit = 6;
			break;
		case 2:
			desteplen = 2;
			numbit = 5;
			break;
		case 3:
			desteplen = 3;
			numbit = 5;
			break;
		case 4:
			desteplen = 4;
			numbit = 4;
			break;
		case 5:
			desteplen = 5;
			numbit = 4;
			break;
		case 6:
			desteplen = 6;
			numbit = 4;
			break;
		case 7:
			desteplen = 7;
			numbit = 4;
			break;
		case 8:
			desteplen = 8;
			numbit = 4;
			break;
		case 9:
			desteplen = 9;
			numbit = 4;
			break;
		case 10:
			desteplen = 11;
			numbit = 4;
			break;
		case 11:
			desteplen = 14;
			numbit = 4;
			break;
		case 12:
			desteplen = 19;
			numbit = 3;
			break;
		case 13:
			desteplen = 28;
			numbit = 3;
			break;
		case 14:
			desteplen = 59;
			numbit = 1;
			break;
		default:
			desteplen = 0;
		}

		head1 = ((decodep_input[ii] << 4) >> (64 - numbit));
		for (unsigned long long j = 0; j < head1; j++) {
			unsigned long long finall = ((decodep_input[ii] << (4 + numbit + desteplen * j)) >> (64 - desteplen));
			alreadnum += 1;
			if (finall & 0x01) {
				ff = (~finall >> 1) + ((finall & 0x01) << 63);//负数
			}
			else {
				ff = finall >> 1;//正数
			}
			if (alreadnum-1 < count) {
				deoutput[alreadnum - 1] = ff;
			}
		}
		ii += 1;
	}
	*value = deoutput;
	delete[] decodep_input;
	return false;
}


bool Compress::timecompress(const int count, const int value[], unsigned char** data, int& length)
{

	long long bitSize = 0, numSize = 0;
	unsigned long long udeltaofdelta;
	unsigned long long output = 0;
	unsigned long long GencodeOutput = 0;
	long long newbitSize = 0;
	long long deltaofdelta;

	int arrnum = 0;
	unsigned long long *timearr = new unsigned long long[100000];
	timearr[arrnum] = count;
	arrnum += 1;
	timearr[arrnum] = value[0];
	arrnum += 1;
	for (int i = 1; i < count; i++) {
		if (i == 1) {
			deltaofdelta = value[1] - value[0] - 0;
		}
		else {
			deltaofdelta = value[i] - value[i - 1] - value[i - 1] + value[i - 2];
		}
		if (deltaofdelta == 0) {
			output = 0;
			numSize = 1;
		}
		else {
			if (deltaofdelta <= 64 && deltaofdelta >= -63) {
				output = 0x08;
				output = output << 60;
				udeltaofdelta = deltaofdelta << (64 - 7);
				udeltaofdelta = udeltaofdelta >> 2;
				output += udeltaofdelta;
				numSize = 2 + 7;
			}
			else {
				if (deltaofdelta <= 256 && deltaofdelta >= -255) {
					output = 0x0C;
					output = output << 60;
					udeltaofdelta = deltaofdelta << (64 - 9);
					udeltaofdelta = udeltaofdelta >> 3;
					output += udeltaofdelta;
					numSize = 3 + 9;
				}
				else {
					if (deltaofdelta <= 2048 && deltaofdelta >= -2047) {
						output = 0x0E;
						output = output << 60;
						udeltaofdelta = deltaofdelta << (64 - 12);
						udeltaofdelta = udeltaofdelta >> 4;
						output += udeltaofdelta;
						numSize = 4 + 12;
					}
					else {
						output = 0x0F;
						output = output << 60;
						udeltaofdelta = deltaofdelta << (64 - 32);
						udeltaofdelta = udeltaofdelta >> 4;
						output += udeltaofdelta;
						numSize = 4 + 32;
					}
				}
			}
		}
		if (numSize + bitSize < 64)
		{
			output = output >> bitSize;
			GencodeOutput += output;
			bitSize += numSize;
		}
		else {
			long long outputH = output >> bitSize;
			GencodeOutput += outputH;
			timearr[arrnum] = GencodeOutput;
			arrnum += 1;
			GencodeOutput = output << (64 - bitSize);
			newbitSize = bitSize + numSize - 64;
			bitSize = newbitSize;
		}
	}
	if (bitSize < 64) {
		timearr[arrnum] = GencodeOutput;
		arrnum += 1;
	}

	length = arrnum * 8;
	unsigned char* uchararr = new unsigned char[length];
	for (int i = 0; i < arrnum; i++) {
		for (int iiii = 0; iiii < 8; iiii++) {
			uchararr[i * 8 + iiii] = ((timearr[i] << (8 * iiii)) >> 56);
		}
	}
	*data = uchararr;
	delete[] timearr;
	return false;
}

bool Compress::timedecompress(unsigned char* data, int length, int** value, int& count)
{
	unsigned long long* decodep_input = new unsigned long long[100000];

	unsigned long long temp = 0;

	for (int i = 0; i < length; i += 8) {
		for (int iii = 0; iii < 7; iii++) {
			temp += (unsigned long long)data[i + iii];
			temp = temp << 8;
		}
		temp += (unsigned long long)data[i + 7];
		decodep_input[i / 8] = temp;
		temp = 0;
	}

	count = decodep_input[0];
	int* timestampout = new int[count];

	unsigned long long decodeI = 0;
	unsigned long long decodeII = 0;

	unsigned long long cur = 1, decodeCur = 0;
	long long decodeOutput = 0;

	long long de_d_front = 0, de_d;

	int iii = 0, ii = 0;

	//int* timestampout0 = (int*)& decodep_input[1];
	//timestampout[0] = *timestampout0;

	timestampout[0] = decodep_input[1];
	ii += 1;
	while (ii < count)
	{
		cur = (decodeCur / 64) + 2;
		unsigned long long decodeI = decodep_input[cur];
		decodeI = decodeI << (decodeCur % 64);
		unsigned long long decodeII = decodep_input[cur + 1];

		if (decodeCur % 64 != 0)
		{
			unsigned long long tmpdecodeII = decodeII >> (64 - decodeCur % 64);
			decodeI += tmpdecodeII;
		}
		int head0 = (decodeI >> 63) & 0x01;
		int head1 = (decodeI >> 62) & 0x01;
		int head2 = (decodeI >> 61) & 0x01;
		int head3 = (decodeI >> 60) & 0x01;


		if (head0 == 0) {
			decodeOutput = 0;
			de_d = decodeOutput + de_d_front;
			//cout << "0 de_d[ii]输出：" << de_d[ii] << "\n";
			timestampout[ii] = de_d + timestampout[ii - 1];
			//cout << ii << "个输出：\t" << timestampout[ii] << "\n";
			ii += 1;
			decodeCur += 1;
			de_d_front = de_d;
		}
		else {
			if (head1 == 0) {
				decodeOutput = decodeI << 2;
				decodeOutput = decodeOutput >> (64 - 7);

				de_d = decodeOutput + de_d_front;
				//cout << "10 de_d[ii]输出：" << de_d[ii] << "\n";
				timestampout[ii] = de_d + timestampout[ii - 1];
				//cout << ii << "个输出：\t" << timestampout[ii] << "\n";
				ii += 1;
				decodeCur += 9;
				de_d_front = de_d;
			}
			else {
				if (head2 == 0) {
					decodeOutput = decodeI << 3;
					decodeOutput = decodeOutput >> (64 - 9);
					de_d = decodeOutput + de_d_front;
					//cout << "110 de_d[ii]输出：" << de_d[ii] << "\n";
					timestampout[ii] = de_d + timestampout[ii - 1];
					//cout << ii << "个输出：\t" << timestampout[ii] << "\n";
					ii += 1;
					decodeCur += 12;
					de_d_front = de_d;
				}
				else {
					if (head3 == 0) {
						decodeOutput = decodeI << 4;
						decodeOutput = decodeOutput >> (64 - 12);

						de_d = decodeOutput + de_d_front;
						//cout << "1110 de_d[ii]输出：" << de_d[ii] << "\n";
						timestampout[ii] = de_d + timestampout[ii - 1];
						//cout << ii << "个输出：\t" << timestampout[ii] << "\n";
						ii += 1;
						decodeCur += 16;
						de_d_front = de_d;
					}
					else {
						decodeOutput = decodeI << 4;
						decodeOutput = decodeOutput >> (64 - 32);

						de_d = decodeOutput + de_d_front;
						//cout << "1111 de_d[ii]输出：" << de_d[ii] << "\n";
						timestampout[ii] = de_d + timestampout[ii - 1];
						//cout << ii << "个输出：\t" << timestampout[ii] << "\n";
						ii += 1;
						decodeCur += 32;
						de_d_front = de_d;
					}
				}
			}
		}

	}

	*value = timestampout;
	delete[] decodep_input;
	return false;
}

unsigned long long Compress::Gxor(unsigned long long* num1, unsigned long long* num2)
{
	unsigned long long Gxor_output;
	int* p1 = (int*)(num1);
	int* p2 = (int*)(num2);
	int* p3 = (int*)(&Gxor_output);

	p1 = (int*)(num1);
	p2 = (int*)(num2);
	p3 = (int*)(&Gxor_output);
	*p3 = (*p1) ^ (*p2);
	p1++;
	p2++;
	p3++;
	*p3 = (*p1) ^ (*p2);
	return Gxor_output;
}

void Compress::count0(unsigned long long* deltaLong, unsigned long long* front0, unsigned long long* back0)
{
	//求前导和后导0
	const int bitsLong = sizeof(deltaLong) * 8;
	*front0 = 0;
	*back0 = 0;
	for (int i = bitsLong / 2 - 1; i >= 0; --i)
	{
		int ch = (*((int*)deltaLong + 1) >> i) & 0x01;
		if (ch == 0) { *front0 += 1; }
		else { break; }
	}
	if (*front0 == 32)
	{
		for (int i = bitsLong / 2 - 1; i >= 0; --i)
		{
			int ch = (*((int*)deltaLong) >> i) & 0x01;
			if (ch == 0) { *front0 += 1; }
			else { break; }
		}
	}

	for (int i = 0; i < bitsLong / 2; i++)
	{
		int ch = (*((int*)deltaLong) >> i) & 0x01;
		if (ch == 0) { *back0 += 1; }
		else { break; }
	}
	if (*back0 == 32)
	{
		for (int i = 0; i < bitsLong / 2; i++)
		{
			int ch = (*((int*)deltaLong + 1) >> i) & 0x01;
			if (ch == 0) { *back0 += 1; }
			else { break; }
		}
	}
}
