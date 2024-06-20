using System.Linq;
using UnityEditor;

namespace KrillAudio.Krilloud.Editor
{
	/// <summary>
	/// Class to detect changes in project files
	/// </summary>
	public class KLPostProcessor : AssetPostprocessor
	{
		public static string FILTER = "Assets/StreamingAssets/KrilloudData";

		private static void OnPostprocessAllAssets(string[] importedAssets, string[] deletedAssets, string[] movedAssets, string[] movedFromAssetPaths)
		{
			if (importedAssets.FirstOrDefault(x => x.StartsWith(FILTER)) != null)
			{
				ForceInitialization();
				return;
			}

			if (deletedAssets.FirstOrDefault(x => x.StartsWith(FILTER)) != null)
			{
				ForceInitialization();
				return;
			}

			if (movedAssets.FirstOrDefault(x => x.StartsWith(FILTER)) != null)
			{
				ForceInitialization();
				return;
			}

			if (movedFromAssetPaths.FirstOrDefault(x => x.StartsWith(FILTER)) != null)
			{
				ForceInitialization();
				return;
			}
		}

		private static void ForceInitialization()
		{
			// Debug.Log($"<b>[KLPostProcessor]</b> Changes detected, force initialization!");
			KLEditorCore.Initialize();
		}
	}
}