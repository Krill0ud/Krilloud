using System;
using UnityEngine;

namespace KrillAudio.Krilloud.Services.Logging
{
	public class KLUnityLogger : ILogger
	{
		public LogLevel logLevel
		{
			get
			{
#if UNITY_EDITOR
				return KLSettings.Instance.editorLogLevel;
#else
				return KLSettings.Instance.buildLogLevel;
#endif
			}
			set
			{
			}
		}

		public void Log(object message)
		{
			if (logLevel > LogLevel.Info) return;
			Debug.Log(string.Format("[{0}][{1}] {2}", "INFO", GetTime(), message));
		}

		public void Log(object message, params object[] args)
		{
			if (logLevel > LogLevel.Info) return;
			Debug.LogFormat(string.Format("[{0}][{1}] {2}", "INFO", GetTime(), message), args);
		}

		public void LogWarning(object message)
		{
			if (logLevel > LogLevel.Warning) return;
			Debug.LogWarning(string.Format("[{0}][{1}] {2}", "WARNING", GetTime(), message));
		}

		public void LogWarning(object message, params object[] args)
		{
			if (logLevel > LogLevel.Warning) return;
			Debug.LogWarningFormat(string.Format("[{0}][{1}] {2}", "WARNING", GetTime(), message), args);
		}

		public void LogError(object message)
		{
			if (logLevel > LogLevel.Error) return;
			Debug.LogError(string.Format("[{0}][{1}] {2}", "ERROR", GetTime(), message));
		}

		public void LogError(object message, params object[] args)
		{
			if (logLevel > LogLevel.Error) return;
			Debug.LogErrorFormat(string.Format("[{0}][{1}] {2}", "ERROR", GetTime(), message), args);
		}

		private string GetTime()
		{
			return DateTime.Now.ToString("HH:mm:ss");
		}
	}
}