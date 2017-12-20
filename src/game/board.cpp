#include "board.hpp"

#include "unit.hpp"

using namespace gamespace;

BoardTile::~BoardTile() {
    delete _unit;
}
