using System;
using System.Diagnostics;

namespace cinqBench
{
	public class testPerf
	{
		Func<int> funct;
		int run_count;
		string name;
		public testPerf (string NAME,Func<int> fun,int rc=100)
		{
			this.funct = fun;
			this.run_count = rc;
			this.name = NAME;
			
		}

		public void run (){
			var sw=System.Diagnostics.Stopwatch.StartNew ();
			for (int i = 0; i < run_count; i++) funct.DynamicInvoke ();
			sw.Stop ();
			Console.WriteLine ("[{0}] {1}\n",sw.ElapsedMilliseconds,name);

		}


	}
}

