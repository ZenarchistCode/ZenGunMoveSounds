modded class PlayerContainer
{
	// ItemAttached() is called whenever an item is attached on the player character from dragging an item onto an attachment slot.
	// For some reason, none of the usual methods work here for tracking where the item came from, so check ItemBase.c for more info.
	// When a player moves an item from their hands to inventory it already makes a sound, so we need to track where the moved item came from.
	// If it came from the player's hands then we don't need to play a sound.
	override void ItemAttached(EntityAI item, string slot_name)
	{
		super.ItemAttached(item, slot_name);

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player)
		{
			if (!player.IsPlayerZenGunSoundsInitialized()) // This stops playing sounds a few seconds after login to prevent spamming attachment sounds etc. as items load.
				return;

			ItemBase itemBase = ItemBase.Cast(item);

			// Don't play a sound if the item was picked up by double-clicking as that is already handled in VicinitySlotsContainer.c and will play a duplicate sound.
			if (itemBase.ItemWasLastDoubleClicked())
			{
				itemBase.SetItemWasDoubleClicked(false);
				return;
			}

			bool fromHands = itemBase.ItemWasLastInHands();

			//player.ZGS_DebugMessage("inventoryLoc=" + il.GetType() + " hands=" + fromHands);
			if (fromHands)
				return;

			int type = 1;
			Weapon_Base weapon = Weapon_Base.Cast(item);
			if (weapon)
			{
				if (weapon.IsInherited(Rifle_Base) || weapon.IsInherited(BoltActionRifle_InnerMagazine_Base) || weapon.IsInherited(BoltActionRifle_Base) || weapon.IsInherited(DoubleBarrel_Base))
				{
					type = 7;
				}
			}

			player.ZenGunMove_SendSoundRequestToServer(type);
		}
	}
}