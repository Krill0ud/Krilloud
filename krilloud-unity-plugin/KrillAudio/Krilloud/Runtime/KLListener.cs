using System.Collections.Generic;
using KrillAudio.Krilloud.Services;
using UnityEngine;

namespace KrillAudio.Krilloud
{
	[AddComponentMenu("Krilloud/KLListener")]
	[DisallowMultipleComponent]
	public sealed class KLListener : KLBehaviour
	{
		private Vector3 m_lastPosition;
		private Vector3 m_velocity;

		private Vector3 m_lastUpdatedPos;
		private Vector3 m_lastUpdatedFwd;
		private Vector3 m_lastUpdatedUp;
		private Vector3 m_lastUpdatedVel;

		#region Properties

		private static List<KLListener> m_listenerStack = new List<KLListener>();
		public static KLListener Current
		{
			get
			{
				if (Application.isPlaying)
				{
					return m_listenerStack.Count > 0 ? m_listenerStack[m_listenerStack.Count - 1] : null;
				}
				else
				{
					return FindObjectOfType<KLListener>();
				}
			}
		}

		#endregion Properties

		#region Unity events

		private void OnEnable()
		{
			m_listenerStack.Add(this);

			m_lastPosition = CachedTransform.position;
			m_velocity = Vector3.zero;

			UpdateListenerPosition();
		}

		private void OnDisable()
		{
			m_listenerStack.Remove(this);
		}

		private void FixedUpdate()
		{
			m_velocity = (CachedTransform.position - m_lastPosition) / Time.fixedDeltaTime;
			m_lastPosition = CachedTransform.position;

			UpdateListenerPosition();
		}

		private void Reset()
		{
			if (FindObjectsOfType<KLListener>().Length > 1)
			{
				KLStartup.Logger.LogWarning("<b>[KLListener]</b> Only one KLListener by scene!");
			}
		}

		#endregion Unity events

		#region Helpers

		private void UpdateListenerPosition(bool force = false)
		{
			UpdateListenerPositionInternal(force, CachedTransform.position, CachedTransform.forward, CachedTransform.up, m_velocity);
		}

		private void UpdateListenerPositionInternal(bool force, Vector3 position, Vector3 forward, Vector3 up, Vector3 velocity)
		{
			velocity = Vector3.zero;

			if (force || m_lastUpdatedPos != position || m_lastUpdatedFwd != forward
				|| m_lastUpdatedUp != up || m_lastUpdatedVel != velocity)
			{
				KLCenter.Instance.SetListenerPosition(position, forward, up, velocity);

				m_lastUpdatedPos = position;
				m_lastUpdatedFwd = forward;
				m_lastUpdatedUp = up;
				m_lastUpdatedVel = velocity;
			}
		}

		#endregion Helpers
	}
}