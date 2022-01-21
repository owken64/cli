#include <dsound.h>
#include <stdio.h>
#include <Windows.h>
#include "Parameter.h"
#include "Const.h"

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
// mmio�ŕK�v
#pragma comment(lib, "winmm.lib")

struct WavData
{
	WAVEFORMATEX WavFormat;
	char* SoundBuffer;
	DWORD Size;
};

bool LoadWavFile(const char* file_name, WavData* out_wave_data);


bool InitSound()
{
	// DirectSound�̐���
	if (FAILED(DirectSoundCreate8(
		NULL,				// GUID
		&g_SoundInterface,	// ��������DirectSound�ۑ���
		NULL)))				// NULL�Œ�
	{
		return false;
	}

	// �������x���̐ݒ�
	if (FAILED(g_SoundInterface->SetCooperativeLevel(
		FindWindow(WINDOW_CLASS_NAME, nullptr),		// �E�B���h�E�n���h��
		DSSCL_NORMAL)))								// �������x��
	{
		return false;
	}

	for (int i = 0; i < SoundFile::SoundFileMax; i++)
	{
		g_SoundBufferList[i] = NULL;
		g_flgSEPlay[i] = false;
	}

	return true;
}

void ReleaseSound()
{
	// �Z�J���_���o�b�t�@�̉��
	for (int i = 0; i < SoundFile::SoundFileMax; i++)
	{
		if (g_SoundBufferList[i] != NULL)
		{
			g_SoundBufferList[i]->Stop();
			g_SoundBufferList[i]->Release();
			g_SoundBufferList[i] = NULL;
		}
	}

	// DirectSound�C���^�[�t�F�[�X�̉��
	if (g_SoundInterface != NULL)
	{
		g_SoundInterface->Release();
		g_SoundInterface = NULL;
	}
}

bool LoadSoundFile(SoundFile file_id, const char* file_name)
{
	WavData wav_data;

	if (LoadWavFile(file_name, &wav_data) == false)
	{
		return false;
	}

	// �o�b�t�@���̐ݒ�
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = wav_data.Size;
	dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
	dsbd.lpwfxFormat = &wav_data.WavFormat;

	// �Z�J���_���o�b�t�@�쐬
	if (FAILED(g_SoundInterface->CreateSoundBuffer(
		&dsbd,							// �o�b�t�@���
		&g_SoundBufferList[file_id],	// �쐬���ꂽ�o�b�t�@�̕ۑ���
		NULL)))
	{
		// �쐬���s
		delete[] wav_data.SoundBuffer;
		return false;
	}

	// �g�`�f�[�^���������ނ��߂ɃZ�J���_���o�b�t�@�����b�N����
	void* buffer;
	DWORD buffer_size;
	if (FAILED(g_SoundBufferList[file_id]->Lock(
		0,				// �I�t�Z�b�g
		wav_data.Size,	// ���b�N����o�b�t�@�T�C�Y
		&buffer,		// ���b�N���ꂽ�o�b�t�@�p�[�g�P�̕ۑ���
		&buffer_size,	// ���b�N���ꂽ�o�b�t�@�p�[�g�P�̃T�C�Y
		NULL,			// ���b�N���ꂽ�o�b�t�@�p�[�g�Q�̕ۑ���
		NULL,			// ���b�N���ꂽ�o�b�t�@�p�[�g�Q�̃T�C�Y
		0)))			// ���b�N�I�v�V����
	{
		// ���b�N���s
		delete[] wav_data.SoundBuffer;
		return false;
	}

	memcpy(buffer, wav_data.SoundBuffer, buffer_size);

	g_SoundBufferList[file_id]->Unlock(
		&buffer,		// �A�����b�N����o�b�t�@�p�[�g�P
		buffer_size,	// �p�[�g�P�̃o�b�t�@�T�C�Y
		NULL,			// �A�����b�N����o�b�t�@�p�[�g�Q
		NULL);			// �p�[�g�Q�̃o�b�t�@�T�C�Y

	// �R�s�[���I������̂ŃT�E���h�f�[�^�����
	delete[] wav_data.SoundBuffer;

	return true;
}

void PlaySoundFile(SoundFile file_id, bool is_loop)
{
	if (g_SoundBufferList[file_id] == NULL)
	{
		return;
	}

	int loop_bit = is_loop == true ? 1 : 0;
	// �Đ�
	g_SoundBufferList[file_id]->Play(
		0,
		0,
		DSBPLAY_LOOPING & loop_bit);
}

void StopSoundFile(SoundFile file_id)
{
	if (g_SoundBufferList[file_id] == NULL)
	{
		return;
	}

	// ��~
	g_SoundBufferList[file_id]->Stop();
	// �Đ��ʒu��擪�ɖ߂�
	g_SoundBufferList[file_id]->SetCurrentPosition(NULL);
}

bool LoadWavFile(const char* file_name, WavData* out_wave_data)
{
	// Windows�}���`���f�B�AAPI�̃n���h��
	HMMIO mmio_handle = NULL;

	// �`�����N���
	MMCKINFO ck_info;
	// RIFF�`�����N�p
	MMCKINFO riffck_info;

	// WAV�t�@�C�������T���v���̃T�C�Y
	DWORD dwWavSize = 0;

	// file_name��WCHAR�ɕϊ�
	WCHAR w_file_name[256] = L"";
	MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, file_name, strlen(file_name), w_file_name, (sizeof w_file_name) / 2);

	// WAV�t�@�C�����J��
	mmio_handle = mmioOpen(
		w_file_name,	// �t�@�C����
		NULL,				// MMIO���
		MMIO_READ);			// �I�[�v�����[�h

	if (mmio_handle == NULL)
	{
		// �I�[�v�����s
		return false;
	}

	// RIFF�`�����N�ɐi�����邽�߂�fccType��WAVE��ݒ肷��
	riffck_info.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	// RIFF�`�����N�ɐN������
	if (MMSYSERR_NOERROR != mmioDescend(
		mmio_handle,	// MMIO�n���h��
		&riffck_info,	// �擾�����`�����N�̏��
		NULL,			// �e�`�����N
		MMIO_FINDRIFF))	// �擾���̎��
	{
		// ���s
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// �i����̃`�����N��"fmt "�Ƃ��Đݒ肷��
	ck_info.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (MMSYSERR_NOERROR != mmioDescend(mmio_handle, &ck_info, &riffck_info, MMIO_FINDCHUNK))
	{
		// fmt�`�����N���Ȃ�
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// fmt�f�[�^�̓ǂݍ���
	LONG read_size = mmioRead(
		mmio_handle,						// �n���h��
		(HPSTR)&out_wave_data->WavFormat,	// �ǂݍ��ݗp�o�b�t�@
		sizeof(out_wave_data->WavFormat));	// �o�b�t�@�T�C�Y

	if (read_size != sizeof(out_wave_data->WavFormat))
	{
		// �ǂݍ��݃T�C�Y����v���ĂȂ��̂ŃG���[
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// �t�H�[�}�b�g�`�F�b�N
	if (out_wave_data->WavFormat.wFormatTag != WAVE_FORMAT_PCM)
	{
		// �t�H�[�}�b�g�G���[
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// fmt�`�����N��ޏo����
	if (mmioAscend(mmio_handle, &ck_info, 0) != MMSYSERR_NOERROR)
	{
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// data�`�����N�ɐi������
	ck_info.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(mmio_handle, &ck_info, &riffck_info, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
	{
		// �i�����s
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// �T�C�Y��ۑ�
	out_wave_data->Size = ck_info.cksize;

	// data�`�����N�ǂݍ���
	out_wave_data->SoundBuffer = new char[ck_info.cksize];
	read_size = mmioRead(mmio_handle, (HPSTR)out_wave_data->SoundBuffer, ck_info.cksize);
	if (read_size != ck_info.cksize)
	{
		mmioClose(mmio_handle, MMIO_FHOPEN);
		delete[] out_wave_data->SoundBuffer;
		return false;
	}

	// �t�@�C�������
	mmioClose(mmio_handle, MMIO_FHOPEN);

	return true;
}
