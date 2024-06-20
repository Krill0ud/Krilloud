using System.Collections.Generic;
using UnityEngine;

namespace KrillAudio.Krilloud.Examples
{
	[RequireComponent(typeof(KLAudioSource))]
	public class KLAudioSourceVariableTest : MonoBehaviour
	{
		public List<Variable> variables = new List<Variable>();

		private KLAudioSource source;

		private void Awake()
		{
			source = GetComponent<KLAudioSource>();
		}

		private void Update()
		{
			foreach (var v in variables)
			{
				source.SetFloatVar(v.name, v.value);
			}
		}

		[System.Serializable]
		public struct Variable
		{
			[KLVariable]
			public string name;

			public float value;
		}
	}
}