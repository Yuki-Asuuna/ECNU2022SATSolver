#include <iostream>
#include "Global.h"
int main() {
    int result;
    int *value;
    Start();
    SATList* cnflist = NULL;
    cnflist= CreateClauseFromCNF("test.cnf");
    //ShowClauses(cnflist);

    value = (int*)malloc(sizeof(int) * (Varnumber + 1));
    for (int i = 1; i <= Varnumber; i++) value[i] = 1;  //初始化，均赋为1
    result= DPLLSolver(cnflist,value);
    if (result == 1)
    {
        for (int i = 1; i <= Varnumber; i++)
        {
            if (value[i] == 1) printf("%d ", i);
            else printf("%d ", -i);
        }
        printf("\n");
    }
    return 0;
}
