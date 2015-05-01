using System;
using System.Diagnostics;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

namespace cinqBench
{
	class MainClass
	{

		public static void Main (string[] args)
		{
			

			List<int> list1 = new List<int> ();

			for (int i = 0; i < 10000; i++)
				list1.Add (i);

			for(int i = 0;i<10000;i++)
				list1.Where ((int x) => x % 2 == 0).ToList ();

			var sw = System.Diagnostics.Stopwatch.StartNew();
			for(int i = 0;i<10000;i++)
				list1.Where ((int x) => x % 2 == 0).ToList ();
			sw.Stop ();

			Console.WriteLine (sw.ElapsedMilliseconds);

			List<int> list2 = new List<int>();






		}


		






	}
}
