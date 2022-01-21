#include <d3dx9.h>
#include <stdlib.h>
#include "Graphics.h"
#include "Texture.h"
#include "Parameter.h"

struct CustomVertex
{
	float X;			// X座標
	float Y;			// Y座標
	float Z;			// Z座標	
	float Rhw;			// 除算数

	float TextureX;		// テクスチャ座標X
	float TexrureY;		// テクスチャ座標Y
};

// IDチェック
bool IsRangeCheck(int texture_id);
bool CreateTexture(const char* file_name, Texture* texture_data);

void InitTexture()
{
	AllReleaseTexture();

	for (int i = 0; i < TextureList::TextureMax; i++)
	{
		g_TextureList[i].TexutreData = nullptr;
	}
}

void ReleaseCategoryTexture()
{
	for (int i = 0; i < TextureList::TextureMax; i++)
	{
		if (g_TextureList[i].TexutreData == nullptr)
		{
			continue;
		}

		g_TextureList[i].TexutreData->Release();
		g_TextureList[i].TexutreData = nullptr;
	}
}

void AllReleaseTexture()
{
	ReleaseCategoryTexture();
}

bool LoadTexture(const char* file_name, int texture_id)
{
	if (IsRangeCheck(texture_id) == false)
	{
		return false;
	}

	return CreateTexture(file_name, &g_TextureList[texture_id]);
}

Texture* GetTexture(int texture_id)
{
	if (IsRangeCheck(texture_id) == false)
	{
		return nullptr;
	}
	return &g_TextureList[texture_id];
}

bool IsRangeCheck(int texture_id)
{
	if (texture_id <= -1 ||
		texture_id >= TextureList::TextureMax)
	{
		return false;
	}

	return true;
}

void DrawTexture(float x, float y, Texture* texture_data)
{
	CustomVertex verticies[4] =
	{
		{ x, y, 0.0f, 1.0f, 0.0f, 0.0f },
		{ x + texture_data->Width, y, 0.0f, 1.0f, 1.0f, 0.0f },
		{ x + texture_data->Width, y + texture_data->Height, 0.0f, 1.0f, 1.0f, 1.0f },
		{ x, y + texture_data->Height, 0.0f, 1.0f, 0.0f, 1.0f },
	};

	// 頂点構造の指定
	g_pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);

	g_pD3DDevice->SetTexture(0, texture_data->TexutreData);

	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, verticies, sizeof(CustomVertex));
}

void DrawTexture(float x, float y, Texture* texture_data, FRECT tutv) {
	float xX, xY;

	xX = tutv.right - tutv.left;
	xY = tutv.bottom - tutv.top;

	CustomVertex verticies[4] =
	{
		{ x, y, 0.0f, 1.0f, tutv.left, tutv.top },
		{ x + (int)(texture_data->Width * xX), y, 0.0f, 1.0f, tutv.right, tutv.top },
		{ x + (int)(texture_data->Width * xX), y + (int)(texture_data->Height * xY), 0.0f, 1.0f, tutv.right, tutv.bottom },
		{ x, y + (int)(texture_data->Height * xY), 0.0f, 1.0f, tutv.left, tutv.bottom },
	};

	// 頂点構造の指定
	g_pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);

	g_pD3DDevice->SetTexture(0, texture_data->TexutreData);

	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, verticies, sizeof(CustomVertex));

}

void DrawTexture(RECT rect, Texture* texture_data)
{
	CustomVertex verticies[4] =
	{
		{ rect.left , rect.top, 0.0f, 1.0f, 0.0f, 0.0f },
		{ rect.right, rect.top, 0.0f, 1.0f, 1.0f, 0.0f },
		{ rect.right, rect.bottom, 0.0f, 1.0f, 1.0f, 1.0f },
		{ rect.left , rect.bottom, 0.0f, 1.0f, 0.0f, 1.0f },
	};

	// 頂点構造の指定
	g_pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);

	g_pD3DDevice->SetTexture(0, texture_data->TexutreData);

	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, verticies, sizeof(CustomVertex));
}

void DrawTexture(RECT rect, Texture* texture_data, FRECT tutv) {
	CustomVertex verticies[4] =
	{
		{ rect.left , rect.top, 0.0f, 1.0f, tutv.left, tutv.top },
		{ rect.right, rect.top, 0.0f, 1.0f, tutv.right, tutv.top },
		{ rect.right, rect.bottom, 0.0f, 1.0f, tutv.right, tutv.bottom },
		{ rect.left , rect.bottom, 0.0f, 1.0f, tutv.left, tutv.bottom },
	};

	// 頂点構造の指定
	g_pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);

	g_pD3DDevice->SetTexture(0, texture_data->TexutreData);

	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, verticies, sizeof(CustomVertex));
}

void DrawUVMappingTexture(float x, float y, UVRect* rect)
{
	if (rect != nullptr)
	{
		DrawUVMappingTexture(
			x,
			y,
			GetTexture(rect->m_TextureId),
			rect->m_TexturePosX,
			rect->m_TexturePosY,
			rect->m_RectWidth,
			rect->m_RectHeight);
	}
}

void DrawUVMappingTexture(float x, float y, Texture* texture_data, float texture_x, float texture_y, float rect_width, float rect_height)
{
	float uv_left = texture_x / texture_data->Width;
	float uv_right = (texture_x + rect_width) / texture_data->Width;
	float uv_top = texture_y / texture_data->Height;
	float uv_bottom = (texture_y + rect_height) / texture_data->Height;

	CustomVertex verticies[4] =
	{
		{ x, y, 0.0f, 1.0f, uv_left, uv_top },
		{ x + rect_width, y, 0.0f, 1.0f, uv_right, uv_top },
		{ x + rect_width, y + rect_height, 0.0f, 1.0f, uv_right, uv_bottom },
		{ x, y + rect_height, 0.0f, 1.0f, uv_left, uv_bottom },
	};

	// 頂点構造の指定
	g_pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);

	g_pD3DDevice->SetTexture(0, texture_data->TexutreData);

	g_pD3DDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		verticies,
		sizeof(CustomVertex)
	);
}

void DrawUVScrollTexture(float x, float y, Texture* texture_data, float tu, float tv)
{
	CustomVertex v[4] =
	{
		{ x, y, 0.0f, 1.0f, tu, tv },
		{ x + texture_data->Width, y, 0.0f, 1.0f, tu + 1.0f, tv },
		{ x + texture_data->Width, y + texture_data->Height, 0.0f, 1.0f, tu + 1.0f, tv + 1.0f },
		{ x, y + texture_data->Height, 0.0f, 1.0f, tu, tv + 1.0f },
	};

	// 頂点構造の指定
	g_pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);

	g_pD3DDevice->SetTexture(0, texture_data->TexutreData);

	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, v, sizeof(CustomVertex));
}

bool CreateTexture(const char* file_name, Texture* texture_data)
{
	D3DXIMAGE_INFO info;

	// 2の累乗じゃないケースを想定して元のサイズを取得してD3DXCreateTextureFromFileExで使う
	D3DXGetImageInfoFromFileA(file_name, &info);

	if (FAILED(D3DXCreateTextureFromFileExA(g_pD3DDevice,
		file_name,
		info.Width,
		info.Height,
		1,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0x0000ff00,
		nullptr,
		nullptr,
		&texture_data->TexutreData)
	))
	{
		return false;
	}
	else
	{
		// テクスチャサイズの取得
		D3DSURFACE_DESC desc;

		if (FAILED(texture_data->TexutreData->GetLevelDesc(0, &desc)))
		{
			texture_data->TexutreData->Release();
			return false;
		}
		texture_data->Width = desc.Width;
		texture_data->Height = desc.Height;
	}

	return true;
}
