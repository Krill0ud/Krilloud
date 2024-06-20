using UnityEngine;

namespace KrillAudio.Krilloud.Examples
{
	[RequireComponent(typeof(KLAudioSource))]
	public class KLAudioSourceTest : MonoBehaviour
	{
		private KLAudioSource source;

		private void Awake()
		{
			source = GetComponent<KLAudioSource>();
		}

		private void OnGUI()
		{
			var tags = source.Tags;
			for (var i = 0; i < tags.Count; i++)
			{
				if (GUI.Button(new Rect(5, 5 + 25 * i, 200, 25), tags[i]))
				{
					source.Play(tags[i]);
				}
			}
		}
	}
}