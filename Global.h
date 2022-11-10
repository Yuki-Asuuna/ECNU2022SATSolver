//
// Created by 俊枫957 on 2022/11/9.
//

#ifndef SATSOLVER_GLOBAL_H
#define SATSOLVER_GLOBAL_H


#include <iostream>
#include <fstream>
#include <stdio.h>
#include <ctime>

#define TRUE 1
#define FALSE 0
typedef int status;
using namespace std;

//存储SAT公式的相关结构，每个SATList是一个子句，每个SATNode是每个SATList（子句）中的文字
typedef struct SATNode {
    int data;  //数据域
    SATNode *next;
} SATNode;

typedef struct SATList {
    SATNode *first;  //该子句第一个布尔变量数据结点
    SATList *next; // 下一个子句
} SATList;

extern int Varnumber;
extern int clauseCount;

//函数定义
void Start();  //初始选择输入文件

SATList *CreateClauseFromCNF(const char *fileName);  //从DIMACS读入文件
void ShowClauses(SATList *cnf);    //显示从DIMACS读入的子句集

int IsUnitClause(SATNode *cnf); //判断是否单位子句
status IsEmptyClause(SATList *LIST);//判断是否含有空子句
int EvaluateLiteral(SATNode *cnf, int v[]); //根据赋值得到子句的真假
int DeleteClause(SATList *&cnf, SATList *&root); //删除子句
int RemoveNode(SATNode *&cnf, SATNode *&head); //删除指定子句中的指定文字
int AddClause(SATList *cnf, SATList *&root);//添加子句
void destroyClause(SATList *&cnf);//释放子句空间
//void CopyClause(SATList*& a, SATList* b); //子句的复制，将子句b复制给子句a
int DPLLSolver(SATList *&cnf, int value[]);

#endif //SATSOLVER_GLOBAL_H
