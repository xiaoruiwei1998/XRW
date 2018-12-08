#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<cmath>
#include<fstream>
#include<iomanip>
#include "benchmark.h"
#include "Algorithm.h"
using namespace std;

/*******主函数*******/
int main()
{
	/*
	ofstream output;
	output.open("dataABC.txt");
	*/

	srand((unsigned)time(NULL));
	initilize();//初始化
	MemorizeBestSource();//保存最好的蜜源
		
	//主要的循环
	int gen=0;
	while(gen<maxCycle)
	{
		sendEmployedBees();
			
		CalculateProbabilities();
			
		sendOnlookerBees();
			
		MemorizeBestSource();
			
		sendScoutBees();
			
		MemorizeBestSource();
 
		//output<<setprecision(30)<<BestSource.fitness<<endl;
			
		gen++;
	}
	
	//output.close();
	cout<<"运行结束!!"<<endl;
	return 0;
}
 
/*****函数的实现****/
double random(double start, double end)//随机产生区间内的随机数
{	
	return start+(end-start)*rand()/(RAND_MAX + 1.0);
}
 
void initilize()//初始化参数
{
	int i,j;
	for (i=0;i<FoodNumber;i++)
	{
		for (j=0;j<D;j++)
		{
			NectarSource[i].dim[j]=random(LOWBOUND[j],UPBOUND[j]);
			EmployedBee[i].dim[j]=NectarSource[i].dim[j];
			OnLooker[i].dim[j]=NectarSource[i].dim[j];
			BestSource.dim[j]=NectarSource[0].dim[j];
		}
		/****蜜源的初始化*****/
		NectarSource[i].fitness=calculationfitness(NectarSource[i]);
		NectarSource[i].F=calculationF(NectarSource[i].fitness);
		NectarSource[i].CR=0;
		NectarSource[i].trail=0;
		/****采蜜蜂的初始化*****/
		EmployedBee[i].fitness=NectarSource[i].fitness;
		EmployedBee[i].F=NectarSource[i].F;
		EmployedBee[i].CR=NectarSource[i].CR;
		EmployedBee[i].trail=NectarSource[i].trail;
		/****观察蜂的初始化****/
		OnLooker[i].fitness=NectarSource[i].fitness;
		OnLooker[i].F=NectarSource[i].F;
		OnLooker[i].CR=NectarSource[i].CR;
		OnLooker[i].trail=NectarSource[i].trail;
	}
	/*****最优蜜源的初始化*****/
	BestSource.fitness=NectarSource[0].fitness;
	BestSource.F=NectarSource[0].F;
	BestSource.CR=NectarSource[0].CR;
	BestSource.trail=NectarSource[0].trail;
}
 
double calculationfitness(individual bee)//计算真实的函数值
{
	double fitness=0;
	/******测试函数1******/


	return fitness;
}
 
double calculationF(double fitness)//计算适应值
{
	double FResult=0;
	if (fitness>=0)
	{
		FResult=1/(fitness+1);
	}
    else
	{
		FResult=1+abs(fitness);
	}
	return FResult;
}
 
void sendEmployedBees()//修改采蜜蜂的函数
{
	int i,j,k;
	int param2change;//需要改变的维数
	double Rij;//[-1,1]之间的随机数
	for (i=0;i<FoodNumber;i++)
	{
		
		param2change=(int)random(0,D);//随机选取任意一维需要改变的维数
 
		/******选取不等于i的k********/
		while (1)
		{
			k=(int)random(0,FoodNumber);
			if (k!=i)
				break;
		}
 
		for (j=0;j<D;j++)
		{
			EmployedBee[i].dim[j]=NectarSource[i].dim[j];//一组解向量置为相等
		}
 
		/*******采蜜蜂去更新信息*******/
		Rij=random(-1,1);
		EmployedBee[i].dim[param2change]=NectarSource[i].dim[param2change]+Rij*(NectarSource[i].dim[param2change]-NectarSource[k].dim[param2change]);
		/*******判断是否越界********/
		if (EmployedBee[i].dim[param2change]>UPBOUND[param2change])
		{
			EmployedBee[i].dim[param2change]=UPBOUND[param2change];
		}
		if (EmployedBee[i].dim[param2change]<LOWBOUND[param2change])
		{
			EmployedBee[i].dim[param2change]=LOWBOUND[param2change];
		}
		EmployedBee[i].fitness=calculationfitness(EmployedBee[i]);
		EmployedBee[i].F=calculationF(EmployedBee[i].fitness);
 
		/******贪婪选择策略*******/
 		if (EmployedBee[i].fitness<NectarSource[i].fitness)
 		{
 			for (j=0;j<D;j++)
 			{
 				NectarSource[i].dim[j]=EmployedBee[i].dim[j];
 			}
			NectarSource[i].trail=0;
			NectarSource[i].fitness=EmployedBee[i].fitness;
			NectarSource[i].F=EmployedBee[i].F;
 		}else
		{
			NectarSource[i].trail++;
		}
	}
}
 
void CalculateProbabilities()//计算轮盘赌的选择概率（选择策略可以不同，比如p = fit[i]/fit[1]+...+fit[n]）
{
	int i;
	double maxfit;
	maxfit=NectarSource[0].F;
	for (i=1;i<FoodNumber;i++)
	{
		if (NectarSource[i].F>maxfit)
			maxfit=NectarSource[i].F;
	}
	
	for (i=0;i<FoodNumber;i++)
	{
		NectarSource[i].CR=(0.9*(NectarSource[i].F/maxfit))+0.1;//？？？
    }
}
 
void sendOnlookerBees()//采蜜蜂与观察蜂交流信息，观察蜂更改信息
{
	int i,j,t,k;
	double R_choosed;//被选中的概率
	int param2change;//需要被改变的维数
	double Rij;//[-1,1]之间的随机数
	i=0;
	t=0;
	while(t<FoodNumber)
	{
        R_choosed=random(0,1);
        if(R_choosed<NectarSource[i].CR)//根据被选择的概率选择
        {        
			t++;
			param2change=(int)random(0,D);
			
			/******选取不等于i的k********/
			while (1)
			{
				k=(int)random(0,FoodNumber);
				if (k!=i)
				{
					break;
				}
			}
 
			for(j=0;j<D;j++)
			{
				OnLooker[i].dim[j]=NectarSource[i].dim[j];
			}
			
			/****更新******///在已有解空间code[i]~code[k]附近求解
			Rij=random(-1,1);
			OnLooker[i].dim[param2change]=NectarSource[i].dim[param2change]+Rij*(NectarSource[i].dim[param2change]-NectarSource[k].dim[param2change]);
			/*******判断是否越界*******/
			if (OnLooker[i].dim[param2change]<LOWBOUND[param2change])
			{
				OnLooker[i].dim[param2change]=LOWBOUND[param2change];
			}
			if (OnLooker[i].dim[param2change]>UPBOUND[param2change])
			{	
				OnLooker[i].dim[param2change]=UPBOUND[param2change];
			}
			OnLooker[i].fitness=calculationfitness(OnLooker[i]);
			OnLooker[i].F=calculationF(OnLooker[i].fitness);
			
			/****贪婪选择策略******/
			if (OnLooker[i].fitness<NectarSource[i].fitness)
			{
				for (j=0;j<D;j++)
				{
					NectarSource[i].dim[j]=OnLooker[i].dim[j];
				}
				NectarSource[i].trail=0;
				NectarSource[i].fitness=OnLooker[i].fitness;
				NectarSource[i].F=OnLooker[i].F;
			}else
			{
				NectarSource[i].trail++;
			}
        } 
        i++;
        if (i==FoodNumber)
		{
			i=0;
		}
	}
}
 
/*******只有一只侦查蜂**********/
void sendScoutBees()//判断是否有侦查蜂的出现，有则重新生成蜜源（侦查蜂只有在试验次数>limit时出现）
{
	int maxtrialindex,i,j;
	double R;//[0,1]之间的随机数
	//maxtrialindex=0;
	for (i=0;i<FoodNumber;i++)
	{
        if(NectarSource[maxtrialindex].trail>=limit)//从循环外搬到循环内，防止该组数据中有一个以上试验次数超过limit的蜜源
        {
            /*******重新初始化*********/
            for (j=0;j<D;j++)
            {
                R=random(0,1);
                NectarSource[maxtrialindex].dim[j]=LOWBOUND[j]+R*(UPBOUND[j]-LOWBOUND[j]);
            }
            NectarSource[maxtrialindex].trail=0;
            NectarSource[maxtrialindex].fitness=calculationfitness(NectarSource[maxtrialindex]);
            NectarSource[maxtrialindex].F=calculationF(NectarSource[maxtrialindex].fitness);
        }
	}
}
 
void MemorizeBestSource()//保存最优的蜜源
{
	int i,j;
	for (i=1;i<FoodNumber;i++)
	{
		if (NectarSource[i].fitness<BestSource.fitness)
		{
			for (j=0;j<D;j++)
			{
				BestSource.dim[j]=NectarSource[i].dim[j];
			}
			BestSource.fitness=NectarSource[i].fitness;
		}
	}
}