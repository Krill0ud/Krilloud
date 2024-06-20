using KrillAudio.Krilloud.Definitions;
using UnityEditor;

namespace KrillAudio.Krilloud.Editor.Drawers
{
	[CustomPropertyDrawer(typeof(KLTagAttribute))]
	public sealed class KLTagDrawer : KLMemberBaseDrawer
	{
		protected override string MemberName => "KLTag";
		protected override string[] MemberStringArray => KLEditorCore.AvailableTagsString;
		protected override IKLMemberDefinition GetMemberByIndex(int index)
		{
			return KLEditorCore.AvailableTags[index];
		}
	}
}