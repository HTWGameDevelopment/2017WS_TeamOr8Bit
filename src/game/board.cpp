#include "gameboard.hpp"

#include "unit.hpp"

using namespace gamespace;

GameBoard::GameBoard(int x, int y): hextile::HexTile<BoardTile>(x, y) {
    for(int i = 0; i < _x; ++i) {
        for(int j = 0; j < _y; ++j) {
            _data[i][j].setBoard(this);
        }
    }
}

GameBoard::GameBoard(GameBoard &&other): hextile::HexTile<BoardTile>(std::move(other)) {
    for(int i = 0; i < _x; ++i) {
        for(int j = 0; j < _y; ++j) {
            _data[i][j].setBoard(this);
        }
    }
}

BoardTile::~BoardTile() {
    delete _unit;
}

void BoardTile::destroyUnit() {
    if(_unit) {
        delete _unit;
        _unit = nullptr;
    }
}

bool BoardTile::mark(unsigned int d) {
    if(_board->currentMarker() == _mark.id) {
        if(_mark.val < d) {
            _mark.val = d;
            return true;
        } else return false;
    } else {
        _mark.id = _board->currentMarker();
        _mark.val = d;
        return true;
    }
}

GameBoard &BoardTile::board() {
    return *_board;
}

bool BoardTile::marked() {
    return _mark.id == _board->currentMarker() && _mark.val != 0;
}

void GameBoard::moveUnit(hexpoint_t from, hexpoint_t to) {
    auto &ft = get(from);
    auto &tt = get(to);
    assert(ft.unit());
    assert(tt.unit() == nullptr);
    tt.setUnit(ft.unit());
    ft.setUnit(nullptr);
}

void GameBoard::attackUnit(hexpoint_t from, hexpoint_t to) {
    auto &ft = get(from);
    auto &tt = get(to);
    assert(ft.unit());
    assert(tt.unit());
    assert(ft.unit()->player() != tt.unit()->player());
    tt.unit()->attackedWith(*ft.unit());
    if(tt.unit()->dead()) tt.destroyUnit();
}
