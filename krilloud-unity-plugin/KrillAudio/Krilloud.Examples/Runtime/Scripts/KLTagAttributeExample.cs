using System.Collections.Generic;
using UnityEngine;

namespace KrillAudio.Krilloud.Examples
{
	public class KLTagAttributeExample : MonoBehaviour
	{
		[Header("Types")]
		[KLTag] public int v1;
		[KLTag] public float v2;
		[KLTag] public string v3;
		[KLTag] public uint v4;
		[KLTag] public bool v5;
		[KLTag] public char v6;
		[KLTag] public Transform v7;

		[Header("Array")]
		[KLTag] public int[] ar1;
		[KLTag] public float[] ar2;
		[KLTag] public string[] ar3;
		[KLTag] public uint[] ar4;
		[KLTag] public bool[] ar5;
		[KLTag] public char[] ar6;

		[Header("List")]
		[KLTag] public List<int> l1;
		[KLTag] public List<float> l2;
		[KLTag] public List<string> l3;
		[KLTag] public List<uint> l4;
		[KLTag] public List<bool> l5;
		[KLTag] public List<char> l6;
		[KLTag] public List<Transform> l7;

		[Header("Struct")]
		[KLTag] public TestStruct s1;
		public TestStruct s2;
		public TestStruct2 s3;

		[Header("Invalid values")]
		[KLTag] public string it1 = "";
		[KLTag] public string it2 = "<>";
		[KLTag] public string it3 = "invalidTagNameGGWP";

		[System.Serializable]
		public struct TestStruct
		{
			public int v1;
			public float v2;
			public string v3;
			public uint v4;
			public bool v5;
			public char v6;
			public Transform v7;
		}

		[System.Serializable]
		public struct TestStruct2
		{
			public int v1;
			public float v2;
			[KLVariable] public string v3;
			public uint v4;
			public bool v5;
			public char v6;
			public Transform v7;
		}
	}
}