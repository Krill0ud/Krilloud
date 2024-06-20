using System.Collections.Generic;
using UnityEngine;

namespace KrillAudio.Krilloud.Examples
{
	public class KLVariableAttributeExample : MonoBehaviour
	{
		[Header("Types")]
		[KLVariable] public int v1;
		[KLVariable] public float v2;
		[KLVariable] public string v3;
		[KLVariable] public uint v4;
		[KLVariable] public bool v5;
		[KLVariable] public char v6;
		[KLVariable] public Transform v7;

		[Header("Array")]
		[KLVariable] public int[] ar1;
		[KLVariable] public float[] ar2;
		[KLVariable] public string[] ar3;
		[KLVariable] public uint[] ar4;
		[KLVariable] public bool[] ar5;
		[KLVariable] public char[] ar6;

		[Header("List")]
		[KLVariable] public List<int> l1;
		[KLVariable] public List<float> l2;
		[KLVariable] public List<string> l3;
		[KLVariable] public List<uint> l4;
		[KLVariable] public List<bool> l5;
		[KLVariable] public List<char> l6;
		[KLVariable] public List<Transform> l7;

		[Header("Struct")]
		[KLVariable] public TestStruct s1;
		public TestStruct s2;
		public TestStruct2 s3;

		[Header("Invalid values")]
		[KLVariable] public string it1 = "";
		[KLVariable] public string it2 = "<>";
		[KLVariable] public string it3 = "invalidTagNameGGWP";

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