#ifndef _GLOBAL_H
#define _GLOBAL_H

#define NP 20//种群的规模，采蜜蜂+观察蜂，解的个数
#define D 30//函数维度

//#define MAXFEs 200000
#define SubSetSize 1
const int FoodNumber=NP/2;//食物的数量，为采蜜蜂的数量
const int limit=20;//限度，超过这个限度没有更新采蜜蜂变成侦查蜂(没有更新的数据舍去)
const int maxCycle=10000;//停止条件：蜂群代数达到10000
const double LOWBOUND[21] = {-100,-10,-100,-100,-30,-100,-1.28,-500,-5.12,-32,-600,-50,-50,-65.536,-5,-5,-5,-2,-10,-10,-10};
const double UPBOUND[21] = {100,10,100,100,30,100,1.28,500,5.12,32,600,50,50,65.536,-5,-5,-2,-10,-10,-10};
 
double result[maxCycle]={0};//数组每一位存每一代运算结果

struct individual//BeeGroup
{
	double dim[D];   //dim[D]
	double fitness;  //truefit
	double F; 		 //fitness
	double CR;		 //rfitness
	int trail;		 //表示实验的次数，用于与limit作比较
	int strategy;
};

struct population
{
	individual indiv[NP];
	individual bestIndiv;
};
 
individual NectarSource[FoodNumber];//蜜源，注意：一切的修改都是针对蜜源而言的
individual EmployedBee[FoodNumber];//采蜜蜂
individual OnLooker[FoodNumber];//观察蜂
individual BestSource;//记录最好蜜源
 
/*****函数的声明*****/
double random(double, double);//产生区间上的随机数
void initilize();//初始化参数
double calculationfitness(individual);//计算真实的函数值
double calculationF(double);//计算适应值
void CalculateProbabilities();//计算轮盘赌的概率
void evalueSource();//评价蜜源
void sendEmployedBees();
void sendOnlookerBees();
void sendScoutBees();
void MemorizeBestSource();

#endif