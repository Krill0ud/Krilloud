using System.Collections.Generic;
using System.Linq;
using KrillAudio.Krilloud.Definitions;
using KrillAudio.Krilloud.Services.Logging;
using KrillAudio.Krilloud.Utils;
using UnityEngine;

namespace KrillAudio.Krilloud
{
    public sealed class KLServer : ResourceSingleton<KLServer>
    {
        [HideInInspector]
        public bool startStopServer;
        public void StartStopServerManually()
        {
            startStopServer = !startStopServer;
            KrilloudServer.Instance.StartStopKrilloudServer();
        }
        [Header("Make the server active only when the project is running")]
        public bool startServerWhenPlay = false;

        /*  [Header("Allow the server to be enabled / disabled in build")]
          public bool startServerWhenBuild = false;*/

        [Header("New Server Port")]
        public int serverPort;
    }
}

