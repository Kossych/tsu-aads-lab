#include <iostream>
#include <ctime>
#include <stdbool.h>


using namespace std;

class route
{
    int *r,n;
    public:
    
    route(int nn);
    route(const route &rr);
    
    route&operator =(const route& rr);
    friend ostream&operator <<(ostream &r, route &a);
    
    ~route(){if(r!=NULL) delete[]r; r=NULL;}
    
    int route_price(int **m);
    bool next_route();
    void SortMas(int pos);
};


route::route(int nn)
{
    n=nn;
    r=new int[nn+1];
    for(int i=0;i<nn;i++) r[i]=i+1;
    r[nn]=1;
}

route::route(const route &rr)
{
    n=rr.n;
    r=new int[n+1];
    for(int i=0;i<=n;i++) r[i]=rr.r[i];
}

int route::route_price(int **m)
{
    int weight=0;
    for(int i=1;i<=n;i++) weight=weight+m[r[i-1]-1][r[i]-1];
    return weight;
}

bool route::next_route()
{
    int i,j;
    int a;
    i=n-1;
    j=n;

    while((r[i]>=r[i+1])&&(i>0)) i--; //пункт #1

    if(i==0) return false; //очередной перестановки не существует, выход из цикла

    while((r[i]>=r[j])&&(i<j)) j--; //пункт #2

    if(i!=j)
    { //пункт #3
        a=r[i];
        r[i]=r[j];
        r[j]=a;
    }
    SortMas(i+1); //пункт #4
    return true;
}
    
void route::SortMas(int pos)
{//сортировка массива методом пузырька. подается позиция, после которой выполняется сортировка
    int j=pos;
    int max;
    int a;
    while(j<(n-1)){
        for(int i=pos;i<(n-1);i++){
            if(r[i]>r[i+1]){
                a=r[i+1];
                r[i+1]=r[i];
                r[i]=a;
            }
        }
        j++;
    }
}    

int** CreateM(int n)
{
    srand( time( 0 ) );
    int **m=NULL;
    m=new int*[n];
    for(int i=0;i<n;i++){
        m[i]=new int[n];
        for(int j=0;j<n;j++)
        if(j==i) m[i][j]=0;
        else m[i][j]=1 + rand() % 20;
    }
    return m;
}

ostream&operator <<(ostream &r, route &a)
{
    for(int i=0;i<=a.n;i++) r<<a.r[i]<<" ";
    return r;
}

route& route::operator =(const route& rr)
{
    if(n!=rr.n)
    {
        n=rr.n;
        delete[]r;
        r=new int[n+1];
    } 
    for (int i=0;i<=n;i++) r[i]=rr.r[i];
    return *this; 
}



int main()
{
    setlocale(LC_ALL, "russian");
    int nn=4;
    int minweight=0xFFFFFFF;
    int weight=0;
    bool b=true;
    cout<<"Введите кол-во городов: ";
    cin>>nn;
    int **m=CreateM(nn);
    for(int i=0;i<nn;i++)
    {
        for(int j=0;j<nn;j++) cout<<m[i][j]<<" ";
        cout<<"\n";
    }
    cout<<"\n";
    route A(4);
    route B(4);
    while(b){
        cout<<A;
        weight=A.route_price(m);
        if(minweight>weight)
        {
            minweight=weight;
            B=A;
        }
        cout<<"Вес: "<<weight;
        cout<<"\n";
        b=A.next_route();
    }
    cout<<A<<"Вес минимального пути: "<<minweight;
    system("pause");
}