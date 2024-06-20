using System;
using KrillAudio.Krilloud.Editor;
using KrillAudio.Krilloud.Utils;
using NUnit.Framework;
using UnityEngine;
using Random = UnityEngine.Random;

namespace KrillAudio.Krilloud.Tests
{
	public class KrilloudTest
	{
		private Krilloud k;

		[Test]
		public void InitDeinitTest()
		{
			InitKrilloud();
			DeinitKrilloud();
		}

		[Test]
		public void InitDeinitMultipleTimes()
		{
			for (var i = 0; i < 32; i++)
			{
				InitDeinitTest();
				Debug.Log(i);
			}
		}

		[Test]
		public void LoadTagTest()
		{
			int id = Random.Range(0, 999999);

			InitKrilloud();

			k.LoadTag(KLEditorCore.AvailableTagsString, id);
			k.UnloadObject(id);

			DeinitKrilloud();
		}

		[Test]
		public void TestLocalParameters()
		{
			throw new NotImplementedException();
		}

		[Test]
		public void TestGlobalParameter()
		{
			throw new NotImplementedException();
		}

		#region Helpers

		private void InitKrilloud()
		{
			k = new Krilloud(KLUtils.KRILLOUD_PROJECT_PATH);
			k.Init();
		}

		private void DeinitKrilloud()
		{
			k.Deinit();
			k = null;
		}

		#endregion Helpers
	}
}