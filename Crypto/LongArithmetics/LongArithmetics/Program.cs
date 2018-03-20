using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LongArithmetics
{
    class Program
    {

        static void Main(string[] args)
        {
            BigNumber a = new BigNumber("12345678910111213141516171819");
            BigNumber b = new BigNumber("987654321");

            Console.WriteLine((a - b).Value);
            Console.WriteLine((a + b).Value);
            Console.WriteLine((a * b).Value);
            Console.WriteLine((a / b).Value);

            Console.WriteLine((a % b).Value);
            Console.WriteLine(BigNumber.Gcd(a, b).Value);
            Console.WriteLine(BigNumber.Lcm(a, b).Value);
            Console.ReadKey();
        }
    }
  
}
