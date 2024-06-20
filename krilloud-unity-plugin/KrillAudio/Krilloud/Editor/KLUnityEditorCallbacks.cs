using UnityEditor;

namespace KrillAudio.Krilloud.Editor
{
	[InitializeOnLoad]
	internal class KLUnityEditorCallbacks
	{
		static KLUnityEditorCallbacks()
		{
			EditorApplication.playModeStateChanged += EditorApplication_playModeStateChanged;
			EditorApplication.pauseStateChanged += EditorApplication_pauseStateChanged;
		}

		private static void EditorApplication_playModeStateChanged(PlayModeStateChange obj)
		{
			if (obj == PlayModeStateChange.EnteredPlayMode && EditorApplication.isPaused)
			{
				KLCenter.PauseKrilloud();
			}
		}

		private static void EditorApplication_pauseStateChanged(PauseState obj)
		{
			if (EditorApplication.isPlaying)
			{
				if (obj == PauseState.Unpaused)
				{
					KLCenter.ResumeKrilloud();
				}
				else if (obj == PauseState.Paused)
				{
					KLCenter.PauseKrilloud();
				}
			}
		}
	}
}