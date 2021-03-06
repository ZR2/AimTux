#include "noflash.h"

bool Settings::Noflash::enabled = true;
float Settings::Noflash::value = 160.0f;

void Noflash::FrameStageNotify(ClientFrameStage_t stage)
{
	if (!engine->IsInGame())
		return;

	if (stage != ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		return;

	C_BasePlayer* localPlayer = (C_BasePlayer*)entitylist->GetClientEntity(engine->GetLocalPlayer());
	if (!localPlayer)
		return;

	if (Settings::Noflash::enabled)
		*localPlayer->GetFlashMaxAlpha() = 255.0f - Settings::Noflash::value;
	else
		*localPlayer->GetFlashMaxAlpha() = 255.0f;
}