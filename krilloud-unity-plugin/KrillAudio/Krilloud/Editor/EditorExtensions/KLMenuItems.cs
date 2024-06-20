using UnityEditor;
using UnityEngine;

namespace KrillAudio.Krilloud.Editor
{
	public static class KLMenuItems
	{
		[MenuItem("GameObject/Krilloud/KLAudioSource", priority = 12)]
		public static void AddKLAudioSource()
		{
			GameObject newObject = new GameObject();
			newObject.name = "KLAudioSource";
			newObject.AddComponent<KLAudioSource>();

			Selection.activeGameObject = newObject;
		}

		[MenuItem("Krilloud/Highlight settings")]
		public static void PingSettings()
		{
			EditorUtility.FocusProjectWindow();
			// EditorGUIUtility.PingObject(KLSettings.Instance);
			Selection.activeObject = KLSettings.Instance;
		}

        [MenuItem("Krilloud/Server")]
        public static void ServerSettings()
        {
            EditorUtility.FocusProjectWindow();
            Selection.activeObject = KLServer.Instance;
        }
    }
}