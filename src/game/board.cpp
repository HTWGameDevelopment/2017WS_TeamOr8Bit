#include "gameboard.hpp"

#include "unit.hpp"

using namespace gamespace;

GameBoard::GameBoard(int x, int y): hextile::HexTile<BoardTile, 4>(x, y) {
    for(int i = 0; i < _x; ++i) {
        for(int j = 0; j < _y; ++j) {
            _data[i * _y + j].setBoard(this);
        }
    }
}

GameBoard::GameBoard(GameBoard &&other): hextile::HexTile<BoardTile, 4>(std::move(other)) {
    for(int i = 0; i < _x; ++i) {
        for(int j = 0; j < _y; ++j) {
            _data[i * _y + j].setBoard(this);
        }
    }
}

void GameBoard::synchronize() {
    for(int i = 0; i < x(); ++i) {
        for(int j = 0; j < y(); ++j) {
            if(operator[](i)[j].unit()) {
                operator[](i)[j].unit()->tile() = &operator[](i)[j];
            }
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

bool BoardTile::mark(unsigned int layer, unsigned int d) {
    if(_board->currentMarker(layer) == _marker_layer[layer].id) {
        if(_marker_layer[layer].val < d) {
            _marker_layer[layer].val = d;
            return true;
        } else return false;
    } else {
        _marker_layer[layer].id = _board->currentMarker(layer);
        _marker_layer[layer].val = d;
        return true;
    }
}

unsigned int BoardTile::marked_value(unsigned int layer) {
    return _marker_layer[layer].val;
}

GameBoard &BoardTile::board() {
    return *_board;
}

bool BoardTile::marked(unsigned int layer) {
    return _marker_layer[layer].id == _board->currentMarker(layer) && _marker_layer[layer].val != 0;
}

void GameBoard::moveUnit(hexpoint_t from, hexpoint_t to) {
    auto &ft = get(from);
    auto &tt = get(to);
    assert(ft.unit());
    assert(tt.unit() == nullptr);
    tt.setUnit(ft.unit());
    ft.setUnit(nullptr);
    tt.unit()->tile() = &tt;
    // TODO Map events
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

void BoardTile::__introspect(size_t off) {
    std::cout << std::string(off, ' ') << "BoardTile[" << _p.x << "," << _p.y << "]" << std::endl;
    if(_unit) _unit->__introspect(off + 2);
}
