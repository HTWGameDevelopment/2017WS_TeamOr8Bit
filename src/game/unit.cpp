#include "unit.hpp"

using namespace gamespace;

unsigned int gamespace::defaultFalloff(BoardTile &t) {
    return 1;
}

std::function<bool(BoardTile&,unsigned int&)> gamespace::getEdgeRelation(relation _v) {
    return [_v](BoardTile &b, unsigned int &p) {if(p == 0) return false; return b.mark(p--);};
}

void Unit::markVisibility(BoardTile &tile) {
    tile.board().markByEdge(tile.coord(), vr, getEdgeRelation(_v));
}
