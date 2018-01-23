#include "unit.hpp"

using namespace gamespace;

unsigned int gamespace::defaultFalloff(BoardTile &t) {
    return 1;
}

std::function<bool(BoardTile&,unsigned int&)> gamespace::getEdgeRelation(unsigned int layer, relation _v, bool noground_block = false) {
    return [layer, _v, noground_block](BoardTile &b, unsigned int &p) {
        if(p == 0) return false;
        if(noground_block && b.marked_value(AOE_LAYER) == 2) return false;
        return b.mark(layer, p--);
    };
}

void Unit::markVisibility(BoardTile &tile) {
    tile.board().markByEdge(tile.coord(), _vr, VISIBILITY_LAYER, getEdgeRelation(VISIBILITY_LAYER, _v));
}

void Unit::markMovement(BoardTile &tile) {
    tile.board().markByEdge(tile.coord(), _dpt, MOVE_LAYER, getEdgeRelation(MOVE_LAYER, _t, true));
}

void Unit::markAttack(BoardTile &tile) {
    tile.board().markByEdge(tile.coord(), _ar, ACTION_LAYER, getEdgeRelation(ACTION_LAYER, _a));
}

void Unit::render(BoardTile &tile, glm::mat4 &mvp, glm::mat4 &m) {
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
