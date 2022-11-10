//
// Created by 俊枫957 on 2022/11/9.
//

#include "Global.h"
//相关全局变量记录
int Varnumber;
int clauseCount;

SATList* CreateClauseFromCNF(const char *fileName){
    FILE* fp;
    char ch;
    int number, i;
    SATList* cnf;
    SATList* lp;
    SATNode* tp;

    if (fopen_s(&fp, fileName, "r")) {
        printf("failed to open the file!\n");
        return 0;
    }
    while ((ch = getc(fp)) == 'c') {
        while ((ch = getc(fp)) != '\n')
            continue;           //弃去一整行
    }   //运行到此，已经读取了字符p
    getc(fp); getc(fp); getc(fp); getc(fp); //弃去cnf三个字母
    fscanf_s(fp, "%d", &Varnumber);    //p后的第1个数值是布尔变元数量
    fscanf_s(fp, "%d", &clauseCount);  //p后的第2个数值是子句数量

    cnf = (SATList*)malloc(sizeof(SATList));
    cnf->next = NULL;
    cnf->first = (SATNode*)malloc(sizeof(SATNode));
    cnf->first->next = NULL;
    lp = cnf;
    tp = cnf->first;

    //创建数量为clauseCount的子句, i为计数器
    for (i = 0; i < clauseCount; i++, lp = lp->next, tp = lp->first)
    {
        fscanf_s(fp, "%d", &number);
        for (; number != 0; tp = tp->next)
        {
            tp->data = number;  //数据域赋值
            tp->next = (SATNode*)malloc(sizeof(SATNode));  //开辟新结点
            fscanf_s(fp, "%d", &number);
            if (number == 0) tp->next = NULL;
        }
        lp->next = (SATList*)malloc(sizeof(SATList));  //开辟新表
        lp->next->first = (SATNode*)malloc(sizeof(SATNode));
        if (i == clauseCount - 1)
        {
            lp->next = NULL;
            break;
        }
    }
    printf("finished reading the cnf file!\n");
    fclose(fp);
    return cnf;
}

void ShowClauses(SATList* cnf){
    SATList* Phead = cnf;
    SATNode* front;
    cout<<"CnfParser result is showed as follows:\n";
    while(Phead != nullptr) {
        front = Phead->first;
        while(front != nullptr) {
            cout<<front->data<<" ";
            front = front->next;
        }
        cout<<endl;
        Phead = Phead->next;
    }

}