// This is an unfortunate "hack fix" - I couldn't think of a better way to achieve this hand-tracking.
modded class ItemBase
{
	// (Client-side) This is used to track if the item recently left our player's hands or was double-clicked to pickup (more info on why I do this in PlayerContainer.c)
	bool m_ItemWasLastInHands = false;
	bool m_WasDoubleClicked = false;

	// This method is called whenever an item changes inventory locations (on both server and client)
	override void EEItemLocationChanged(notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc)
	{
		if (GetGame().IsClient()) // Client-side only
		{
			// If the old location was in our hands, save that info to the item for later reference in PlayerContainer.c
			m_ItemWasLastInHands = oldLoc.GetType() == InventoryLocationType.HANDS;
		}

		super.EEItemLocationChanged(oldLoc, newLoc);
	}

	// (Client-side) Return whether or not this item was last in the player's hands
	bool ItemWasLastInHands()
	{
		return m_ItemWasLastInHands;
	}

	// (Client-side) Return whether or not this item was double-clicked to pickup.
	bool ItemWasLastDoubleClicked()
	{
		return m_WasDoubleClicked;
	}

	// (Client-side) Set whether or not this item was double-clicked to pickup.
	void SetItemWasDoubleClicked(bool b)
	{
		m_WasDoubleClicked = b;
	}
};