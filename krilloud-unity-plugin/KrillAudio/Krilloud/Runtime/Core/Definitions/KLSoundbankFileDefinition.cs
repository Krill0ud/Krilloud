using System;
using Newtonsoft.Json;

namespace KrillAudio.Krilloud.Definitions
{
	[Serializable]
	public sealed class KLSoundbankFileDefinition
	{
		[JsonProperty("id")]
		public int id;

		[JsonProperty("bankFile")]
		public string bankFile;

		[JsonProperty("offsetBytes")]
		public int offsetBytes;

		[JsonProperty("size")]
		public int size;

		public KLSoundbankFileDefinition()
		{
		}

		public KLSoundbankFileDefinition(KLSoundbankFileDefinition other)
		{
			this.id = other.id;
			this.bankFile = other.bankFile;
			this.offsetBytes = other.offsetBytes;
			this.size = other.size;
		}
	}
}