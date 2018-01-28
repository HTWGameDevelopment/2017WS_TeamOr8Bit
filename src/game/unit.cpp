#include "unit.hpp"

#include<game/match.hpp>

using namespace gamespace;

unsigned int gamespace::defaultFalloff(BoardTile &t) {
    return t.terrain().falloff();
}

std::function<bool(BoardTile&,unsigned int&)> gamespace::getEdgeRelation(Unit *u, unsigned int layer, relation _v, Player *player, bool noground_block = false, bool unit_block = false) {
    return [player, layer, _v, noground_block, unit_block, u](BoardTile &b, unsigned int &p) {
        unsigned int v = _v(b);
        if(p < v) return false;
        if(noground_block && b.marked_value(AOE_LAYER) == 2) return false;
        if(unit_block && b.unit() && b.unit()->player() == *player
            && (b.unit()->containerMatchesType(u->name()) == false
                || b.unit()->getLastTurnId() == b.board().match()->getTurnId())) return false;
        bool r = b.mark(layer, p);
        p -= v;
        if(unit_block && b.unit()) return false;
        return r;
    };
}

void Unit::markVisibility(BoardTile &tile) {
    assert(tile.unit());
    tile.board().markByEdge(tile.coord(), _vr, VISIBILITY_LAYER, getEdgeRelation(tile.unit(), VISIBILITY_LAYER, _v, _player));
}

void Unit::markMovement(BoardTile &tile) {
    assert(tile.unit());
    tile.board().markByEdge(tile.coord(), _dpt, MOVE_LAYER, getEdgeRelation(tile.unit(), MOVE_LAYER, _t, _player, considersNoground(), true));
}

void Unit::markAttack(BoardTile &tile) {
    assert(tile.unit());
    tile.board().markByEdge(tile.coord(), _ar, ACTION_LAYER, getEdgeRelation(tile.unit(), ACTION_LAYER, _a, _player, false, true));
}

void Unit::render(BoardTile &, glm::mat4 &mvp, glm::mat4 &m) {
    qe::Cache::objv3->use();
    qe::Cache::objv3->setUniform<qe::UNIMVP>(mvp);
    qe::Cache::objv3->setUniform<qe::UNIM>(m);
    qe::Cache::objv3->setUniform<qe::UNICOLOR>(glm::vec3(0.448, 0.844, 1));
    qe::Cache::objv3->setUniform<qe::UNICOLOR>(_player->color());
    _mesh->render();
}

void Unit::__introspect(size_t off) {
    std::cout << std::string(off, ' ') << "Unit<" << _player->name() << ">[" << std::endl;
    std::string os(off + 8 + _player->name().size(), ' ');
    std::cout << os << "maxhp=" << _max_hp << std::endl;
    std::cout << os << "hp=" << _hp << std::endl;
    std::cout << os << "dp=" << _dp << std::endl;
    std::cout << os << "ap=" << _ap << std::endl;
    std::cout << os << "ar=" << _ar << std::endl;
    std::cout << os << "vr=" << _vr << std::endl;
    std::cout << os << "dpt=" << _dpt << std::endl;
}
