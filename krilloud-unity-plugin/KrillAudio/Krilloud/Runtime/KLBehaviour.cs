using UnityEngine;

namespace KrillAudio.Krilloud
{
	public abstract class KLBehaviour : MonoBehaviour
	{
		private bool transformWasCached;

		private Transform m_cachedTransform;
		public Transform CachedTransform
		{
			get
			{
				if (!transformWasCached)
				{
					m_cachedTransform = transform;
					transformWasCached = true;
				}

				return m_cachedTransform;
			}
		}
	}
}