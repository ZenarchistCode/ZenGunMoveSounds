class ZenGunMoveSoundsConfig
{
	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenGunMoveSoundsConfig.json";

	// Main config data
	bool PlayOtherItemSounds = true;

	// Load config file or create default file if config doesn't exsit
	void Load()
	{
		if (GetGame().IsServer())
		{
			if (FileExist(zenModFolder + zenConfigName))
			{ // If config exists, load file
				JsonFileLoader<ZenGunMoveSoundsConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);
			}
			else // Config file does not exist, create default file
			{
				// Save config
				Save();
			}
		}
	}

	// Save config
	void Save()
	{
		if (!FileExist(zenModFolder))
		{ // If config folder doesn't exist, create it.
			MakeDirectory(zenModFolder);
		}

		// Save JSON config
		JsonFileLoader<ZenGunMoveSoundsConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}
}

// Save config data
ref ZenGunMoveSoundsConfig m_ZenGunMoveSoundsConfig;

// Helper function to return Config data storage object
static ZenGunMoveSoundsConfig GetZenGunMoveSoundsConfig()
{
	if (!m_ZenGunMoveSoundsConfig)
	{
		Print("[ZenGunMoveSoundsConfig] Init");
		m_ZenGunMoveSoundsConfig = new ZenGunMoveSoundsConfig;
		m_ZenGunMoveSoundsConfig.Load();
	}

	return m_ZenGunMoveSoundsConfig;
};