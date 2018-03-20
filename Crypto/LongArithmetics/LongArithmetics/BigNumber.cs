using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LongArithmetics
{
    class BigNumber : ICloneable, IComparable<BigNumber>
    {
        //Поля + свойства
        private bool isNegative;
        private int[] Number; //представление числа в виде массива его цифр
        public int Length //длина числа
        {
            get { return Number.Length; }
        }
        private int this[int i]
        {
            get { return Number[i]; }
            set { Number[i] = value; }
        }
        public string Value //свойство-обёртка для Number. На вход и выход принимает/отдаёт строку
        {
            get
            {
                int k = Number.Length - 1;
                while (k != 0 && Number[k] == 0)
                {
                    k--;
                }
                string num = k >= 0 ? "" : "0";
                for (int i = k; i >= 0; i--)
                {
                    num = $"{num}{Number[i].ToString()}";
                }
                if (isNegative)
                    num = "-" + num;
                return num;
            }
            set
            {
                value = value.Trim();
                isNegative = value[0] == '-' ? true : false;
                if (isNegative)
                    value = value.Substring(1);
                Number = new int[value.Length];
                while (value.Length != 1 && value[0]=='0')
                    value = value.Substring(1);
                for (int i = value.Length - 1, j = 0; i >= 0; i--, j++)
                {
                    Number[j] = ((int)value[i] - 48);
                }
            }
        }
        
        //Конструкторы        

        public BigNumber(int[] num)
        {
            Number = num;
        }
        public BigNumber (int Len)
        {
            Number = new int[Len];
            for (int i = 0; i < Len; i++)
            {
                Number[i] = 0;
            }
        }
        public BigNumber (string Num)
        {
            Num = Num.Trim();
            isNegative = Num[0] == '-' ? true : false;
            if (isNegative)
                Num = Num.Substring(1);

            Number = new int[Num.Length];
            while (Num.Length!=1 && Num[0] == '0')
                Num = Num.Substring(1);
            for (int i = Num.Length - 1, j = 0; i >= 0; i--, j++)
            {
                Number[j] = ((int)Num[i] - 48);
            }
        }

        //Методы, не соответствующие операциям
        private BigNumber ChangeSign()
        {
            isNegative = !isNegative;
            return this;
        }
        public object Clone()
        {
            BigNumber @new = new BigNumber(this.Length);
            @new.isNegative = this.isNegative;
            @new.Number = this.Number;
            return @new;
        }
        public int CompareTo(BigNumber other)
        {
            if (this.Length > other.Length)
                return 1;
            else if (other.Length > this.Length)
                return -1;
            else
                for (int i = Length - 1; i >= 0; --i)
                {
                    if (this.Number[i] > other.Number[i])
                        return 1;
                    if (other.Number[i] > this.Number[i])
                        return -1;
                }
            return 0;
        }
        private static int[] CleanFromInsign0(int[] withInsign0)
        {
            int k = withInsign0.Length - 1;
            while (k != 0 && withInsign0[k] == 0)
            {
                k--;
            }
            int[] withoutInsign0 = new int[k + 1];
            for (int i = 0; i < withoutInsign0.Length; i++)
            {
                withoutInsign0[i] = withInsign0[i];
            }
            return withoutInsign0;
        }
        private BigNumber CleanFromInsign0()
        {
            int k = this.Length - 1;
            while (k != 0 && this[k] == 0)
            {
                k--;
            }
            BigNumber withoutInsign0 = new BigNumber(k + 1);
            for (int i = 0; i < withoutInsign0.Length; i++)
            {
                withoutInsign0[i] = this[i];
            }
            return withoutInsign0;

        }
        private BigNumber GetSubNumber(int len)
        {
            int baseLen = Length;

            if (baseLen < len)
                throw new ArgumentException("Can't take a subnumber longer than base number");
            if (baseLen == len)
                return this;

            int[] arr = new int[len];
            for(int i = 0;i<len;++i)
            {
                arr[i] = Number[baseLen - len + i];
            }

            return new BigNumber(arr);
        }


        #region Operations
        public static BigNumber operator +(BigNumber first, BigNumber second)
        {
            BigNumber first_ = (BigNumber)first.Clone();
            BigNumber second_ = (BigNumber)second.Clone();

            if (first_.isNegative && second_.isNegative)
                return (first_.ChangeSign() + second_.ChangeSign()).ChangeSign(); //(-a)+(-b) = -(a+b)
            else if (first_.isNegative)                                          //(-a)+b = b-a
                return second_ - first_.ChangeSign();
            else if (second_.isNegative)                                         //a+(-b) = a-b 
                return first_ - second_.ChangeSign();


            int Len = first_.Length >= second_.Length ? first_.Length + 1  : second_.Length + 1;
            int smallerLen = first_.Length > second_.Length ? second_.Length : first_.Length;
            
            int[] result = new int[Len];
            int buffer = 0;
            int i = 0;

            for (; i<smallerLen; i++)
            {
                int digitSum = first_.Number[i] + second_.Number[i] + buffer;
                buffer = digitSum / 10;
                result[i] = digitSum % 10;
            }
            if (first_.Length >= second_.Length)
            {
                for (; i < Len - 1; i++)
                {
                    result[i] = (first_.Number[i] + buffer);
                    buffer = result[i] / 10;
                    result[i] %= 10;
                }
                result[Len-1] = buffer;
            }
            else
            {
                for (; i < Len - 1; i++)
                {
                    result[i] = (second_.Number[i] + buffer);
                    buffer = result[i] / 10;
                    result[i] %= 10;
                }
                result[Len-1] = buffer;
            }

            BigNumber res = new BigNumber(Len);
            res.Number = result;
            return res;
        }
        public static BigNumber operator -(BigNumber first, BigNumber second)
        {
            BigNumber first_ = (BigNumber)first.Clone();
            BigNumber second_ = (BigNumber)second.Clone();

            if (first_.isNegative && second_.isNegative)                  //(-a)-(-b) = b-a
                return second_.ChangeSign() - first_.ChangeSign();
            else if (first_.isNegative)                                  //(-a)-b = -(a+b)
                return (first_.ChangeSign() + second_).ChangeSign();
            else if (second_.isNegative)                                 //a-(-b) = a+b
                return first_ + second_.ChangeSign();

            if (first_ < second_)
                return (second_ - first_).ChangeSign();
            if (first_ == second_)
                return new BigNumber("0");


            int Len = first_.Length;

            int[] result = new int[Len];
            int i = 0;
            int buffer = 0;
            for (; i < second_.Length; i++)
            {
                int sub = second_[i] + buffer;
                if (first_[i] >= sub)
                {
                    result[i] = first_[i] - sub;
                    buffer = 0;
                }
                else
                {
                    result[i] = first_[i] + 10 - sub;
                    buffer = 1;
                }
            }
            for (; i < Len; i++)
            {
                if (first_[i] < buffer)
                {
                    result[i] = first_[i] + 10 - buffer;
                    buffer = 1;
                }
                else
                {
                    result[i] = first_[i] - buffer;
                    buffer = 0;
                }
            }
            if (result[Len - 1] == 0)
                result = CleanFromInsign0(result);

            BigNumber res = new BigNumber(Len);
            res.Number = result;
            return res;
        }
        public static BigNumber operator *(BigNumber first, BigNumber second)
        {
            BigNumber first_ = (BigNumber)first.Clone();
            BigNumber second_ = (BigNumber)second.Clone();

            if (first_.isNegative && second_.isNegative)                      //(-a)*(-b) = a*b
                return first_.ChangeSign() * second_.ChangeSign();
            else if (first_.isNegative)                                      //(-a)*b = -(a*b)
                return (first_.ChangeSign() * second_).ChangeSign();
            else if (second_.isNegative)                                     //a*(-b) = -(a*b)
                return (first_ * second_.ChangeSign()).ChangeSign();

            if (first_.Length < second_.Length)
                return second_ * first_;

            BigNumber answer = new BigNumber(first_.Length + second_.Length);
            for (int i = 0; i < second_.Length; i++)
            {
                BigNumber mltpledByCurDigit = new BigNumber(first_.Length + second_.Length);
                int buffer = 0;
                for (int j = 0; j < first_.Length; j++)
                {
                    mltpledByCurDigit[i + j] = (first_[j] * second_[i] + buffer) % 10;
                    buffer = (first_[j] * second_[i] + buffer) / 10;
                }
                if (buffer > 0)
                {
                    mltpledByCurDigit[first_.Length] += buffer;
                }

                answer = answer + mltpledByCurDigit;
            }
            
            return answer.CleanFromInsign0();
        }
        public static BigNumber operator /(BigNumber first, BigNumber second)
        {
            BigNumber first_ = (BigNumber)first.Clone();
            BigNumber second_ = (BigNumber)second.Clone();

            if (first_.isNegative && second_.isNegative)
                return first_.ChangeSign() / second_.ChangeSign();
            if (first_.isNegative)
                return (first_.ChangeSign() / second_).ChangeSign();
            if (second_.isNegative)
                return (first_ / second_.ChangeSign()).ChangeSign();

            if (first_ < second_)
                return new BigNumber("0");
            if (first_ == second_)
                return new BigNumber("1");

            int[] res = new int[first_.Length];
            for (int i = 0; i < res.Length; ++i)
                res[i] = -1;
            
            for(int i = 0; new BigNumber(first_.Value) >= second_ ; ++i)
            {
                BigNumber a = first_.GetSubNumber(second_.Length);
                if (a < second_)
                    a = first_.GetSubNumber(second_.Length + 1);
                int n = 1;
                while((new BigNumber(n.ToString()))*second_ <= a)
                {
                    n++;
                }
                --n;
                res[i] = n;

                int t = a.Length;
                a -= (new BigNumber(n.ToString())) * second_;
                int aLen = a.Length;
                if (a.Length == 1 && a[0] == 0)
                    aLen = 0;

                int[] arr = new int[first_.Length - t + aLen];
                for (int j = 0, k = 0; j < arr.Length; ++j)
                {
                    if (j < first_.Length - t)
                        arr[j] = first_[j];
                    else
                    {
                        arr[j] = a[k];
                        k++;
                    }
                }

                if (arr.Length > 0)
                    first_ = new BigNumber(arr);
                else
                    first_ = new BigNumber("0");

                //bool flag = false;
                //for (int p = 0; p < arr.Length; ++p)
                //    if (arr[p] != 0)
                //        flag = true;
                //if (flag == false)
                //    break;
                
            }
            int lenOfCorrectRes = 0;
            for (; lenOfCorrectRes < res.Length && res[lenOfCorrectRes] != -1; ++lenOfCorrectRes) { }

            if (lenOfCorrectRes < res.Length && first_.Value == "0")
                for (int i = 0; i < first_.Length; ++i)
                    res[lenOfCorrectRes + i] = 0;
            int lenOfCorrectRes_ = 0;
            for (; lenOfCorrectRes_ < res.Length && res[lenOfCorrectRes_] != -1; ++lenOfCorrectRes_) { }
            Array.Resize(ref res, lenOfCorrectRes_);
            Array.Reverse(res);
            return new BigNumber(res);
        }

        public static BigNumber operator %(BigNumber first, BigNumber second)
        {
            BigNumber result = first - (first / second) * second;
            return result;
        }

        public static BigNumber Gcd(BigNumber first, BigNumber second)
        {
            BigNumber first_ = (BigNumber)first.Clone();
            BigNumber second_ = (BigNumber)second.Clone();

            first_.isNegative = false;
            second_.isNegative = false;

            BigNumber zero = new BigNumber("0");
            while (first_ > zero && second_ > zero)
            {
                if (first_ > second_)
                    first_ = first_ % second_;
                else
                    second_ = second_ % first_;
            }
            if (first_ > second_ ) return first_;
            else return second_;
        }

        public static BigNumber Lcm(BigNumber first, BigNumber second)
        {
            BigNumber first_ = (BigNumber)first.Clone();
            BigNumber second_ = (BigNumber)second.Clone();

            first_.isNegative = false;
            second_.isNegative = false;
            return (first_ * second_) / Gcd(first_,second_);
        }

        public static bool operator >(BigNumber first, BigNumber second)
        {
            return first.CompareTo(second) == 1;
        }
        public static bool operator <(BigNumber first, BigNumber second)
        {
            return first.CompareTo(second) == -1;
        }
        public static bool operator >=(BigNumber first, BigNumber second)
        {
            return first.CompareTo(second) >= 0 ;
        }
        public static bool operator <=(BigNumber first, BigNumber second)
        {
            return first.CompareTo(second) <= 0;
        }
        public static bool operator ==(BigNumber first, BigNumber second)
        {
            return first.CompareTo(second) == 0;
        }
        public static bool operator !=(BigNumber first, BigNumber second)
        {
            return !(first.CompareTo(second)==0);
        }
        #endregion
    }
}


