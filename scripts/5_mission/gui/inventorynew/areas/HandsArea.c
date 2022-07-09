modded class HandsArea
{
	override void OnHandsPanelDropReceived(Widget w, int x, int y, Widget receiver)
	{
		super.OnHandsPanelDropReceived(w, x, y, receiver);

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

		if (!ipw || !ipw.GetItem() || !ipw.IsInherited(ItemPreviewWidget))
		{
			return;
		}

		if (!ipw.GetItem().GetInventory().CanRemoveEntity())
			return;

		EntityAI item = ipw.GetItem();
		InventoryLocation il = new InventoryLocation;
		item.GetInventory().GetCurrentInventoryLocation(il);
		bool fromGround = il && il.GetType() == InventoryLocationType.GROUND;

		if (!fromGround || item.IsHeavyBehaviour()) // Don't play sound for items moved from cargo/inventory OR if item is a heavy item that needs picking up
			return;

		// If all previous checks have passed, then we have a valid item we just took from the ground. Check what its type is and send the relevant soundset type to nearby clients.
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player)
		{
			//player.ZGS_DebugMessage("Dropped item into hands - type=" + ipw.GetItem().GetType() + " fromGround=" + fromGround + " isheavy=" + item.IsHeavyBehaviour());

			int type = 1;
			Weapon_Base weapon = Weapon_Base.Cast(item);
			if (weapon)
			{
				if (weapon.IsInherited(Pistol_Base) || weapon.IsInherited(Magnum_Base))
					type = 2;
				else
				if (weapon.IsInherited(Rifle_Base) || weapon.IsInherited(BoltActionRifle_InnerMagazine_Base) || weapon.IsInherited(BoltActionRifle_Base) || weapon.IsInherited(DoubleBarrel_Base) || weapon.IsInherited(BoltRifle_Base))
					type = 3;
			}

			player.ZenGunMove_SendSoundRequestToServer(type);
		}
	}
}