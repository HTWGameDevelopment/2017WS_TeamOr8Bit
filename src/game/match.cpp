#include "match.hpp"

using namespace gamespace;

void Match::setMarkers() {
    const hextile::hexpoint_t aoe[] = {
        {7, 0},
        {8, 0},
        {9, 0},
        {10, 0},
        {11, 0},
        {12, 0},
        {7, 1},
        {8, 1},
        {9, 1},
        {10, 1},
        {11, 1},
        {12, 1},
        {7, 2},
        {8, 2},
        {9, 2},
        {10, 2},
        {11, 2},
        {12, 2},
        {7, 3},
        {8, 3},
        {9, 3},
        {10, 3},
        {11, 3},
        {12, 3},
        {7, 4},
        {8, 4},
        {9, 4},
        {10, 4},
        {11, 4},
        {12, 4},
        {7, 5},
        {8, 5},
        {9, 5},
        {10, 5},
        {11, 5},
        {12, 5},
        {7, 6},
        {8, 6},
        {9, 6},
        {10, 6},
        {11, 6},
        {12, 6},
        {7, 7},
        {8, 7},
        {9, 7},
        {10, 7},
        {11, 7},
        {12, 7},
        {7, 8},
        {8, 8},
        {9, 8},
        {10, 8},
        {11, 8},
        {12, 8},
        {7, 9},
        {8, 9},
        {9, 9},
        {10, 9},
        {11, 9},
        {12, 9},
        {7, 10},
        {8, 10},
        {9, 10},
        {10, 10},
        {11, 10},
        {12, 10}
    };
    const hextile::hexpoint_t noground[] = {
        {2,8},
        {2,7},
        {2,9},
        {2,10},
        {3,11},
        {2,11},
        {4,11},
        {5,11},
        {3,12},
        {4,12},
        {5,12},
        {6,11},
        {7,11},
        {7,12},
        {6,12},
        {8,11},
        {8,12},
        {9,12},
        {10,12},
        {11,12},
        {12,13},
        {11,13},
        {10,13},
        {9,13},
        {8,13},
        {12,12},
        {12,11},
        {13,11},
        {13,12},
        {14,11},
        {15,12},
        {15,11},
        {14,12},
        {16,11},
        {16,12},
        {17,11},
        {17,10},
        {18,9},
        {17,8}
    };
    const hextile::hexpoint_t water[] = {
        {11,11},
        {10,11},
        {9,11},
        {8,10},
        {9,10},
        {10,10},
        {11,10},
        {11,9},
        {10,9},
        {9,9},
        {8,9},
        {12,9},
        {11,8},
        {10,8},
        {9,8},
        {8,8},
        {8,7},
        {9,7},
        {10,7},
        {11,7},
        {12,7},
        {11,6},
        {10,6},
        {9,6},
        {8,6},
        {9,5},
        {10,5},
        {11,5},
        {10,4},
        {9,4},
        {10,3},
        {10,2},
        {9,2},
        {10,1},
        {10,0},
        {9,0}
    };
    for(unsigned int i = 0; i < sizeof(aoe) / sizeof(hextile::hexpoint_t); ++i) {
        _board[aoe[i].x][aoe[i].y].mark(AOE_LAYER, 1);
    }
    for(unsigned int i = 0; i < sizeof(noground) / sizeof(hextile::hexpoint_t); ++i) {
        _board[noground[i].x][noground[i].y].mark(AOE_LAYER, 2);
    }
    for(unsigned int i = 0; i < sizeof(water) / sizeof(hextile::hexpoint_t); ++i) {
        _board[water[i].x][water[i].y].terrain() = Terrain::getWaterTile();
    }
}

struct roff_t {
    hextile::hexpoint_t c;
    glm::mat4 prem;
};

void Match::setRenderOffsets(UnitManager &manager) {
    const roff_t offs[] = {
        roff_t{hextile::hexpoint_t{0,4}, glm::translate(glm::vec3(0.0, 0.2, 0.0))*glm::rotate(-44.2f, glm::vec3(1, 0, 0))},
        roff_t{hextile::hexpoint_t{1,4}, glm::translate(glm::vec3(0.0, 0.1, 0.0))},
        roff_t{hextile::hexpoint_t{2,5}, glm::translate(glm::vec3(0.0, 0.1, 0.0))*glm::rotate(-44.2f, glm::vec3(1, 0, 1))},
        roff_t{hextile::hexpoint_t{2,6}, glm::translate(glm::vec3(0.0, 0.1, 0.0))},
        roff_t{hextile::hexpoint_t{1,6}, glm::translate(glm::vec3(0.0, 0.5, 0.0))},
        roff_t{hextile::hexpoint_t{1,5}, glm::translate(glm::vec3(0.0, 0.4, 0.0))*glm::rotate(-44.15f, glm::vec3(1, 0, 1))},
        roff_t{hextile::hexpoint_t{0,5}, glm::translate(glm::vec3(0.0, 0.6, 0.0))*glm::rotate(-44.15f, glm::vec3(1, 0, 0))},
        roff_t{hextile::hexpoint_t{0,6}, glm::translate(glm::vec3(0.0, 0.8, 0.0))*glm::rotate(-44.15f, glm::vec3(1, 0, 0))},
        roff_t{hextile::hexpoint_t{1,7}, glm::translate(glm::vec3(0.0, 1, 0.0))},
        roff_t{hextile::hexpoint_t{0,7}, glm::translate(glm::vec3(0.0, 0.9, 0.0))},
        roff_t{hextile::hexpoint_t{1,8}, glm::translate(glm::vec3(0.0, 1.2, 0.0))},
        roff_t{hextile::hexpoint_t{0,8}, glm::translate(glm::vec3(0.0, 1.2, 0.0))*glm::rotate(44.15f, glm::vec3(0, 0, 1))},
        roff_t{hextile::hexpoint_t{1,9}, glm::translate(glm::vec3(0.0, 1.4, 0.0))},
        roff_t{hextile::hexpoint_t{1,10}, glm::translate(glm::vec3(0.0, 1.7, 0.0))},
        roff_t{hextile::hexpoint_t{0,10}, glm::translate(glm::vec3(0.0, 1.5, 0.0))*glm::rotate(44.2f, glm::vec3(0, 0, 1))},
        roff_t{hextile::hexpoint_t{0,9}, glm::translate(glm::vec3(0.0, 1.1, 0.0))*glm::rotate(44.35f, glm::vec3(0, 0, 1))},
        roff_t{hextile::hexpoint_t{0,11}, glm::translate(glm::vec3(0.0, 1.2, 0.0))*glm::rotate(44.55f, glm::vec3(0, 0, 1))},
        roff_t{hextile::hexpoint_t{1,11}, glm::translate(glm::vec3(0.0, 2, 0.0))*glm::rotate(-44.15f, glm::vec3(1, 0, 0))},
        roff_t{hextile::hexpoint_t{1,12}, glm::translate(glm::vec3(0.0, 2.3, 0.0))},
        roff_t{hextile::hexpoint_t{0,12}, glm::translate(glm::vec3(0.0, 1.8, 0.0))*glm::rotate(44.5f, glm::vec3(0, 0, 1))},
        roff_t{hextile::hexpoint_t{0,13}, glm::translate(glm::vec3(0.0, 1.4, 0.0))*glm::rotate(44.55f, glm::vec3(0, 0, 1))},
        roff_t{hextile::hexpoint_t{1,13}, glm::translate(glm::vec3(0.0, 2.1, 0.0))*glm::rotate(44.3f, glm::vec3(0, 0, 1))},
        roff_t{hextile::hexpoint_t{2,13}, glm::translate(glm::vec3(0.0, 2.5, 0.0))},
        roff_t{hextile::hexpoint_t{2,12}, glm::translate(glm::vec3(0.0, 2.4, 0.0))*glm::rotate(-44.15f, glm::vec3(1, 0, 0))},
        roff_t{hextile::hexpoint_t{3,13}, glm::translate(glm::vec3(0.0, 2.6, 0.0))},
        roff_t{hextile::hexpoint_t{4,13}, glm::translate(glm::vec3(0.0, 2.8, 0.0))},
        roff_t{hextile::hexpoint_t{5,13}, glm::translate(glm::vec3(0.0, 3.0, 0.0))},
        roff_t{hextile::hexpoint_t{6,13}, glm::translate(glm::vec3(0.0, 2.9, 0.0))*glm::rotate(-44.00f, glm::vec3(1, 0, 1))},
        roff_t{hextile::hexpoint_t{7,13}, glm::translate(glm::vec3(0.0, 2.8, 0.0))},
        roff_t{hextile::hexpoint_t{13,13}, glm::translate(glm::vec3(0.0, 2.8, 0.0))},
        roff_t{hextile::hexpoint_t{14,13}, glm::translate(glm::vec3(0.0, 2.9, 0.0))},
        roff_t{hextile::hexpoint_t{15,13}, glm::translate(glm::vec3(0.0, 2.9, 0.0))},
        roff_t{hextile::hexpoint_t{16,13}, glm::translate(glm::vec3(0.0, 2.8, 0.0))},
        roff_t{hextile::hexpoint_t{17,13}, glm::translate(glm::vec3(0.0, 2.6, 0.0))},
        roff_t{hextile::hexpoint_t{18,12}, glm::translate(glm::vec3(0.0, 2.2, 0.0))},
        roff_t{hextile::hexpoint_t{17,12}, glm::translate(glm::vec3(0.0, 2.3, 0.0))*glm::rotate(-44.15f, glm::vec3(1, 0, 0))},
        roff_t{hextile::hexpoint_t{18,13}, glm::translate(glm::vec3(0.0, 2.4, 0.0))},
        roff_t{hextile::hexpoint_t{19,13}, glm::translate(glm::vec3(0.0, 2.1, 0.0))*glm::rotate(-44.2f, glm::vec3(1, 0, 1))},
        roff_t{hextile::hexpoint_t{19,11}, glm::translate(glm::vec3(0.0, 2.0, 0.0))*glm::rotate(-44.15f, glm::vec3(0, 0, 1))},
        roff_t{hextile::hexpoint_t{18,10}, glm::translate(glm::vec3(0.0, 1.7, 0.0))*glm::rotate(-44.15f, glm::vec3(1, 0, 1))},
        roff_t{hextile::hexpoint_t{18,11}, glm::translate(glm::vec3(0.0, 2.1, 0.0))*glm::rotate(44.25f, glm::vec3(0, 0, 1))},
        roff_t{hextile::hexpoint_t{19,10}, glm::translate(glm::vec3(0.0, 1.4, 0.0))*glm::rotate(-44.2f, glm::vec3(1, 0, 1))},
        roff_t{hextile::hexpoint_t{19,9}, glm::translate(glm::vec3(0.0, 1.4, 0.0))},
        roff_t{hextile::hexpoint_t{18,8}, glm::translate(glm::vec3(0.0, 1.3, 0.0))},
        roff_t{hextile::hexpoint_t{19,8}, glm::translate(glm::vec3(0.0, 1.1, 0.0))*glm::rotate(-44.15f, glm::vec3(1, 0, 1))},
        roff_t{hextile::hexpoint_t{19,7}, glm::translate(glm::vec3(0.0, 1.0, 0.0))},
        roff_t{hextile::hexpoint_t{18,7}, glm::translate(glm::vec3(0.0, 0.8, 0.0))*glm::rotate(44.35f, glm::vec3(0, 0, 1))},
        roff_t{hextile::hexpoint_t{18,6}, glm::translate(glm::vec3(0.0, 0.8, 0.0))*glm::rotate(44.15f, glm::vec3(0, 0, 1))},
        roff_t{hextile::hexpoint_t{19,6}, glm::translate(glm::vec3(0.0, 0.8, 0.0))},
        roff_t{hextile::hexpoint_t{17,6}, glm::translate(glm::vec3(0.0, 0.1, 0.0))*glm::rotate(44.15f, glm::vec3(0, 0, 1))},
        roff_t{hextile::hexpoint_t{18,5}, glm::translate(glm::vec3(0.0, 0.2, 0.0))*glm::rotate(44.15f, glm::vec3(0, 0, 1))},
        roff_t{hextile::hexpoint_t{19,5}, glm::translate(glm::vec3(0.0, 0.4, 0.0))*glm::rotate(-44.15f, glm::vec3(1, 0, 0))},
        roff_t{hextile::hexpoint_t{19,4}, glm::translate(glm::vec3(0.0, 0.2, 0.0))*glm::rotate(-44.2f, glm::vec3(1, 0, 1))}
    };
    for(unsigned int i = 0; i < sizeof(offs) / sizeof(roff_t); ++i) {
        _board[offs[i].c.x][offs[i].c.y].renderData() = offs[i].prem;
        if(i % 2 == 1)
            _board[offs[i].c.x][offs[i].c.y].setUnit(manager.createInfantry(&player2()));
        else
            _board[offs[i].c.x][offs[i].c.y].setUnit(manager.createGTrans(&player2()));
    }
    _board.synchronize();
}

void Match::checkWinningCondition() {
    return;
    for(size_t i = 0; i < _board.x(); ++i) {
        for(size_t j = 0; j < _board.y(); ++j) {
            if(_board[i][j].unit() && _board[i][j].unit()->player() != *_currentPlayer)
                return;
        }
    }
    // trigger win for current player
    for(size_t i = 0; i < _on_win.size(); ++i)
        _on_win[i](currentPlayer());
}
