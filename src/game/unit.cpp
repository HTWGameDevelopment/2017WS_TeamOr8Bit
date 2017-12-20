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

void Unit::markMovement(BoardTile &tile) {
    tile.board().markByEdge(tile.coord(), vr, getEdgeRelation(_t));
}

void Unit::render(BoardTile &tile, glm::mat4 &mvp, glm::mat4 &m) {
    qe::Cache::objv3->use();
    qe::Cache::objv3->setUniform<qe::UNIMVP>(mvp);
    qe::Cache::objv3->setUniform<qe::UNIM>(m);
    qe::Cache::objv3->setUniform<qe::UNICOLOR>(glm::vec3(0.448, 0.844, 1));
    qe::Cache::objv3->setUniform<qe::UNICOLOR>(_player->color());
    _mesh->render();
}
