#include "MemeField.h"
#include "RectI.h"
#include <random>
#include <assert.h>

bool MemeField::gameOver = false;

MemeField::Tile& MemeField::TileAt(const Vei2 & gridPos)
{
	return tiles[gridPos.y * tileNumX + gridPos.x];
}

const MemeField::Tile& MemeField::TileAt(const Vei2 & gridPos) const
{
	return tiles[gridPos.y * tileNumX + gridPos.x];
}

int MemeField::CalcSurroundingMemes(Vei2 gridPos)
{
	int numOfMemes = 0;
	for (int x = gridPos.x - 1; x <= gridPos.x + 1; x++)
	{
		for (int y = gridPos.y - 1; y <= gridPos.y + 1; y++)
		{
			if (x >= 0 && x < tileNumX
				&& y >= 0 && y < tileNumY)
			{
				if (TileAt({ x, y }).HasMeme())
				{
					numOfMemes++;
				}
			}
		}
	}
	return numOfMemes;
}

MemeField::MemeField(int nMemes)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<int> xDist(0, tileNumX - 1);
	std::uniform_int_distribution<int> yDist(0, tileNumY - 1);
	// loop through all the tiles and spawn memes randomly
	for (int i = 0; i < nMemes; i++)
	{
		Vei2 randTilePos;
		do
		{
			randTilePos = { xDist(rng), yDist(rng) };
			TileAt(randTilePos).SpawnMeme();
		} while (!TileAt(randTilePos).HasMeme());
	}
	// loop through all the tiles and calculate the number of memes surrounding each
	for (Vei2 gridPos = { 0, 0 }; gridPos.y < tileNumY; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < tileNumX; gridPos.x++)
		{
			TileAt(gridPos).SetSurroundingMemes(CalcSurroundingMemes(gridPos));
		}
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
			TileAt(gridPos).Draw(TileAt(gridPos), pos + gridPos * SpriteCodex::tileSize, gfx);
		}
	}
}

void MemeField::OnLeftClick(const Vei2& screenPos)
{
	Vei2 gridPos = ScreenToGrid(screenPos);
	// if screenPos is bottom right outside grid, do not attempt to flag or reveal
	if (gridPos.x != tileNumX && gridPos.y != tileNumY)
	{
		Tile& tile = TileAt(gridPos);
		if (!tile.IsRevealed() && !tile.IsFlagged())
		{
			tile.Reveal();
			if (tile.HasMeme()) gameOver = true;
		}
	}
}

void MemeField::OnRightClick(const Vei2& screenPos)
{
	Vei2 gridPos = ScreenToGrid(screenPos);
	if (gridPos.x != tileNumX && gridPos.y != tileNumY)
	{
		Tile& tile = TileAt(gridPos);
		if (!tile.IsFlagged() && !tile.IsRevealed())
		{
			tile.Flag();
		}
	}
}

Vei2 MemeField::ScreenToGrid(const Vei2 & screenPos)
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

void MemeField::Tile::Draw(const Tile& tile, const Vei2& screenPos, Graphics& gfx) const
{
	if (!gameOver)
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
			if (!hasMeme)
			{
				switch (tile.surroundingMemes)
				{
				case 0:
					SpriteCodex::DrawTile0(screenPos, gfx);
					break;
				case 1:
					SpriteCodex::DrawTile1(screenPos, gfx);
					break;
				case 2:
					SpriteCodex::DrawTile2(screenPos, gfx);
					break;
				case 3:
					SpriteCodex::DrawTile3(screenPos, gfx);
					break;
				case 4:
					SpriteCodex::DrawTile4(screenPos, gfx);
					break;
				case 5:
					SpriteCodex::DrawTile5(screenPos, gfx);
					break;
				case 6:
					SpriteCodex::DrawTile6(screenPos, gfx);
					break;
				case 7:
					SpriteCodex::DrawTile7(screenPos, gfx);
					break;
				case 8:
					SpriteCodex::DrawTile8(screenPos, gfx);
					break;
				}
			}
			else
			{
				SpriteCodex::DrawTileBombRed(screenPos, gfx);
			}
			break;
		}
	}
	else // game over (i like a little sensorship compared to chili :D )
	{
		switch (state)
		{
		case State::Hidden:
			if (hasMeme) SpriteCodex::DrawTileBomb(screenPos, gfx);
			else SpriteCodex::DrawTileButton(screenPos, gfx);
			break;
		case State::Flagged:
			if (!hasMeme)
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
				SpriteCodex::DrawTileCross(screenPos, gfx);
			}
			else
			{
				SpriteCodex::DrawTileButton(screenPos, gfx);
				SpriteCodex::DrawTileFlag(screenPos, gfx);
			}
			break;
		case State::Revealed:
			if (!hasMeme)
			{
				switch (tile.surroundingMemes)
				{
				case 0:
					SpriteCodex::DrawTile0(screenPos, gfx);
					break;
				case 1:
					SpriteCodex::DrawTile1(screenPos, gfx);
					break;
				case 2:
					SpriteCodex::DrawTile2(screenPos, gfx);
					break;
				case 3:
					SpriteCodex::DrawTile3(screenPos, gfx);
					break;
				case 4:
					SpriteCodex::DrawTile4(screenPos, gfx);
					break;
				case 5:
					SpriteCodex::DrawTile5(screenPos, gfx);
					break;
				case 6:
					SpriteCodex::DrawTile6(screenPos, gfx);
					break;
				case 7:
					SpriteCodex::DrawTile7(screenPos, gfx);
					break;
				case 8:
					SpriteCodex::DrawTile8(screenPos, gfx);
					break;
				}
			}
			else
			{
				SpriteCodex::DrawTileBombRed(screenPos, gfx);
			}
		}
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
	state = state == State::Hidden ? State::Flagged : State::Hidden;
}

void MemeField::Tile::SetSurroundingMemes(int x)
{
	surroundingMemes = x;
}
