using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using KrillAudio.Krilloud;
using System;
using System.Runtime.InteropServices;

namespace KrillAudio.Krilloud
{
    public class KrilloudServer : ScriptableObject
    {
        private static KrilloudServer instance;
        public static KrilloudServer Instance
        {
            get { return instance ?? (instance = ScriptableObject.CreateInstance<KrilloudServer>()); }
        }

        public void StartStopKrilloudServer()
        {
            ChangeServerPort();
            bool value;
            if (KLServer.Instance.startStopServer)
            {
                if (!IsKrilloudServerActive())
                {
                    value = StartKrilloudServer();

                }
            }
            else
            {
                if (IsKrilloudServerActive())
                {
                    value = StopKrilloudServer();
                }
            }
        }

        public void ChangeServerPort()
        {
            if(KLServer.Instance.serverPort == 0) SetKrilloudServerPort(GetKrilloudServerPort());
            else
            {
                bool port = SetKrilloudServerPort(KLServer.Instance.serverPort);
                if (!port)
                {
                    SetKrilloudServerPort(GetKrilloudServerPort());
                }
            }         
        }

        #region SERVER
        [DllImport(Krilloud.KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool EX_IsKrilloudServerActive();
        public bool IsKrilloudServerActive()
        {
            return EX_IsKrilloudServerActive();
        }
        [DllImport(Krilloud.KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool EX_StartServer();
        public bool StartKrilloudServer()
        {
            return EX_StartServer();
        }

        [DllImport(Krilloud.KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool EX_StoptServer();
        public bool StopKrilloudServer()
        {
            return EX_StoptServer();
        }

        [DllImport(Krilloud.KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool EX_KeepServerOnlyOnPlay(bool value);
        public bool StartServerWhenPlay(bool value)
        {
            return EX_KeepServerOnlyOnPlay(value);
        }

        [DllImport(Krilloud.KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool EX_KeepServerOnBuild(bool value);
        public bool StartServerWhenBuild(bool value)
        {
            return EX_KeepServerOnBuild(value);
        }

        [DllImport(Krilloud.KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool EX_SetServerPort(int value);
        public bool SetKrilloudServerPort(int value)
        {
            return EX_SetServerPort(value);
        }
        [DllImport(Krilloud.KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int EX_GetServerPort();
        public int GetKrilloudServerPort()
        {
            return EX_GetServerPort();
        }
        #endregion SERVER
    }
}
