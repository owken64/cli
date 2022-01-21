#pragma once
#include "Const.h"

bool InitSound();
void ReleaseSound();
bool LoadSoundFile(SoundFile file_id, const char* file_name);
void PlaySoundFile(SoundFile file_id, bool is_loop);
void StopSoundFile(SoundFile file_id);
