using KrillAudio.Krilloud.Definitions;
using UnityEditor;
using UnityEngine;
using System.Collections.Generic;
using System;

namespace KrillAudio.Krilloud.Editor
{
	using Editor = UnityEditor.Editor;

	[CustomEditor(typeof(KLSettings))]
	public sealed class KLSettingsEditor : Editor
	{
		private KLSettings m_target;
		private bool m_lastAutogencode;

		private void OnEnable()
		{
			m_target = target as KLSettings;

		}

		public override void OnInspectorGUI()
		{
			KLEditorUtils.DrawKrillHeader();

			m_lastAutogencode = m_target.autogenerateCode;

			EditorGUI.BeginChangeCheck();

			base.OnInspectorGUI();

			if (EditorGUI.EndChangeCheck())
			{
				// Detech changes to autogenerate code!
				if (m_lastAutogencode != m_target.autogenerateCode)
				{
					RefreshCore();
				}

				// m_isDirty = m_isDirty || true;
			}

			EditorGUILayout.Space();

			DrawButtons();

			EditorGUILayout.Space();

		}

		private void DrawButtons()
		{
			EditorGUILayout.BeginHorizontal();

			GUI.enabled = CacheHasSomething();
			if (GUILayout.Button("Clear cache"))
			{
				ClearCache();
			}
			GUI.enabled = true;

			EditorGUILayout.EndHorizontal();
		}

		#region Helpers

		private void RefreshCore()
		{
			KLEditorCore.RefreshCore();
			m_target.RemoveDuplicates();
		}

		#endregion Helpers

		#region Public Static API

		public static bool CacheHasSomething()
		{
			return
				KLSettings.Instance.cacheContract.tags.Count != KLEditorCore.MainContract.tags.Count ||
				KLSettings.Instance.cacheContract.variables.Count != KLEditorCore.MainContract.variables.Count;
		}

		public static void ClearCache()
		{
			KLSettings.Instance.cacheContract = new KLContractDefinition();
			KLEditorCore.RefreshCore();
		}

		#endregion Public Static API
	}
}