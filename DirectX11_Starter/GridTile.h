#pragma once
class GridTile
{
public:
	GridTile();
	~GridTile();

	bool IsOccupied();
	void setStatus(bool status);

private:

	bool occupied;
};

