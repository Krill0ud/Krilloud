using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using KrillAudio.Krilloud;
using System;
using System.Runtime.InteropServices;

namespace KrillAudio.Krilloud
{
    public class KLContractUpdate : ScriptableObject
    {
        //public const string KRILLOUD_LIB = "Krilloud_x64";

        private static KLContractUpdate instance;
        public static KLContractUpdate Instance
        {
            get { return instance ?? (instance = ScriptableObject.CreateInstance<KLContractUpdate>()); }
        }

        [DllImport(Krilloud.KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr EX_GetContractData([MarshalAs(UnmanagedType.LPStr)] string contract_folder_path);
        public string GetContractData(string contract_folder_path)
        {
            return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(EX_GetContractData(contract_folder_path));
        }

    }

}
