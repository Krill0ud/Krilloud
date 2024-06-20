Krilloud - Unity Plugin

The goal of the plugin is to make it easy to integrate the Krilloud engine into Unity

Unity components

*  KLListener: Direct replacement for Unity built in AudioListener component. The component communicates the current position, rotation and velocity of the listener to the audio engine. Note: This component must exist in the scene.
*  KLAudioSource: Direct replacement for Unity built-in AudioSource component. The component communicates the current position, rotation and velocity of the source to the audio engine. It also handles the tag lifetime. Public API: Play(), Play(tagIndex), Play(tagName), Stop(), IsPlaying(), IsPlaying(tagIndex), IsPlaying(tagName), GetIntVar(), GetFloatVar(), GetBoolVar(), SetIntVar(value), SetFloatVar(value), SetBoolVar(value), SetChannelVolume(channelId, volume)


To help developers create a seamless workflow with Unity Editor, this plugin provides a set of tools to make it easier when using Krilloud:

*  Tag Explorer: Custom window to show the developer the available tags or variables in the current project. The color of the tag/variable uses the color to show the origin:

*  Placeholder Contract: It allows adding tags and variables to the scene to then be controlled by Krilloud. The programmer can simulate all the necessary variables and tags for the project without having to wait for the audio designer to finish work.
*  Code generator: It uses the information about contract, placeholder contract and an internal cache to generate codes with the information about tags and variables. It also includes comments that can add more information for developers.
*  Custom Attributes (KLTag, KLVariable): Only for the Unity version.To make it easier to work with tags and variables, the plugin has two custom attributes that can be used in C# code: KLTagAttribute: can be used with string type variables, in the inspector this will be shown as a dropdown with the tag names of the current contract, placeholder contract or cached data. KLVariableAttribute: can be used with string type variables, in Unity inspector this will be shown as a dropdown with the variable names of the current contract, placeholder contract or cached data.
*  



