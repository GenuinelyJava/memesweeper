#include "MemeField.h"
#include "RectI.h"
#include <random>

MemeField::Tile& MemeField::TileAt(const Vei2 & gridPos)
{
	return tiles[gridPos.y * tileNumX + gridPos.x];
}

const MemeField::Tile & MemeField::TileAt(const Vei2 & gridPos) const
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
		else { SpriteCodex::DrawTileBomb(screenPos, gfx); }
		break;
	}
}

bool MemeField::Tile::HasMeme() const
{
	return hasMeme;
}
