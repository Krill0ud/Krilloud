using UnityEditor;
using UnityEngine;

namespace KrillAudio.Krilloud.Editor
{
	[InitializeOnLoad]
	public class KLHierarchyIcons
	{
		static KLHierarchyIcons()
		{
			EditorApplication.hierarchyWindowItemOnGUI += HighlightItems;
		}

		private static void HighlightItems(int instanceID, Rect selectionRect)
		{
			var target = EditorUtility.InstanceIDToObject(instanceID) as GameObject;

			if (target == null) return;

			if (target.GetComponent<KLCenter>() || target.GetComponent<KLAudioSource>() || target.GetComponent<KLListener>())
			{
				GUI.Label(selectionRect, KLEditorStyles.HIERARCHY_ICON_CONTENT, KLEditorStyles.HIERARCHY_ICON_STYLE);
				return;
			}
			else
			{
				GUI.Label(selectionRect, GUIContent.none);
				return;
			}
		}
	}
}