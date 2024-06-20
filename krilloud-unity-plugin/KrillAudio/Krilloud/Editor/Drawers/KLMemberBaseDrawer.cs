using System;
using KrillAudio.Krilloud.Definitions;
using UnityEditor;
using UnityEngine;

namespace KrillAudio.Krilloud.Editor.Drawers
{
	public abstract class KLMemberBaseDrawer : PropertyDrawer
	{
		protected abstract string MemberName { get; }
		
		protected abstract string[] MemberStringArray { get; }
		
		protected abstract IKLMemberDefinition GetMemberByIndex(int index);

		public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
		{
			if (property.propertyType == SerializedPropertyType.String)
			{
				EditorGUI.BeginProperty(position, label, property);

				int currentIndex = Array.IndexOf(MemberStringArray, property.stringValue);

				// The current member is unavailable, show a custom enum with the current value
				if (currentIndex < 0)
				{
					GUI.backgroundColor = Color.red;
					GUIContent c = new GUIContent();
					c.text = property.stringValue;

					EditorGUI.BeginChangeCheck();
					currentIndex = EditorGUI.Popup(position, label.text, 0, GetMembersAddingCustom(property.stringValue));
					if (EditorGUI.EndChangeCheck() && currentIndex > 0)
					{
						property.stringValue = MemberStringArray[currentIndex - 1];
					}
				}
				// The current member is correct, show the members loaded from JSON
				else
				{
					var lastColor = GUI.backgroundColor;
					GUI.backgroundColor = KLEditorUtils.GetMemberColor(GetMemberByIndex(currentIndex), lastColor);

					EditorGUI.BeginChangeCheck();
					currentIndex = EditorGUI.Popup(position, label.text, currentIndex, MemberStringArray);
					if (EditorGUI.EndChangeCheck())
					{
						property.stringValue = MemberStringArray[currentIndex];
					}

					GUI.backgroundColor = lastColor;
				}

				EditorGUI.EndProperty();
			}
			else
			{
				EditorGUI.BeginProperty(position, label, property);

				GUI.backgroundColor = Color.red;
				EditorGUI.LabelField(position, label.text, $"Use [{MemberName}] with strings.", EditorStyles.helpBox);

				EditorGUI.EndProperty();
			}
		}

		private string[] GetMembersAddingCustom(string customValue)
		{
			if (string.IsNullOrEmpty(customValue)) customValue = "- (empty) -";
			
			string[] values = new string[MemberStringArray.Length + 1];
			values[0] = customValue;

			for (var i = 0; i < MemberStringArray.Length; i++)
			{
				values[i + 1] = MemberStringArray[i];
			}

			return values;
		}
	}
}