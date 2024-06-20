using UnityEditor;
using UnityEngine;

namespace KrillAudio.Krilloud.Editor.Tools
{
	public static class KrilloudTools
	{
		private const string PACKAGE_NAME = "krilloud-unity-plugin";
		private const string DLL_VERSION = "20190702";
		private const int MAIN_VERSION = 0;
		private const int MIN_VERSION = 1;
		private const int BUILD_NUMBER = 4;

		private const string STATE = "alpha";
		private const string RELEASE_PATH = "Assets\\krillaudio-unity-plugin\\KrillAudio\\Krilloud.Releases";

		private static readonly string[] INCLUDED_PACKAGE_PATH = new string[]
		{
			"Assets\\krillaudio-unity-plugin\\KrillAudio\\Krilloud",
			"Assets\\krillaudio-unity-plugin\\KrillAudio\\Krilloud.Examples"
		};

		[MenuItem("Krilloud/Development Tools/Publishing/Generate release package", priority = -100)]
		public static void GenerateRelease()
		{
			string fullPath = RELEASE_PATH + "\\" + PACKAGE_NAME + "-" +
				string.Format("{0}.{1:00}.{2:00}-{3}-{4}", MAIN_VERSION, MIN_VERSION, BUILD_NUMBER, DLL_VERSION, STATE) +
				".unitypackage";

			if (EditorUtility.DisplayDialog("Remember!", "You must upload this file to Google Drive!", "Upload now", "Not now"))
			{
				Application.OpenURL("https://drive.google.com");
			}

			AssetDatabase.ExportPackage(INCLUDED_PACKAGE_PATH, fullPath, ExportPackageOptions.Recurse | ExportPackageOptions.Interactive);
		}
	}
}