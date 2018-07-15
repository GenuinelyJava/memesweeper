#pragma once

#include "Vei2.h"
#include "Graphics.h"
#include "SpriteCodex.h"

class MemeField
{
private:
	enum class State
	{
		Hidden,
		Revealed,
		Flagged
	};
	class Tile
	{
	public:
		void SpawnMeme();
		void Draw(const Vei2& screenPos, Graphics& gfx) const;
		bool HasMeme() const;
	private:
		State state = State::Hidden;
		bool hasMeme = false;
	};
private:
	static const int tileNumX = 20;
	static const int tileNumY = 20;
	Vei2 pos = { Graphics::ScreenWidth/2 - tileNumX*SpriteCodex::tileSize/2,
		Graphics::ScreenHeight/2 - tileNumY*SpriteCodex::tileSize/2 }; // center the field 
	Tile tiles[tileNumX * tileNumY];
private:
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos) const;
public:
	MemeField(int nMemes);
	void Draw(Graphics& gfx) const;
};