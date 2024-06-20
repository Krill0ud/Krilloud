using UnityEditor;
using UnityEngine;

namespace KrillAudio.Krilloud.Editor
{
	using Editor = UnityEditor.Editor;

	[CustomEditor(typeof(KLListener))]
	public class KLListenerEditor : Editor
	{
		private KLListener m_target;

		private void OnEnable()
		{
			m_target = target as KLListener;
		}

		public override void OnInspectorGUI()
		{
			KLEditorUtils.DrawKrillHeader();

			DrawDebugInfo();
		}

		private void DrawDebugInfo()
		{
			// EditorGUILayout.Space();
			KLEditorUtils.DrawUILine();

			EditorGUILayout.BeginVertical(EditorStyles.helpBox);
			EditorGUILayout.LabelField("Velocity: " + m_target.GetPrivateFieldValue<Vector3>("m_velocity"));
			EditorGUILayout.EndVertical();
		}
	}
}