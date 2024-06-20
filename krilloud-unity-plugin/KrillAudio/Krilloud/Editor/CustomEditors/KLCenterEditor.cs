using UnityEditor;
using UnityEngine;

namespace KrillAudio.Krilloud.Editor
{
	using Editor = UnityEditor.Editor;

	[CanEditMultipleObjects]
	[CustomEditor(typeof(KLCenter))]
	public sealed class KLCenterEditor : Editor
	{
		private KLCenter m_target;

		private void OnEnable()
		{
			m_target = target as KLCenter;
		}

		public override void OnInspectorGUI()
		{
			KLEditorUtils.DrawKrillHeader();
			KLEditorUtils.DrawUILine();
			DrawDebugInfo();
		}

		private void DrawDebugInfo()
		{
			DrawDebugEngineInitialized();

			EditorGUILayout.Space();
			DrawListenerInfo();
			DrawEngineState();
		}

		private void DrawDebugEngineInitialized()
		{
			var startColor = GUI.backgroundColor;
			GUI.backgroundColor = m_target.Initialized ? Color.green : Color.red;
			EditorGUILayout.LabelField("Initialized: " + m_target.Initialized, EditorStyles.helpBox);
			GUI.backgroundColor = startColor;
		}

		private void DrawListenerInfo()
		{
			EditorGUILayout.LabelField("Listener: " + (KLListener.Current != null ? KLListener.Current.name : "none"), EditorStyles.helpBox);
		}

		private void DrawEngineState()
		{
			bool paused = KLCenter.IsKrilloudPaused();
			EditorGUILayout.LabelField("IsPaused: " + paused, EditorStyles.helpBox);
		}
	}
}