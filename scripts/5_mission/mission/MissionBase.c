// This class is used to handle communication between the client & server for sending data relating to this mod
modded class MissionBase extends MissionBaseWorld
{
	// Called whenever the "mission" loads
	void MissionBase()
	{
		if (GetGame().IsDedicatedServer()) // Register server->client RPCs
		{
			GetRPCManager().AddRPC("ZenGS_RPC", "RPC_SendGunMoveSoundToServer", this, SingeplayerExecutionType.Server);
		}
	}

	// This is used by the client to request the server play a gun move sound
	void RPC_SendGunMoveSoundToServer(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
	{
		if (type == CallType.Server && GetGame().IsDedicatedServer())
		{
			Param2 <PlayerBase, int> data;
			if (!ctx.Read(data))
			{
				Print("[ZenGunMoveSounds] Error receiving data from player");
				return;
			}

			data.param1.ZenGunMove_SoundSync(data.param2);
		}
	}
}