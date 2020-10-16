#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include<cstring>
#include<math.h>
#include<cmath>
#include<ctime>
#define pi 3.1415926535 
using namespace std;
const int N = 9999999;//�����λ�� 
const int maxN = 100;
int n = 10;//Ⱥ���ģ
double a[maxN];//Ⱥ������
double  temp[maxN];//���ڼ�¼ÿ�θ��º����Ⱥ
double ans[maxN];//��Ӧֵ����
int loop_num = 10000;//��������
double crossover = 0.88;//Ⱦɫ�彻�����
double mutation = 0.1;//Ⱦɫ��������
double test_function(double x)
{
	double d = exp(-pow(x - 0.1, 2))*pow(sin(6 * pi*pow(x, 0.75)), 2);
	return d;
}
double get_rand()//����0-1֮�������� 
{
	long long a_l=rand(); 
	double a = (a_l*a_l) % (N + 1) / (float)(N + 1);
	return a;
}
void init()
{
	for (int i = 1; i <= n; i++)
		a[i] = get_rand();
}
double solve()
{
	init();
	double  best = -1;//bestȾɫ��
	double best_eval = -1;//best���
	for (int t = 0; t < loop_num; t++)
	{
		double  sum = 0;
		for (int i = 1; i <= n; i++)
		{
			ans[i] = test_function(a[i]);//������Ӧֵ
			sum += ans[i];//�����Ӧֵ�ĺ�
			if (ans[i] > best_eval)
			{
				best_eval = ans[i];
				best = a[i];
			}
		}
		for (int i = 1; i <= n; i++)
		{
			ans[i] /= sum;//����ÿ��Ⱦɫ����ӦֵͬȺ����Ӧֵ�ܺ͵ı�
		}
		for (int c = 1; c <= n; c++)//n��ѡ��(���̶��㷨)
		{
			double r = get_rand();
			int i = 1;
			double  sum = ans[i];
			while (i <= n && sum < r) i++, sum += ans[i];//��һ��ѡ������i
			temp[c] = a[i];
		}
		memcpy(a, temp, sizeof temp);//���º����Ⱥ��Ϊ�µ���Ⱥ
		//һ��Ⱦɫ��ֻ��һλ����ô���䰡��
		for (int i = 1; i <= n; i++)
		{
			if (get_rand() < mutation)
				a[i] = get_rand();
		}
	}
	cout << "ʹ�ñ���ʽȡ�����ֵʱx��ֵ:"<<best << endl;
	return best_eval;
}
int main()
{
	srand((unsigned)time(NULL));
	cout <<"����ʽ����ֵ:" <<solve() << endl;
	system("pause");
}
