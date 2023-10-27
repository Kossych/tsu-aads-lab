#include <iostream>
#include <string>
#include <vector>
#include <array>

using namespace std;


void PrintVector(vector<int> a)
{
    for(int i = 0; i < a.size(); i++)
    {
        cout<<a[i]<<" ";
    }
    cout<<"\n";
}



vector <int> factorize(int x)
{
    vector <int> factors;
    int sqrtx = sqrt(x);
    for (int i = 2; i <= sqrtx; i++)
    {
        while (x % i == 0) {
            factors.push_back(i);
            x /= i;
        }
    }
    if (x != 1) {
        factors.push_back(x);
    }
    return factors;
}

int InverseInField(int a, int field)
{
    if(field == 0)
    {
        return 1/a;
    }
    for(int i = 0; i < field; i++)
    {
        if(a * i % field  == 1)
            return i;
    }
    return 0;
}

int deg(vector<int> fx)
{
    return fx.size() - 1;
}


vector<int> norm(vector<int> a, int field)
{
    if(a.size() == 0) return a;
    vector<int> res;
    res.assign(a.begin(), a.end());
    if(field != 0)
    {
        int deg = res.size();
        for(int i = 0; i < deg; i++)
        {
            while (res[i] < 0)
            {
                res[i] += field;
            }
            res[i] %= field;
        }
    }
    return res;
}

vector<int> poly_mul(vector<int> a, vector<int> b, int field)
{
    vector<int> res(a.size() + b.size());
    for(int i = 0; i < a.size(); i++)
    {
        for(int j = 0; j < b.size(); j++)
        {
            res[i + j] += a[i] * b[j];
        }
    }
    if(res[deg(res)] == 0) res.pop_back();
    res = norm(res, field);
    return res;
}

vector<int> poly_mod(vector<int> a, vector<int> fx, int field)
{
    vector<int> mod(a);
    while(mod.size() > 0 && fx.size() > 0 && (deg(mod) >= deg(fx)))
    {
        int k = deg(mod) - deg(fx);
        int inv = InverseInField(fx[deg(fx)], field);
        int d = mod.back() * inv;
        d %= field;
        for(int i = deg(fx); i >= 0; i--)
        {
            mod[i + k] -= (d * fx[i]);
            if(mod[i + k] > field)
                mod[i + k] %= field;
            while(mod[i + k] < 0)
                mod[i + k] += field;
        }
        while(mod.size() > 0 && mod.back() == 0)
        {
            if(mod.size() == 1)
                return norm(mod, field);
            mod.pop_back();
        }
    }
    return norm(mod, field);
}

vector<int> poly_gcd(vector<int> a, vector<int> b, int field)
{
    vector<vector<int>> r{a, b};
    int i = 1;
    while(r[i].size() != 1 || r[i][0] != 0)
    {
        while(r[i].size() > 0 && r[i][deg(r[i])] == 0)
        {
            if(r[i].size() == 1) break;
            r[i].pop_back();
        }
        r.push_back(poly_mod(r[i-1], r[i], field));
        i++;
    }
    int inv = InverseInField(r[i-1][deg(r[i-1])], field);
    for(int j = 0; j < r[i - 1].size(); j++)
        r[i-1][j] *= inv;
    return norm(r[i-1], field);
}

vector<int> poly_pow(vector<int> a, int pow = 2, int field = 0)
{
    if(pow == 0)
    {
        vector<int> one{1};
        return one;
    }
    vector<int> res(a);
    for(int i = 0; i < pow; i++)
    {
        res = poly_mul(res, a, field);
    }
    return norm(res, field);
}

bool IsIrreducible(vector<int> a, int field = 0)
{
    vector<int> u{0, 1};
    for(int i = 0; i < deg(a) / 2; i++)
    {
        u = poly_pow(u, field, field);
        u = poly_mod(u, a, field);
        vector<int> tmp(u);
        if(deg(tmp) == 0) 
            tmp.push_back(0);
        tmp[1] -= 1;
        tmp = norm(tmp, field);
        vector<int> d = poly_gcd(a, tmp, field);
        if(d.size() != 1 || d[0] != 1)
            return true;
    }
    return false;
}

bool IsPrimitive(vector<int> a, int field = 0)
{
    int pn = field;
    for(int i = 1; i < deg(a); i++)
    {
        pn *= field;
    }
    pn--;

    vector<int> factors = factorize(pn);
    int tmp;
    for(int i = 0; i < factors.size(); i++)
    {
        tmp = pn / factors[i];
        vector<int> r(tmp+1);
        r[deg(r)] = 1;
        r = poly_mod(r, a, field);
        if(r.size() == 1 && r[0] == 1)
            return false;
    }
    return true;
}

int main()
{
    vector<int> f{1, 0, 0, 4, 0, 3};
    int p = 5;

    if (!IsIrreducible(f, p))
    {
        cout<<"f is not reducible"<<'\n';
        if(IsPrimitive(f, p))
            cout<<"f is primitive";
        else
            cout<<"f is not primitive";
    }
    else
    {
        cout<<"f is reducible";
    }
}