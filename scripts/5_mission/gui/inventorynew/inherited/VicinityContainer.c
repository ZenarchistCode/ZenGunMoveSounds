modded class VicinityContainer
{
	// Called whenever the GUI detects an item being dropped in the left-side vicinity widget
	override void OnDropReceivedFromHeader(Widget w, int x, int y, Widget receiver)
	{
		super.OnDropReceivedFromHeader(w, x, y, receiver);

		ItemPreviewWidget ipw = ItemPreviewWidget.Cast(w.FindAnyWidget("Render"));

		if (!ipw)
		{
			string name = w.GetName();
			name.Replace("PanelWidget", "Render");
			ipw = ItemPreviewWidget.Cast(w.FindAnyWidget(name));
		}

		if (!ipw)
		{
			ipw = ItemPreviewWidget.Cast(w);
		}
		if (!ipw)
			return;

		EntityAI item = ipw.GetItem();
		if (!ipw.IsInherited(ItemPreviewWidget) || !item)
		{
			return;
		}

		InventoryLocation il = new InventoryLocation;

		if (!item.GetInventory())
			return;

		item.GetInventory().GetCurrentInventoryLocation(il);
		bool isFromInventory = il && (il.GetType() == InventoryLocationType.CARGO || il.GetType() == InventoryLocationType.HANDS || il.GetType() == InventoryLocationType.ATTACHMENT);

		if (!isFromInventory) // If item location is not from our inventory OR it is a heavy item, don't play the drop sound as it sounds weird
			return;

		// If all previous checks have passed, then we have a valid item we just dropped on the ground. Check what its type is and send the relevant soundset type to nearby clients.
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player)
		{
			//player.ZGS_DebugMessage("Vicinity dropped item - type=" + item.GetType() + " fromInventory=" + isFromInventory + " isheavy=" + item.IsHeavyBehaviour());
			
			int type = 0;
			Weapon_Base weapon = Weapon_Base.Cast(item);
			if (weapon)
			{
				if (weapon.IsInherited(Pistol_Base) || weapon.IsInherited(Magnum_Base))
				{
					type = 4;
				}
				else
				if (weapon.IsInherited(Rifle_Base) || weapon.IsInherited(BoltActionRifle_InnerMagazine_Base) || weapon.IsInherited(BoltActionRifle_Base) || weapon.IsInherited(DoubleBarrel_Base))
				{
					if (il.GetType() == InventoryLocationType.ATTACHMENT)
						type = 5;
					else
						type = 6;
				}
			}
			else
			if (item.IsHeavyBehaviour())
			{
				if (item.GetType().Contains("Barrel_"))
				{
					type = 8;
				}
				else
				{
					// Don't play a standard drop sound for unknown heavy items as it sounds weird usually.
					// TODO: Add other heavy item drop soundsets? Kinda outside the scope of this mod. Core idea is just to stop players swapping guns silently.
					return;
				}
			}

			player.ZenGunMove_SendSoundRequestToServer(type);
		}
	}
}