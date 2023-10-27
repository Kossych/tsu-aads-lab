#include "BigNum.cpp"
#include <chrono>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;



BASE Jacobi(const BigNum& a, const BigNum& n){
    if(a == 0 || a == 1){
        return a.coef[0];
    }
    BigNum a1 = a;
    BigNum k = 0;
    while (a1.isEven()) {
        a1 /= 2;
        k += 1;
    }

    BASE s;
    if (k.isEven()) {
        s = 1;
    } else {
        BigNum rem = n % 8;
        if (rem == 1 || rem == 7) {
            s = 1;
        } else {
            s = -1; 
        }
    }

    if (n % 4 == 3 && a1 % 4 == 3) {
        s = -s;
    }

    if (a1 == 1) {
        return s;
    }
    return s * Jacobi(n % a1, a1);
}


bool BigNum::ferma_test(BASE t){
    if(*this == BigNum(3) || *this == BigNum(2)){
        return true;
    }
    if(isEven()){
        return false;
    }
    while(t){
        BigNum a(len, 1);
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
        a.coef[len] = a.coef[len] % coef[len];
        if(a < 2 || a > (*this-2)){
            continue;
        }
        BigNum r = pow_mod(nm/2, *this);
        if(r != 1 || r != nm){  
            return false;
        }
        BASE s = Jacobi(a, *this);
        if(s == 1){
            if(r != 1){
                return false;
            }
        }else{
            if(r != nm){
                return false;
            }
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

BigNum generator_prime(BASE size){
    BigNum prime;
    do{
        prime = BigNum(size, 1);
    }while(!prime.solovay_strassen_test(100));
    return prime;
}

BigNum generator_strong_prime(BASE size){
    BigNum s(size, 1);
    BigNum t(size, 1);
    
    BigNum i(1, 1);
    BigNum r = i * t * 2 + 1;
    while(!r.solovay_strassen_test(100)){
        i += 1;
        r = i * t * 2 + 1;
    }

    BigNum p0 = s.pow_mod(r-2, r) * s * 2 - 1;

    BigNum j(1, 1);
    BigNum p = (j * r * s * 2) + p0;
    while(!r.solovay_strassen_test(100)){
        j += 1;
        p = (j * r * s * 2) + p0;
    }
    return p;
}


int main() {
    BigNum strong_prime = generator_strong_prime(2);
    std::cout << "Generated strong prime: ";
    strong_prime.DecOutput();
    std::cout << (strong_prime.solovay_strassen_test(100) == 1
                  ? "Is prime acc. to Solovay Strassen"
                  : "Is not prime acc. Solovay Strassen")
              << std::endl;


    // False positive examples
    BigNum number(BASE(565));

    size_t attempt = 0;
    while (number.ferma_test(3) == false) { attempt++; }
    std::cout << "Fermat prime test lied on " << attempt << " attempt" << std::endl;

    attempt = 0;
    while (number.solovay_strassen_test(2) == false) { attempt++; }
    std::cout << "Solovay Strassen prime test lied on " << attempt << " attempt" << std::endl;
}