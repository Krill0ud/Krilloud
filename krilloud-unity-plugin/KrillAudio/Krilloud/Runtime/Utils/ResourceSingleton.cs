using System.IO;
using UnityEngine;

#if UNITY_EDITOR

using System.Linq;
using UnityEditor;

#endif

namespace KrillAudio.Krilloud.Utils
{
	public abstract class ResourceSingleton<T> : ScriptableObject where T : ScriptableObject
	{
#if UNITY_EDITOR
		private static string AssetPath = Path.Combine("Assets", "Resources");
#endif

		private static T _instance;
		public static T Instance
		{
			get
			{
				if (_instance == null)
				{
					// Try load instance	
#if UNITY_EDITOR
					_instance = Resources.Load<T>(typeof(T).Name);
					if (_instance == null)
					{
						// TODO: Move asset to resources folder?
						_instance = AssetDatabase.FindAssets($"t:{typeof(T).Name}")
							.Select(AssetDatabase.GUIDToAssetPath)
							.Select(AssetDatabase.LoadAssetAtPath<T>)
							.FirstOrDefault();
					}
#else
					_instance = Resources.Load<T>(typeof(T).Name);
#endif
					
#if UNITY_EDITOR
					// Instance not found, create one...
					if (!Application.isPlaying && _instance == null && FileNotFound())
					{
						CreateAsset();
					}
#endif
					
					var inst = _instance as ResourceSingleton<T>;
					if (inst != null)
					{
						inst.OnInstanceLoaded();
					}
				}
				
				return _instance;
			}
		}

#if UNITY_EDITOR

		private static bool FileNotFound()
		{
			string resourceFolderPath = Path.Combine(Application.dataPath, "Resources");
			string assetPath = Path.Combine(AssetPath, typeof(T).Name + ".asset");

			return !File.Exists(resourceFolderPath) || !File.Exists(assetPath);
		}
		
		private static void CreateAsset()
		{
			// Create directory
			var resourceFolderPath = Path.Combine(Application.dataPath, "Resources");
			if (!File.Exists(resourceFolderPath))
			{
				AssetDatabase.CreateFolder("Assets", "Resources");
				AssetDatabase.Refresh();
			}
			
			// Create singleton
			_instance = ScriptableObject.CreateInstance<T>();
			
			string assetPath = Path.Combine(AssetPath, typeof(T).Name + ".asset");
			assetPath = AssetDatabase.GenerateUniqueAssetPath(assetPath);
			
			AssetDatabase.CreateAsset(_instance, assetPath);
			AssetDatabase.Refresh();
			AssetDatabase.SaveAssets();
		}

#endif

		protected virtual void OnInstanceLoaded()
		{
		}
	}
}