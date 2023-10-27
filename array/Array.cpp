#include <iostream>
#include <ctime>
#include <stdbool.h>
#include <iomanip>


using namespace std;

class Array
{ int *a; int m;
public:

Array(int mm = 1, int t = 1, int d = 10);
Array(int *, int);
Array(Array &);
~Array(){if(a!=NULL) delete[]a;}
int Search(int);
Array& operator = (const Array &);
int &operator [](int);
void Shift(int i, int n);
bool Test(); // проверка на упорядоченность по неубыванию
bool operator == (Array&); // равенство элементов массивов (но не порядка)
friend istream & operator >> (istream &, Array &);
friend ostream & operator << (ostream &, Array &);
void SortShell();
void SortShaker();
void SortPyramid();
void SortHoar(int, int);
};

int Array::Search(int key){
    for(int i=0;i<m;i++) if(a[i]==key) return i;
    return -1;
}


Array::Array(int mm, int t, int d){
    if(mm>0) m=mm;
    else m=1;
    a=new int[m];
    if(t==1){
        for(int i=0;i<m;i++) a[i]=rand()%d;
    }
    else if(t==2){
        a[0]=rand()%(d);
        for(int i=1;i<m;i++){
            a[i]=a[i-1]+rand()%(d);
        }
    }
    else if(t==3){
        a[m-1]=rand()%d;
        for(int i=(m-2);i>=0;i--){
            a[i]=a[i+1]+rand()%d;
        }
    }
}


Array::Array(int *A,int mm){
    if(this->a!=A){
        a=new int[mm];
        m=mm;
        for(int i=0;i<mm;i++) a[i]=A[i];
    }
}

Array::Array(Array &A){
    if(this!=&A){
        m=A.m;
        a=new int[m];
        for(int i=0;i<m;i++) a[i]=A.a[i];        
    }
}

Array& Array::operator = (const Array &A){
    if(this!=&A){
        m=A.m;
        a=new int[m];
        for(int i=0;i<m;i++) a[i]=A.a[i];  
        return *this;
    }
    return *this;
}

int &Array::operator [](int count){
    if((count<this->m)&&(count>=0)) 
    return (this->a[count]);
    else{
        cout<<"Элемента с данной позицией не существует";
        return a[0];
    }
}

bool Array::operator == (Array &A){
    if (this!=&A){
        if(m==A.m){
            int *T=new int[m];
            for(int i=0;i<m;i++) T[i]=1;
            for(int i=0;i<m;i++)
                for(int j=0;j<A.m;j++)
                    if((A.a[j]==a[i])&&(T[j]==1))
                        T[j]--;
            for(int i=0;i<m;i++) if(T[i]!=0) return false;
            return true;
        }
        return false;
    }
    return true;
}

ostream & operator << (ostream &out, Array &A){
    for(int i=0;i<A.m;i++) out<<A.a[i]<<' ';//<<"\n";
    out<<'\n';
    return out;
}

istream & operator >> (istream &in, Array &A){
    in>>A.m;
    if(A.a!=NULL) delete[]A.a;
    A.a=new int[A.m];
    for(int i=0;i<A.m;i++) in>>A.a[i];
    return in;
}

bool Array::Test(){
    if((this->a!=NULL)&&(this->m>0)){
        for(int i=0;i<(m-1);i++)
        if(a[i]>a[i+1]) return false;
        return true;
    }
    return false;
} 

void Array::SortShell(){
    int x;
    int k;
    for(int h=m/2;h>0;h/=2){
       for(int b=0;b<h;b++){
           for(int j=b+h;j<m;j+=h){
               x=a[j];
               for(k=j-h;(k>=0)&&(a[k]>x);k-=h){
                    a[k+h]=a[k];
               }
               a[k+h]=x;
           }
       }
       //cout<<h<<' '<<*this;
    }
}

void Array::SortShaker(){
    int L=0, R=m-1;
    int R1, L1;
    int x;
    while(L<R){
        R1=L;
        for(int i=L;i<R;i++){
            if(a[i]>a[i+1]){
                x=a[i+1];
                a[i+1]=a[i];
                a[i]=x;
                R1=i;
            }
        }
        R=R1;
        L1=R;
        for(int i=R;i>L;i--){
            if(a[i-1]>a[i]){
            x=a[i-1];
            a[i-1]=a[i];
            a[i]=x;
            L1=i;
            }
        }
        L=L1;
    }
}

void Array::SortHoar(int l, int r){
    if (l<r){
        int xm=a[(l+r)/2];
        int i=l, j=r;
        int tmp;
        while(i<j){
            while(i<=j&&a[i]<xm) i++;
            while(j>=i&&a[j]>xm) j--;
            if(i<=j){
                tmp=a[i];
                a[i]=a[j];
                a[j]=tmp;
                i++;
                j--;
            }
        }
        this->SortHoar(l,j);
        this->SortHoar(i,r);

    }
}

void Array::SortBit(int l, int r, int k){
    if (l > r || k < 0) return;
       int i=l,j=r;
       int tmp;
       int mask=1<<k;
       while(i<j){
           while((i<=j)&&((a[i]&mask)==0)) i++;
           while((j>=i)&&((a[j]&mask)==mask)) j--;
           if(i<j){
               tmp=a[i];
               a[i]=a[j];
               a[j]=tmp;
               i++;
               j--;
           }
       }
       k--;
       this->SortBit(l,j,k);
       this->SortBit(i,r,k);
}

void Array::Shift(int i, int n){
	int j=2*i+1; 
	int x=a[i], b=1;
	while ((j<=n)&&(b)){
		if (j+1<=n)
			if (a[j+1]>a[j]) j++;
		if (a[j]>x){
			a[i]=a[j];
			i=j;
			j=2*i+1;
		}
		else b=0;
	}
	a[i] = x;
}


void Array::SortPyramid(){
	int tmp;
	for(int i=m/2-1; i>=0;i--)	Shift(i,m-1);
    //cout<<*this<<endl;
	for (int i=m-1; i>=1; i--)
	{		
		tmp=a[0];
		a[0]=a[i];
		a[i]=tmp;
		Shift(0,i-1);
        //cout<<i<<") "<<*this<<endl;
	}
}

//Подается переменная k, которая отвечает за то, какую сортировку вызывать
double CheckSort(int m){
    unsigned int end_time,start_time;
    double second;
    Array B(m,2,50);
    Array A(B);
    start_time =  clock(); 
    B.SortShell();
    end_time = clock();
    if((A==B)&&(B.Test())) cout<<'1';
    else cout<<'0';
    cout<<'\n';
    second = (double)(end_time - start_time);
    cout<<"SortSell: "<<second<<endl;
    B=A;
    start_time =  clock(); 
    B.SortShaker();
    end_time = clock();
    if(A==B&&B.Test()) cout<<'1';
    else cout<<'0';
    cout<<'\n';
    second = (double)(end_time - start_time);
    cout<<"SortShaker: "<<second<<endl;
    B=A;
    start_time =  clock(); 
    B.SortHoar(0,m-1);
    end_time = clock();
    if(A==B&&B.Test()) cout<<'1';
    else cout<<'0';
    cout<<'\n';
    second = (double)(end_time - start_time);
    cout<<"SortHoar: "<<second<<endl;
    B=A;
    start_time =  clock(); 
    B.SortPyramid();
    end_time = clock();
    if(A==B&&B.Test()) cout<<'1';
    else cout<<'0';
    cout<<'\n';
    second = (double)(end_time - start_time);
    cout<<"SortPyramid: "<<second<<endl;
    B=A;
    return second;
}

int main(){
    //srand(time(0));
    //double search_time;
    //int m=50000;
    //int b=1;

    Array B(12,1,20);
    cout<<B<<endl;
    
    //cout<<B<<endl;
    //cout<<B<<'\n';
   // B.SortPyramid();
    //cout<<B<<endl;
    //B.SortShell();
    //B.SortShaker();
    B.SortBit(0,11,4);
    //B.SortHoar(0,9999);
    cout<<B<<'\n';
    
    //cout<<'\n';
    //cout<<B;
    //cout<<'\n';
    //cout<<A;
    //if(A==B) cout<<'1';
    //if(b){
     //       search_time=CheckSort(m);
   // }
    //system("pause");
}