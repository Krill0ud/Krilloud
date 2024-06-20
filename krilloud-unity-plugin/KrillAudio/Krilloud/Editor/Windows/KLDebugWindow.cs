using KrillAudio.Krilloud.Definitions;
using UnityEditor;
using UnityEngine;

namespace KrillAudio.Krilloud.Editor.Tools
{
	public sealed class KLDebugWindow : EditorWindow
	{
		private KLContractDefinition m_contractCache;
		private Vector2 m_scroll;

		[MenuItem("Krilloud/Window/Contract Info", priority = -100)]
		private static void ShowWindow()
		{
			var w = GetWindow<KLDebugWindow>(false, "Krilloud - Contract Info", true);
			w.minSize = new Vector2(350, 350);
			w.maxSize = new Vector2(350, 500);

			w.m_contractCache = KLEditorCore.GetContractCopy();
		}

		private void OnGUI()
		{
			m_scroll = EditorGUILayout.BeginScrollView(m_scroll);
			KLEditorUtils.DrawKrillHeader();

			Draw(m_contractCache);

			EditorGUILayout.EndScrollView();
		}

		private void Draw(KLContractDefinition contract)
		{
			// ===== BEGIN TAGS =====
			EditorGUILayout.BeginVertical(EditorStyles.helpBox);
			EditorGUILayout.LabelField("Tags", EditorStyles.boldLabel);
			KLEditorUtils.DrawUILine();

			contract.tags.ForEach(x => Draw(x));

			EditorGUILayout.EndVertical();
			// ===== END TAGS =====

			// ===== BEGIN VARIABLES =====
			EditorGUILayout.BeginVertical(EditorStyles.helpBox);
			EditorGUILayout.LabelField("Variables", EditorStyles.boldLabel);
			KLEditorUtils.DrawUILine();

			contract.variables.ForEach(x => Draw(x));

			EditorGUILayout.EndVertical();
			// ===== END VARIABLES =====

			// ===== BEGIN SOUNDBANKS =====
			EditorGUILayout.BeginVertical(EditorStyles.helpBox);
			EditorGUILayout.LabelField("Soundbank files", EditorStyles.boldLabel);
			KLEditorUtils.DrawUILine();

			contract.soundbank_files.ForEach(x => Draw(x));
			EditorGUILayout.EndVertical();
			// ===== END SOUNDBANKS =====

			// ===== BEGIN CHANNELS =====
			EditorGUILayout.BeginVertical(EditorStyles.helpBox);
			EditorGUILayout.LabelField("Channels", EditorStyles.boldLabel);
			KLEditorUtils.DrawUILine();

			contract.channels.ForEach(x => Draw(x));

			EditorGUILayout.EndVertical();
			// ===== END CHANNELS =====
		}

		private void Draw(KLTagDefinition tag)
		{
			KLEditorUtils.DrawTag(tag);
		}

		private void Draw(KLVariableDefinition variable)
		{
			KLEditorUtils.DrawVariable(variable);
		}

		private void Draw(KLSoundbankFileDefinition soundbank)
		{
			KLEditorUtils.DrawSoundbank(soundbank);
		}

		private void Draw(KLChannelDefinition channel)
		{
			KLEditorUtils.DrawChannel(channel);
		}
	}
}