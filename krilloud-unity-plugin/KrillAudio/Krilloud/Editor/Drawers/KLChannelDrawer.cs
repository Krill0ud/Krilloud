using KrillAudio.Krilloud.Definitions;
using UnityEditor;

namespace KrillAudio.Krilloud.Editor.Drawers
{
	[CustomPropertyDrawer(typeof(KLChannelAttribute))]
	public class KLChannelDrawer : KLMemberBaseDrawer
	{
		protected override string MemberName => "KLChannel";
		protected override string[] MemberStringArray => KLEditorCore.AvailableChannelsString;

		protected override IKLMemberDefinition GetMemberByIndex(int index)
		{
			return KLEditorCore.AvailableChannels[index];
		}
	}
}