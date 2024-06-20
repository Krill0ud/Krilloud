using System;
using Newtonsoft.Json;
using UnityEngine;

namespace KrillAudio.Krilloud.Definitions
{
	[Serializable]
	public sealed class KLTagDefinition : IKLMemberDefinition
	{
		[JsonProperty("tagname")]
		public string name;

		[JsonProperty("description")]
		public string description;

		[JsonProperty("is3DTag")]
		public bool is3D;

		[JsonProperty("Max3DDistance")]
		public float max3DDistance;

		[JsonProperty("isLoopable")]
		public bool isLoop;

		[JsonProperty("channel_id")]
		public int channelId;

		[JsonIgnore]
		[HideInInspector]
		public SourceContract sourceContract = SourceContract.Krilloud;

		public string Name => name;
		public SourceContract SourceContract => sourceContract;
		
		public KLTagDefinition()
		{
			name = "";
			description = "";
		}

		public KLTagDefinition(KLTagDefinition other)
		{
			this.name = other.name;
			this.description = other.description;
			this.is3D = other.is3D;
			this.max3DDistance = other.max3DDistance;
			this.isLoop = other.isLoop;
			this.channelId = other.channelId;

			this.sourceContract = other.sourceContract;
		}
	}
}