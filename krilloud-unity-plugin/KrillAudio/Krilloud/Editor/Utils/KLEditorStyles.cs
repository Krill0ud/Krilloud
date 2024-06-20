using UnityEditor;
using UnityEngine;

namespace KrillAudio.Krilloud.Editor
{
	public static class KLEditorStyles
	{
		public static Color KRILLAUDIO_COLOR
		{
			get { return new Color(0.4824f, 0.7647f, 0f); }
		}

		public static Color SEPARATOR_LINE_COLOR_LIGHT
		{
			get { return new Color(.545f, .545f, .545f); }
		}

		public static Color SEPARATOR_LINE_COLOR_DARK
		{
			get { return new Color(.145f, .145f, .145f); }
		}

		private static Texture2D m_krillAudioIcon;
		public static Texture2D KRILLAUDIO_ICON
		{
			get
			{
				if (m_krillAudioIcon == null) m_krillAudioIcon = Resources.Load("krillaudio-icon") as Texture2D;
				return m_krillAudioIcon;
			}
		}

		private static Texture2D m_krillAudioLogoLight;
		public static Texture2D KRILLAUDIO_LOGO_LIGHT
		{
			get
			{
				if (m_krillAudioLogoLight == null) m_krillAudioLogoLight = Resources.Load("krillaudio-logo-light") as Texture2D;
				return m_krillAudioLogoLight;
			}
		}

		private static Texture2D m_krillAudioLogoDark;
		public static Texture2D KRILLAUDIO_LOGO_DARK
		{
			get
			{
				if (m_krillAudioLogoDark == null) m_krillAudioLogoDark = Resources.Load("krillaudio-logo-dark") as Texture2D;
				return m_krillAudioLogoDark;
			}
		}

		public static Texture2D KRILLAUDIO_LOGO
		{
			get
			{
				return EditorGUIUtility.isProSkin ? KRILLAUDIO_LOGO_DARK : KRILLAUDIO_LOGO_LIGHT;
			}
		}

		private static GUIContent m_hierarchyIconContent;
		public static GUIContent HIERARCHY_ICON_CONTENT
		{
			get
			{
				if (m_hierarchyIconContent == null)
				{
					m_hierarchyIconContent = new GUIContent(KRILLAUDIO_ICON);
				}

				return m_hierarchyIconContent;
			}
		}

		private static GUIStyle m_hierarchyIconStyle;
		public static GUIStyle HIERARCHY_ICON_STYLE
		{
			get
			{
				if (m_hierarchyIconStyle == null)
				{
					m_hierarchyIconStyle = new GUIStyle(EditorStyles.whiteLabel);
					m_hierarchyIconStyle.alignment = TextAnchor.MiddleRight;
				}

				return m_hierarchyIconStyle;
			}
		}
	}
}