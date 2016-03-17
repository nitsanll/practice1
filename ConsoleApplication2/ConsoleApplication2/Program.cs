using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace ConsoleApplication2
{
    class Program
    {
        static void Main(string[] args)
        {
            /* int sum = 1, n = 7;
             for(int i = 1; i <= n; i++ )
             {
                 sum *= i;
             }
             Console.WriteLine("the factorial of " + n + " is " + sum);      
             */
            int num = 4;
            Console.WriteLine(factorial(num));
        }

        public static int factorial(int num)
        {
            if (num == 0 || num == 1)
            {
                return 1;
            }
            else
            {
                return num * factorial(num - 1);
            }
        }
    }
}
