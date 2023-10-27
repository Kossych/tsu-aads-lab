#include <iostream>
#include <math.h>
#include <string.h>

using namespace std;


class String
{ 
    char *str;
    int len;
    void CreateNewStr(int);

    public:
    
    int Len();
    String(int l = 0);
    String(const char *s);
    String(const String &S);
    ~String();
    char &operator [](int);
    String & operator = (const String &);
    String & operator += (const String &);
    String operator + (String &);
    friend istream & operator >> (istream &, String &);
    friend ostream & operator << (ostream &, String &);
    int BMH_search(String &);
    int KMP_search(String &);
};

int String::Len()
{
    int length=0;
    while(str[length+1]) length++;
    return length;
}

String::String(int l)
{
    str=new char[len=l];
}

String::String(const char *s)
{
    len=strlen(s);
    str=new char[len+1];
    for(int i=0;i<len;i++) str[i]=s[i];
    str[len]='\0';
}

String::String(const String &S)
{
    len=S.len;
    str=new char[len];
    for(int i=0;i<len;i++) str[i]=S.str[i];
}

String::~String()
{
    delete[]str;
    str=NULL;
}

char &String::operator[](int num)
{
    return str[num];
}

String & String::operator = (const String &S)
{
    if(this!=&S)
    {
        if(len!=S.len)
        {
            CreateNewStr(S.len);
        } 
        for(int i=0;i<len;i++) str[i]=S.str[i];
        return *this;
    }
    return *this;
}


String & String::operator += (const String &S)
{
    String C(*this);
    int l=len;
    CreateNewStr(S.len+len);
    int i=0;
    for(;C.str[i]!='\0';i++) str[i]=C.str[i];
    for(;S.str[i-l];i++) str[i]=S.str[i-l];
    return *this;
}

String String::operator + (String &S)
{
    String res(len+S.len);
    int i=0;
    for(;str[i];i++) res.str[i]=str[i];
    for(;S.str[i-len];i++) res.str[i]=S.str[i-len];
    return res;
}

ostream & operator << (ostream &out, String &S)
{
    for(int i=0;i<S.len;i++) out<<S.str[i];
    return out;
}

istream & operator >> (istream &in, String &S)
{
    in>>S.len;
    delete[]S.str;
    S.str=new char[S.len];
    for(int i=0;i<S.len;i++) in>>S.str[i];
    return in;
}

void String::CreateNewStr(int l)
{
    delete[]str;
    if(l>0)
    str=new char[len=l];
}


int String::BMH_search(String &S)
{
    int T[255];
    int i,k,j;
    //формирование ассоциативного массива
    for(i=0;i<255;i++) T[i]=S.len;
    for(i=0;i<S.len;i++) T[S.str[i]]=S.len-i;
    k=S.len-1;
    j=S.len-1;
    i=S.len-1;
    while(i<len)
    {
        if(j<0) return (i+1-S.len);
        k=i;
        j=S.len-1;
        while(j>=0)
        {
            if(S.str[j]==str[k])
            {
                j--; k--;
            }
            else
            {
                i=i+T[str[i]];
                j=S.len-1;
                break;
            }
        }
    }
    if(j>=0) return (-1);
    else return i+1-S.len;
}

int String::KMP_search(String &S)
{
    if(len>=S.len)
    {
        int i,j,k;
        int* NewJ=new int(S.len);
        j=0; k=-1; NewJ[0]=-1;
        //формирование NewJ
        while(j<S.len-1)
        {
            if((k>=0)&&(S.str[j]!=S.str[k]))
                k=NewJ[k];
            else
            {
                k++; j++;
                if(S.str[j]==S.str[k]) NewJ[j]=NewJ[k];
                else NewJ[j]=k;
            }
        }
        i=0; j=0;
        for(int i=0;i<S.len;i++) cout<<NewJ[i]<<' ';
        cout<<'\n';
        while(j<S.len)
        {
            if(i<len)
            {
                while((j>=0)&&(str[i]!=S.str[j])) j=NewJ[j];
                i++; 
                j++;
            }
            else return (-1);
        }
        return (i-S.len);
    }
    else return(-1);
}
    


int main()
{
    String b("aaabaaaab");
    String c("aaabacbaaaaabaaaababbaaaaaaaab");
    int k=c.KMP_search(b);
    //int k=c.BMH_search(b);
    //cout<<"результат: "<<k<<'\n';
  //  c+=b;
  //  cout<<c<<'\n';
    //char q=c[2];
    //q='o';
    cout<<k;
 //   cout<<q<<'\n';
   // String a(c);
  //  cout<<a<<'\n';
  //  String g;
  //  g=c+b;
  //  cout<<g<<'\n';
    system("pause");
} 