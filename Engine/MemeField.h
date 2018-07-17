#pragma once

#include "Vei2.h"
#include "Graphics.h"
#include "SpriteCodex.h"
#include "Mouse.h"

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
		void Draw(const Tile& tile, const Vei2& screenPos, Graphics& gfx) const;
		bool HasMeme() const;
		bool IsRevealed() const;
		bool IsFlagged() const;
		void Reveal();
		void Flag();
		void SetSurroundingMemes(int x);
	private:
		State state = State::Hidden;
		bool hasMeme = false;
		int surroundingMemes = 0;
	};
private:
	static const int tileNumX = 20;
	static const int tileNumY = 20;
	//Vei2 pos = { 0,0 };
	Vei2 pos = { Graphics::ScreenWidth/2 - tileNumX*SpriteCodex::tileSize/2,
		Graphics::ScreenHeight/2 - tileNumY*SpriteCodex::tileSize/2 }; // center the field 
	Tile tiles[tileNumX * tileNumY];
private:
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos) const;
	int CalcSurroundingMemes(Vei2 gridPos);
public:
	MemeField(int nMemes);
	void Draw(Graphics& gfx) const;
	void OnLeftClick(const Vei2& gridPos);
	void OnRightClick(const Vei2& gridPos);
	Vei2 ScreenToGrid(const Vei2& screenPos);
};