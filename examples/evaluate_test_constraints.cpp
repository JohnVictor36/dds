#include "evaluate.h"
int main(){
    constraints cond;
    cond.initialize();
    cond.is_fixed[0]=0;
    cond.lowest_HCP[0]=15;
    cond.lowest_HCP[2]=15;
    cond.lowest_suit_amount[0][0]=7;
    vector<int>a(52);rep(i,0,51)a[i]=i;
    vector<int>ans;
    rep(tms,1,10){
        ans=generatesuit(cond,a);
        ddTableDeal deal=vectortodeal(ans);
        ddTableResults results;
        CalcDDtable(deal,&results);
        PrintHand(line,deal.cards);
        PrintTable(&results,1);
        ////
    }
    return 0;
}