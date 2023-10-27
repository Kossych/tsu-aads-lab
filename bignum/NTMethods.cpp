#include "BigNum.cpp"
#include <chrono>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

BigNum BigNum::FastSqr(){
    BigNum res(2*len, 0);
    for(int i = 0; i < len; i++){
        EXT_BASE cuv = EXT_BASE(coef[i])*coef[i] + res.coef[2*i];
        res.coef[2*i] = cuv;
        //2)
        for(int j = i + 1; j < len; j++){
            cuv = EXT_BASE(2) * coef[i] * coef[j] + res.coef[i+j] + (cuv >> BASE_SIZE);
            res.coef[i+j] = cuv;
        }

        TMP* y = (TMP*) &res.coef[i+len];
        *y += cuv >> BASE_SIZE;
    }

    res.NormLen();
    return res;
}

BigNum BigNum::Pow(BigNum &n) {
    BigNum q(*this);
    BigNum z(1,0);
    if(n.coef[0]&1){
        z = BigNum(*this);
    }else{
        z+=1;
    }
    BASE n_bits = n.bits();
    for(int i=1; i < n_bits; i++){
        q = q.FastSqr();
        if(n.bit(i)) z *= q;
    }
    return z;       
}

BigNum BigNum::Pow(BASE n){
    BigNum q(*this);
    BigNum z(1,0);
    if(n&1){
        z = BigNum(*this);
    }else{
        z+=1;
    }
    n = n >> 1;
    while(n > 0){
        q = q.FastSqr();
        if(n&1) z *= q;
        n = n >> 1;
    }
    return z;
}


BigNum BigNum::StandardPow(BigNum &n) {
    BigNum res(*this);
    BigNum i;
    i = 1;
    for(;i<n;i+=1){
        res *= *this;
    }
    return res;
}

BigNum BigNum::StandardPow(BASE n) {
    BigNum res(*this);
    BASE i = 1;
    for(;i<n;i+=1){
        res *= *this;
    }
    return res;
}


BigNum BigNum::BarretMod(BigNum &m, BigNum &z){
    if(len > 2 * m.len) throw invalid_argument("This number is too big for this method");
    
    BigNum q = ShiftR(m.len-1);
    q*=z;
    q = ShiftR(m.len + 1);

    BigNum r1 = *this;
    BigNum r2 = q * m;
    BigNum r(m.len+2, 0);

    if(r1>=r2){
        r = (r1 - r2);
    } else{
        r.coef[m.len+1] = 1;
        r.len = m.len + 2;
        r += (r1-r2);
    }

    while(r>=m){
        r-=m;
    }
    
    return r;
}


bool BigNum::fermat_test(BASE t){
    if(*this == BigNum(3) || *this == BigNum(2)){
        return true;
    }
    if(isEven()){
        return false;
    }
    while(t){
        BigNum a(len, 1);
        a.coef[len-1] = a.coef[len-1] % coef[len-1];
        if(a < 2 || a > (*this-2)){
            continue;
        }
        BigNum r = a.pow_mod(*this-1, *this);
        if(r != 1){
            return false;
        }
        t--;
    }
    return true;
}

bool BigNum::solovay_strassen_test(BASE t){
    if(*this == BigNum(3) || *this == BigNum(2)){
        return true;
    }
    if(isEven()){
        return false;
    }
    BigNum nm(*this-1);
    while(t){
        BigNum a(len, 1);
        a.coef[len-1] = a.coef[len-1] % coef[len-1];
        if(a < 2 || a > (*this-2)){
            continue;
        }
        BigNum r = a.pow_mod(nm/2, *this);
        if(r != 1 && r != nm){  
            return false;   
        }
        int s = Jacobi(a, *this);
        if(s != 1 && s != -1){
            return false;
        }
        t--;
    }
    return true;
}

bool BigNum::miller_rabin_test(BASE t){
    if(*this == BigNum(3) || *this == BigNum(2)){
        return true;
    }
    if(isEven()){
        return false;
    }
    
    BigNum r = BigNum(*this - 1);
    BigNum s = 0;
    while(r.isEven()){
        r /= 2;
        s += 1;
    }
    while(t){
        BigNum b(len, 1);
        b.coef[len-1] = b.coef[len-1] % coef[len-1];
        if(b < 2 || b > (*this-2)){
            continue;
        }

        BigNum y = b.pow_mod(r, *this);
        
        BigNum nm(*this-1);

        if(y != 1 && y != nm){
            BigNum j = 1;
            while(j < s && y != nm){
                y = y.FastSqr()%(*this);
                if(y == 1){
                    return false;
                }
                j += 1;
            }
            if(y != nm){
                return false;
            }
        }
        t--;
    }
    return true;

}

BigNum generator_strong_prime(BASE size){
    BigNum s = generator_prime(size);
    BigNum t = generator_prime(size);
    
    BigNum i(1, 1);
    BigNum r = i * t * 2 + 1;
    while(!r.miller_rabin_test(100)){
        i += 1;
        r = i * t * 2 + 1;
    }

    BigNum p0 = s.pow_mod(r-2, r) * s * 2 - 1;

    BigNum j(1, 1);
    BigNum p = (j * r * s * 2) + p0;
    while(!p.miller_rabin_test(100)){
        j += 1;
        p = (j * r * s * 2) + p0;
    }
    return p;
}

BigNum BigNum::phi(){
    BigNum result = *this;
    BigNum n = *this;
    for (BigNum i = 2; i.FastSqr() <= n; i += 1) {
        if (n % i == 0) {
            do {
                n /= i;
            } while (n % i == 0);
            result -= result / i;
        }
    }

    if (n > 1)
        result -= result / n;

    return result;
}

float BigNum::eps_fermat(){
    BASE shift = 10000;
    BigNum eps_bn = (this->phi()*shift / ((*this)));
    float eps = eps_bn.coef[0];
    return eps / shift;
}

float BigNum::eps_solovay_strassen(){
    BASE shift = 10000;
    BigNum eps_bn = (this->phi()*shift / ((*this * 2)));
    float eps = eps_bn.coef[0];
    return eps / shift;
}

float BigNum::eps_miller_rabin(){
    BASE shift = 10000;
    BigNum eps_bn = (this->phi()*shift / ((*this * 4)));
    float eps = eps_bn.coef[0];
    return eps / shift;

}

float pow(float eps, BASE t){
    t--;
    while(t){
        eps*=eps;
        t--;
    }
    return eps;
}
