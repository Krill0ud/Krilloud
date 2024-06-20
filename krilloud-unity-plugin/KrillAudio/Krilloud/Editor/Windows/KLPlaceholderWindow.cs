using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

namespace KrillAudio.Krilloud.Editor.Windows
{
	public class KLPlaceholderWindow : EditorWindow
	{
		public const float WIDTH = 350;

		private string m_tagName;
		private Vector2 m_tagScroll;

		private string m_variableName;
		private Vector2 m_variableScroll;

		private string m_channelName;
		private Vector2 m_channelScroll;

		private bool m_isDirty;

		[MenuItem("Krilloud/Window/Placeholder contract", priority = 20)]
		private static void ShowWindow()
		{
			var w = GetWindow<KLPlaceholderWindow>(false, "Placeholder contract", true);
			w.minSize = new Vector2(WIDTH, 500);
			w.maxSize = new Vector2(WIDTH, 1000);
		}

		private void OnEnable()
		{
			m_tagName = "";
			m_variableName = "";
			m_channelName = "";
			
			m_isDirty = false;
		}

		private void OnDisable()
		{
			RefreshCore();
		}

		private void OnLostFocus()
		{
			RefreshCore();
		}

		private void OnGUI()
		{
			KLEditorUtils.DrawKrillHeader();

			GUI.enabled = m_isDirty;
			if (GUILayout.Button("Refresh"))
			{
				RefreshCore();
			}
			GUI.enabled = true;

			DrawEditableList(KLSettings.Instance.placeholderContract.tags, ref m_tagScroll,
				ref m_tagName, string.Format("Tags [{0}]", KLSettings.Instance.placeholderContract.tags.Count));

			DrawEditableList(KLSettings.Instance.placeholderContract.variables, ref m_variableScroll,
				ref m_variableName, string.Format("Variables [{0}]", KLSettings.Instance.placeholderContract.variables.Count));
			
			DrawEditableList(KLSettings.Instance.placeholderContract.channels, ref m_channelScroll,
				ref m_channelName, string.Format("Channels [{0}]", KLSettings.Instance.placeholderContract.channels.Count));
		}

		private void DrawEditableList(List<string> list, ref Vector2 scroll, ref string newValue, string title)
		{
			EditorGUILayout.BeginVertical(EditorStyles.helpBox);
			EditorGUILayout.LabelField(title, EditorStyles.boldLabel);
			KLEditorUtils.DrawUILine();

			// == BEGIN SCROLL ==
			scroll = EditorGUILayout.BeginScrollView(scroll);
			for (var i = 0; i < list.Count; i++)
			{
				DrawEditableListValue(list, i);
			}
			EditorGUILayout.EndScrollView();
			// == END SCROLL ==

			KLEditorUtils.DrawUILine();
			if (DrawInputFieldWithButton(ref newValue, "Add"))
			{
				m_isDirty = true;

				if (!string.IsNullOrEmpty(newValue))
				{
					list.Add(newValue);
					newValue = "";
				}
			}
			EditorGUILayout.EndVertical();
		}

		private void DrawEditableListValue(List<string> list, int index)
		{
			float buttonWidth = 20f;

			GUILayout.BeginHorizontal();

			GUI.backgroundColor = Color.red;
			GUILayout.Label(list[index]);
			if (GUILayout.Button("X", GUILayout.MinWidth(buttonWidth), GUILayout.MaxWidth(buttonWidth)))
			{
				m_isDirty = true;
				list.RemoveAt(index);
			}
			GUI.backgroundColor = Color.white;

			GUILayout.EndHorizontal();
		}

		private bool DrawInputFieldWithButton(ref string text, string button)
		{
			float buttonWidth = 75f;
			float textWidth = this.position.size.x - buttonWidth;

			GUILayout.BeginHorizontal();

			text = GUILayout.TextField(text, GUILayout.MaxWidth(textWidth));
			text = CheckInput(text);

			bool r = GUILayout.Button(button, GUILayout.MaxWidth(buttonWidth));

			GUILayout.EndHorizontal();

			return r;
		}

		private string CheckInput(string input)
		{
			input = input.Replace(" ", "_");
			return input;
		}

		private void RefreshCore()
		{
			KLEditorCore.RefreshCore();
			m_isDirty = false;
		}
	}
}