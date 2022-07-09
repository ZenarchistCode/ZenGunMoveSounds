modded class PlayerBase
{
	// Declare network sync'd variables
	protected bool m_GunMoveSoundSyncServer = false;
	protected bool m_GunMoveSoundSyncClient = false;
	protected int m_GunMoveSoundType = -1;

	// (Client-side) This flag is used to stop attachment sounds playing when the player logs in or loads
	protected bool m_PlayerSoundsReady = false;

	// Initialize player character object
	override void Init()
	{
		super.Init();

		// Register network sync'd variables
		RegisterNetSyncVariableBool("m_GunMoveSoundSyncServer");
		RegisterNetSyncVariableInt("m_GunMoveSoundType");
	}

	// Called when the player loads
	override void OnPlayerLoaded()
	{
		super.OnPlayerLoaded();

		if (GetGame().IsClient())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetZenGunSoundsInitialized, 2500, false);
		}
	}

	// (Client-side) Delayed method to set this player object to "ready" to play movement sounds (stops the client playing sounds on login as items are attached to player etc)
	protected void SetZenGunSoundsInitialized()
	{
		m_PlayerSoundsReady = true;
	}

	// (Client-side) Returns true if the player is ready to start playing sounds
	bool IsPlayerZenGunSoundsInitialized()
	{
		return m_PlayerSoundsReady;
	}

	// Called whenever network variables are sync'd from server -> client
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		//ZGS_DebugMessage("Variables sync'd - gundrop=" + m_GunMoveSoundSyncServer + " type=" + m_GunMoveSoundType);
		if (ZenGunMove_ShouldPlaySound())
		{
			m_GunMoveSoundSyncClient = m_GunMoveSoundSyncServer;
			PlayZenGunMoveSound();
		}
	}

	// (Client-side) Plays the relevant soundset type for the given movement type/ID
	protected void PlayZenGunMoveSound()
	{
		// If this code is being executed on the server then stop here - we can't play sounds on the server, only client-side.
		if (GetGame().IsServer() && GetGame().IsMultiplayer()) return;

		string sound_set;
		switch (m_GunMoveSoundType) // TODO: Add more sounds?
		{
			case 0: // Drop unknown item to ground
			case 1: // Take unknown item to hand
				sound_set = "putItemInventory_SoundSet";
				break;
			case 2: // Put pistol in hand
				sound_set = "pickUpPistol_SoundSet";
				break; 
			case 3: // Put rifle in hand
				sound_set = "ShoulderL_Hide_Rifle_SoundSet";//"pickUpRifleLight_SoundSet";
				break;
			case 4: // Drop pistol on ground
				sound_set = "pistol_drop_SoundSet";
				break;
			case 5: // Drop rifle on ground from shoulder or bag attachment
				sound_set = "ShoulderL_Hide_Rifle_SoundSet";
				break;
			case 6: // Drop rifle on ground from anywhere else (ie. hands/inventory/bag)
				sound_set = "rifle_drop_SoundSet";
				break;
			case 7: // Moved rifle to shoulder slot
				sound_set = "ShoulderL_Hide_Rifle_SoundSet";
				break;
			case 8: // Drop barrel
				sound_set = "placeBarrel_SoundSet";
				break;
			default: // Default soundset (this should never happen - all types are accounted for)
				sound_set = "putItemInventory_SoundSet";
		}

		// Play soundset
		EffectSound sound;
		PlaySoundSet(sound, sound_set, 0, 0);
		sound.SetSoundAutodestroy(true);
	}

	// Called server side. Flips the net sync flag to force the client to play the given GunDropSoundType.
	void ZenGunMove_SoundSync(int type)
	{
		if (!GetZenGunMoveSoundsConfig().PlayOtherItemSounds && (type == 0 || type == 1)) // Don't send general item sounds if they're disabled
			return;

		m_GunMoveSoundSyncServer = !m_GunMoveSoundSyncServer;
		m_GunMoveSoundType = type;
		SetSynchDirty();
	}

	// Called client-side
	bool ZenGunMove_ShouldPlaySound()
	{
		return m_GunMoveSoundSyncClient != m_GunMoveSoundSyncServer && m_PlayerSoundsReady;
	}

	// (Client-side) Sends an RPC to the server to play a gun move sound
	void ZenGunMove_SendSoundRequestToServer(int type)
	{
		GetRPCManager().SendRPC("ZenGS_RPC", "RPC_SendGunMoveSoundToServer", new Param2<PlayerBase, int>(this, type), true, NULL);
	}

	// Print a debug message (client-only)
	/*void ZGS_DebugMessage(string message)
	{
		if (GetGame().GetPlayer())
		{
			GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "[ZenGunMoveSounds:" + GetIdentity().GetName() + "] " + message, ""));
		}
	}

	// Sends a text message to the client from the server (for debug purposes)
	void ZGS_SendMessage(string message)
	{
		Param1<string> m_MessageParam = new Param1<string>("");
		if (GetGame().IsServer() && m_MessageParam && IsAlive() && message != "" && !IsPlayerDisconnected())
		{
			m_MessageParam.param1 = message;
			GetGame().RPCSingleParam(this, ERPCs.RPC_USER_ACTION_MESSAGE, m_MessageParam, true, GetIdentity());
		}
	}*/
};