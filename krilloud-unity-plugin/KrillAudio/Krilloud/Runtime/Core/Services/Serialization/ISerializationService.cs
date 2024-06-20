namespace KrillAudio.Krilloud.Services.Serialization
{
	public interface ISerializationService
	{
		void Serialize<T>(T target, string path);

		T Deserialize<T>(string path);
	}
}