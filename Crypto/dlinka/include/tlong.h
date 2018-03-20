#ifndef TLONG_H
#define TLONG_H

#include<vector>
#include<iostream>
#include<string>

class tlong
{
    static const int BASE = 1e9;
    static const int BASE_LENGTH = 9;
    public:
        tlong();
        tlong(long long value);
        tlong(const std::string &s);
        tlong(const tlong& value);
        tlong operator=(const tlong& value);
        static tlong zero();
        static tlong one();
        virtual ~tlong();

        friend const tlong operator + (const tlong &v);
        friend const tlong operator - (const tlong &v);
        //friend const tlong& operator ++ (tlong &v);
        //friend const tlong operator ++ (tlong &v, int);
        //friend const tlong& operator -- (tlong &v);
        //friend const tlong operator -- (tlong &v, int);

        friend const tlong operator + (const tlong& x, const tlong& y);
        friend const tlong operator + (const tlong& x, long long y);
        friend const tlong operator - (const tlong& x, const tlong& y);
        friend const tlong operator - (const tlong& x, long long y);
        friend const tlong operator * (const tlong& x, const tlong& y);
        friend const tlong operator * (const tlong& x, long long y);
        friend const tlong operator / (const tlong& x, const tlong& y);
        friend const tlong operator / (const tlong& x, long long y);
        friend const tlong operator % (const tlong& x, const tlong& y);
        friend const long long operator % (const tlong& x, long long y);

        friend const bool operator < (const tlong& x, const tlong& y);
        friend const bool operator > (const tlong& x, const tlong& y);
        friend const bool operator <= (const tlong& x, const tlong& y);
        friend const bool operator >= (const tlong& x, const tlong& y);
        friend const bool operator ==(const tlong& x, const tlong& y);
        friend const bool operator ==(const tlong& x, const long long y);
        friend const bool operator !=(const tlong& x, const tlong& y);

        friend const tlong gcd(tlong x, tlong y);
        friend const tlong lcm(tlong x, tlong y);
        friend const tlong pow(tlong x, tlong y, tlong mod);
        friend const tlong pow(tlong x, tlong y);
        friend const tlong rand(tlong r);
        friend const tlong rand(tlong l, tlong r);
        friend const tlong sqrt(tlong n);

        operator bool() const;
        operator long long() const;
        bool operator ! ();

        friend std::ostream& operator<<(std::ostream& os, const tlong& v);
        friend std::istream& operator>>(std::istream& is, tlong& v);

        friend tlong abs(const tlong& v);

        //friend ostream

    private:

        int sign;
        std::vector<int> a;//digits

        tlong(const std::vector<int> &a_, int sign_ = 0);
        void removeLeadingZeros();
        static tlong addUnsigned(const tlong &x, const tlong &y);
        static tlong subUnsigned(const tlong &x, const tlong &y);
        int compareTo(const tlong& v) const;

};

#endif // TLONG_H
