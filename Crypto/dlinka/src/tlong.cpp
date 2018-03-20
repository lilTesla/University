#include "tlong.h"

#include<cstdlib>
#include<iomanip>
#include<cmath>
#include<cassert>
#include<algorithm>

tlong::tlong()
{
    sign = 0;
    a.clear();
}

tlong::tlong(long long value) : tlong()
{
    if(value < 0)
    {
        sign = 1;
        value = (-value);
    }

    if(value == 0)
        a.push_back(0);
    else
    {
        while(value)
        {
            a.push_back(value % tlong::BASE);
            value /= tlong::BASE;
        }
    }
}

tlong::tlong(const std::string &s) : tlong()
{
    int firstDigit = 0;
    if(s[0] == '-')
    {
        sign = 1;
        firstDigit = 1;
    }
    for(int i = (int)s.length(); i > firstDigit; i -= tlong::BASE_LENGTH)
        if(i < tlong::BASE_LENGTH + firstDigit)
        {
            std::string tmp = s.substr(firstDigit, i);
            int x = atoi(tmp.c_str());
            a.push_back(atoi(s.substr(firstDigit, i).c_str()));
        }
        else
            a.push_back(atoi(s.substr(i - tlong::BASE_LENGTH, tlong::BASE_LENGTH).c_str()));
}

tlong::tlong(const tlong& v) : tlong(v.a, v.sign)
{

}

tlong::tlong(const std::vector<int> &a_, int sign_)
{
    sign = sign_;
    a = a_;
}

const tlong operator +(const tlong& v)
{
    return v;
}

const tlong operator -(const tlong& v)
{
    return tlong(v.a, -v.sign);
}

tlong::operator bool() const
{
    return (*this != zero());
}
const bool operator !(tlong & v)
{
    return !(bool)v;
}

tlong::operator long long() const
{
    long long res = 0;
    for(int i=(int)a.size()-1;i>=0;--i)
        res=res*tlong::BASE+a[i];
    if(sign)
        res = -res;
    return res;
}

tlong tlong::operator=(const tlong& value)
{
    if(this == &value)
        return *this;
    a = value.a;
    sign = value.sign;
    return *this;
}

const bool operator < (const tlong& x, const tlong& y)
{
    return x.compareTo(y) == -1;
}
const bool operator <= (const tlong& x, const tlong& y)
{
    return x.compareTo(y) <= 0;
}
const bool operator > (const tlong& x, const tlong& y)
{
    return x.compareTo(y) == 1;
}
const bool operator >= (const tlong& x, const tlong& y)
{
    return x.compareTo(y) >= 0;
}

const bool operator ==(const tlong& x, const tlong& y)
{
    return x.compareTo(y) == 0;
}

const bool operator ==(const tlong& x, const long long y)
{
    return x.compareTo(tlong(y)) == 0;
}



const bool operator !=(const tlong& x, const tlong& y)
{
    return !(x == y);
}

int tlong::compareTo(const tlong &v) const
{
    if(sign == v.sign)
    {
        int res = 0;
        if(a.size() < v.a.size())
            res = -1;
        else if(a.size() > v.a.size())
            res = 1;
        else
        {
            for(int i = (int)a.size()-1; i >= 0; --i)
                if(a[i] < v.a[i])
                {
                    res = -1;
                    break;
                }
                else if(a[i] > v.a[i])
                {
                    res = 1;
                    break;
                }
        }
        return (sign ? -res : res);
    }
    else return v.sign - sign;
}

std::ostream& operator<<(std::ostream& os, const tlong& v)
{
    if(v.sign)
        os << '-';
    os << (v.a.empty() ? 0 : v.a.back());
    for(int i = (int)v.a.size() - 2; i >= 0; --i)
        os << std::setfill('0') << std::setw(tlong::BASE_LENGTH) << v.a[i];
    return os;
}
std::istream& operator>>(std::istream& is, tlong& v)
{
    std::string s;
    is >> s;
    //tlong tmp = tlong(s);
    //v.a = tmp.a;
    //v.sign = tmp.sign;
    v = tlong(s);
    return is;
}


const tlong operator + (const tlong& x, const tlong& y)
{
    if(x.sign == y.sign)
    {
        tlong res = tlong::addUnsigned(x, y);
        res.sign = x.sign;
        return res;
    }
    else if(x.sign)
        return tlong::subUnsigned(y, x);
    else
        return tlong::subUnsigned(x, y);
}
const tlong operator - (const tlong& x, const tlong& y)
{
    if(x.sign != y.sign)
    {
        tlong res = tlong::addUnsigned(x, y);
        res.sign = x.sign;
        return res;
    }
    else if(x.sign)
        return tlong::subUnsigned(y,x);
    else
        return tlong::subUnsigned(x,y);
}


const tlong operator + (const tlong& x, long long y)
{
    return x + tlong(y);
}
const tlong operator - (const tlong& x, long long y)
{
    return x - tlong(y);
}

const tlong operator * (const tlong& x, const tlong& y)
{
    tlong c;
    c.a.resize(x.a.size()+y.a.size());
    c.sign = x.sign^y.sign;
    for (size_t i=0; i<x.a.size(); ++i)
        for (int j=0, carry=0; j<(int)y.a.size() || carry; ++j) {
            long long cur = c.a[i+j] + x.a[i] * 1ll * (j < (int)y.a.size() ? y.a[j] : 0) + carry;
            c.a[i+j] = int (cur % tlong::BASE);
            carry = int (cur / tlong::BASE);
        }
    c.removeLeadingZeros();
    return c;
}

const tlong operator * (const tlong& x, long long y)
{
    tlong c = x;
    c.sign ^= (y < 0);
    int carry = 0;
    for (size_t i=0; i<c.a.size() || carry; ++i) {
        if (i == c.a.size())
            c.a.push_back (0);
        long long cur = carry + c.a[i] * 1ll * abs(y);
        c.a[i] = int (cur % tlong::BASE);
        carry = int (cur / tlong::BASE);
    }
    c.removeLeadingZeros();
    return c;
}

const tlong operator / (const tlong& x, const tlong& y)
{
    tlong res;
    res.sign = x.sign^y.sign;
    tlong carry = tlong::zero();
    for(int i = (int)x.a.size() - 1; i>=0; --i)
    {
        carry = carry * tlong::BASE + x.a[i];
        int l = 0;
        int r = tlong::BASE + 1;
        while(r-l>1)
        {
            int m = (l+r)>>1;
            if(abs(y) * m > carry)
                r = m;
            else
                l = m;
        }
        int cnt = l;
        if(abs(y) * r <= carry)
            cnt = r;
        carry = carry - abs(y) * cnt;
        res.a.push_back(cnt);
    }
    reverse(res.a.begin(),res.a.end());
    res.removeLeadingZeros();
    return res;
}

const tlong operator / (const tlong& x, long long y)
{
    tlong c = x;
    c.sign ^= (y < 0);
    y=abs(y);
    int carry = 0;
    for (int i=(int)c.a.size()-1; i>=0; --i) {
        long long cur = c.a[i] + carry * 1ll * tlong::BASE;
        c.a[i] = int (cur / y);
        carry = int (cur % y);
    }
    c.removeLeadingZeros();
    return c;
}
const tlong operator % (const tlong& x, const tlong& y)
{
    return x - x/y * y;
}

const long long operator % (const tlong& x, long long y)
{
    return x - x/y * y;
}

const tlong gcd(tlong x, tlong y)
{
    return y?gcd(y,x%y):x;
}
const tlong lcm(tlong x, tlong y)
{
    return x / gcd(x,y) * y;
}


const tlong pow(tlong x, tlong y, tlong mod)
{
    if (y == tlong::zero())
        return tlong::one();
    else if (y % 2 == 1)
        return x * pow(x, y - 1, mod) % mod;
    else
        return pow(x * x % mod, y / 2, mod);
}

const tlong pow(tlong x, tlong y)
{
    if (y == tlong::zero())
        return tlong::one();
    else if (y % 2 == 1)
        return x * pow(x, y - 1);
    else
        return pow(x * x, y / 2);
}


const tlong rand(tlong r)
{
    return rand(0, r);
}
const tlong rand(tlong l, tlong r)
{
    if (l == r)
        return l;
    tlong m = (l + r) / 2;
    if (rand()&1)
        return rand(l, m);
    else
        return rand(m + 1, r);
}


const tlong sqrt(tlong n)
{
    tlong l = 0;
    tlong r = n;
    while (r > l + 1)
    {
        tlong m = (l + r) / 2;
        tlong dd = m * m;
        int z = dd.compareTo(n);
        if (z == 0)
            return m;
        else if (z < 0)
            l = m;
        else
            r = m;
    }
    return l;
}

tlong tlong::addUnsigned(const tlong& x, const tlong& y)
{
    int carry = 0;
    tlong res = x;
    res.sign = 0;
    for(int i = 0; i < std::max(res.a.size(), y.a.size()) || carry; ++i)
    {
        if(i == res.a.size())
            res.a.push_back(0);
        res.a[i] += carry + (i < y.a.size() ? y.a[i] : 0);
        carry = res.a[i] >= tlong::BASE;
        if(carry)
            res.a[i] -= tlong::BASE;
    }
    return res;
}

tlong tlong::subUnsigned(const tlong& x, const tlong& y)
{
    if(abs(y) > abs(x))
    {
        tlong res = subUnsigned(y, x);
        res.sign = 1;
        return res;
    }
    int carry = 0;
    tlong res = x;
    res.sign = 0;
    for(int i = 0; i < std::max(res.a.size(), y.a.size()) || carry; ++i)
    {
        res.a[i] -= carry + (i < y.a.size() ? y.a[i] : 0);
        carry = res.a[i] < 0;
        if(carry)
            res.a[i] += tlong::BASE;
    }
    res.removeLeadingZeros();
    return res;
}


void tlong::removeLeadingZeros()
{
    while (a.size() > 1 && a.back() == 0)
	a.pop_back();
}

tlong tlong::zero()
{
    static tlong ZERO = tlong(0);
    return ZERO;
}
tlong tlong::one()
{
    static tlong ONE = tlong(1);
    return ONE;
}

tlong abs(const tlong& v)
{
    return tlong(v.a, 0);
}

tlong::~tlong()
{
    //dtor
}
