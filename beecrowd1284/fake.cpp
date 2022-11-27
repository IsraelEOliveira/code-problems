#include<bits/stdc++.h>
using namespace std;
struct NODE {
    int rep;
    int cross;
    bool flag;
    NODE *next[26];
};
typedef NODE Trie;
NODE *newnode(){
    Trie *tmp;
    tmp= new Trie;
    for(int i=0;i<26;i++)
        tmp->next[i]=NULL;
    tmp->flag=0;
    tmp->cross=0;
    tmp->rep=0;
    return tmp;
}
void Insert ( Trie *rt, char *s ) {
    int len = strlen ( s );
    int idx;
    for ( int i = 0; i < len; i++ ) {
        idx = s[i] - 'a';
        if(rt->next[idx] == NULL)
        {
            rt->next[idx] = newnode() ;
            rt->cross++;
        }
        rt=rt->next[idx];
        rt->rep++;
    }
    rt->flag=1;
}
double dfs(Trie *rt){
    double ret=0;
    for(int i=0;i<26;i++){
        if(rt->next[i]!=NULL){
            ret+=dfs(rt->next[i]);
        }
    }
    if(rt->cross>1){
        ret+=rt->rep;
        if(rt->flag==1){
            ret--;
        }
    }else if(rt->flag==1){
        ret+=rt->rep-1;
    }
    delete rt;
    return ret;
}
int main() {

    int n;
    char str[100];
    while ( scanf ( "%d", &n ) != EOF ) {
        Trie *root;
        root=newnode();
        for ( int i = 0; i < n; i++ ) {
            scanf ( "%s", str );
            Insert ( root, str );
        }
        double ans=dfs(root);
        printf("%.2lf\n",(n+ans)/n*1.0);
    }
    return 0;
}