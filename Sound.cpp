#include <dsound.h>
#include <stdio.h>
#include <Windows.h>
#include "Parameter.h"
#include "Const.h"

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
// mmioで必要
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
	// DirectSoundの生成
	if (FAILED(DirectSoundCreate8(
		NULL,				// GUID
		&g_SoundInterface,	// 生成したDirectSound保存先
		NULL)))				// NULL固定
	{
		return false;
	}

	// 協調レベルの設定
	if (FAILED(g_SoundInterface->SetCooperativeLevel(
		FindWindow(WINDOW_CLASS_NAME, nullptr),		// ウィンドウハンドル
		DSSCL_NORMAL)))								// 協調レベル
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
	// セカンダリバッファの解放
	for (int i = 0; i < SoundFile::SoundFileMax; i++)
	{
		if (g_SoundBufferList[i] != NULL)
		{
			g_SoundBufferList[i]->Stop();
			g_SoundBufferList[i]->Release();
			g_SoundBufferList[i] = NULL;
		}
	}

	// DirectSoundインターフェースの解放
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

	// バッファ情報の設定
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = wav_data.Size;
	dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
	dsbd.lpwfxFormat = &wav_data.WavFormat;

	// セカンダリバッファ作成
	if (FAILED(g_SoundInterface->CreateSoundBuffer(
		&dsbd,							// バッファ情報
		&g_SoundBufferList[file_id],	// 作成されたバッファの保存先
		NULL)))
	{
		// 作成失敗
		delete[] wav_data.SoundBuffer;
		return false;
	}

	// 波形データを書き込むためにセカンダリバッファをロックする
	void* buffer;
	DWORD buffer_size;
	if (FAILED(g_SoundBufferList[file_id]->Lock(
		0,				// オフセット
		wav_data.Size,	// ロックするバッファサイズ
		&buffer,		// ロックされたバッファパート１の保存先
		&buffer_size,	// ロックされたバッファパート１のサイズ
		NULL,			// ロックされたバッファパート２の保存先
		NULL,			// ロックされたバッファパート２のサイズ
		0)))			// ロックオプション
	{
		// ロック失敗
		delete[] wav_data.SoundBuffer;
		return false;
	}

	memcpy(buffer, wav_data.SoundBuffer, buffer_size);

	g_SoundBufferList[file_id]->Unlock(
		&buffer,		// アンロックするバッファパート１
		buffer_size,	// パート１のバッファサイズ
		NULL,			// アンロックするバッファパート２
		NULL);			// パート２のバッファサイズ

	// コピーが終わったのでサウンドデータを解放
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
	// 再生
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

	// 停止
	g_SoundBufferList[file_id]->Stop();
	// 再生位置を先頭に戻す
	g_SoundBufferList[file_id]->SetCurrentPosition(NULL);
}

bool LoadWavFile(const char* file_name, WavData* out_wave_data)
{
	// WindowsマルチメディアAPIのハンドル
	HMMIO mmio_handle = NULL;

	// チャンク情報
	MMCKINFO ck_info;
	// RIFFチャンク用
	MMCKINFO riffck_info;

	// WAVファイル内音サンプルのサイズ
	DWORD dwWavSize = 0;

	// file_nameをWCHARに変換
	WCHAR w_file_name[256] = L"";
	MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, file_name, strlen(file_name), w_file_name, (sizeof w_file_name) / 2);

	// WAVファイルを開く
	mmio_handle = mmioOpen(
		w_file_name,	// ファイル名
		NULL,				// MMIO情報
		MMIO_READ);			// オープンモード

	if (mmio_handle == NULL)
	{
		// オープン失敗
		return false;
	}

	// RIFFチャンクに進入するためにfccTypeにWAVEを設定する
	riffck_info.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	// RIFFチャンクに侵入する
	if (MMSYSERR_NOERROR != mmioDescend(
		mmio_handle,	// MMIOハンドル
		&riffck_info,	// 取得したチャンクの情報
		NULL,			// 親チャンク
		MMIO_FINDRIFF))	// 取得情報の種類
	{
		// 失敗
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// 進入先のチャンクを"fmt "として設定する
	ck_info.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (MMSYSERR_NOERROR != mmioDescend(mmio_handle, &ck_info, &riffck_info, MMIO_FINDCHUNK))
	{
		// fmtチャンクがない
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// fmtデータの読み込み
	LONG read_size = mmioRead(
		mmio_handle,						// ハンドル
		(HPSTR)&out_wave_data->WavFormat,	// 読み込み用バッファ
		sizeof(out_wave_data->WavFormat));	// バッファサイズ

	if (read_size != sizeof(out_wave_data->WavFormat))
	{
		// 読み込みサイズが一致してないのでエラー
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// フォーマットチェック
	if (out_wave_data->WavFormat.wFormatTag != WAVE_FORMAT_PCM)
	{
		// フォーマットエラー
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// fmtチャンクを退出する
	if (mmioAscend(mmio_handle, &ck_info, 0) != MMSYSERR_NOERROR)
	{
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// dataチャンクに進入する
	ck_info.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(mmio_handle, &ck_info, &riffck_info, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
	{
		// 進入失敗
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// サイズを保存
	out_wave_data->Size = ck_info.cksize;

	// dataチャンク読み込み
	out_wave_data->SoundBuffer = new char[ck_info.cksize];
	read_size = mmioRead(mmio_handle, (HPSTR)out_wave_data->SoundBuffer, ck_info.cksize);
	if (read_size != ck_info.cksize)
	{
		mmioClose(mmio_handle, MMIO_FHOPEN);
		delete[] out_wave_data->SoundBuffer;
		return false;
	}

	// ファイルを閉じる
	mmioClose(mmio_handle, MMIO_FHOPEN);

	return true;
}
