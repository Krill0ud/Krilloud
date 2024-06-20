using System;
using System.Collections.Generic;
using Newtonsoft.Json;

namespace KrillAudio.Krilloud.Definitions
{
	[Serializable]
	public sealed class KLContractDefinition
	{
		[JsonProperty("tags")]
		public List<KLTagDefinition> tags;

		[JsonProperty("variables")]
		public List<KLVariableDefinition> variables;

		[JsonProperty("sounbank_files")]
		public List<KLSoundbankFileDefinition> soundbank_files;

		[JsonProperty("channels")]
		public List<KLChannelDefinition> channels;

		public KLContractDefinition()
		{
			tags = new List<KLTagDefinition>();
			variables = new List<KLVariableDefinition>();
			soundbank_files = new List<KLSoundbankFileDefinition>();
			channels = new List<KLChannelDefinition>();
		}

		/// <summary>
		/// Generate a full copy of contract
		/// </summary>
		public KLContractDefinition(KLContractDefinition other)
		{
			// Tags
			tags = new List<KLTagDefinition>();
			other.tags.ForEach(x => tags.Add(new KLTagDefinition(x)));

			// Variables
			variables = new List<KLVariableDefinition>();
			other.variables.ForEach(x => variables.Add(new KLVariableDefinition(x)));

			// Soundbanks
			soundbank_files = new List<KLSoundbankFileDefinition>();
			other.soundbank_files.ForEach(x => soundbank_files.Add(new KLSoundbankFileDefinition(x)));

			// Channels
			channels = new List<KLChannelDefinition>();
			other.channels.ForEach(x => channels.Add(new KLChannelDefinition(x)));
		}

		public static KLContractDefinition MergeContracts(KLContractDefinition main, KLContractDefinition other)
		{
			KLContractDefinition contract = new KLContractDefinition();

			// Merge tags
			contract.tags = new List<KLTagDefinition>(main.tags);
			foreach (var tag in other.tags)
			{
				if (contract.tags.Find(x => x.name.Equals(tag.name)) == null)
				{
					contract.tags.Add(tag);
				}
			}

			// Merge variables
			contract.variables = new List<KLVariableDefinition>(main.variables);
			foreach (var variable in other.variables)
			{
				if (contract.variables.Find(x => x.name.Equals(variable.name)) == null)
				{
					contract.variables.Add(variable);
				}
			}

			// Merge soundbanks
			contract.soundbank_files = new List<KLSoundbankFileDefinition>(main.soundbank_files);
			foreach (var soundbank in other.soundbank_files)
			{
				if (contract.soundbank_files.Find(x => x.id == soundbank.id) == null)
				{
					contract.soundbank_files.Add(soundbank);
				}
			}

			// Merge channels
			contract.channels = new List<KLChannelDefinition>(main.channels);
			foreach (var channel in other.channels)
			{
				if (contract.channels.Find(x => x.name.Equals(channel.name)) == null)
				{
					contract.channels.Add(channel);
				}
			}

			return contract;
		}
	}
}