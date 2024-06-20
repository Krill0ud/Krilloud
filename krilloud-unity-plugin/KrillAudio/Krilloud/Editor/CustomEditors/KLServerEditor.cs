using KrillAudio.Krilloud.Definitions;
using UnityEditor;
using UnityEngine;
using KrillAudio.Krilloud;

namespace KrillAudio.Krilloud.Editor
{
    using Editor = UnityEditor.Editor;

    [CustomEditor(typeof(KLServer))]
    public sealed class KLServerEditor : Editor
    {
        private KLServer m_target;
        private bool serverPlayState;

        private void OnEnable()
        {
            m_target = target as KLServer;

            KLServer.Instance.serverPort = KrilloudServer.Instance.GetKrilloudServerPort();
            KrilloudServer.Instance.ChangeServerPort();
        }
        public override void OnInspectorGUI()
        {
            KLEditorUtils.DrawKrillHeader();

            EditorGUI.BeginChangeCheck();

            base.OnInspectorGUI();


            EditorGUILayout.Space();
            if (KLServer.Instance.startServerWhenPlay && serverPlayState)
            {
                KrilloudServer.Instance.ChangeServerPort();
                KrilloudServer.Instance.StartServerWhenPlay(KLServer.Instance.startServerWhenPlay);
                serverPlayState = false;

            }
            else if (!KLServer.Instance.startServerWhenPlay && !serverPlayState)
            {
                KrilloudServer.Instance.StartServerWhenPlay(KLServer.Instance.startServerWhenPlay);
                serverPlayState = true;
            }
            GUI.enabled = serverPlayState;

            DrawServerPortButton();
            DrawButtons();
        }
        private void DrawServerPortButton()
        {
            EditorGUILayout.BeginHorizontal();
            if (GUILayout.Button("Change Server Port"))
            {
                KrilloudServer.Instance.ChangeServerPort();
            }
            EditorGUILayout.LabelField("Current Server Port: " + KrilloudServer.Instance.GetKrilloudServerPort());
            EditorGUILayout.EndHorizontal();
        }
        private void DrawButtons()
        {
            EditorGUILayout.BeginHorizontal();
            string state;
            if (KrilloudServer.Instance.IsKrilloudServerActive())
            {
                // KLServer.Instance.startStopServer = false;
                state = "Stop";
            }
            else
            {
               // KLServer.Instance.startStopServer = false;
                state = "Start";
            }
            if (GUILayout.Button(state + " Server Manually"))
            {
                KLServer.Instance.StartStopServerManually();
                //SaveServerState();
            }
            GUI.enabled = true;

            EditorGUILayout.EndHorizontal();
        }
       /* string LoadServerState()
        {
            string state;
            if (PlayerPrefs.HasKey("ServerState"))
            {
                string s = PlayerPrefs.GetString("ServerState");
                if (s == "True") KLServer.Instance.startStopServer = true;
                else KLServer.Instance.startStopServer = false;
            }
            if (KLServer.Instance.startStopServer) state = "Stop";
            else state = "Start";
            return state;
        }
        void SaveServerState()
        {
            if (KLServer.Instance.startStopServer) PlayerPrefs.SetString("ServerState", "True");
            else PlayerPrefs.SetString("ServerState", "False");
        }*/
    }
}