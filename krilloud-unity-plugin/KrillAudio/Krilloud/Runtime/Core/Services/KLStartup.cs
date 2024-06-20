using KrillAudio.Krilloud.Services.Logging;
using KrillAudio.Krilloud.Services.Serialization;

namespace KrillAudio.Krilloud.Services
{
	public static class KLStartup
	{
		public static ISerializationService Serialization { get; private set; }
		public static ILogger Logger { get; private set; }

		static KLStartup()
		{
			Serialization = new CerealJSONSerializationService();
			Logger = new KLUnityLogger();
		}
	}
}