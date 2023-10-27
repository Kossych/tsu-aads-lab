#include <iostream>
#include <ctime>
#include <stdbool.h>
#include <string.h>
#include <type_traits> 
#include <string>
#include <bitset>
#include <vector>
#define BASE_SIZE (sizeof(BASE)*8)
#define MAX_BASE ((unsigned long long)1<<BASE_SIZE)

using namespace std;
typedef unsigned short BASE;
typedef unsigned long long EXT_BASE;
typedef conditional<BASE_SIZE<32,conditional<BASE_SIZE<16,unsigned short, unsigned int >::type, unsigned long long>::type TMP; 


class BigNum
{
    public:
    BASE *coef;
    int maxLEN;
    int len;
        BigNum(BASE num = 0);
        BigNum(int, bool);
        BigNum(const BigNum &);
        ~BigNum(){delete[]coef; coef=NULL; len=0; maxLEN=0;}
        BigNum& operator = (const BigNum &);
        BigNum& operator = (const BASE);

        void HexInput(const char *);
        void HexOutput();

        bool operator == (const BigNum&) const;
        bool operator != (const BigNum&);
        bool operator >= (const BigNum&);
        bool operator <= (const BigNum&);
        bool operator > (const BigNum&) const;
        bool operator < (const BigNum&) const;

        BigNum operator + (const BigNum &);
        BigNum& operator += (const BigNum &);
        BigNum operator + (const BASE);
        BigNum& operator += (const BASE);

        BigNum operator - (const BASE);
        BigNum& operator -= (const BASE);
        BigNum operator - (const BigNum &);
        BigNum& operator -= (const BigNum &);

        BigNum operator * (const BigNum &);
        BigNum& operator *= (const BigNum &);
        BigNum operator * (const BASE);
        BigNum& operator *= (const BASE);

        BigNum operator /(const BASE);
        BigNum& operator /=(const BASE);
        BigNum operator /(const BigNum &);
        BigNum& operator /=(const BigNum &);

        BASE operator %(const BASE) const;
        BigNum& operator %=(const BASE) ;
        BigNum operator %(const BigNum &) const;
        BigNum& operator %=(const BigNum &) ;

        friend ostream& operator << (ostream &, const string&);

        BigNum FastSqr();
        BigNum Pow(BigNum&);
        BigNum Pow(BASE);
        BigNum StandardPow(BigNum&);
        BigNum StandardPow(BASE);
        BigNum BarretMod(BigNum&, BigNum&);
        BigNum pow_mod(const BigNum&, const BigNum&);
        bool fermat_test(BASE);
        bool solovay_strassen_test(BASE);
        bool miller_rabin_test(BASE);


        void DecInput(const char *);
        void DecOutput();
        void PrintCoef();
        bool isEven();
        BigNum phi();

        float eps_fermat();
        float eps_solovay_strassen();
        float eps_miller_rabin();

        BigNum sqrt();
        BigNum cbrt();
        
        vector<BigNum> fact();
        BigNum methodAlway(BigNum);
        pair<BigNum, BigNum> methodFermat();
        BigNum Polard();
        BigNum Gelfond(BigNum, BigNum);
        BigNum pPolard(BigNum, BigNum);

        BigNum FunPolard(BigNum, int);
        BigNum MinusMod(BigNum);

        BigNum gcd(const BigNum&);
        vector<BigNum> Fx(vector<BigNum>, BigNum, BigNum);
    private:
        void ExpMaxLen(int);
        void NormLen();
        BigNum ShiftR(int); 
        bool bit(BASE) const;
        BASE bits() const;  
        int SubBN(const BigNum&, int);
        void AddBN(const BigNum&, int);
        BigNum DivBN(const BASE , int);


        
};

    BigNum::BigNum(BASE num)
    {
        maxLEN = 1;
        coef = new BASE[len =maxLEN];
        coef[0] = num;
    }

    BigNum::BigNum(int Len, bool k)
    {
        maxLEN=Len;
        coef=new BASE[maxLEN];
        if(k==0){
            len=1;
            for(int i=0;i<Len;i++) coef[i]=0;
        }
        else if(k==1){
            len=Len;
            int a;
            for(int i=0;i<Len;i++) 
            for(int j=0;j<BASE_SIZE;j=j+8){
                a=rand()%256;
                coef[i]=coef[i]|(a<<j);
            }
            this->NormLen();
        }

    }

    BigNum::BigNum(const BigNum &LN)
    {
        if(&LN!=this){
            maxLEN=LN.maxLEN;
            len=LN.len;
            coef=new BASE[maxLEN];
            for(int i=0;i<maxLEN;i++) coef[i]=LN.coef[i];
        }

    }
    
    BigNum& BigNum::operator = (const BigNum &LN)
    {
        if(&LN!=this){
            maxLEN=LN.maxLEN;
            len=LN.len;
            delete[]coef;
            coef=new BASE[maxLEN];
            for(int i=0;i<maxLEN;i++) coef[i]=LN.coef[i];
            return *this;
        }
        return *this;
    }

    BigNum& BigNum::operator = (const BASE num)
    {
        delete[]coef;
        len=1;
        maxLEN=len;
        coef=new BASE[len];
        coef[0]=num;
        return *this;
    }

    void BigNum::HexInput(const char *S)
    {
        int BS=BASE_SIZE/4;
        int Slen=strlen(S);
        len=(Slen-1)/BS+1;
        maxLEN=len+1;
        delete[]coef;
        coef=new BASE[maxLEN];
        for(int i=0;i<maxLEN;i++) coef[i]=0;
        for(int i=Slen-1;i>=0;i--)
        {
            char tmp=0;
            int iRev=Slen-1-i;
            if((S[i]>='0')&&(S[i]<='9')) tmp=S[i]-'0';
            else if((S[i]>='a')&&(S[i]<='f')) tmp=S[i]-'a'+10;
            else if((S[i]>='A')&&(S[i]<='F')) tmp=S[i]-'A'+10;
            else continue;
            coef[iRev/BS]=coef[iRev/BS]|(tmp<<((iRev%BS)*4)); 
        }
        this->NormLen();
    }

    void BigNum::HexOutput()
    {
        bool k=true;
        for(int i=len-1;i>=0;i--)
        {
            for(int j=BASE_SIZE-4;j>=0;j=j-4)
            {
                char tmp=((coef[i]&(0xF<<j))>>j);
                if(k&&(tmp==0)) continue;
                else k=false;
                if(tmp<10)
                {
                    tmp=tmp+'0';
                    cout<<tmp;
                }
                else if(tmp<16)
                {
                    tmp=tmp+'a'-10;
                    cout<<tmp;
                }
            }
        }
        if(k) cout<<0;
    }

    bool BigNum::operator == (const BigNum &LN) const
    {
        if(&LN==this) return 1;
        if(len!=LN.len) return 0;
        int i=len-1;
        while(i>=0)
        {
            if(coef[i]==LN.coef[i]) i--;
            else return false;
        }
        return true;
    }

    bool BigNum::operator != (const BigNum &LN)
    {
        if(&LN==this) return 0;
        if(len!=LN.len) return 1;
        int i=len-1;
        while(i>=0)
        {
            if(coef[i]==LN.coef[i]) i--;
            else return true;
        }
        return false;
    }

    bool BigNum::operator >= (const BigNum &LN)
    {
        if(&LN==this) return 1;
        if(len>LN.len) return 1;
        if(len<LN.len) return 0;
        int i=len-1;
        while(i>=0)
        {
            if(coef[i]==LN.coef[i]) i--;
            else if(coef[i]>LN.coef[i]) return 1;
            else return 0;
        }
        return true;
    }

    bool BigNum::operator <= (const BigNum &LN)
    {
        if(&LN==this) return 1;
        if(len>LN.len) return 0;
        if(len<LN.len) return 1;
        int i=len-1;
        while(i>=0)
        {
            if(coef[i]==LN.coef[i]) i--;
            else if(coef[i]<LN.coef[i]) return 1;
            else return 0;
        }
        return true;
    }

    bool BigNum::operator > (const BigNum &LN) const
    {
        if(&LN==this) return 0;
        if(len>LN.len) return 1;
        if(len<LN.len) return 0;
        int i=len-1;
        while(i>=0)
        {
            if(coef[i]==LN.coef[i]) i--;
            else if(coef[i]>LN.coef[i]) return true;
            else return false;
        }
        return false;
    }

    bool BigNum::operator < (const BigNum &LN) const
    {
        if(&LN==this) return 0;
        if(len>LN.len) return 0;
        if(len<LN.len) return 1;
        int i=len-1;
        while(i>=0)
        {
            if(coef[i]==LN.coef[i]) i--;
            else if(coef[i]<LN.coef[i]) return true;
            else return false;
        }
        return false;
    }

    BigNum BigNum::operator+ (const BigNum &LN)
    {
        int min,max;
        BASE *MaxCoef;
        bool k=0;
        TMP tmp;
        if(len>LN.len) {min=LN.len; max=len+1; MaxCoef=coef;}
        else {min=len; max=LN.len+1; MaxCoef=LN.coef;}
        BigNum A(max,0);
        for(int i=0;i<min;i++)
        {
            tmp=(TMP)LN.coef[i]+(TMP)coef[i]+k;
            A.coef[i]=(BASE)tmp;
            k=(bool)(tmp>>BASE_SIZE);
        }
        for(int i=min;i<max-1;i++)
        {
            tmp=(TMP)MaxCoef[i]+k;
            A.coef[i]=(BASE)tmp;
            k=(bool)(tmp>>BASE_SIZE);
        }
        A.coef[max-1]=k;
        A.NormLen();
        return A;
    }

    BigNum BigNum::operator+ (const BASE num)
    {
        TMP tmp=0;
        bool k=0;
        int Len=maxLEN;
        if(len+1>maxLEN) Len=len+1;
        BigNum A(Len,0);
        tmp=(TMP)coef[0]+(TMP)num+k;
        k=(bool)(tmp>>BASE_SIZE);
        A.coef[0]=tmp;
        for(int i=1;i<len;i++)
        {
            tmp=coef[i]+k;
            k=bool(tmp>>BASE_SIZE);
            A.coef[i]=tmp;
        }
        A.coef[len]=k;
        A.NormLen();
        return A;
    }

    BigNum& BigNum::operator+= (const BASE num)
    {
        *this=*this+num;
        return *this;
    }

    BigNum& BigNum::operator+= (const BigNum &LN)
    {
        *this=*this+LN;
        return *this;
    }

    BigNum BigNum::operator - (const BASE num)
    {
        if (*this < num) 
        {
        throw invalid_argument("first value should be bigger than second to subtract");
        }
        TMP tmp;
        bool k=0;
        BigNum A(*this);
        tmp=A.coef[0]+MAX_BASE-num;
        A.coef[0]=(BASE)tmp;
        k=!((tmp)>>BASE_SIZE);
        for(int i=A.len;k&&i<=A.len;i++)
        {
            tmp=A.coef[i]+MAX_BASE-k;
            A.coef[i]=(BASE)tmp;
            k=!((tmp)>>BASE_SIZE);
        }
        A.NormLen();
        return A;
    }
    
    BigNum& BigNum::operator -= (const BASE num)
    {
        *this=*this-num;
        return *this;
    }

    BigNum BigNum::operator - (const BigNum &LN)
    {
        if (*this < LN) 
        {
        throw std::invalid_argument("first value should be bigger than second to subtract");
        }
        TMP tmp;
        bool k=0;
        BigNum A(*this);
        for(int i=0;i<LN.len;i++)
        {
            tmp=A.coef[i]+MAX_BASE-LN.coef[i]-k;
            A.coef[i]=(BASE)tmp;
            k=!((tmp)>>BASE_SIZE);
        }
        for(int i=LN.len;k&&i<=A.len;i++)
        {
            tmp=A.coef[i]+MAX_BASE-k;
            A.coef[i]=(BASE)tmp;
            k=!((tmp)>>BASE_SIZE);
        }
        A.NormLen();
        return A;
    }

    BigNum& BigNum::operator -= (const BigNum &LN)
    {
        if (*this < LN) 
        {
        throw invalid_argument("first value should be bigger than second to subtract");
        }
        int min;
        bool k=0;
        TMP tmp;
        for(int i=0;i<LN.len;i++)
        {
            tmp=coef[i]+MAX_BASE-LN.coef[i]-k;
            k=!((tmp)>>BASE_SIZE);
            coef[i]=(BASE)tmp;
        }
        for(int i=LN.len;k&&i<=len;i++)
        {
            tmp=coef[i]+MAX_BASE-k;
            k=!((tmp)>>BASE_SIZE);
            coef[i]=(BASE)tmp;
        }
        this->NormLen();
        return *this;
    }

    BigNum BigNum::operator* (const BigNum &LN)
    {
        TMP tmp;
        BASE k;
        BigNum A(len+LN.len,0);
        for(int i=0;i<len;i++)
        {
            k=0;
            for(int j=0;j<LN.len;j++){
                tmp=(TMP)LN.coef[j]*(TMP)coef[i]+A.coef[i+j]+k;
                k=(tmp>>BASE_SIZE);
                A.coef[i+j]=tmp;
            }
            A.coef[LN.len+i]+=k;
        }
        A.NormLen();
        return A;
    }

    BigNum BigNum::operator* (const BASE num)
    {
        TMP tmp=0;
        BASE k=0;
        BigNum A(len+1,0);
        for(int i=0;i<len;i++)
        {
            tmp=(TMP)coef[i]*(TMP)num+k;
            k=(tmp>>BASE_SIZE);
            A.coef[i]=(BASE)tmp;
        }
        A.coef[len]=k;
        A.NormLen();
        return A;
    }

    BigNum& BigNum::operator*= (const BASE num)
    {
        *this=*this*num;
        return *this;
    }
            

    BigNum& BigNum::operator *= (const BigNum &LN)
    {
        *this=*this*LN;
        return *this;
    }

    BigNum BigNum::operator/(const BASE num)
    {
        TMP tmp,k=0;
        BigNum A(len,0);
        for(int i=len-1;i>=0;i--)
        {
            tmp=(k<<BASE_SIZE)+coef[i];
            k=tmp%num;
            A.coef[i]=(tmp)/num;
        }
        A.NormLen();
        return A;
    }

    BigNum& BigNum::operator/=(const BASE num)
    {
        *this=*this/num;
        return *this;
    }
    
    BASE BigNum::operator%(const BASE num) const
    {
        TMP tmp,k=0;
        for(int i=len-1;i>=0;i--){
            tmp=(k<<BASE_SIZE)+coef[i];
            k=tmp%num;
        }
        return k;
    }

    BigNum& BigNum::operator%=(const BASE num)
    {
        *this=*this%num;
        return *this;
    }

    BigNum BigNum::operator/(const BigNum &LN)
    {
        if((LN.len==1)&&(LN.coef[0]!=0)){BigNum q=*this/LN.coef[0]; return q;}   
        if((LN.len==1)&&(LN.coef[0]==0)){BigNum q(1,0); q.coef[0]=0; return q;}   
        if(*this==LN){BigNum q(1,0); q.coef[0]=1; return q;}
        if(*this<LN){BigNum q; return q;}
        BigNum q(len-LN.len+1, 0);
        TMP b=MAX_BASE;
        //D1
        TMP d=b/((TMP)LN.coef[LN.len-1]+1);
        BigNum dU(*this);
        BigNum dV(LN);
        dU*=d;
        dV*=d;   
        if(len==dU.len)
        {
            if(dU.len==dU.maxLEN){
                dU.ExpMaxLen(1);
            }
            dU.coef[len]=0;
            dU.len=len+1;
        }   
        //D2
        for(int j=len-LN.len;j>=0;j--)//D7(Цикл по j)
        {
            //D3
            int _j=j+LN.len;
            TMP ff=((TMP)dU.coef[_j]<<BASE_SIZE)+dU.coef[_j-1];
            TMP _q=(ff/dV.coef[LN.len-1]);
            TMP _r=(ff%dV.coef[LN.len-1]);
            if((_q==b)||((_q*dV.coef[LN.len-2])>(b*_r+dU.coef[_j-2])))
            {
                _q--; _r=_r+dV.coef[LN.len-1];
                if((_r<b)&&((_q==b)||(_q*dV.coef[LN.len-2]>b*_r+dU.coef[_j-2])))
                {
                    _q--; _r=_r+dV.coef[LN.len-1];
                }
            }
            //D5
            q.coef[j]=_q;   
            //D4
            int k=dU.SubBN( dV*_q,j);
            if(k)
            {
                //D6
                q.coef[j]--;
                dU.AddBN(dV,j);
            }
        }
        //D8
        q.NormLen();
        return q;
    }

    BigNum& BigNum::operator/=(const BigNum &LN)
    {
        *this=*this/LN;
        return *this;
    }

    BigNum BigNum::operator%(const BigNum &LN) const
    {
        if((LN.len==1)&&(LN.coef[0]!=0)){BigNum r(1,0); r.coef[0]=*this%LN.coef[0]; return r;}   
        if((LN.len==1)&&(LN.coef[0]==0)){BigNum r(*this); return r;}
        if(*this==LN){BigNum r(1,0); return r;}
        if(*this<LN){BigNum r(*this); return r;}
        BigNum r(LN.len-1, 0);
        TMP b=MAX_BASE;
        TMP d=b/((TMP)LN.coef[LN.len-1]+1);
        BigNum dU(*this);
        BigNum dV(LN);
        dU*=d;
        dV*=d;
        if(len==dU.len)
        {
            if(dU.len==dU.maxLEN)
            {
                dU.ExpMaxLen(1);
            }
            dU.coef[len]=0;
            dU.len=len+1;
        }   
        for(int j=len-LN.len;j>=0;j--)
        {
            int _j=j+LN.len;
            TMP ff=((TMP)dU.coef[_j]<<BASE_SIZE)+dU.coef[_j-1];
            TMP _q=(ff/dV.coef[LN.len-1]);
            TMP _r=(ff%dV.coef[LN.len-1]);
            if((_q==b)||((_q*dV.coef[LN.len-2])>(b*_r+dU.coef[_j-2])))
            {
                _q--; _r=_r+dV.coef[LN.len-1];
                if((_r<b)&&((_q==b)||(_q*dV.coef[LN.len-2]>b*_r+dU.coef[_j-2])))
                {
                    _q--; _r=_r+dV.coef[LN.len-1];
                }
            }
            int k=dU.SubBN( dV*_q,j);
            if(k){
                //D6
                //q.coef[j]--;
                dU.AddBN(dV,j);
            }
        }
        dU.len=LN.len;
        r=dU/d;
        
        //r.NormLen();
        return r;
    }

    BigNum& BigNum::operator%=(const BigNum &LN) 
    {
        *this=*this%LN;
        return *this;
    }

    void BigNum::DecInput(const char *S)
    {
        TMP tmp=0;
        BASE k=0;
        int BS=BASE_SIZE/4;
        int Slen=strlen(S);
        maxLEN=(Slen-1)/BS+2;
        len=1;
        delete[]coef;
        coef=new BASE[maxLEN];
        for(int i=0;i<maxLEN;i++) coef[i]=0;
        
        for(int i=0;i<Slen;i++)
        {
            char s=0;
            if((S[i]>='0')&&(S[i]<='9')) s=S[i]-'0';
            else continue;
            (*this*=10)+=s;
        }
        NormLen();
    }

    void BigNum::DecOutput()
    {
        if(len==1&&coef[0]==0) cout<<0;
        else
        {
            BigNum A(*this);
            int SLen=len*10;
            int k=0;
            //unsigned char tmp;
            char* S=new char[SLen];
            while(A.coef[0]!=0||A.len>1)
            {
                S[k]=(A%10)+'0';
                //cout<<S[k]<<' ';
                A/=10;
                k++;
            }
            k--;
            for(;k>=0;k--) cout<<S[k];
            delete[]S;
        }
    }   


    int BigNum::SubBN(const BigNum &LN, int j)
    {
        TMP tmp;
        bool k=0;
        for(int i=0;i<LN.len;i++)
        {
            //cout<<i+j<<" "<<len<<endl;
            tmp=(TMP)coef[i+j]+MAX_BASE-LN.coef[i]-k;
            coef[i+j]=(BASE)tmp;
            k=!((tmp)>>BASE_SIZE);
        }
        for(int i=LN.len;k&&(i+j)<len;i++)
        {
            //cout<<i+j<<" "<<len<<endl;
            tmp=(TMP)coef[i+j]+MAX_BASE-k;
            coef[i+j]=(BASE)tmp;
            k=!((tmp)>>BASE_SIZE);
        }
        NormLen();
        return k;
    }


    void BigNum::AddBN(const BigNum &LN, int j)
    {
        bool k=0;
        for(int i=0;i<LN.len;i++){
            TMP tmp=(TMP)coef[i+j]+LN.coef[i]+k;
            k=bool(tmp>>BASE_SIZE);
            coef[i+j]=tmp;
        }
        for(int i=LN.len;k&&(i+j)<len;i++){
            TMP tmp=(TMP)coef[i+j]+k;
            k=bool(tmp>>BASE_SIZE);
            coef[i+j]=tmp;
        }
        //NormLen();
    }

    BigNum BigNum::DivBN(const BASE num, int j)
    {
        TMP tmp,k=0;
        BigNum A(j+1,0);
        for(int i=j;i>=0;i--){
            tmp=(k<<BASE_SIZE)+coef[i];
            k=tmp%num;
            A.coef[i]=(tmp)/num;
        }
        A.NormLen();
        return A;
    }

    BigNum BigNum::gcd(const BigNum& bn)
    {
        BigNum a = *this;
        BigNum b = bn;

        while (a != b)
        {
            if (a > b)
                a = a - b;
            else b = b - a;
        }
        return a;
    }


    void BigNum::ExpMaxLen(int b)
    {
        maxLEN+=b;
        BASE *NewCoef=new BASE[maxLEN];
        for(int i=0;i<len;i++) NewCoef[i]=coef[i];
        for(int i=len;i<maxLEN;i++) NewCoef[i]=0;
        delete[]coef;
        coef=NewCoef;
    }

    void BigNum::NormLen()
    {
        for(len=maxLEN;coef[len-1]==0&&len>0;len--);
        if(len==0) len=1;
    }
    
    void BigNum::PrintCoef()
    {
        for(int i=len-1;i>=0;i--) cout<<bitset<8>(coef[i])<<" ";
        cout<<endl;

    }

 
    BASE BigNum::bits() const
    {
        for(int i = len-1; i >= 0; i--)
        {
            for(int bit = BASE_SIZE-1; bit >= 0; bit--)
            {
                if((coef[i] >> bit) & 1) return (i * BASE_SIZE + bit + 1);
            }
        }
        return 0;
    }

    bool BigNum::bit(BASE i) const
    {
        int pos = i/BASE_SIZE;
        return ((coef[pos] >> (i % BASE_SIZE)) & 1);
    }

    BigNum BigNum::ShiftR(int i)
    {
        int new_len = (i>len) ? 0 : len - i;
        BigNum res(new_len, 0);
        res.len = new_len;
        for(int cf = 0; cf < new_len; cf++)
        {
            res.coef[cf] = coef[cf + i];
        }
        return res;
    }

    bool BigNum::isEven()
    {
        return !(coef[0]&1);
    }

    BigNum BigNum::pow_mod(const BigNum &n, const BigNum &m)
    {
        if(m == 1)
        {
            return BigNum(0);
        }
        BigNum q = *this % m;

        BigNum z(1);
        if(n.coef[0]&1){
            z = BigNum(*this) % m;
        }

        BASE n_bits = n.bits();
        for(int i=1; i < n_bits; i++)
        {
            q = q.FastSqr() % m;
            if(n.bit(i)) z = (z * q) % m;
        }
        return z; 
    }   

    BigNum GetBarretZ(BigNum& m)
    {
        BigNum z(2 * m.len + 1, 0);
        z.coef[2 * m.len] = 1;
        z.len = 2 * m.len + 1;
        z /= m;
        return z;
    }

    int Jacobi(const BigNum& a, const BigNum& n)
    {
        if(a == 0 || a == 1)
        {
            return a.coef[0];
        }
        BigNum a1 = a;      
        BigNum k = 0;
        while (a1.isEven()) 
        {
            a1 /= 2;
            k += 1;
        }

        int s;
        if (k.isEven()) 
        {
            s = 1;
        } else 
        {
            BigNum m = n % 8;
            if (m == 1 || m == 7) 
            {
                s = 1;
            } else {
                s = -1; 
            }
        }

        if (n % 4 == 3 && a1 % 4 == 3) 
        {
            s = -s;
        }

        if (a1 == 1) 
        {
            return s;
        }
        return s * Jacobi(n % a1, a1);
    }

    BigNum generator_prime(BASE size)
    {
        BigNum prime;
        do{
            prime = BigNum(size, 1);
        }while(!prime.miller_rabin_test(100));
        return prime;
    }

    BigNum BigNum::FunPolard(BigNum x, int c = 1)
    {
        return (x.FastSqr() + c) % *this;
    }

    BigNum BigNum::MinusMod(BigNum x)
    {
        if (*this > x) return *this - x;
        else return x - *this;
    }

    vector<pair<BigNum, BigNum>> Shall(vector<pair<BigNum, BigNum>> str)
    {
        int len = str.size();
        int d = (len) / 2, j;
        pair<BigNum, BigNum> swap;
        for (; d > 0; d = d / 2)
            for (int j = d; j < len; j++)
                for (int i = j - d; i >= 0 && str[i].first > str[i + d].first; i = i - d)
                {
                    swap = str[i];
                    str[i] = str[i + d];
                    str[i + d] = swap;
                }
        return str;
    }

    pair<bool, BigNum> BitSearch(vector<pair<BigNum, BigNum>> str, BigNum key)
    {
        int l = 0;
        int r = str.size();
        bool flag = false;
        while ((l <= r) && (flag != true))
        {
            int mid = (l + r) / 2;

            if (str[mid].first == key)  return make_pair(true, str[mid].second);
            if (str[mid].first > key) r = mid - 1;
            else l = mid + 1;
        }
        return make_pair(flag, BASE(0));
    }

    vector<BigNum> BigNum::Fx(vector<BigNum>str, BigNum g, BigNum a)
    {
        BigNum S, x, One(BASE(1)), Two(BASE(2)), Three(BASE(3)), Zero(BASE(0));
        vector<BigNum>tr(3);
        x = str[0];
        S = x % Three;
        
        if (S == One)
        {
            tr[0] = (a * x) % *this;
            tr[1] = str[1];
            tr[2] = (str[2] + 1) % (*this - One);
            return tr;
        }
        if (S == Two)
        {
            tr[0] = str[0].FastSqr() % *this;
            tr[1] = (Two * str[1]) % (*this - One);
            tr[2] = (Two * str[2]) % (*this - One);
            return tr;
        }
        else 
        {
            tr[0] = (g * x) % *this;
            tr[1] = (str[1] + 1) % (*this - One);
            tr[2] = str[2];
            return tr;
        }
    }

    BigNum p_mod_1(BigNum x, BigNum n)
    {
        BigNum u1 = n;
        BigNum u2;
        BigNum One(BASE(1));
        BigNum v1 = x;
        BigNum v2 = One;
        BigNum Zero;
        while (v1 != Zero)
        {
            BigNum q = u1 / v1;
            BigNum t1 = u1 % v1;
            BigNum str = (q * v2) % n;
            if (u2 < str)
            {
                u2 += n;
            }
            BigNum t2 = u2 - str;
            u1 = v1;
            v1 = t1;
            u2 = v2;
            v2 = t2;
        }
        return (u2 % n);
    }

BigNum BigNum::sqrt()
{
	BigNum x(*this), x0;
	do
	{
		x0 = x;
		x = (*this / x + x) / 2;
	} while (x < x0);
	return x0;
}

BigNum BigNum::cbrt()
{
    BigNum left(BASE(1));
    BigNum right = *this/2 + 1;
    BigNum middle, result;
    while(left <= right)
    {
        middle = left + (right - left)/2;
        if(((middle * middle) * middle) <= *this)
        {
            left = middle + 1;
            result = middle;
        }
        else
        {
            right = middle - 1;
        }
    }
    return result;
}