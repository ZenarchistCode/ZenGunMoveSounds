modded class VicinitySlotsContainer
{
	// Handles double-clicks on vicinity items
	override void DoubleClick(Widget w, int x, int y, int button)
	{
		super.DoubleClick(w, x, y, button);

		if (button == MouseState.LEFT)
		{
			if (w == null)
			{
				return;
			}

			ItemPreviewWidget iw = ItemPreviewWidget.Cast(w.FindAnyWidget("Render"));
			if (!iw)
			{
				string name = w.GetName();
				name.Replace("PanelWidget", "Render");
				iw = ItemPreviewWidget.Cast(w.FindAnyWidget(name));
			}

			if (!iw)
			{
				iw = ItemPreviewWidget.Cast(w);
			}

			ItemBase item = ItemBase.Cast(iw.GetItem());
			if (!item)
			{
				return;
			}

			if (!item.GetInventory().CanRemoveEntity())
				return;

			InventoryLocation il = new InventoryLocation;

			if (!item.GetInventory())
				return;

			item.GetInventory().GetCurrentInventoryLocation(il);
			item.SetItemWasDoubleClicked(true);
			bool fromGround = il && il.GetType() == InventoryLocationType.GROUND;

			if (!fromGround)
				return;

			PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
			if (player)
			{
				//player.ZGS_DebugMessage("Double-clicked item into hands - " + item.GetType()); //+ " - fromGround=" + fromGround);

				int type = 1;
				Weapon_Base weapon = Weapon_Base.Cast(item);
				if (weapon)
				{
					if (weapon.IsInherited(Pistol_Base) || weapon.IsInherited(Magnum_Base))
					{
						type = 2;
					}
					else
					if (weapon.IsInherited(Rifle_Base) || weapon.IsInherited(BoltActionRifle_InnerMagazine_Base) || weapon.IsInherited(BoltActionRifle_Base) || weapon.IsInherited(DoubleBarrel_Base))
					{
						type = 3;
					}
				}
				else
				{
					if (item.IsHeavyBehaviour())
					{
						return; // Don't play double-click sounds for heavy items as sounds weird with some items.
					}
				}

				player.ZenGunMove_SendSoundRequestToServer(type);
			}
		}
	}
}