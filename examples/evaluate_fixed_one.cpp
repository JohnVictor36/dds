#include "evaluate.h"
int main(){
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    srand(time(0));
    SetMaxThreads(0);
    ddTableDeal tableDeal;
    ddTableResults table;
    //printf("Please input the number of decks per testcase.\n");
    int tms;cin>>tms;
    //printf("Please input the number of testcases.\n");
    int _;cin>>_;while(_--){
        int res=0;
        vector<int> a=readdeck();
        tableDeal=vectortodeal(a);
        table=genexpectedresult(tableDeal,tms);
        //PrintHand(line, tableDeal.cards);
        //PrintTable(&table);
        PrintTable(&table,tms);
    }
	return 0;
}
