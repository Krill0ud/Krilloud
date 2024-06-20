using System.Collections.Generic;
using System.Linq;
using KrillAudio.Krilloud.Definitions;
using KrillAudio.Krilloud.Services.Logging;
using KrillAudio.Krilloud.Utils;
using UnityEngine;

namespace KrillAudio.Krilloud
{
	public sealed class KLSettings : ResourceSingleton<KLSettings>
	{
		[Header("Settings")]
		public bool autogenerateCode = true;

		[Header("Logs")]
		public LogLevel editorLogLevel = LogLevel.Warning;
		public LogLevel buildLogLevel = LogLevel.Error;

		[Header("Debug")]
		[SerializeField] private bool m_krilloudInDebugMode = false;
		public bool krilloudInDebugMode
		{
			get { return m_krilloudInDebugMode; }
		}

		[Header("Internal")]
		[SerializeField]
		[HideInInspector]
		public PlaceholderContract placeholderContract = new PlaceholderContract();

		[SerializeField]
		[HideInInspector]
		public KLContractDefinition cacheContract = new KLContractDefinition();

		public void ClearCache()
		{
			cacheContract = new KLContractDefinition();
		}

		public void RemoveDuplicates()
		{
			placeholderContract.tags = placeholderContract.tags.Distinct().ToList();
			placeholderContract.variables = placeholderContract.variables.Distinct().ToList();
		}

		#region Nested

		[System.Serializable]
		public class PlaceholderContract
		{
			public List<string> tags = new List<string>();
			public List<string> variables = new List<string>();
			public List<string> channels = new List<string>();
		}

		#endregion Nested
	}
}