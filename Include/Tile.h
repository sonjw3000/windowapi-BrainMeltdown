#pragma once

#include "Type.h"
#include <atlimage.h>


// TileSize 40x40;

class Tile {
public:
	Tile() = delete;
	Tile(TILE_DATA TD);
	~Tile();

private:
	TILE_DATA m_tTile;
	static CImage m_imgSprite;

public:
	TILE_DATA getTile() { return m_tTile; }
};

