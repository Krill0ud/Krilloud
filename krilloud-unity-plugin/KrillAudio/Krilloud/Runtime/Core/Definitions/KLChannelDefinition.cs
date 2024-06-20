using System;
using Newtonsoft.Json;
using UnityEngine;

namespace KrillAudio.Krilloud.Definitions
{
	[Serializable]
	public sealed class KLChannelDefinition : IKLMemberDefinition
	{
		public string Name => name;
		public SourceContract SourceContract => sourceContract;
		
		[JsonProperty("channel_name")]
		public string name;
		
		[JsonProperty("parent_id")]
		public int parent;

		[JsonProperty("volume")]
		public float volume;

		[JsonIgnore]
		[HideInInspector]
		public SourceContract sourceContract = SourceContract.Krilloud;
		
		public KLChannelDefinition()
		{
		}

		public KLChannelDefinition(KLChannelDefinition other)
		{
			this.name = other.name;
			this.parent = other.parent;
			this.volume = other.volume;
			this.sourceContract = other.sourceContract;
		}
	}
}