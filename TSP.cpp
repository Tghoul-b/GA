#include<iostream>
#include<algorithm>
#include<cmath>
#include<cstring>
#include<vector>
#include<math.h>
#include<fstream>
#include<string>
#include<time.h>
#include<stdlib.h>
#include<cstdlib>
#include<omp.h>
using namespace std;
#define Mod 999998
const double mating = 0.3;//交配概率
const double variation = 0.5;//变异概率
const int N = 200;//种群规模
int m = 100;//初始化群体规模
int n = 0;//城市的数量
double dist[N][N];//城市之间的距离
int chromosome[N][N];//染色的数量以及大小
int num_kid1[N];//记录交配之后两个孩子中的城市数量 
int num_kid2[N];
int loop_time = 10000;//迭代次数
int temp[N][N];//临时存储染色体的数组
vector<int> kid1, kid2;//记录重复的点
bool st1[N], st2[N];//不要重复加入多的点
double result = 1000000;//存放最小值
int result_array[N];//以及最小值对应的数组
void SplitString(const string& s, vector<string>& v, const string& c)//分割字符串函数
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));
		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}
void get_data()
{
	fill(dist[0], dist[0] + N * N, 1000000);
	fstream in_file("./data/data.txt", ios::in);
	string s;
	vector<string> v;
	while (getline(in_file, s))
	{
		SplitString(s, v, ",");
		int a = stoi(v[0]);
		int b = stoi(v[1]);
		int c = atof(v[2].c_str());
		n = max(n, max(a, b));
		dist[a][b] = dist[b][a] = c;
		v.clear();
	}
}
double get_rand()
{
	long long a = rand()*rand();
	return (double)(a*a % (Mod + 1)) / (Mod + 1);
}
void init()//初始化种群
{
	vector<int> start(n, 0);
	for (int i = 0; i < n; i++)
	{
		start[i] = i + 1;
	}
	int cnt = 0;
	for (int i = 0; i < m; i++)
	{
		random_shuffle(start.begin(), start.end());
		for (int j = 0; j < n; j++)
		{
			chromosome[i][j] = start[j];
		}
	}
}
double get_result(int i)//i代表第几个染色体
{
	double sum = 0;
	for (int j = 0; j < n - 1; j++)
	{
		sum += dist[chromosome[i][j]][chromosome[i][j + 1]];
	}
	sum+=dist[chromosome[i][n-1]][chromosome[i][0]];
	return sum;
}
bool test()
{
	for (int i = 0; i < m; i++)
	{
		int num1[1000] = { 0 };
		for (int j = 0; j < n; j++)
		{
			num1[chromosome[i][j]]++;
			if (num1[chromosome[i][j]] >= 2)
			{
				cout << "第" << i << "个染色体出现了问题" << endl;
				return true;
			}
		}
	}
	return false;
}
void solve()
{
	get_data();//得到数据
	init();//初始化种群

	vector<int> C;//用于存放选出来的交配染色体的下标
	vector<double> sum(m + 5, 0);
	for (int c = 0; c < loop_time; c++)
	{
		double  total = 0;//计算最新
		for (int i = 0; i < m; i++)
		{
			sum[i] = get_result(i);
			total += sum[i];
			if (sum[i] < result)
			{
				result = sum[i];//降低代价值
				memcpy(result_array, chromosome[i], sizeof chromosome[i]);//将更好结果的染色体作为best染色体
			}
		}
		for (int i = 0; i < m; i++)
		{
			sum[i] /= total;
		}
		
		int t = 0;
		for (int i = 0; i < m; i++)//轮盘赌算法,m个种群
		{
			double a = get_rand();
			int j = 0;
			double f = sum[j];

			while (f < a)  j++, f += sum[j];//选出第j条染色体
			memcpy(temp[t], chromosome[j], sizeof chromosome[j]);//将这一条染色体存入临时染色体组中 
			t++;
		}
		memcpy(chromosome, temp, sizeof temp);//将临时存放的值还是赋给原来的数组
		//选出交配个体 
		C.clear();
		for (int i = 0; i < m; i++)
		{
			if (get_rand() < mating)
			{
				C.push_back(i);
			}
		}
		if (C.size() & 1) C.pop_back();//如果选出来的染色体的数目是奇数,则删去最后一个
		random_shuffle(C.begin(), C.end());
		for (int i = 0; i < C.size(); i += 2)
		{
			int fa = C[i], mo = C[i + 1];
			int F = rand() % n;
			int w;
			do
			{
				w = max(rand() % n, F + n / 2);
			} while (w == F);
			if (F > w)
				swap(F, w);
			//cout << F << ' ' << w;
			for (int j = F; j < w; j++)
			{
				swap(chromosome[fa][j], chromosome[mo][j]);//交换两个片段的后两段
			}
			memset(num_kid1, 0, sizeof num_kid1);
			memset(num_kid2, 0, sizeof num_kid2);//先都将其置零
			for (int j = 0; j < n; j++)
			{
				num_kid1[chromosome[fa][j]]++;
				num_kid2[chromosome[mo][j]]++;
			}
			kid1.clear(), kid2.clear();//先将其清空
			memset(st1, 0, sizeof st1);
			memset(st2, 0, sizeof st2);
			for (int j = 0; j < n; j++)//
			{
				if (num_kid1[chromosome[fa][j]] == 2 && !st1[chromosome[fa][j]])
					kid1.push_back(chromosome[fa][j]), st1[chromosome[fa][j]] = 1;
				if (num_kid2[chromosome[mo][j]] == 2&&  !st1[chromosome[mo][j]])
					kid2.push_back(chromosome[mo][j]), st1[chromosome[mo][j]] = 1;
			}
			int p1 = 0, p2 = 0;
			for (int j = F; j < w; j++)
			{
				//cout << ' ' << chromosome[fa][j] << ' ' << chromosome[mo][j]<<' '<<kid1.size()<<' '<< kid2.size();
				if (num_kid1[chromosome[fa][j]] == 2 && p2 < kid2.size())
					chromosome[fa][j] = kid2[p2++];
				if (num_kid2[chromosome[mo][j]] == 2 && p1 < kid1.size())
					chromosome[mo][j] = kid1[p1++];

			}
		}
		//test();
		for (int i = 0; i < m; i++)
		{
			if (get_rand() < variation)//发生变异
			{
				for (int k = 0; k < 1; k++)
				{
					int u, v, w;
					u = rand() % n;
					do {
						v = rand() % n;
					} while (u == v);
					do {
						w = rand() % n;
					} while (w == u || w == v);
					if (u > v)
						swap(u, v);
					if (v > w)
						swap(v, w);
					if (u > v)
						swap(u, v);
					vector<int> vec;
					for (int j = 0; j < u; j++)
						vec.push_back(chromosome[i][j]);
					for (int j = v; j < w; j++)
						vec.push_back(chromosome[i][j]);
					for (int j = u; j < v; j++)
						vec.push_back(chromosome[i][j]);
					for (int j = w; j < n; j++)
						vec.push_back(chromosome[i][j]);
					for (int j = 0; j < n; j++)
					{
						chromosome[i][j] = vec[j];
					}
				}
			}
		}
	}
	
}
int main()
{
	clock_t startTime, endTime;
	startTime = clock();//计时开始
	srand((unsigned)time(NULL));
	for(int i=0;i<3;i++)
	solve();
	for (int i = 0; i < n; i++)
	{
		cout << result_array[i];
			cout << "->";
	}
	cout<<result_array[0];
	cout << endl;
	cout << result << endl;
	endTime = clock();//计时结束
	cout << "The run time is: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
}