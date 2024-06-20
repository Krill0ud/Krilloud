using System;
using System.IO;
using System.Linq;
using KrillAudio.Krilloud.Definitions;
using KrillAudio.Krilloud.Services;
using UnityEditor;

namespace KrillAudio.Krilloud.Editor
{
	[InitializeOnLoad]
	public static class KLEditorCore
	{
		//private static FileSystemWatcher m_watcher;
		//private static long m_lastWatcherUpdate;

		static KLEditorCore()
		{
			//if (m_watcher == null)
			//{
			//	Debug.Log($"<b>[{nameof(KLEditorCore)}]</b> Initializing watcher!");

			//	m_watcher = new FileSystemWatcher(KLUtils.KRILLOUD_PROJECT_PATH);
			//	m_watcher.NotifyFilter = NotifyFilters.LastAccess;
			//	m_watcher.Filter = "*";

			//	m_watcher.Created += OnWatcherDetectedChanges;
			//	m_watcher.Changed += OnWatcherDetectedChanges;
			//	m_watcher.Deleted += OnWatcherDetectedChanges;
			//	m_watcher.Renamed += OnWatcherDetectedChanges;

			//	m_watcher.EnableRaisingEvents = true;
			//}

			Initialize();
		}

		private static KLContractDefinition m_mainContract;
		private static KLContractDefinition m_placeholderContract;
		private static KLContractDefinition m_editorContract;
		
		/// <summary>
		/// Array of current tags definitions
		/// </summary>
		public static KLTagDefinition[] AvailableTags { get; private set; }

		public static KLVariableDefinition[] AvailableVariables { get; private set; }
		
		public static KLChannelDefinition[] AvailableChannels { get; private set; }

		/// <summary>
		/// Array of current tags names
		/// </summary>
		public static string[] AvailableTagsString { get; private set; }

		/// <summary>
		/// Array of current variables names
		/// </summary>
		public static string[] AvailableVariablesString { get; private set; }
		
		/// <summary>
		/// Array of current channel names
		/// </summary>
		public static string[] AvailableChannelsString { get; private set; }

		static internal KLContractDefinition MainContract
		{
			get { return m_mainContract; }
		}

		public static void Initialize()
		{
			KLStartup.Logger.Log("<b>[KLEditorCore]</b> Initializing!");

			// Settings
			var i = KLSettings.Instance;

			UpdateContract();
			
			// Update variables
			UpdateAvailableTags();
			UpdateAvailableVariables();
			UpdateAvailableChannels();
			
			
			CreateKrilloudDataFolder();

			if (!EditorApplication.isPlayingOrWillChangePlaymode)
			{
				UpdateGeneratedCode();
			}

			KrilloudBuildProcessor.UpdateSerialsList();
		}
		
		public static void RefreshCore()
		{
			Initialize();
		}

		/// <summary>
		/// Get a copy of main contract (EXPENSIVE METHOD!)
		/// </summary>
		public static KLContractDefinition GetContractCopy()
		{
			return new KLContractDefinition(m_mainContract);
		}

		private static void UpdateAvailableTags()
		{
			AvailableTags = m_editorContract.tags.ToArray();
			Array.Sort(AvailableTags, (x, y) => string.Compare(x.name, y.name));

			// Build string tag array
			AvailableTagsString = AvailableTags.Select(x => x.name).ToArray();
		}

		private static void UpdateAvailableVariables()
		{
			AvailableVariables = m_editorContract.variables.Where(x => x.source == KLVariableDefinition.VariableSource.Ingame).ToArray();
			Array.Sort(AvailableVariables, (x, y) => string.Compare(x.name, y.name));

			// Build string variable array
			AvailableVariablesString = AvailableVariables.Select(x => x.name).ToArray();
		}

		private static void UpdateAvailableChannels()
		{
			AvailableChannels = m_editorContract.channels.ToArray();
			Array.Sort(AvailableChannels, (x, y) => string.Compare(x.name, y.name));
			
			// Build string variable array
			AvailableChannelsString = AvailableChannels.Select(x => x.name).ToArray();
		}

		private static void UpdateContract()
		{
			// Read main contract
			var contractPath = KLEditorUtils.KRILLOUD_CONTRACT_PATH;
			if (File.Exists(contractPath))
			{
				m_mainContract = KLStartup.Serialization.Deserialize<KLContractDefinition>(contractPath);
				KLContractUpdate.Instance.GetContractData(Utils.KLUtils.KRILLOUD_PROJECT_PATH);
			}
			else
			{
				m_mainContract = new KLContractDefinition();
				KLStartup.Logger.Log("<b>[KLEditorCore]</b> Contract doesn't exist! " + contractPath);
			}

			m_placeholderContract = BuildPlaceholderContract();
			m_editorContract = KLContractDefinition.MergeContracts(m_mainContract, m_placeholderContract);
			m_editorContract = KLContractDefinition.MergeContracts(m_editorContract, KLSettings.Instance.cacheContract);

			UpdateCacheContract(m_mainContract);
			UpdateCacheContract(m_placeholderContract);
		}


		private static void UpdateCacheContract(KLContractDefinition contract)
		{
			var cacheContract = KLSettings.Instance.cacheContract;

			foreach (var tag in contract.tags)
			{
				var oldTag = cacheContract.tags.Find(x => x.name.Equals(tag.name));
				if (oldTag != null)
				{
					cacheContract.tags.Remove(oldTag);
				}

				cacheContract.tags.Add(new KLTagDefinition(tag)
				{
					sourceContract = SourceContract.Cache
				});
			}

			foreach (var variable in contract.variables)
			{
				if (variable.source != KLVariableDefinition.VariableSource.Ingame)
					continue;

				var oldVar = cacheContract.variables.Find(x => x.name.Equals(variable.name));
				if (oldVar != null)
				{
					cacheContract.variables.Remove(oldVar);
				}

				cacheContract.variables.Add(new KLVariableDefinition(variable)
				{
					sourceContract = SourceContract.Cache
				});
			}

			// Set cache
			cacheContract.tags.ForEach(x => x.sourceContract = SourceContract.Cache);
			cacheContract.variables.ForEach(x => x.sourceContract = SourceContract.Cache);

			EditorUtility.SetDirty(KLSettings.Instance);
		}

		private static void UpdateGeneratedCode()
		{
			if (KLSettings.Instance.autogenerateCode)
			{
				KLCodeGenerator.GenerateCode();
			}
			else
			{
				if (File.Exists(KLCodeGenerator.KRILLOUD_AUTOGENERATED_CODE_GLOBAL_PATH))
				{
					KLStartup.Logger.Log("<b>[KLEditorCore]</b> Deleting autogen code...");

					try
					{
						AssetDatabase.DeleteAsset(Path.Combine("Assets", KLCodeGenerator.KRILLOUD_AUTOGENERATED_CODE_LOCAL_PATH));
						// File.Delete(KLCodeGenerator.KRILLOUD_AUTOGENERATED_CODE_FULL_PATH);
					}
					catch (Exception e)
					{
						KLStartup.Logger.LogWarning("<b>[KLEditorCore]</b> Delete autogen code failed!\n" + e.Message);
					}
				}
			}

			AssetDatabase.Refresh();
		}

		static void CreateKrilloudDataFolder()
		{
			var assetPath = "Assets/StreamingAssets/KrilloudData/";
			Directory.CreateDirectory(Path.GetDirectoryName(assetPath));
			System.IO.File.WriteAllText(assetPath + "info.txt", "The folder for KrilloudData");
		}

		private static KLContractDefinition BuildPlaceholderContract()
		{
			KLContractDefinition result = new KLContractDefinition();

			// Add all placeholder tags
			foreach (var tag in KLSettings.Instance.placeholderContract.tags)
			{
				result.tags.Add(new KLTagDefinition()
				{
					name = tag,
					sourceContract = SourceContract.Placeholder
				});
			}

			// Add all placeholder variables
			foreach (var variable in KLSettings.Instance.placeholderContract.variables)
			{
				result.variables.Add(
					new KLVariableDefinition(
						variable,
						false,
						KLVariableDefinition.VariableSource.Ingame,
						SourceContract.Placeholder
					)
				);
			}
			
			// Add all placeholder channels
			foreach (var channel in KLSettings.Instance.placeholderContract.channels)
			{
				result.channels.Add(
					new KLChannelDefinition()
					{
						name = channel,
						sourceContract = SourceContract.Placeholder
					}
				);
			}

			return result;
		}

		#region Event Handling

		//private static void OnWatcherDetectedChanges(object sender, FileSystemEventArgs e)
		//{
		//	Initialize();
		//}

		#endregion Event Handling       
	}
}