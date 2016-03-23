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
            Circle circle = new Circle(7);
            circle.PrintDetails();
            Console.WriteLine("circle's area =" + circle.Area());
            Console.WriteLine("circle's perimeter =" + circle.Perimeter());


            /* int sum = 1, n = 7;
             for(int i = 1; i <= n; i++ )
             {
                 sum *= i;
             }
             Console.WriteLine("the factorial of " + n + " is " + sum);      
             */
            /*int num = 4;
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
            */
        }
    }

    class Circle
    {
        private double radius;

        public Circle()
        {
            Radius = 10;
        }

        public Circle(double r)
        {
            Radius = r;
        }

        public double Perimeter()
        {
            return 2 * Math.PI * Radius;
        }

        public double Area()
        {
            return Math.PI * Radius * Radius;
        }

        public void PrintDetails()
        {
            Console.WriteLine("Radius = " + Radius);
        }

        public double Radius
        {
            get
            {
                return radius;
            }
            set
            {
                if (value > 0) { radius = value; }
                else { Console.WriteLine("width value was not set..."); }
            }
        }
    }
 }

