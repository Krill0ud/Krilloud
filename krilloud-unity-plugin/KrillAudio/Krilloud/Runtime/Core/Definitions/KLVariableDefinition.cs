using System;
using Newtonsoft.Json;
using UnityEngine;

namespace KrillAudio.Krilloud.Definitions
{
	[Serializable]
	public sealed class KLVariableDefinition : IKLMemberDefinition
	{
		public string Name => name;
		public SourceContract SourceContract => sourceContract;
		
		[JsonProperty("variable_name")]
		public string name;

		[JsonProperty("current_value")]
		public float defaultValue;

		[JsonProperty("type")]
		public bool isGlobal;

		[JsonProperty("source")]
		public VariableSource source = VariableSource.Ingame;

		[JsonProperty("content")]
		public ContentType content;

		[JsonProperty("range_min")]
		public float min;

		[JsonProperty("range_max")]
		public float max;

		[JsonIgnore]
		[HideInInspector]
		public SourceContract sourceContract = SourceContract.Krilloud;

		public KLVariableDefinition()
		{
		}

		public KLVariableDefinition(string name, bool isGlobal, VariableSource source, SourceContract sourceContract)
		{
			this.name = name;
			this.isGlobal = isGlobal;
			this.source = source;

			this.sourceContract = sourceContract;
		}

		public KLVariableDefinition(KLVariableDefinition other)
		{
			this.name = other.name;
			this.defaultValue = other.defaultValue;
			this.isGlobal = other.isGlobal;
			this.source = other.source;
			this.content = other.content;
			this.min = other.min;
			this.max = other.max;

			this.sourceContract = other.sourceContract;
		}

		public enum VariableSource
		{
			RandomRepetition,
			RandomNoRepetition,
			Ingame
		}

		public enum ContentType
		{
			Ranged,
			Raw
		}
	}
}