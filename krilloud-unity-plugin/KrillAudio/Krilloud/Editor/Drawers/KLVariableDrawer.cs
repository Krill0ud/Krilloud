using KrillAudio.Krilloud.Definitions;
using UnityEditor;

namespace KrillAudio.Krilloud.Editor.Drawers
{
	[CustomPropertyDrawer(typeof(KLVariableAttribute))]
	public class KLVariableDrawer : KLMemberBaseDrawer
	{
		protected override string MemberName => "KLTag";
		protected override string[] MemberStringArray => KLEditorCore.AvailableVariablesString;
		protected override IKLMemberDefinition GetMemberByIndex(int index)
		{
			return KLEditorCore.AvailableVariables[index];
		}
	}
}