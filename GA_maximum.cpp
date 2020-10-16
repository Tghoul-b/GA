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
const int N = 9999999;//随机数位数 
const int maxN = 100;
int n = 10;//群体规模
double a[maxN];//群体数组
double  temp[maxN];//用于记录每次更新后的种群
double ans[maxN];//适应值数组
int loop_num = 10000;//迭代次数
double crossover = 0.88;//染色体交配概率
double mutation = 0.1;//染色体变异概率
double test_function(double x)
{
	double d = exp(-pow(x - 0.1, 2))*pow(sin(6 * pi*pow(x, 0.75)), 2);
	return d;
}
double get_rand()//生成0-1之间的随机数 
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
	double  best = -1;//best染色体
	double best_eval = -1;//best结果
	for (int t = 0; t < loop_num; t++)
	{
		double  sum = 0;
		for (int i = 1; i <= n; i++)
		{
			ans[i] = test_function(a[i]);//计算适应值
			sum += ans[i];//求出适应值的和
			if (ans[i] > best_eval)
			{
				best_eval = ans[i];
				best = a[i];
			}
		}
		for (int i = 1; i <= n; i++)
		{
			ans[i] /= sum;//计算每个染色体适应值同群体适应值总和的比
		}
		for (int c = 1; c <= n; c++)//n次选择(轮盘赌算法)
		{
			double r = get_rand();
			int i = 1;
			double  sum = ans[i];
			while (i <= n && sum < r) i++, sum += ans[i];//这一次选出的是i
			temp[c] = a[i];
		}
		memcpy(a, temp, sizeof temp);//更新后的种群作为新的种群
		//一个染色体只有一位，怎么交配啊？
		for (int i = 1; i <= n; i++)
		{
			if (get_rand() < mutation)
				a[i] = get_rand();
		}
	}
	cout << "使得表达式取到最大值时x的值:"<<best << endl;
	return best_eval;
}
int main()
{
	srand((unsigned)time(NULL));
	cout <<"表达式最大的值:" <<solve() << endl;
	system("pause");
}
