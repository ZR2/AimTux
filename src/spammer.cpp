#include "spammer.h"

bool Settings::Spammer::NormalSpammer::enabled = false;
bool Settings::Spammer::KillSpammer::enabled = false;

std::vector<Spammer::SpamCollection> Spammer::collections =
{
	Spammer::SpamCollection("AimTux",
		{
			"AimTux owns me and all",
			"Your Windows p2c sucks my AimTux dry",
			"It's free as in FREEDOM!",
			"Tux only let me out so I could play this game, please be nice!",
			"Tux nutted but you keep sucken",
			">tfw no vac on Linux"
		}, 0)
};

Spammer::SpamCollection* Spammer::currentSpamCollection = &collections[0];

void Spammer::CreateMove(CUserCmd* cmd)
{
	if (!Settings::Spammer::NormalSpammer::enabled)
		return;

	// Give the random number generator a new seed based of the current time
	std::srand(std::time(NULL));

	// Grab the current time in milliseconds
	long currentTime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()).count();
	static long timeStamp = currentTime_ms;

	if (currentTime_ms - timeStamp < (1000 + currentSpamCollection->delay))
		return;

	// Grab a random message string
	std::string message = currentSpamCollection->messages[std::rand() % currentSpamCollection->messages.size()];

	// Construct a command with our message
	pstring str;
	str << "say " << message;

	// Execute our constructed command
	engine->ExecuteClientCmd(str.c_str());

	// Update the time stamp
	timeStamp = currentTime_ms;
}


void Spammer::FireEventClientSide(IGameEvent* event)
{
	if (!Settings::Spammer::KillSpammer::enabled)
		return;

	if (!engine->IsInGame())
		return;

	if (std::strcmp(event->GetName(), "player_death") != 0)
		return;

	int attacker_id = engine->GetPlayerForUserID(event->GetInt("attacker"));
	int deadPlayer_id = engine->GetPlayerForUserID(event->GetInt("userid"));

	// Make sure both IDs are valid
	if (!attacker_id || !deadPlayer_id)
		return;

	// Make sure it's not a suicide.
	if (attacker_id == deadPlayer_id)
		return;

	// Make sure we're the one who killed someone...
	if (attacker_id != engine->GetLocalPlayer())
		return;

	// Get the attackers information
	IEngineClient::player_info_t attacker_info;
	engine->GetPlayerInfo(attacker_id, &attacker_info);

	// Get the dead players information
	IEngineClient::player_info_t deadPlayer_info;
	engine->GetPlayerInfo(deadPlayer_id, &deadPlayer_info);

	// Prepare dead player's nickname without ';' & '"' characters
	// as they might cause user to execute a command.
	std::string dead_player_name = std::string(deadPlayer_info.name);
	dead_player_name.erase(std::remove(dead_player_name.begin(), dead_player_name.end(), ';'), dead_player_name.end());
	dead_player_name.erase(std::remove(dead_player_name.begin(), dead_player_name.end(), '"'), dead_player_name.end());

	// Construct a command with our message
	pstring str;
	str << "say \"" << dead_player_name << " just got OWNED by AimTux!\"";

	// Execute our constructed command
	engine->ExecuteClientCmd(str.c_str());
}

