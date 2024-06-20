namespace KrillAudio.Krilloud.Definitions
{
	public enum SourceContract
	{
		Krilloud,
		Placeholder,
		Cache
	}
	
	public interface IKLMemberDefinition
	{
		string Name { get; }
		SourceContract SourceContract { get; }
	}
}