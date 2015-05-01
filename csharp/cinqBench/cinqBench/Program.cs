using System;
using System.Diagnostics;
using System.Collections;
using System.Collections.Generic;

using System.Linq;
using System.IO;


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


		public static List<WeatherPoint> loadWeather(string path){


			string[] lines = System.IO.File.ReadAllLines(Path.Combine ("..","..","..","data","weather_kjfk_1948-2014.csv"));
			string[] headers=lines[0].Split (',');
			List<Dictionary<string,string>> dictList = new List<Dictionary<string,string>> ();
			for (int i = 1; i < lines.Length; i++) {
				string[] tok=lines [i].Split (',');
				Dictionary<string,string> dict = new Dictionary<string,string> (); 
				int count = 0;
				foreach (string header in headers) {
					
					dict [header] = tok [count];
					count++;
				}
				dictList.Add (dict);
			}





						






		}






	}
}
