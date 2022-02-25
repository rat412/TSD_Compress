#include "Compress.h"
#include <ctime>
#include <iostream>

int main()
{

	Compress *object = new Compress();
	unsigned char* data;				//压缩后字节流的空指针
	int length = 0;						//压缩后字节流的长度
	const int count = 10000;			//自定义每个数组中生成数据数目
	int decount = 0;					//解压后字节流的长度
	int lastprint = 10;					//显示数组的倒数n位数据

	//测试bool型数据的压缩及解压
	cout << "\nbool型测试\n原数据\t";
	bool arr_bool[count];				//定义一个空bool型数组
	for (int i = 0; i < count; i++) {	
		arr_bool[i] = rand() % 2;		//随机生成count个数据
		if (count - i - 1 < lastprint) {
			cout << arr_bool[i];
		}								//显示输入数组的最后n个数据，若解压后的数组与此相同，
										//则说明压缩、解压正常运行
	}
	bool boolint = object->compress(count, arr_bool, &data, length);
	//压缩(数组的size，bool型数组，压缩后字节流的地址，压缩后字节流长度)
	cout << "\n压缩率\t"<<((float)length/ (float)count);
	cout <<"\n解压\t";
	bool *output_bool;					//解压后的数据
	object->decompress(data, length, &output_bool, decount);
	//解压(压缩后的字节流，压缩后字节流长度，解压后的数据，解压后数据的长度)
	for (int i = 0; i < decount; i++) {
		if (decount - i - 1 < lastprint) {
			cout << output_bool[i];
		}
	}//显示解压后数组的倒数n位数据，若解压后的数组与原数据相同，则说明压缩、解压正常运行



	//测试float型数据的压缩及解压(输入数据必须是时序数据，非纯随机)
	cout << "\n\nfloat型测试\n原数据\t";
	float arr_f[count];							//定义一个空float型数组
	for (int i = 0; i < count; i++) {

		//arr_f[i] = rand() / (float)RAND_MAX;			//随机生成count个数据
		if (i % 60 == 0) {
			
			arr_f[i] = rand() / float(RAND_MAX);
		}
		else {
			arr_f[i] = arr_f[i - 1] + 0.000001;
		}
		if (count - i - 1 < lastprint) {
			cout << arr_f[i] <<" ";
		}										//显示输入数组的倒数n位数据，若解压后的数组与此相同，
												//则说明压缩、解压正常运行
	}
	object->compress(count, arr_f, &data, length);
	//压缩(数组的size，float型数组，压缩后字节流的地址，压缩后字节流长度)
	cout << "\n压缩率\t" << ((float)length/4 / (float)count);
	cout << "\n解压\t";
	float* outputf;								//解压后的数据
	object->decompress(data, length, &outputf, decount);
	//解压(压缩后的字节流，压缩后字节流长度，解压后的数据，解压后数据的长度)
	for (int i = 0; i < decount; i++) {
		if (decount - i - 1 < lastprint) {
			cout << outputf[i]<< " ";
		}
	}//显示解压后数组的倒数n位数据，若解压后的数组与原数据相同，则说明压缩、解压正常运行



	//测试double型数据的压缩及解压(输入数据必须是时序数据，非纯随机)
	cout << "\n\ndouble型测试\n原数据\t";
	double arr_d[count];						//定义一个空double型数组
	for (int i = 0; i < count; i++) {
		arr_d[i] = rand()/(float)RAND_MAX;				//随机生成count个数据
		if (count - i - 1 < lastprint) {
			cout << arr_d[i]<<" ";
		}//显示输入数组的倒数n位数据，若解压后的数组与此相同，则说明压缩、解压正常运行
	}
	object->compress(count, arr_d, &data, length);
	//压缩(数组的size，double型数组，压缩后字节流的地址，压缩后字节流长度)
	cout << "\n压缩率\t" << ((float)length /8/ (float)count);
	double* outputd;							//解压后的数据
	object->decompress(data, length, &outputd, decount);
	//解压(压缩后的字节流，压缩后字节流长度，解压后的数据，解压后数据的长度)
	cout << "\n解压\t";
	for (int i = 0; i < decount; i++) {
		//if(outputd[i]!=arr_d[i]){ cout << "\n" << outputd[i] << "\t" << arr_d[i] << "\n"; }
		if (decount - i - 1< lastprint) {
			cout << outputd[i]<< " ";
		}
	}//显示解压后数组的倒数n位数据，若解压后的数组与原数据相同，则说明压缩、解压正常运行



	//测试int型数据的压缩及解压
	cout << "\n\nint型测试\n原数据\t";
	int arr_int[count];						//定义一个空int型数组
	for (int i = 0; i < count; i++) {		//随机生成count个数据
		if (i % 2 == 0) {
			arr_int[i] = rand()%1000;
		}
		else {
			arr_int[i] = rand() % 1000 *(-1);
		}
		if (count - i - 1 < lastprint) {
			cout << arr_int[i]<<" ";
		}//显示输入数组的倒数n位数据，若解压后的数组与此相同，则说明压缩、解压正常运行
	}
	object->compress(count, arr_int, &data, length);
	//压缩(数组的size，int型数组，压缩后字节流的地址，压缩后字节流长度)
	cout << "\n压缩率\t" << ((float)length/4 / (float)count);
	cout << "\n解压\t";
	int* outputi;							//解压后的数据
	object->decompress(data, length, &outputi, decount);
	//解压(压缩后的字节流，压缩后字节流长度，解压后的数据，解压后数据的长度)
	for (int i = 0; i < decount; i++) {
		if (decount - i - 1 < lastprint) {
			cout << outputi[i]<< " ";
		}
	}//显示解压后数组的倒数n位数据，若解压后的数组与原数据相同，则说明压缩、解压正常运行



	//测试timestamp(int)型数据的压缩及解压
	cout << "\n\ntimestamp(int)型测试\n原数据\t";
	int arr_time[count];					//定义一个空int型数组
	for (int i = 0; i < count; i++) {		//随机生成count个数据
		arr_time[i] = time(NULL)+rand()%5;
		if (decount - i - 1 < lastprint) {
			cout << arr_time[i] << " ";
		}//显示输入数组的倒数n位数据，若解压后的数组与此相同，则说明压缩、解压正常运行
	}
	object->timecompress(count, arr_time, &data, length);
	//压缩(数组的size，int型数组，压缩后字节流的地址，压缩后字节流长度)
	cout << "\n压缩率\t" << ((float)length /4/ (float)count);
	cout << "\n解压\t";
	int* outputT;							//解压后的数据
	object->timedecompress(data, length, &outputT, decount);
	//解压(压缩后的字节流，压缩后字节流长度，解压后的数据，解压后数据的长度)
	for (int i = 0; i < decount; i++) {
		if (decount - i - 1 < lastprint) {
			cout << outputT[i] << " ";
		}
	}//显示解压后数组的倒数n位数据，若解压后的数组与原数据相同，则说明压缩、解压正常运行


	cout << "\n";
	system("pause");
	return 0;
}