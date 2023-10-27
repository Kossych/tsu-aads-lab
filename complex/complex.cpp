#include <iostream>
#include <math.h>

using namespace std;

class Complex
{
    double re,im;
    public:
    
    Complex(double _re=0, double _im=0)
    {
    re=_re;
    im=_im;
    }

    Complex operator +(Complex &c);
    Complex operator -(Complex &c);
    Complex operator *(Complex &c);
    friend ostream&operator <<(ostream &out, Complex &c);
    friend istream&operator >>(istream &in, Complex &c);
    
    Complex& Add(Complex &c);
    Complex& Sub(Complex &c);
    Complex& Mul(Complex &c);
    double Abs();
    void Input();
    void Output();
    double GetRe(){return re;}
    double GetIm(){return im;}
    void SetRe(double _re){re=_re;}
    void SetIm(double _im){im=_im;}
};



inline Complex& Complex::Add(Complex &c)
{
    re=re+c.re;
    im=im+c.im;
    return *this;
}

inline Complex& Complex::Sub(Complex &c)
{
    re=re-c.re;
    im=im-c.im;
    return *this;
}

inline Complex& Complex::Mul(Complex &c)
{
    re=re*c.re-im*c.im;
    im=re*c.im+im*c.re;
    return *this;
}

inline double Complex::Abs()
{
    double _re=re;
    double _im=im;
    return sqrt(_re*_re+_im*_im);
}

inline void Complex::Output()
{
    cout<<re;
    if (im>0) cout<<'+';
    if (im!=0) cout<<im<<"i";
    cout<<"\n";
}

inline void Complex::Input()
{
    cout<<"input re, im: ";
    cin>>re>>im;
}

Complex Complex::operator +(Complex &c)
{
    Complex res=*this;
    res.Add(c);
    return res;
}

Complex Complex::operator -(Complex &c)
{
    Complex res=*this;
    res.Sub(c);
    return res;
}

Complex Complex::operator *(Complex &c)
{
    Complex res=*this;
    res.Mul(c);
    return res;
}

ostream& operator <<(ostream &out, Complex &c)
{
    double re=c.re;
    double im=c.im;
    char sign;
    if(im>0)
        sign='+';
    else
    {
        sign='-';
        im=-im;
    }
    out<<re<<sign<<im<<'i';
    return out;
}

istream& operator >>(istream &in, Complex &c)
{
    in>>c.re>>c.im;
    return in;
}

int main()
{
    Complex a(2,4);
    Complex b(2,4);
    a=a+b;
    a=a*b;
    cout<<a;
}