#include<bits/stdc++.h>
#include "../include/dll.h"
#include "hands.h"
using namespace std;
#define rep(i,a,b) for(int i=(a);i<=(b);i++)

typedef long long ll;
#define pb push_back
#define pii pair<int,int>
#define mp make_pair
#define fi first
#define se second

char line[80]="Deck";
ddTableDeal randgendeal(){
    vector<int>a;a.resize(52);
    rep(i,0,51)a[i]=i;
    random_shuffle(a.begin(),a.end());
    //rep(i,0,51)printf("%d ",a[i]);puts("");
    ddTableDeal tableDeal;
    rep(i,0,3)rep(j,0,3)tableDeal.cards[i][j]=0;
    rep(i,0,51)tableDeal.cards[i/13][a[i]/13]|=4<<(a[i]%13);
    //rep(i,0,3)rep(j,0,3)printf("%d ",tableDeal.cards[i][j]);puts("");
    return tableDeal;
}

ddTableDeal vectortodeal(vector<int>a){
    //It should be garenteed that the size of a is greater than 52,and we will use the first 52 bits.
    ddTableDeal tableDeal;
    rep(i,0,3)rep(j,0,3)tableDeal.cards[i][j]=0;
    rep(i,0,51)tableDeal.cards[i/13][a[i]/13]|=4<<(a[i]%13);
    //rep(i,0,3)rep(j,0,3)printf("%d ",tableDeal.cards[i][j]);
    return tableDeal;
}
vector<int> dealtovector(ddTableDeal tabledeal){
    vector<int>a;a.resize(52);
    rep(i,0,3){
        int cnt=0;
        rep(j,0,3){
            int u=tabledeal.cards[i][j];
            rep(k,0,12)if(u&(4<<k)){
                ++cnt;
                a[i*13+cnt-1]=j*13+k;
            }
        }
    }
    return a;
}
ddTableResults genexpectedresult(ddTableDeal tabledeal,int tot_decks){
    /*
    We only fix the deal of the first person,
    and randomly generate the decks of the others for 100 deals. 
    The result is printed as a four-digit integer,
    representing the sum of all the 100 deals.
    */
    ddTableResults result;
    rep(i,0,4)rep(j,0,3)result.resTable[i][j]=0;
    vector<int>a=dealtovector(tabledeal);
    //rep(i,0,51)printf("%d ",a[i]);puts("");
    rep(tms,1,tot_decks){
        random_shuffle(a.begin()+13,a.end());
        //rep(i,0,51)printf("%d ",a[i]);puts("");
        ddTableDeal tableDeal=vectortodeal(a);
        ddTableResults table;
        int res=CalcDDtable(tableDeal,&table);
        assert(res==1);
        rep(i,0,4)rep(j,0,3)result.resTable[i][j]+=table.resTable[i][j];
        //PrintHand(line, tableDeal.cards);
        //PrintTable(&table);
        //PrintTable(&result);
        if(tms%100==0)cerr<<tms/100<<"%"<<endl;
    }
    return result;
}
vector<int> complementdeck(vector<int>a){
    //We will complement a vector of size 13 to one with size 52.
    a.resize(52);
    vector<int>vis;vis.resize(52);
    rep(i,0,12)vis[a[i]]=1;
    int cnt=12;
    rep(i,0,51)if(!vis[i])a[++cnt]=i;
    assert(cnt=51);
    //rep(i,0,51)printf("%d ",a[i]);puts("");
    return a;
}
char col[4][10]={"Spades","Hearts","Diamonds","Clubs"},coll[4]={'S','H','D','C'};
int parse(char x){
    if('2'<=x && x<= '9')return x-'2';
    if(x=='t'||x=='T')return 8;
    if(x=='j'||x=='J')return 9;
    if(x=='q'||x=='Q')return 10;
    if(x=='k'||x=='K')return 11;
    if(x=='a'||x=='A')return 12;
}
vector<int>parse(string s){
    vector<int>ret;
    if(s[0]=='-')return ret;
    int l=s.length();ret.resize(l);
    rep(i,0,l-1)ret[i]=parse(s[i]);
    //rep(i,0,l-1)printf("%d ",ret[i]);puts("");
    return ret;
}
char rev[13]={'2','3','4','5','6','7','8','9','T','J','Q','K','A'};
vector<int> readdeck(){   
    vector<int>a(13);int cnt=-1;
    /*rep(i,0,3){
        printf("Please input number of %s.\n",col[i]);
        int l;cin>>l;
        printf("Please input your %s deck, ranging from 12 to 0.\n",col[i]);
        rep(j,1,l){
            ++cnt;assert(cnt<=12);cin>>a[cnt];a[cnt]+=i*13;
        }
    }*/
    rep(i,0,3){
        //printf("Please input your %s deck.\n",col[i]);
        string s;cin>>s;
        vector<int>b=parse(s);
        int l=b.size();
        //printf("l=%d\n",l);
        printf("%15s:",col[i]);
        rep(j,0,l-1){
             ++cnt;assert(cnt<=12);
             a[cnt]=b[j];a[cnt]+=i*13;
        }
        sort(b.begin(),b.end());
        reverse(b.begin(),b.end());
        rep(j,0,l-1)printf("%c",rev[b[j]]);
        puts("");
    }
    assert(cnt==12);
    //rep(i,0,12)printf("%d ",a[i]);puts("");
    a=complementdeck(a);
    return a;
}

struct constraints{
    int is_fixed[4];
    int lowest_HCP[4],highest_HCP[4];
    int lowest_suit_amount[4][4],highest_suit_amount[4][4];
    void initialize(){
        is_fixed[0]=1;rep(i,1,3)is_fixed[i]=0;
        rep(i,0,3)lowest_HCP[i]=0,highest_HCP[i]=37;
        rep(i,0,3)rep(j,0,3)lowest_suit_amount[i][j]=0,highest_suit_amount[i][j]=13;
    }
    bool satisfy(vector<int>a){
        //we won't check is_fixed
        assert(a.size()==52);
        //printf("Hello?satisfy\n");
        //rep(i,0,51)printf("%d ",a[i]);puts("");fflush(stdout);
        rep(i,0,3){
            vector<int>suit_amount(4);int HCP=0;
            rep(j,i*13,i*13+12)suit_amount[a[j]/13]++;
            rep(j,i*13,i*13+12)HCP+=max(0,a[j]%13-8);
            //printf("dbg satisfy\n");printf("HCP=%d\n",HCP);printf("Suit amount:%d %d %d %d\n",suit_amount[0],suit_amount[1],suit_amount[2],suit_amount[3]);
            if(HCP<lowest_HCP[i]||HCP>highest_HCP[i])return false;
            rep(j,0,3)if(suit_amount[j]<lowest_suit_amount[i][j]||suit_amount[j]>highest_suit_amount[i][j])return false;
        }
        return true;
    }
};
vector<int> generatesuit(constraints constraints,vector<int>a){
    /*
     It may take a long time to generate a single data with strict constraints,
     as we just simply implement random sampling without any DP optimizations,
     until we find a valid solution.
     For example,if we set the limit if lowest_HCP[1] to 28,
     it may be very hard to find an answer.
     */
    assert(a.size()==52);
    while(1){
        vector<int>pool,ans(52);
        rep(i,0,3)if(!constraints.is_fixed[i])rep(j,i*13,i*13+12)pool.pb(a[j]);
        random_shuffle(pool.begin(),pool.end());int cnt=0;
        rep(i,0,3){
            if(constraints.is_fixed[i])rep(j,i*13,i*13+12)ans[j]=a[j];
            else rep(j,i*13,i*13+12)ans[j]=pool[cnt++];
        }
        if(constraints.satisfy(ans))return ans;
    }
}
void PrintTable(ddTableResults * table,int tms)
{
  printf("%5s %-5s %-5s %-5s %-5s\n",
         "", "North", "South", "East", "West");

  printf("%5s %5.2lf %5.2lf %5.2lf %5.2lf\n",
         "NT",
         1.0*table->resTable[4][0]/tms,
         1.0*table->resTable[4][2]/tms,
         1.0*table->resTable[4][1]/tms,
         1.0*table->resTable[4][3]/tms);

  for (int suit = 0; suit < DDS_SUITS; suit++)
  {
    printf("%5c %5.2lf %5.2lf %5.2lf %5.2lf\n",
           coll[suit],
           1.0*table->resTable[suit][0]/tms,
           1.0*table->resTable[suit][2]/tms,
           1.0*table->resTable[suit][1]/tms,
           1.0*table->resTable[suit][3]/tms);
  }
  printf("\n");
}
