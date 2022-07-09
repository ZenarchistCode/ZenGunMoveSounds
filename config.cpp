/* 
This mod fixes a vanilla issue when moving guns around.

In vanilla if you drag a gun from your inventory onto the ground,
or from the ground into your hands, no sound is made.

Players often abuse this to silently swap guns. This mod fixes that issue.

Hopefully the DayZ devs fix this in the future but until now this is one solution.

The code for this mod is partially inspired by this mod:
AimSounds: https://steamcommunity.com/sharedfiles/filedetails/?id=2550932214

Combining AimSounds with ZenGunMoveSounds makes for a more hardcore and immersive gunplay experience.
*/
class CfgPatches
{
	class ZenGunMoveSounds
	{
		requiredVersion = 0.1;
		units[] = {};
		requiredAddons[] =
		{
			"DZ_Data"
		};
	};
};

class CfgMods
{
	class ZenGunMoveSounds
	{
		dir = "ZenGunMoveSounds";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "ZenGunMoveSounds";
		credits = "";
		author = "Zenarchist";
		authorID = "0";
		version = "1.0";
		extra = 0;
		type = "mod";
		dependencies[] = { "Game","World","Mission" };
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = { "ZenGunMoveSounds/scripts/3_game"; };
			};
			class worldScriptModule
			{
				value = "";
				files[] = { "ZenGunMoveSounds/scripts/4_world" };
			};
			class missionScriptModule
			{
				value = "";
				files[] = { "ZenGunMoveSounds/scripts/5_mission" };
			};
		};
	};
};