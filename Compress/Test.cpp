#include "Compress.h"
#include <ctime>
#include <iostream>

int main()
{

	Compress *object = new Compress();
	unsigned char* data;				//ѹ�����ֽ����Ŀ�ָ��
	int length = 0;						//ѹ�����ֽ����ĳ���
	const int count = 10000;			//�Զ���ÿ������������������Ŀ
	int decount = 0;					//��ѹ���ֽ����ĳ���
	int lastprint = 10;					//��ʾ����ĵ���nλ����

	//����bool�����ݵ�ѹ������ѹ
	cout << "\nbool�Ͳ���\nԭ����\t";
	bool arr_bool[count];				//����һ����bool������
	for (int i = 0; i < count; i++) {	
		arr_bool[i] = rand() % 2;		//�������count������
		if (count - i - 1 < lastprint) {
			cout << arr_bool[i];
		}								//��ʾ������������n�����ݣ�����ѹ������������ͬ��
										//��˵��ѹ������ѹ��������
	}
	bool boolint = object->compress(count, arr_bool, &data, length);
	//ѹ��(�����size��bool�����飬ѹ�����ֽ����ĵ�ַ��ѹ�����ֽ�������)
	cout << "\nѹ����\t"<<((float)length/ (float)count);
	cout <<"\n��ѹ\t";
	bool *output_bool;					//��ѹ�������
	object->decompress(data, length, &output_bool, decount);
	//��ѹ(ѹ������ֽ�����ѹ�����ֽ������ȣ���ѹ������ݣ���ѹ�����ݵĳ���)
	for (int i = 0; i < decount; i++) {
		if (decount - i - 1 < lastprint) {
			cout << output_bool[i];
		}
	}//��ʾ��ѹ������ĵ���nλ���ݣ�����ѹ���������ԭ������ͬ����˵��ѹ������ѹ��������



	//����float�����ݵ�ѹ������ѹ(�������ݱ�����ʱ�����ݣ��Ǵ����)
	cout << "\n\nfloat�Ͳ���\nԭ����\t";
	float arr_f[count];							//����һ����float������
	for (int i = 0; i < count; i++) {

		//arr_f[i] = rand() / (float)RAND_MAX;			//�������count������
		if (i % 60 == 0) {
			
			arr_f[i] = rand() / float(RAND_MAX);
		}
		else {
			arr_f[i] = arr_f[i - 1] + 0.000001;
		}
		if (count - i - 1 < lastprint) {
			cout << arr_f[i] <<" ";
		}										//��ʾ��������ĵ���nλ���ݣ�����ѹ������������ͬ��
												//��˵��ѹ������ѹ��������
	}
	object->compress(count, arr_f, &data, length);
	//ѹ��(�����size��float�����飬ѹ�����ֽ����ĵ�ַ��ѹ�����ֽ�������)
	cout << "\nѹ����\t" << ((float)length/4 / (float)count);
	cout << "\n��ѹ\t";
	float* outputf;								//��ѹ�������
	object->decompress(data, length, &outputf, decount);
	//��ѹ(ѹ������ֽ�����ѹ�����ֽ������ȣ���ѹ������ݣ���ѹ�����ݵĳ���)
	for (int i = 0; i < decount; i++) {
		if (decount - i - 1 < lastprint) {
			cout << outputf[i]<< " ";
		}
	}//��ʾ��ѹ������ĵ���nλ���ݣ�����ѹ���������ԭ������ͬ����˵��ѹ������ѹ��������



	//����double�����ݵ�ѹ������ѹ(�������ݱ�����ʱ�����ݣ��Ǵ����)
	cout << "\n\ndouble�Ͳ���\nԭ����\t";
	double arr_d[count];						//����һ����double������
	for (int i = 0; i < count; i++) {
		arr_d[i] = rand()/(float)RAND_MAX;				//�������count������
		if (count - i - 1 < lastprint) {
			cout << arr_d[i]<<" ";
		}//��ʾ��������ĵ���nλ���ݣ�����ѹ������������ͬ����˵��ѹ������ѹ��������
	}
	object->compress(count, arr_d, &data, length);
	//ѹ��(�����size��double�����飬ѹ�����ֽ����ĵ�ַ��ѹ�����ֽ�������)
	cout << "\nѹ����\t" << ((float)length /8/ (float)count);
	double* outputd;							//��ѹ�������
	object->decompress(data, length, &outputd, decount);
	//��ѹ(ѹ������ֽ�����ѹ�����ֽ������ȣ���ѹ������ݣ���ѹ�����ݵĳ���)
	cout << "\n��ѹ\t";
	for (int i = 0; i < decount; i++) {
		//if(outputd[i]!=arr_d[i]){ cout << "\n" << outputd[i] << "\t" << arr_d[i] << "\n"; }
		if (decount - i - 1< lastprint) {
			cout << outputd[i]<< " ";
		}
	}//��ʾ��ѹ������ĵ���nλ���ݣ�����ѹ���������ԭ������ͬ����˵��ѹ������ѹ��������



	//����int�����ݵ�ѹ������ѹ
	cout << "\n\nint�Ͳ���\nԭ����\t";
	int arr_int[count];						//����һ����int������
	for (int i = 0; i < count; i++) {		//�������count������
		if (i % 2 == 0) {
			arr_int[i] = rand()%1000;
		}
		else {
			arr_int[i] = rand() % 1000 *(-1);
		}
		if (count - i - 1 < lastprint) {
			cout << arr_int[i]<<" ";
		}//��ʾ��������ĵ���nλ���ݣ�����ѹ������������ͬ����˵��ѹ������ѹ��������
	}
	object->compress(count, arr_int, &data, length);
	//ѹ��(�����size��int�����飬ѹ�����ֽ����ĵ�ַ��ѹ�����ֽ�������)
	cout << "\nѹ����\t" << ((float)length/4 / (float)count);
	cout << "\n��ѹ\t";
	int* outputi;							//��ѹ�������
	object->decompress(data, length, &outputi, decount);
	//��ѹ(ѹ������ֽ�����ѹ�����ֽ������ȣ���ѹ������ݣ���ѹ�����ݵĳ���)
	for (int i = 0; i < decount; i++) {
		if (decount - i - 1 < lastprint) {
			cout << outputi[i]<< " ";
		}
	}//��ʾ��ѹ������ĵ���nλ���ݣ�����ѹ���������ԭ������ͬ����˵��ѹ������ѹ��������



	//����timestamp(int)�����ݵ�ѹ������ѹ
	cout << "\n\ntimestamp(int)�Ͳ���\nԭ����\t";
	int arr_time[count];					//����һ����int������
	for (int i = 0; i < count; i++) {		//�������count������
		arr_time[i] = time(NULL)+rand()%5;
		if (decount - i - 1 < lastprint) {
			cout << arr_time[i] << " ";
		}//��ʾ��������ĵ���nλ���ݣ�����ѹ������������ͬ����˵��ѹ������ѹ��������
	}
	object->timecompress(count, arr_time, &data, length);
	//ѹ��(�����size��int�����飬ѹ�����ֽ����ĵ�ַ��ѹ�����ֽ�������)
	cout << "\nѹ����\t" << ((float)length /4/ (float)count);
	cout << "\n��ѹ\t";
	int* outputT;							//��ѹ�������
	object->timedecompress(data, length, &outputT, decount);
	//��ѹ(ѹ������ֽ�����ѹ�����ֽ������ȣ���ѹ������ݣ���ѹ�����ݵĳ���)
	for (int i = 0; i < decount; i++) {
		if (decount - i - 1 < lastprint) {
			cout << outputT[i] << " ";
		}
	}//��ʾ��ѹ������ĵ���nλ���ݣ�����ѹ���������ԭ������ͬ����˵��ѹ������ѹ��������


	cout << "\n";
	system("pause");
	return 0;
}