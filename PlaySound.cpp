#include "PlaySound.h"
#include "Sound.h"
#include "Mouse.h"
#include "Button.h"
#include "Const.h"
#include "Parameter.h"

void PlaySound() {
	for (int i = 0; i < SoundFile::SoundFileMax; i++) {
		if (g_flgSEPlay[i]) PlaySoundFile((SoundFile)i, false);
		g_flgSEPlay[i] = false;
	}

	// BGMØ‚è‘Ö‚¦
	if (g_SoundBGM != g_PrevSoundBGM) {
		StopSoundFile(g_PrevSoundBGM);
		PlaySoundFile(g_SoundBGM, true);
	}
}