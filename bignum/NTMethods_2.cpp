#include "NTMethods.cpp"


vector<BigNum> BigNum::fact()
{
	vector<BigNum> p;
	if (fermat_test(10))
	{
		p.push_back(*this);
		return p;
	}
	BigNum n(*this), dk, dkPrev, one(BASE(1)), two(BASE(2));
	while (n % BASE(2) == BASE(0))
	{
		p.push_back(two);
		n /= BASE(2);
	}
	auto ds = sqrt();
	auto k = 1,
	preK = 0;
	while (dk < ds && n != one)
	{
		if (k != preK) 
			switch (k)
			{
				case 1:
					dk = BASE(3);
					break;
				case 2:
					dkPrev = dk;
					dk = BASE(5);
					break;
				case 3:
					dkPrev = dk;
					dk = BASE(7);
					break;
				default:
					auto tmp = dk;
					dk = dkPrev + BASE(6);
					dkPrev = tmp;
					break;
			}
		auto q = n / dk, // 3
		r = n % dk; // 3
		if (r == 0) // 4
		{
			p.push_back(dk);
			preK = k;
			n = q;
		}
		else if (q > dk) // 5
		{
			k++;
		}
		else
		{
			p.push_back(n);
			break;
		}
	}
	return p;
}

BigNum BigNum::methodAlway(BigNum d)
{
	if (fermat_test(10)) return *this;
	if (*this % BASE(2) == BASE(0)) throw invalid_argument("n must be odd");
	BigNum r1, r2, s, q, r;
	BigNum zero(BigNum(0));
	r1 = *this % d;
	r2 = *this % (d - (BASE(2)));
	s = sqrt();
	q = ((*this / (d - BASE(2)) - (*this / d))) * BASE(4);
	while (1)
	{
		d += BASE(2);
		if (d > s) { cout << "d>s" << endl; return BASE(0); }
		if ((r1 * BASE(2) + q) >= r2)
		{
			r = r1 * BASE(2) + q - r2;
			r2 = r1;
			r1 = r;
		}
		else
		{
			r = r1 * BASE(2) + q + d - r2;
			r2 = r1;
			r1 = r;
			q += BASE(4);
		}
		while (r1 >= d)
		{
			r1 -= d;
			q -= BASE(4);
		}
		if (r1 == 0) return d;
	}

}


pair<BigNum, BigNum> BigNum::methodFermat()
{
	if (fermat_test(10)) { cout << "n is a prime number" << endl; return make_pair(BASE(1), *this); }
	if (*this % BigNum(BASE(2)) == BASE(0)) { cout << "n must be odd" << endl; return make_pair(*this, *this); }
	BigNum x = sqrt();
	BigNum z = x.FastSqr();
	if (z == *this) return make_pair(x, x);
	BigNum  y;
	do
	{
		x += BASE(1);
		//if (x == ((*this + BASE(1)) / BASE(2))) { cout << "n is a prime number" << endl; return make_pair(BASE(1), *this); }
		z = x.FastSqr() - *this;
		y = z.sqrt();
	}while (y.FastSqr() != z);
	return make_pair(x + y, x - y);
}

BigNum BigNum::Polard()
{
	BigNum d, One(BASE(1));
	BigNum a(BASE(2)), b(BASE(2));
	int c = 1;
	do
	{
		a = FunPolard(a, c);
		b = FunPolard(FunPolard(b,c), c);
		if (a == b)
		{
			return *this;
			//c++;
			//continue;
		}
		d = gcd(a.MinusMod(b));
	}while (d == One);
	return d;
}

BigNum BigNum::Gelfond(BigNum g, BigNum a)
{
	BigNum h, b, One(BASE(1)), i(BASE(1)), z, q;
	h = sqrt() + BASE(1);
	b = g.pow_mod(h, *this);
	vector<pair<BigNum, BigNum>> Gigant;
	for (; i <= h; i += BASE(1))
		Gigant.push_back(make_pair(b.pow_mod(i, *this), i));
	Gigant = Shall(Gigant);
	i = BASE(1);
	for (; i <= h; i += BASE(1))
	{
		q = (a * g.pow_mod(i, *this) % *this);
		auto  X = BitSearch(Gigant, q);
		if (X.first)
			return ((h * X.second) - i) % *this;
	}
	return One;
}

BigNum  BigNum::pPolard(BigNum g, BigNum a)
{
	BigNum d, r, rx, x, Zero(BASE(0));
	vector <BigNum> xuv1(3); // x1 u1 v1;
	vector <BigNum> xuv2(3); // x2 u2 v2;
	xuv1[0] = BASE(1); xuv1[1] = BASE(0); xuv1[2] = BASE(0);
	xuv2[0] = BASE(1); xuv2[1] = BASE(0); xuv2[2] = BASE(0);
	while(1)
	{
		xuv1 = Fx(xuv1, g, a);
		xuv2 = Fx(xuv2, g, a);
		xuv2 = Fx(xuv2, g, a);
		if (xuv1[0] == xuv2[0]) break;
	}
	if (xuv1[2] < xuv2[2]) xuv1[2] += (*this - BASE(1));
	r = (xuv1[2] - xuv2[2]) % (*this - BASE(1)); // 4
	if (r == BASE(0)) return Zero;
	d = r.gcd((*this) - BASE(1)); // gcd (r,n-1)
	if (xuv2[1] < xuv1[1]) xuv2[1] += (*this - BASE(1));
	rx = ((xuv2[1] - xuv1[1]) / d);
	r = r / d;
	x = p_mod_1(r, (*this - BASE(1)) / d);
	x = (rx * x) % ((*this - BASE(1)) / d);
	if (g.pow_mod(x, *this) == a) return x;
	for (BigNum i = BASE(1); i < d; i += BASE(1))
	{
		x = x + (*this - 1) / d * i;
		if (g.pow_mod(x, *this) == a) return x;
	}
	return Zero;
}


void TestFact()
{
	BigNum a;
	a.DecInput("8671");
	vector<BigNum> t=a.fact();
	for (int i = 0; i < t.size(); ++i) {
		t[i].DecOutput();
		cout << endl;
	}
}

void TestAlway()
{
	BigNum a;
	BigNum d;
	BigNum res;
	a.DecInput("41063625");
	a.DecInput("8671");
	d = a.cbrt() * BASE(2) + BASE(1);
	res = a.methodAlway(d);
	res.DecOutput();
	cout<<"\n";
	d.DecOutput();
}

void TestFermat()
{
	BigNum n;
	n.DecInput("256590620791");
	//n.DecOutput();
	pair<BigNum, BigNum> ab = n.methodFermat();
	ab.first.DecOutput();
	cout<<"  ";
	ab.second.DecOutput();
}

void TestPolard()
{
	BigNum n, d;
	n.DecInput("5324233");
	d = n.Polard();
	d.DecOutput();
}

void TestGelfond()
{
	BigNum a, n, g;
	n.DecInput("1579");
	g.DecInput("423");
	a.DecInput("523");
	n.Gelfond(g, a).DecOutput();
	cout<<"\n";
}

void TestpPolard()
{
	BigNum a, n, g;
	n.DecInput("1579");
	g.DecInput("423");
	a.DecInput("523");
	n.pPolard(g, a).DecOutput();
}


int main()
{
    TestFact();
	//TestAlway();
	//TestFermat();
	//TestPolard();
	//TestGelfond();
	//TestpPolard();
	return 0;
}