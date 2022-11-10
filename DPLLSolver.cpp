//
// Created by 俊枫957 on 2022/11/9.
//
#include "Global.h"

//判断子句集中是否有空子句
status IsEmptyClause(SATList *cnf) {
    SATList *lp = cnf;
    while (lp != NULL) {
        if (lp->first == NULL) return 1;
        lp = lp->next;
    }
    return FALSE;
}

//判断当前子句是否为单位子句
int IsUnitClause(SATNode *cnf) {
    if (cnf != NULL && cnf->next == NULL)
        return 1;
    else
        return 0;
}

//根据赋值v得到文字cnf的真假
int EvaluateLiteral(SATNode *cnf, int v[]) {
    SATNode *tp = cnf;
    if (tp != NULL) {
        if (tp->data > 0 && v[tp->data] == 1 ||
            tp->data < 0 && v[-tp->data] == 0)
            return 1;
    }
    return 0;
}

//从子句集中删除指定子句
int DeleteClause(SATList *&cnf, SATList *&root) {
    SATList *lp = root;
    if (lp == cnf) root = root->next;  //删除为头
    else {
        while (lp != NULL && lp->next != cnf) lp = lp->next;
        lp->next = lp->next->next;
    }
    free(cnf);
    cnf = NULL;
    return 1;
}

//在指定的子句中删除指定文字
int RemoveNode(SATNode *&cnf, SATNode *&head) {
    SATNode *lp = head;
    if (lp == cnf) head = head->next;  //删除为头
    else {
        while (lp != NULL && lp->next != cnf) lp = lp->next;
        lp->next = lp->next->next;
    }
    free(cnf);
    cnf = NULL;
    return 1;
}

//在子句集中添加子句
int AddClause(SATList *cnf, SATList *&root) {
    //直接插入在表头
    if (cnf != NULL) {
        cnf->next = root;
        root = cnf;
        return 1;
    }
    return 0;
}

void CopyClause(SATList *&a, SATList *b) {
    SATList *lpa, *lpb;
    SATNode *tpa, *tpb;
    a = (SATList *) malloc(sizeof(SATList));
    a->first = (SATNode *) malloc(sizeof(SATNode));
    a->next = NULL;
    a->first->next = NULL;
    for (lpb = b, lpa = a; lpb != NULL; lpb = lpb->next, lpa = lpa->next) {
        for (tpb = lpb->first, tpa = lpa->first; tpb != NULL; tpb = tpb->next, tpa = tpa->next) {
            tpa->data = tpb->data;
            tpa->next = (SATNode *) malloc(sizeof(SATNode));
            tpa->next->next = NULL;
            if (tpb->next == NULL) {
                free(tpa->next);
                tpa->next = NULL;
            }
        }
        lpa->next = (SATList *) malloc(sizeof(SATList));
        lpa->next->first = (SATNode *) malloc(sizeof(SATNode));
        lpa->next->next = NULL;
        lpa->next->first->next = NULL;
        if (lpb->next == NULL) {
            free(lpa->next->first);
            free(lpa->next);
            lpa->next = NULL;
        }
    }
}

void destroyClause(SATList *&cnf) {
    SATList *lp1, *lp2;
    SATNode *tp1, *tp2;
    for (lp1 = cnf; lp1 != NULL; lp1 = lp2) {
        lp2 = lp1->next;
        for (tp1 = lp1->first; tp1 != NULL; tp1 = tp2) {
            tp2 = tp1->next;
            free(tp1);
        }
        free(lp1);
    }
    cnf = NULL;
}

int DPLLSolver(SATList *&cnf, int value[]) {
    SATList *tp = cnf, *lp = cnf, *sp;
    SATNode *dp;
    int *count, i, MaxWord, max, re; //count记录每个文字出现次数,MaxWord记录出现最多次数的文字
    count = (int *) malloc(sizeof(int) * (Varnumber * 2 + 1));
    FIND:
    while (tp != NULL && IsUnitClause(tp->first) == 0) tp = tp->next;  //找到表中的单子句
    if (tp != NULL) {
        //单子句规则简化
        if (tp->first->data > 0) value[tp->first->data] = 1;
        else value[-tp->first->data] = 0;
        re = tp->first->data;
        for (lp = cnf; lp != NULL; lp = sp) {
            sp = lp->next;

            //查找含有核心文字的句子
            for (dp = lp->first; dp != NULL; dp = dp->next) {
                if (dp->data == re) {
                    DeleteClause(lp, cnf);  //删除子句，简化式子
                    break;
                }
                if (dp->data == -re) {
                    RemoveNode(dp, lp->first);  //删除文字，简化子句
                    break;
                }
            }
        }
        //极简化规则简化后
        if (cnf == NULL) {
            free(count);
            return 1;
        } else if (IsEmptyClause(cnf)) {
            free(count);
            destroyClause(cnf);
            return 0;
        }
        tp = cnf;
        goto FIND;  //继续简化
    }
    for (i = 0; i <= Varnumber * 2; i++) count[i] = 0;  //初始化

    //计算子句中各文字出现次数
    for (lp = cnf; lp != NULL; lp = lp->next) {
        for (dp = lp->first; dp != NULL; dp = dp->next) {
            if (dp->data > 0) count[dp->data]++;
            else count[Varnumber - dp->data]++;
        }
    }
    max = 0;

    //找到出现次数最多的正文字
    for (i = 2; i <= Varnumber; i++) {
        if (max < count[i]) {
            max = count[i];
            MaxWord = i;
        }
    }

    if (max == 0) {
        //若没有出现正文字,找到出现次数最多的负文字
        for (i = Varnumber + 1; i <= Varnumber * 2; i++) {
            if (max < count[i]) {
                max = count[i];
                MaxWord = -i;
            }
        }
    }
    free(count);
    lp = (SATList *) malloc(sizeof(SATList));
    lp->first = (SATNode *) malloc(sizeof(SATNode));
    lp->first->data = MaxWord;
    lp->first->next = NULL;
    lp->next = NULL;
    CopyClause(tp, cnf);
    AddClause(lp, tp);
    if (DPLLSolver(tp, value) == 1) return 1;  //在第一分支中搜索
    destroyClause(tp);
    lp = (SATList *) malloc(sizeof(SATList));
    lp->first = (SATNode *) malloc(sizeof(SATNode));
    lp->first->data = -MaxWord;
    lp->first->next = NULL;
    lp->next = NULL;
    AddClause(lp, cnf);
    re = DPLLSolver(cnf, value); //回溯到执行分支策略的初态进入另一分支
    destroyClause(cnf);
    return re;
}