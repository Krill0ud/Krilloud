namespace KrillAudio.Krilloud.Services.Logging
{
	public interface ILogger
	{
		LogLevel logLevel { get; set; }

		void Log(object message);

		void Log(object message, params object[] args);

		void LogWarning(object message);

		void LogWarning(object message, params object[] args);

		void LogError(object message);

		void LogError(object message, params object[] args);
	}

	public enum LogLevel
	{
		Info = 0,
		Warning = 1,
		Error = 2,
		None = 999
	}
}