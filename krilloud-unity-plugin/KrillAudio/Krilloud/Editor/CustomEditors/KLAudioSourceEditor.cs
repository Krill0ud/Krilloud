using System.Collections.Generic;
using System.Linq;
using KrillAudio.Krilloud.Definitions;
using KrillAudio.Krilloud.Services;
using UnityEditor;
using UnityEditorInternal;
using UnityEngine;

namespace KrillAudio.Krilloud.Editor
{
	using Editor = UnityEditor.Editor;

	[CustomEditor(typeof(KLAudioSource))]
	public sealed class KLAudioSourceEditor : Editor
	{
		public const float HANDLE_SIZE = .2f;

		private KLAudioSource m_target;
		private ReorderableList rList;

		private void OnEnable()
		{
			m_target = target as KLAudioSource;

			rList = new ReorderableList(serializedObject, serializedObject.FindProperty("m_tags"), true, true, true, true);
			rList.drawHeaderCallback = DrawListHeader;
			rList.drawElementCallback = DrawListElement;
			rList.onAddDropdownCallback = OnAddDropdownCallback;
			rList.elementHeightCallback = ElementHeightCallback;
		}

		#region Reorderable list callbacks

		private void DrawListHeader(Rect rect)
		{
			EditorGUI.LabelField(rect, "Available tags", EditorStyles.boldLabel);
		}

		private void DrawListElement(Rect rect, int index, bool isActive, bool isFocused)
		{
			rect.y += 2;

			var element = rList.serializedProperty.GetArrayElementAtIndex(index);

			float tw = rect.width;
			float buttonWidth = 45;

			GUI.enabled = !Application.isPlaying;
			EditorGUI.PropertyField(new Rect(rect.x, rect.y, tw - buttonWidth - 5, EditorGUIUtility.singleLineHeight), element, GUIContent.none);
			GUI.enabled = true;

			// Draw button
			if (Application.isPlaying)
			{
				GUI.backgroundColor = m_target.IsPlaying(index) ? Color.green : Color.white;
				if (GUI.Button(new Rect(rect.x + tw - buttonWidth, rect.y, buttonWidth, EditorGUIUtility.singleLineHeight), "Play"))
				{
					m_target.Play(index);
				}
				GUI.backgroundColor = Color.white;
			}
			else
			{
				if (GUI.Button(new Rect(rect.x + tw - buttonWidth, rect.y, buttonWidth, EditorGUIUtility.singleLineHeight), "Test"))
				{
					KLStartup.Logger.LogWarning(string.Format("<b>[KLAudioSourceEditor]</b> Test {0} tag is not implemented!", element.stringValue));
				}
			}
		}

		private void OnAddDropdownCallback(Rect buttonRect, ReorderableList l)
		{
			var available = KLEditorCore.AvailableTags.ToList();
			var main = available.Where(x => x.sourceContract == SourceContract.Krilloud).ToList();
			var placeholder = available.Where(x => x.sourceContract == SourceContract.Placeholder).ToList();
			var cache = available.Where(x => x.sourceContract == SourceContract.Cache).ToList();

			var menu = new GenericMenu();
			AddTagsToMenu(menu, main, "Krilloud");
			AddTagsToMenu(menu, placeholder, "Placeholder");
			AddTagsToMenu(menu, cache, "Cache");
			menu.ShowAsContext();
		}

		private void AddTagsToMenu(GenericMenu menu, List<KLTagDefinition> tagList, string separatorName)
		{
			if (tagList.Count > 0)
			{
				menu.AddDisabledItem(new GUIContent($"== {separatorName} =="));
				tagList.ForEach(tag => AddTagToMenu(menu, tag));
			}
		}

		private void AddTagToMenu(GenericMenu menu, KLTagDefinition tag)
		{
			bool isAdded = m_target.Tags.Contains(tag.name);

			if (isAdded)
			{
				menu.AddDisabledItem(GenerateDropdownContentForTag(tag));
			}
			else
			{
				menu.AddItem(GenerateDropdownContentForTag(tag), false, OnAddTag, tag.name);
			}
		}

		private GUIContent GenerateDropdownContentForTag(KLTagDefinition tag)
		{
			return new GUIContent(tag.name);
		}

		private void OnAddTag(object target)
		{
			var tag = (string)target;
			var index = rList.serializedProperty.arraySize;
			rList.serializedProperty.arraySize++;
			rList.index = index;

			var element = rList.serializedProperty.GetArrayElementAtIndex(index);
			element.stringValue = tag;

			element.serializedObject.ApplyModifiedProperties();
		}

		private float ElementHeightCallback(int index)
		{
			return rList.elementHeight;
		}

		#endregion Reorderable list callbacks

		#region InspectorGUI

		public override void OnInspectorGUI()
		{
			KLEditorUtils.DrawKrillHeader();
			DrawInspectorGUI();

			EditorGUILayout.Space();
			DrawDebugInfo();
		}

		private void DrawInspectorGUI()
		{
			serializedObject.Update();

			DrawSourceTypeGUI();

			EditorGUILayout.Space();
			rList.DoLayoutList();

			EditorGUILayout.Space();
			EditorGUILayout.PropertyField(serializedObject.FindProperty(nameof(KLAudioSource.playOnAwake)));

			serializedObject.ApplyModifiedProperties();
		}

		private void DrawSourceTypeGUI()
		{
			EditorGUILayout.BeginVertical(EditorStyles.helpBox);

			EditorGUILayout.PropertyField(serializedObject.FindProperty(nameof(KLAudioSource.sourceShape)));
			DrawSourceShapeGUI();

			EditorGUILayout.EndVertical();
		}

		private void DrawSourceShapeGUI()
		{
			switch (m_target.sourceShape)
			{
				case KLAudioSource.SourceType.Point:
					DrawGUIPointShape();
					break;

				case KLAudioSource.SourceType.Sphere:
					DrawGUISphereShape();
					break;

				case KLAudioSource.SourceType.Line:
					DrawGUILineShape();
					break;

				case KLAudioSource.SourceType.Area:
					DrawGUIAreaShape();
					break;
			}
		}

		private void DrawGUIPointShape()
		{
		}

		private void DrawGUISphereShape()
		{
			EditorGUILayout.PropertyField(serializedObject.FindProperty(nameof(KLAudioSource.sphereShapeRadius)));
			m_target.sphereShapeRadius = Mathf.Max(m_target.sphereShapeRadius, 0);
		}

		private void DrawGUILineShape()
		{
			EditorGUILayout.PropertyField(serializedObject.FindProperty(nameof(KLAudioSource.lineShapeStart)));
			EditorGUILayout.PropertyField(serializedObject.FindProperty(nameof(KLAudioSource.lineShapeEnd)));
		}

		private void DrawGUIAreaShape()
		{
			EditorGUILayout.PropertyField(serializedObject.FindProperty(nameof(KLAudioSource.areaShapeSize)));
		}

		private void DrawDebugInfo()
		{
			KLEditorUtils.DrawUILine();

			EditorGUILayout.BeginVertical(EditorStyles.helpBox);
			DrawDebugInfoRow1();
			DrawDebugInfoRow2();
			EditorGUILayout.EndVertical();
		}

		private void DrawDebugInfoRow1()
		{
			EditorGUILayout.BeginHorizontal();
			EditorGUILayout.LabelField("ID: " + m_target.InstanceID);
			EditorGUILayout.LabelField("IsPlaying: " + m_target.IsPlaying());
			EditorGUILayout.EndHorizontal();
		}

		private void DrawDebugInfoRow2()
		{
			EditorGUILayout.BeginHorizontal();

			if (Application.isPlaying)
			{
				EditorGUILayout.LabelField("Position: " + m_target.GetPrivateFieldValue<Vector3>("m_lastUpdatedPos"));
			}
			else
			{
				EditorGUILayout.LabelField("Position: " + m_target.CachedTransform.position);
			}

			EditorGUILayout.LabelField("Velocity: " + m_target.GetPrivateFieldValue<Vector3>("m_lastUpdatedVel"));
			EditorGUILayout.EndHorizontal();
		}

		#endregion InspectorGUI

		#region SceneGUI

		private void OnSceneGUI()
		{
			DrawSourceRealPosition();
			DrawDottedLineToListener();
			DrawSourceShapeHandles();
		}

		private void DrawSourceRealPosition()
		{
			var color = Handles.color;
			Handles.color = m_target.IsPlaying() ? (Mathf.Sin(2 * Mathf.PI * Time.time) > 0 ? Color.red : color) : color;
			Handles.SphereHandleCap(0, m_target.GetSourcePosition(), Quaternion.identity, .25f, EventType.Repaint);
			Handles.color = color;
		}

		private void DrawDottedLineToListener()
		{
			if (KLListener.Current != null)
			{
				var color = Handles.color;

				Handles.color = m_target.IsPlaying() ? (Mathf.Sin(2 * Mathf.PI * Time.time) > 0 ? Color.red : Color.grey) : Color.grey;
				Handles.DrawDottedLine(m_target.GetSourcePosition(), KLListener.Current.CachedTransform.position, 1);
				Handles.color = color;
			}
		}

		private void DrawSourceShapeHandles()
		{
			switch (m_target.sourceShape)
			{
				case KLAudioSource.SourceType.Point:
					break;

				case KLAudioSource.SourceType.Sphere:
					DrawHandlesSphereType();
					break;

				case KLAudioSource.SourceType.Line:
					DrawHandlesLineType();
					break;

				case KLAudioSource.SourceType.Area:
					DrawHandlesAreaType();
					break;
			}
		}

		private void DrawHandlesSphereType()
		{
			EditorGUI.BeginChangeCheck();

			float targetRadius = Handles.RadiusHandle(Quaternion.identity, m_target.CachedTransform.position, m_target.sphereShapeRadius);

			if (EditorGUI.EndChangeCheck())
			{
				Undo.RecordObject(m_target, "Radius change");

				m_target.sphereShapeRadius = Mathf.Max(targetRadius, 0);
			}
		}

		private void DrawHandlesLineType()
		{
			Vector3 startPos = m_target.CachedTransform.TransformPoint(m_target.lineShapeStart);
			Vector3 endPos = m_target.CachedTransform.TransformPoint(m_target.lineShapeEnd);

			float sizeStart = HandleUtility.GetHandleSize(startPos) * HANDLE_SIZE / 4f;
			float sizeEnd = HandleUtility.GetHandleSize(endPos) * HANDLE_SIZE / 4f;

			// Draw handles
			EditorGUI.BeginChangeCheck();
			Vector3 targetStart = Handles.FreeMoveHandle(startPos, Quaternion.identity, sizeStart, Vector3.zero, Handles.DotHandleCap);
			Vector3 targetEnd = Handles.FreeMoveHandle(endPos, Quaternion.identity, sizeEnd, Vector3.zero, Handles.DotHandleCap);
			if (EditorGUI.EndChangeCheck())
			{
				Undo.RecordObject(m_target, "Line points change");

				m_target.lineShapeStart = m_target.CachedTransform.InverseTransformPoint(targetStart);
				m_target.lineShapeEnd = m_target.CachedTransform.InverseTransformPoint(targetEnd);
			}

			// Draw line
			Handles.DrawLine(targetStart, targetEnd);
		}

		private void DrawHandlesAreaType()
		{
			// Draw box wire
			var m = Matrix4x4.TRS(m_target.CachedTransform.position, m_target.CachedTransform.rotation, Vector3.one);
			using (new Handles.DrawingScope(m))
			{
				Handles.DrawWireCube(Vector3.zero, m_target.areaShapeSize);
			}

			Vector3 center = m_target.CachedTransform.position;

			Vector3 px = center + m_target.CachedTransform.right * m_target.areaShapeSize.x / 2f;
			Vector3 py = center + m_target.CachedTransform.up * m_target.areaShapeSize.y / 2f;
			Vector3 pz = center + m_target.CachedTransform.forward * m_target.areaShapeSize.z / 2f;

			float sx = HandleUtility.GetHandleSize(px) * HANDLE_SIZE;
			float sy = HandleUtility.GetHandleSize(py) * HANDLE_SIZE;
			float sz = HandleUtility.GetHandleSize(pz) * HANDLE_SIZE;

			// Draw change size handles
			EditorGUI.BeginChangeCheck();

			float tx = Handles.ScaleValueHandle(m_target.areaShapeSize.x, px, Quaternion.identity, sx, Handles.DotHandleCap, 0);
			float ty = Handles.ScaleValueHandle(m_target.areaShapeSize.y, py, Quaternion.identity, sy, Handles.DotHandleCap, 0);
			float tz = Handles.ScaleValueHandle(m_target.areaShapeSize.z, pz, Quaternion.identity, sz, Handles.DotHandleCap, 0);

			if (EditorGUI.EndChangeCheck())
			{
				Undo.RecordObject(m_target, "Change area size");

				m_target.areaShapeSize = new Vector3(Mathf.Abs(tx), Mathf.Abs(ty), Mathf.Abs(tz));
			}
		}

		#endregion SceneGUI

	}
}