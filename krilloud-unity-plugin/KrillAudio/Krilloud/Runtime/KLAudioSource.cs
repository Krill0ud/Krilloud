using System;
using System.Collections.Generic;
using KrillAudio.Krilloud.Services;
using KrillAudio.Krilloud.Utils;
using UnityEngine;
using Random = UnityEngine.Random;

namespace KrillAudio.Krilloud
{
	[AddComponentMenu("Krilloud/KLAudioSource")]
	[DisallowMultipleComponent]
	public class KLAudioSource : KLBehaviour
	{
		public SourceType sourceShape = SourceType.Point;

		// Sphere source type
		public float sphereShapeRadius = 1;

		// Line source type
		public Vector3 lineShapeStart = -Vector3.forward;
		public Vector3 lineShapeEnd = Vector3.forward;

		// Area source type
		public Vector3 areaShapeSize = Vector3.one;

		public bool playOnAwake;

		[KLTag]
		[SerializeField]
		private List<string> m_tags = new List<string>();

		private Vector3 m_lastPosition;
		private Vector3 m_velocity;

		private Vector3 m_lastUpdatedPos;
		private Vector3 m_lastUpdatedVel;

		private int m_frameOffset;

		#region Properties

		public List<string> Tags
		{
			get { return new List<string>(m_tags); }
		}

		public int InstanceID
		{
			get { return gameObject.GetInstanceID(); }
		}

		#endregion Properties

		#region Unity Events

		protected void Awake()
		{
			InitSource();
		}

		private void Start()
		{
			m_frameOffset = Random.Range(0, 4);

			if (playOnAwake)
			{
				Play();
			}
		}

		private void OnEnable()
		{
			m_lastPosition = CachedTransform.position;
			m_velocity = Vector3.zero;

			UpdateObjectPosition(true);
		}

		private void OnDisable()
		{
			Stop();
		}

		private void OnDestroy()
		{
			DeinitSource();
		}

		private void Update()
		{
			Vector3 pos = CachedTransform.position;

			m_velocity = (pos - m_lastPosition) / Time.deltaTime;
			m_lastPosition = pos;

			if (Time.frameCount % 4 == m_frameOffset)
			{
				UpdateObjectPosition();
			}
		}

		#endregion Unity Events

		#region Public API

		/// <summary>
		/// Play first clip
		/// </summary>
		public void Play()
		{
			Play(0);
		}

		/// <summary>
		/// Play clip using tag index
		/// </summary>
		public void Play(int index)
		{
			if (index < 0 || index > m_tags.Count)
			{
				KLStartup.Logger.LogError("Invalid index!");
				return;
			}

			Play(m_tags[index]);
		}

		/// <summary>
		/// Play clip with tag
		/// </summary>
		public void Play(string tagName)
		{
			if (!enabled) return;

			KLCenter.Instance.Play(tagName, InstanceID);
		}

		/// <summary>
		/// Stop KLAudioSource
		/// </summary>
		public void Stop()
		{
			KLCenter.Instance.Stop(this);
		}

		/// <summary>
		/// Stop specific tag
		/// </summary>
		public void StopTag(string tagname)
		{
			KLCenter.Instance.Stop(tagname, InstanceID);
		}

		/// <summary>
		/// Returns true if any tag is playing
		/// </summary>
		public bool IsPlaying()
		{
			foreach (var tag in m_tags)
			{
				if (IsPlaying(tag)) return true;
			}

			return false;
		}

		public bool IsPlaying(int index)
		{
			return IsPlaying(m_tags[index]);
		}

		/// <summary>
		/// Returns true if tag is playing
		/// </summary>
		public bool IsPlaying(string tagName)
		{
#if UNITY_EDITOR
			if (Application.isPlaying)
				return KLCenter.Instance.IsPlaying(tagName, InstanceID);
			else return false;
#else
			return KLCenter.Instance.IsPlaying(tagName, InstanceID);
#endif
		}

		/// <summary>
		/// Get a local param from the KLAudioSource instance
		/// </summary>
		public int GetIntVar(string parameter)
		{
			return KLCenter.Instance.GetIntVar(parameter, InstanceID);
		}

		/// <summary>
		/// Get a local param from the KLAudioSource instance
		/// </summary>
		public float GetFloatVar(string parameter)
		{
			return KLCenter.Instance.GetFloatVar(parameter, InstanceID);
		}

		/// <summary>
		/// Get a local param from the KLAudioSource instance
		/// </summary>
		public bool GetBoolVar(string parameter)
		{
			return KLCenter.Instance.GetBoolVar(parameter, InstanceID);
		}

		/// <summary>
		/// Set a local param in the KLAudioSource instance
		/// </summary>
		public void SetIntVar(string parameter, int value)
		{
			KLCenter.Instance.SetIntVar(parameter, value, InstanceID);
		}

		/// <summary>
		/// Set a local param in the KLAudioSource instance
		/// </summary>
		public void SetFloatVar(string parameter, float value)
		{
			KLCenter.Instance.SetFloatVar(parameter, value, InstanceID);
		}

		/// <summary>
		/// Set a local param in the KLAudioSource instance
		/// </summary>
		public void SetBoolVar(string parameter, bool value)
		{
			KLCenter.Instance.SetBoolVar(parameter, value, InstanceID);
		}

		/// <summary>
		/// Set the channel volume
		/// </summary>
		[Obsolete("Please use KLCenter.SetChannelVolume instead", false)]
		public void SetChannelVolume(string channelName, float value)
		{
			KLCenter.SetChannelVolume(channelName, value);
		}
		
		/// <summary>
		/// Get the channel volume
		/// </summary>
		[Obsolete("Please use KLCenter.GetChannelVolume instead", false)]
		public float GetChannelVolume(string channelName)
		{
			return KLCenter.GetChannelVolume(channelName);
		}
		
		#endregion Public API

		#region Helpers

		private void InitSource()
		{
			KLCenter.Instance.LoadTag(m_tags.ToArray(), InstanceID);
		}

		private void DeinitSource()
		{
			KLCenter.Instance.UnloadObject(InstanceID);
		}

		/// <summary>
		/// Updates instance position in Krilloud engine
		/// </summary>
		private void UpdateObjectPosition(bool force = false)
		{
			UpdateObjectPositionInternal(force, GetSourcePosition(), m_velocity);
		}

		private void UpdateObjectPositionInternal(bool force, Vector3 position, Vector3 velocity)
		{
			velocity = Vector3.zero;

			if (force || m_lastUpdatedPos != position || m_lastUpdatedVel != velocity)
			{
				KLCenter.Instance.SetObjectPosition(InstanceID, position, velocity);

				m_lastUpdatedPos = position;
				m_lastUpdatedVel = velocity;
			}
		}

		/// <summary>
		/// Calculate KLAudioSource position using current KLListener
		/// </summary>
		public Vector3 GetSourcePosition()
		{
			if (ReferenceEquals(KLListener.Current, null)) return CachedTransform.position;

			switch (sourceShape)
			{
				case SourceType.Sphere:
					return KLMath.NearPointToSphere(CachedTransform.position, sphereShapeRadius, KLListener.Current.CachedTransform.position);

				case SourceType.Line:
					return KLMath.ProjectPointOnLineSegment(
						CachedTransform.TransformPoint(lineShapeStart),
						CachedTransform.TransformPoint(lineShapeEnd),
						KLListener.Current.CachedTransform.position
					);

				case SourceType.Area:
					// TODO: Move this to KLMath
					Vector3 half = areaShapeSize / 2f;
					Vector3 d = KLListener.Current.CachedTransform.position - CachedTransform.position;
					Vector3 ld = CachedTransform.InverseTransformDirection(d);
					ld = new Vector3(
						Mathf.Clamp(ld.x, -half.x, half.x),
						Mathf.Clamp(ld.y, -half.y, half.y),
						Mathf.Clamp(ld.z, -half.z, half.z)
					);

					return CachedTransform.position + CachedTransform.TransformDirection(ld);

				default:
					return CachedTransform.position;
			}
		}

		#endregion Helpers

		#region Nested

		public enum SourceType
		{
			Point,
			Sphere,
			Line,
			Area
		}

		#endregion Nested
	}
}