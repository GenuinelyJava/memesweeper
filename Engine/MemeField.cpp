#include "MemeField.h"
#include "RectI.h"
#include <random>
#include <assert.h>

MemeField::Tile& MemeField::TileAt(const Vei2 & gridPos)
{
	return tiles[gridPos.y * tileNumX + gridPos.x];
}

const MemeField::Tile& MemeField::TileAt(const Vei2 & gridPos) const
{
	return tiles[gridPos.y * tileNumX + gridPos.x];
}

MemeField::MemeField(int nMemes)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<int> xDist(0, tileNumX - 1);
	std::uniform_int_distribution<int> yDist(0, tileNumY - 1);

	for (int i = 0; i < nMemes; i++)
	{
		Vei2 randTilePos;
		do
		{
			randTilePos = { xDist(rng), yDist(rng) };
			TileAt(randTilePos).SpawnMeme();
		} while (!TileAt(randTilePos).HasMeme());
	}
}

void MemeField::Draw(Graphics& gfx) const
{
	// todo: draw a rectangle covering the whole memefield with rgb(192, 192, 192)
	RectI background(pos.x, pos.x + tileNumX * SpriteCodex::tileSize, pos.y, pos.y + tileNumY * SpriteCodex::tileSize);
	gfx.DrawRect(background, SpriteCodex::baseColor);
	for (Vei2 gridPos = { 0, 0 }; gridPos.y < tileNumY; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < tileNumX; gridPos.x++)
		{
			TileAt(gridPos).Draw(pos + gridPos * SpriteCodex::tileSize, gfx);
		}
	}
}

void MemeField::OnLeftClick(const Vei2& screenPos)
{
	Vei2 gridPos = ScreenToGrid(screenPos);
	if (gridPos.x != tileNumX && gridPos.y != tileNumY)
	{
		Tile& tile = TileAt(gridPos);
		if (!tile.IsRevealed() && !tile.IsFlagged())
		{
			tile.Reveal();
		}
	}
}

void MemeField::OnRightClick(const Vei2& screenPos)
{
	Vei2 gridPos = ScreenToGrid(screenPos);
	Tile& tile = TileAt(gridPos);
	if (gridPos.x != tileNumX && gridPos.y != tileNumY)
	{
		if (!tile.IsFlagged() && !tile.IsRevealed())
		{
			tile.Flag();
		}
	}
}

Vei2 MemeField::ScreenToGrid(const Vei2 & screenPos) const
{
	Vei2 gridPos = (screenPos - pos) / SpriteCodex::tileSize;
	return (gridPos.x >= 0 && gridPos.x < tileNumX &&
		gridPos.y >= 0 && gridPos.y < tileNumY) ? gridPos : Vei2(tileNumX, tileNumY); 
	// if screenPos is outside grid return bottom right pos outside grid
}

void MemeField::Tile::SpawnMeme()
{
	hasMeme = true;
}

void MemeField::Tile::Draw(const Vei2& screenPos, Graphics& gfx) const
{
	switch (state)
	{
	case State::Hidden:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		break;
	case State::Flagged:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		SpriteCodex::DrawTileFlag(screenPos, gfx);
		break;
	case State::Revealed:
		if (!hasMeme) { SpriteCodex::DrawTile0(screenPos, gfx); }
		else { SpriteCodex::DrawTileBombRed(screenPos, gfx); }
		break;
	}
}

bool MemeField::Tile::HasMeme() const
{
	return hasMeme;
}

bool MemeField::Tile::IsRevealed() const
{
	return state == State::Revealed;
}

bool MemeField::Tile::IsFlagged() const
{
	return state == State::Flagged;
}

void MemeField::Tile::Reveal()
{
	state = State::Revealed;
}

void MemeField::Tile::Flag()
{

	state = State::Flagged;
}
