using UnityEditor;
using UnityEngine;

namespace KrillAudio.Krilloud.Editor.Windows
{
	public sealed class KLTagExplorerWindow : EditorWindow
	{
		private Vector2 m_tagScrollPosition;
		private Vector2 m_variableScrollPosition;

		[MenuItem("Krilloud/Window/Tag Explorer", priority = 20)]
		private static void ShowWindow()
		{
			var w = GetWindow<KLTagExplorerWindow>(false, "Tag Explorer", true);
			w.minSize = new Vector2(350, 350);
			w.maxSize = new Vector2(350, 500);
		}

		private void OnGUI()
		{
			KLEditorUtils.DrawKrillHeader();

			DrawTags();
			DrawVariables();
		}

		private void DrawTags()
		{
			EditorGUILayout.BeginVertical(EditorStyles.helpBox);

			EditorGUILayout.LabelField("Available tags", EditorStyles.boldLabel);
			KLEditorUtils.DrawUILine();

			m_tagScrollPosition = EditorGUILayout.BeginScrollView(m_tagScrollPosition);
			foreach (var tag in KLEditorCore.AvailableTags)
			{
				KLEditorUtils.DrawTag(tag);
			}

			EditorGUILayout.EndScrollView();

			EditorGUILayout.EndVertical();

			// KLEditorUtility.DrawWindowDebug(this);
		}

		private void DrawVariables()
		{
			EditorGUILayout.BeginVertical(EditorStyles.helpBox);

			EditorGUILayout.LabelField("Available variables", EditorStyles.boldLabel);
			KLEditorUtils.DrawUILine();

			m_variableScrollPosition = GUILayout.BeginScrollView(m_variableScrollPosition);
			foreach (var variable in KLEditorCore.AvailableVariables)
			{
				KLEditorUtils.DrawVariable(variable);
			}

			EditorGUILayout.EndScrollView();

			EditorGUILayout.EndVertical();
		}
	}
}