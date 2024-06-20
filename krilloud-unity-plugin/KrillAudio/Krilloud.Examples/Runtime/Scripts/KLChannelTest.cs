using UnityEngine;

namespace KrillAudio.Krilloud.Examples
{
	[RequireComponent(typeof(KLAudioSource))]
	public class KLChannelTest : MonoBehaviour
	{
        public string channelName = "Master";

		[Range(0f, 1f)]
		public float volume = 1;

		private KLAudioSource m_asource;
		public KLAudioSource asource
		{
			get
			{
				if (m_asource == null) m_asource = GetComponent<KLAudioSource>();
				return m_asource;
			}
		}

		#region Unity Events

		private void OnEnable()
		{
			asource.SetChannelVolume(channelName, volume);
		}

		private void OnValidate()
		{
			if (!Application.isPlaying || !enabled) return;

			asource.SetChannelVolume(channelName, volume);
		}

		#endregion Unity Events
	}
}