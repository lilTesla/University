#include <bits/stdc++.h>
#include "tlong.h"

using namespace std;

bool isPrime(tlong n, int iterations = 10) //miler rabin +
{
    if (n == 2 || n == 3)
        return true;
    if (n == tlong::one() || n % 2 == 0)
        return false;
    tlong d = n - 1;
    long long s = 0;
    while (d % 2 == 0)
    {
        s = s + 1;
        d = d / 2;
    }
    while (iterations--)
    {
        tlong a = rand(2, n - 2);
        tlong x = pow(a, d, n);
        if (x == tlong::one() || x == n - 1)
            continue;
        bool goNextIteration = false;
        for (long long i = 0; i < s - 1; ++i)
        {
            x = pow(x, 2, n);
            if (x == tlong::one())
                return false;
            if (x == n - 1)
            {
                goNextIteration = true;
                break;
            }
        }
        if (goNextIteration)
            continue;
        else
            return false;
    }
    return true;
}

map<tlong, int> make_map(tlong key, int value)
{
    map<tlong, int> m;
    m[key] = value;
    return m;
}
map<tlong, int> factor(tlong n, int iterations = 100)//+
{
    if (n == tlong::one())
        return map<tlong,int>();
    if (isPrime(n))
        return make_map(n, 1);
    tlong sq = sqrt(n);
    if (sq * sq == n)
    {
        map<tlong, int> v = factor(sq);
        for (auto it = v.begin(); it != v.end(); ++it)
            it->second *= 2;
        return v;
    }
    int step = 2;
    tlong g = tlong::one();
    while(true)
    {
        tlong x0 = step++;
        tlong x1 = (x0 * x0 + 1) % n;
        g = gcd(abs(x0 - x1), n);
        int i = 0;
        while (g == tlong::one())
        {
            x0 = (x0 * x0 + 1) % n;
            x1 = (x1 * x1 + 1) % n;
            x1 = (x1 * x1 + 1) % n;
            g = gcd(abs(x0 - x1), n);
        }
        if (g != n)
            break;
    }
    map<tlong, int> v1 = factor(g);
    map<tlong, int> v2 = factor(n / g);
    for (auto p : v2)
        v1[p.first] += p.second;
    return v1;
}


int mobius(tlong n)//+
{
    map<tlong, int> f = factor(n);
    for (auto p : f)
        if (p.second > 1)
            return 0;
    return f.size() % 2 ? -1 : 1;
}

tlong euler(tlong n)//+
{
    map<tlong, int> f = factor(n);
    tlong result = n;
    for (auto p : f)
    {
        result = result - result / p.first;
    }
    return result;
}

int jacobi(tlong a, tlong b)
{
    if (gcd(a, b) != tlong::one())
        return 0;

    int r = 1;
    if (a < tlong::zero())
    {
        a = -a;
        if (b % 4 == 3)
            r = -r;
    }

    while(true)
    {
        tlong t = 0;
        while (a % 2 == 0)
        {
            t = t + 1;
            a = a / 2;
        }
        if (t % 2 == 1)
            if (b % 8 == 3 || b % 8 == 5)
                r = -r;

        if (a % 4 == b % 4 && b % 4 == 3)
            r = -r;
        tlong c = a;
        a = b % c;
        b = c;

        if (a == tlong::zero())
            return r;

    }
}

int lejandra(tlong a, tlong b)
{
    return jacobi(a, b);
}

tlong diskret_log(tlong a, tlong b, tlong p)
{
    tlong p1 = p - 1;
    tlong d1 = p / 3 + 1;
    tlong d2 = p * 2 / 3 + 1;

    tlong u0 = 0;
    tlong v0 = 0;
    tlong z0 = 1;
    tlong u1 = 0;
    tlong v1 = 0;
    tlong z1 = 1;

    u1 = (z1 < d1 ? u1 + 1 : (z1 < d2 ? u1 * 2 : u1)) % p1;
    v1 = (z1 < d1 ? v1 : (z1 < d2 ? v1 * 2 : v1 + 1)) % p1;
    z1 = pow(b, u1, p) * pow(a, v1, p) % p;
    long long iter = 2;
    while (true)
    {
        iter++;
        if(iter > 1e3)
        {
            return tlong(-1);
        }
        u0 = (z0 < d1 ? u0 + 1 : (z0 < d2 ? u0 * 2 : u0)) % p1;
        v0 = (z0 < d1 ? v0 : (z0 < d2 ? v0 * 2 : v0 + 1)) % p1;
        z0 = pow(b, u0, p) * pow(a, v0, p) % p;

        u1 = (z1 < d1 ? u1 + 1 : (z1 < d2 ? u1 * 2 : u1)) % p1;
        v1 = (z1 < d1 ? v1 : (z1 < d2 ? v1 * 2 : v1 + 1)) % p1;
        z1 = pow(b, u1, p) * pow(a, v1, p) % p;

        u1 = (z1 < d1 ? u1 + 1 : (z1 < d2 ? u1 * 2 : u1)) % p1;
        v1 = (z1 < d1 ? v1 : (z1 < d2 ? v1 * 2 : v1 + 1)) % p1;
        z1 = pow(b, u1, p) * pow(a, v1, p) % p;

        if (z0 == z1)
        {
            tlong du = abs(u1 - u0);
            tlong dv = abs(v0 - v1);
            tlong d = gcd(du, p1);
            if (d == tlong::one())
            {
                tlong x = pow(du, p - 3, p1) * dv % p1;
                if (pow(a, x, p) == b)
                    return x;
            }
            if (d > tlong::one() && d < tlong(iter))
            {
                long long g = (long long)d;
                tlong newP = p1 / d;
                tlong x0 = 0;
                if (du > tlong::zero())
                    x0 = pow(du, newP - 2, newP) * dv % newP;
                for (long long m = 0; m < g; ++m)
                {
                    tlong x = x0 + newP * m;
                    if (pow(a, x, p) == b)
                        return x;
                }
            }
        }
    }

}

pair<tlong, tlong> rsa_gen2primes()
{
    tlong p = rand(tlong(1000), tlong(2000));
    while (!isPrime(p))
        p = p + 1;
    tlong q = p + 1;
    while (!isPrime(q))
        q = q + 1;
    return {p, q};
}

pair<pair<tlong, tlong>, pair<tlong, tlong> >  rsa_generate_keys(tlong p, tlong q)
{
    tlong n = p * q;
    tlong phi = (p - 1) * (q - 1);
    tlong e = 3;/*
    if(phi%257!=)*/
    while (gcd(e, phi) != tlong::one())
        e = e + 1;
    tlong phi2 = euler(phi);
    tlong d = pow(e, phi2 - 1, phi);
    d = d + phi * 5;
    return {{e,n},{d,n}};
}

tlong rsa_op(tlong v, pair<tlong, tlong> key)
{
    return pow(v, key.first, key.second);
}

void test_rsa()
{
    //auto p = make_pair(tlong(3557),tlong(2579));//rsa_gen2primes();
    //auto p = make_pair(tlong(3),tlong(5));//rsa_gen2primes();
    auto p = rsa_gen2primes();
    cout << "primes: " << p.first <<":"<< (isPrime(p.first)?1:0) << " " << p.second << ":"<<(isPrime(p.second)?1:0) << "\n";
    auto k = rsa_generate_keys(p.first, p.second);
    cout << "public-key: [" << k.first.first << ", " << k.first.second << "]\n";
    cout << "private-key: [" << k.second.first << ", " << k.second.second << "]\n";
    tlong x;
    while (cin >> x)
    {
        tlong v = rsa_op(x, k.first);
        cout << v << " " << rsa_op(v, k.second) << "\n";
    }
}

void test_diskrete()
{
    vector<tlong> pr;
    for(int i=4;i<1000;++i)
        if(isPrime(tlong(i)))
            pr.push_back(tlong(i));
    tlong a,b,p,x,x1;
    for(int i = 0; i < 10;++i)
    {
        int ind = rand()%pr.size();
        p = pr[ind];
        a = rand(tlong(2), p);
        x = rand(tlong(2), tlong(100));
        b = pow(a,x,p);
        x1 = diskret_log(a, b, p);
        cout << a << " ^ " << x << " % " << p << " = " << b <<"\n";
        cout << "x = " << x1 << " -> a^x%p = " << pow(a, x1, p) << "\n";
    }
    while(cin >> a >> b >> p)
        cout << diskret_log(a,b,p) << "\n";
}

void test_prime()
{
    tlong x;
    while (cin >> x)
        cout << (isPrime(x)?"prime":"non-prime") << "\n";
}

void test_factor()
{
    tlong x;
    while (cin >> x)
    {
        auto f = factor(x);
        for (auto p : f)
            cout << p.first << ":" << p.second << " ";
        cout << "\n";
    }
}

void test_mobius()
{
    tlong x;
    while (cin >> x)
        cout << mobius(x) << "\n";
}

void test_euler()
{
    tlong x;
    while (cin >> x)
        cout << euler(x) << "\n";
}

void test_jacobi_lejandra()
{
    tlong a, p;
    while (cin >> a >> p)
        cout << jacobi(a, p) << "\n";
}

int main()
{
    srand(time(0));
    test_rsa();
}
