using System.IO;
using Newtonsoft.Json;

namespace KrillAudio.Krilloud.Services.Serialization
{
	public class CerealJSONSerializationService : ISerializationService
	{
		public void Serialize<T>(T target, string path)
		{
			using (StreamWriter file = File.CreateText(path))
			{
				JsonSerializer serializer = new JsonSerializer();
				serializer.Serialize(file, new KLCerealWrapper<T>(target));
			}
		}

		public T Deserialize<T>(string path)
		{
			using (StreamReader file = File.OpenText(path))
			{
				JsonSerializer serializer = new JsonSerializer();
				var wrapper = (KLCerealWrapper<T>)serializer.Deserialize(file, typeof(KLCerealWrapper<T>));
				return wrapper.value;
			}
		}

		private class KLCerealWrapper<T>
		{
			[JsonProperty("value0")]
			public T value;

			public KLCerealWrapper(T value)
			{
				this.value = value;
			}
		}
	}
}