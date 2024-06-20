using System.IO;
using KrillAudio.Krilloud.Definitions;
using KrillAudio.Krilloud.Utils;
using UnityEditor;
using UnityEngine;

namespace KrillAudio.Krilloud.Editor
{
	public static class KLEditorUtils
	{
		public static string KRILLOUD_CONTRACT_PATH
		{
			get { return Path.Combine(KLUtils.KRILLOUD_PROJECT_PATH, "Contract.json"); }
		}

		public static Color KRILLOUD_COLOR { get { return new Color(123f / 255f, 195f / 255f, 0f); } }

		public static void DrawKrillHeader()
		{
			GUILayout.Space(10);
			DrawCenteredImage(KLEditorStyles.KRILLAUDIO_LOGO);
		}

		private static void DrawCenteredImage(Texture2D texture)
		{
			float inspectorWidth = EditorGUIUtility.currentViewWidth;
			float targetHeight = inspectorWidth / texture.width * texture.height;
			float targetWidth = texture.width / texture.height * targetHeight;

			GUILayout.BeginHorizontal(EditorStyles.helpBox);
			GUILayout.FlexibleSpace();

			var bc = GUI.backgroundColor;
			GUI.backgroundColor = Color.clear;
			GUILayout.Box(texture, GUILayout.Height(targetHeight), GUILayout.Width(targetWidth));
			GUI.backgroundColor = bc;

			GUILayout.FlexibleSpace();
			GUILayout.EndHorizontal();
		}

		public static void DrawUILine()
		{
			// TODO: Get light color for this line

			if (EditorGUIUtility.isProSkin) DrawUILine(KLEditorStyles.SEPARATOR_LINE_COLOR_DARK, 1, 1);
			else DrawUILine(KLEditorStyles.SEPARATOR_LINE_COLOR_LIGHT, 1, 1);
		}

		public static void DrawUILine(Color color, int thickness = 2, int padding = 10)
		{
			Rect r = EditorGUILayout.GetControlRect(GUILayout.Height(padding + thickness));
			r.height = thickness;
			r.y += padding / 2;
			r.x -= 3;
			r.width += 6;
			EditorGUI.DrawRect(r, color);
		}

		public static void DrawWindowDebug(EditorWindow window)
		{
			EditorGUILayout.Space();

			DrawUILine();

			EditorGUILayout.Vector2Field("Window size", window.position.size);
		}

		#region Drawers

		public static void DrawTag(KLTagDefinition tag)
		{
			string text = string.Format("[{0}] {1}", tag.is3D ? "3D" : "2D", tag.name);
			string description = tag.description;

			var content = new GUIContent(text, description);

			var style = new GUIStyle(EditorStyles.miniLabel);
			style.normal.textColor = GetMemberColor(tag, style.normal.textColor);

			EditorGUILayout.LabelField(content, style);
		}

		public static void DrawVariable(KLVariableDefinition variable)
		{
			string text = string.Format("[{0}] {1}", variable.isGlobal ? "G" : "L", variable.name);

			var content = new GUIContent(text);

			var style = new GUIStyle(EditorStyles.miniLabel);
			style.normal.textColor = GetMemberColor(variable, style.normal.textColor);

			EditorGUILayout.LabelField(content, style);
		}

		public static Color GetMemberColor(IKLMemberDefinition member, Color defaultColor)
		{
			switch (member.SourceContract)
			{
				case SourceContract.Krilloud:
					return defaultColor;

				case SourceContract.Placeholder:
					return Color.yellow;

				case SourceContract.Cache:
					return Color.red;
			}

			return defaultColor;
		}

		public static void DrawSoundbank(KLSoundbankFileDefinition soundbank)
		{
			EditorGUILayout.LabelField(soundbank.id.ToString(), EditorStyles.miniLabel);
		}

		public static void DrawChannel(KLChannelDefinition channel)
		{
			EditorGUILayout.LabelField(channel.name, EditorStyles.miniLabel);
		}

		#endregion Drawers
	}
}