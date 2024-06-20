using System.IO;
using UnityEngine;

namespace KrillAudio.Krilloud.Utils
{
	public static class KLUtils
	{
		public static string KRILLOUD_PROJECT_PATH
		{
			get { return Path.Combine(Application.streamingAssetsPath, "KrilloudData"); }
		}
	}
}