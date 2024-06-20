using UnityEngine;

namespace KrillAudio.Krilloud.Utils
{
	public static class KLMath
	{
		public static Vector3 NearPointToSphere(Vector3 sphereCenter, float sphereRadius, Vector3 targetPoint)
		{
			float dist = Vector3.Distance(sphereCenter, targetPoint);
			if (dist < sphereRadius) return targetPoint;

			Vector3 dir = (targetPoint - sphereCenter).normalized;
			return sphereCenter + dir * sphereRadius;
		}

		// This function returns a point which is a projection from a point to a line.
		// The line is regarded infinite. If the line is finite, use ProjectPointOnLineSegment() instead.
		public static Vector3 ProjectPointOnLine(Vector3 linePoint, Vector3 lineVec, Vector3 point)
		{
			// get vector from point on line to point in space
			Vector3 linePointToPoint = point - linePoint;

			float t = Vector3.Dot(linePointToPoint, lineVec);

			return linePoint + lineVec * t;
		}

		// This function returns a point which is a projection from a point to a line segment.
		// If the projected point lies outside of the line segment, the projected point will
		// be clamped to the appropriate line edge.
		// If the line is infinite instead of a segment, use ProjectPointOnLine() instead.
		public static Vector3 ProjectPointOnLineSegment(Vector3 linePoint1, Vector3 linePoint2, Vector3 point)
		{
			Vector3 vector = linePoint2 - linePoint1;

			Vector3 projectedPoint = ProjectPointOnLine(linePoint1, vector.normalized, point);

			int side = PointOnWhichSideOfLineSegment(linePoint1, linePoint2, projectedPoint);

			// The projected point is on the line segment
			if (side == 0)
			{
				return projectedPoint;
			}

			if (side == 1)
			{
				return linePoint1;
			}

			if (side == 2)
			{
				return linePoint2;
			}

			// output is invalid
			return Vector3.zero;
		}

		// This function finds out on which side of a line segment the point is located.
		// The point is assumed to be on a line created by linePoint1 and linePoint2. If the point is not on
		// the line segment, project it on the line using ProjectPointOnLine() first.
		// Returns 0 if point is on the line segment.
		// Returns 1 if point is outside of the line segment and located on the side of linePoint1.
		// Returns 2 if point is outside of the line segment and located on the side of linePoint2.
		public static int PointOnWhichSideOfLineSegment(Vector3 linePoint1, Vector3 linePoint2, Vector3 point)
		{
			Vector3 lineVec = linePoint2 - linePoint1;
			Vector3 pointVec = point - linePoint1;

			float dot = Vector3.Dot(pointVec, lineVec);

			// point is on side of linePoint2, compared to linePoint1
			if (dot > 0)
			{
				// point is on the line segment
				if (pointVec.magnitude <= lineVec.magnitude)
				{
					return 0;
				}

				// point is not on the line segment and it is on the side of linePoint2
				else
				{
					return 2;
				}
			}

			// Point is not on side of linePoint2, compared to linePoint1.
			// Point is not on the line segment and it is on the side of linePoint1.
			else
			{
				return 1;
			}
		}
	}
}