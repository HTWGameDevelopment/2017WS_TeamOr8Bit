#include "game_impl.hpp"

using namespace gamespace;

bool isLookedAtTile(glm::vec2 ori, glm::vec3 planecoord) {
    glm::vec2 pc(planecoord.x, planecoord.z);
    auto dx = 0.5 * gamespace::BoardTile::dim_x;
    auto dy = 0.5 * gamespace::BoardTile::dim_y;
    pc += glm::vec2(dx, dy);
    glm::vec2 edge1 = glm::normalize(glm::vec2(0, 1));
    glm::vec2 edge2 = glm::normalize(glm::vec2(dx, 0.5));
    glm::vec2 edge3 = glm::normalize(glm::vec2(dx, -0.5));
    glm::vec2 edge4 = glm::normalize(glm::vec2(0,-1));
    glm::vec2 edge5 = glm::normalize(glm::vec2(-dx, 0.5));
    glm::vec2 edge6 = glm::normalize(glm::vec2(-dx,-0.5));
    if(abs(glm::dot(edge1, ori - pc)) > 0.7) return false;
    if(abs(glm::dot(edge2, ori - pc)) > 0.7) return false;
    if(abs(glm::dot(edge3, ori - pc)) > 0.7) return false;
    if(abs(glm::dot(edge4, ori - pc)) > 0.7) return false;
    if(abs(glm::dot(edge5, ori - pc)) > 0.7) return false;
    if(abs(glm::dot(edge6, ori - pc)) > 0.7) return false;
    return true;
}

GameScreenImpl::GameScreenImpl(gamespace::Match &&match, qe::Context *ctxt, std::shared_ptr<font::Font> font)
: _match(std::move(match)), _ctxt(ctxt), _font(font), _shouldClose(true) {
    initializeSelection();
    initializeHUD();
    initializeAssets();
    initializeMap();
}

void GameScreenImpl::initializeSelection() {
    _selection.selected = nullptr;
    _selection.hovering = nullptr;
    _selection.type = SelectionState::Type::SEL_NONE;
}

void GameScreenImpl::initializeHUD() {
#ifdef HAS_FREETYPE
    std::cout << "Using font " << _font->bpath() << std::endl;
    qe::Cache::glyphlatin = new qe::GlyphmapLatin(_font->bpath(), _font->face(), 32, _ctxt->getResolution());
#else
    std::cout << "Using baked font " << _font->bpath() << std::endl;
    qe::Cache::glyphlatin = new qe::GlyphmapLatin(_font->bpath(), _ctxt->getResolution());
#endif
    _strings.gamename = qe::Text<qe::GlyphmapLatin>(qe::Cache::glyphlatin, glm::ivec2(500, 100), qe::PositionMode::TOP);
    _strings.help1 = qe::Text<qe::GlyphmapLatin>(qe::Cache::glyphlatin, glm::ivec2(500, 150), qe::PositionMode::TOP);
    _strings.help2 = qe::Text<qe::GlyphmapLatin>(qe::Cache::glyphlatin, glm::ivec2(500, 200), qe::PositionMode::TOP);
    _strings.gamename.text() = "Or8Bit - (c) 2017 Team Or8Bit";
    _strings.help1.text() = "LMB to move, RMB to attack";
    _strings.help2_tmp = "Current player (. for ending a turn): ";
    _strings.help2.text() = _strings.help2_tmp + _match.currentPlayer().name();
}

void GameScreenImpl::initializeAssets() {
    // MODELS
    _cube.reset(new qe::Mesh<qe::OBJV1>(qe::Loader<qe::OBJV1>("assets/models/cube.objv1"_p)));
    _tile.reset(new qe::Mesh<qe::OBJV2>(qe::Loader<qe::OBJV2>("assets/models/hextile.objv2"_p)));
    _tank.reset(new qe::Mesh<qe::OBJV3>(qe::Loader<qe::OBJV3>("assets/models/tank.objv3"_p)));
    // TEXTURES
    _textures.hextile_grass.reset(new qe::Texture<qe::PNGRGBA, qe::DIFFTEXBIND_GL>(qe::Loader<qe::PNGRGBA>("assets/textures/hextile-grass.png"_p)));
    _cam.camera.reset(new qe::Camera(
                          glm::vec3(4, 4, 4),
                          glm::vec2(-45, -45),
                          0.1,
                          30,
                          _ctxt->getAR(),
                          90
                      ));
}

void GameScreenImpl::initializeMap() {
    auto b = _match.board().begin();
    auto e = _match.board().end();
    for(; b != e; ++b) {
        b->setUnit(nullptr);
    }
    auto *u1 = new gamespace::Unit(_tank.get(), &_match.player1(),
        100,
        50,
        50,
        3,
        3,
        2,
        gamespace::defaultFalloff,
        gamespace::defaultFalloff,
        gamespace::defaultFalloff,
        gamespace::defaultFalloff);
    auto *u2 = new gamespace::Unit(_tank.get(), &_match.player2(),
        100,
        50,
        50,
        3,
        3,
        2,
        gamespace::defaultFalloff,
        gamespace::defaultFalloff,
        gamespace::defaultFalloff,
        gamespace::defaultFalloff);
    // TESTING PURPOSES. MOVE THIS TO MATCH SOMEHOW
    _match.board()[0][0].setUnit(new gamespace::Unit(*u1));
    _match.board()[1][0].setUnit(new gamespace::Unit(*u1));
    _match.board()[0][1].setUnit(new gamespace::Unit(*u1));
    _match.board()[1][1].setUnit(new gamespace::Unit(*u1));
    _match.board()[0][5].setUnit(new gamespace::Unit(*u2));
    _match.board()[1][5].setUnit(new gamespace::Unit(*u2));
    _match.board()[0][6].setUnit(new gamespace::Unit(*u2));
    _match.board()[1][6].setUnit(new gamespace::Unit(*u2));
    delete u1;
    delete u2;
    // _match.board()[0][0].unit()->markVisibility(_match.board()[0][0]);
}

void GameScreenImpl::enableMoveMask() {
    if(_selection.hovering == nullptr) return; // no tile to select
    if(_selection.type == SelectionState::Type::SEL_TO_MOVE) {
        if(_selection.hovering->marked(MOVE_LAYER) && _selection.hovering != _selection.selected) {
            assert(_selection.selected);
            if(_selection.hovering->unit() != nullptr) return; // cannot move to an occupied tile
            _match.board().moveUnit(_selection.selected->coord(), _selection.hovering->coord());
        }
        _selection.type = SelectionState::Type::SEL_NONE;
        _selection.selected = nullptr;
        _selection.hovering = nullptr;
        _match.board().clearMarker(MOVE_LAYER);
    } else {
        if(_selection.hovering->unit() == nullptr) return; // no unit to select
        if(_selection.hovering->unit()->player() != _match.currentPlayer()) return; // can only select own units
        GDBG("enable movement mask on " << _selection.hovering->coord().string());
        _selection.hovering->unit()->markMovement(*_selection.hovering);
        _selection.selected = _selection.hovering;
        _selection.type = SelectionState::Type::SEL_TO_MOVE;
    }
}

void GameScreenImpl::enableAttackMask() {
    if(_selection.hovering == nullptr) return; // no tile to select
    if(_selection.type == SelectionState::Type::SEL_TO_ATTACK) {
        if(_selection.hovering->marked(ACTION_LAYER) && _selection.hovering != _selection.selected) {
            assert(_selection.selected);
            if(_selection.hovering->unit() == nullptr) return; // cannot attack empty tile
            if(_selection.hovering->unit()->player() == _match.currentPlayer()) return; // cannot attack own unit
            _match.board().attackUnit(_selection.selected->coord(), _selection.hovering->coord());
        }
        _selection.type = SelectionState::Type::SEL_NONE;
        _selection.selected = nullptr;
        _selection.hovering = nullptr;
        _match.board().clearMarker(ACTION_LAYER);
    } else {
        if(_selection.hovering->unit() == nullptr) return; // no unit to select
        if(_selection.hovering->unit()->player() != _match.currentPlayer()) return; // can only select own units
        _selection.hovering->unit()->markAttack(*_selection.hovering);
        _selection.selected = _selection.hovering;
        _selection.type = SelectionState::Type::SEL_TO_ATTACK;
    }
}

void GameScreenImpl::pre_run() {
    _ctxt->hideCursor();
    _ctxt->events();
}

void GameScreenImpl::run() {
    GDBG("activating game screen");
    _shouldClose = false;
    unsigned int fps = 0;
    glm::mat4 m = glm::translate(glm::vec3(0, 0, 0));
    glClearColor(0.3, 0.3, 0.3, 1);

    _textures.hextile_grass->bindTo();

    while(!_ctxt->shouldClose() && _shouldClose == false) {
        // calculate lookat tile
        glm::mat4 mvp = _cam.camera->matrices().pv * m;
        qe::Cache::objv2->use();
        qe::Cache::objv2->setUniform<qe::UNIMVP>(mvp);
        qe::Cache::objv2->setUniform<qe::UNIM>(m);
        qe::Cache::objv2->setUniform<qe::UNIV>(_cam.camera->matrices().v);
        qe::Cache::objv3->use();
        qe::Cache::objv3->setUniform<qe::UNIV>(_cam.camera->matrices().v);
        _ctxt->start();
        render();
        _ctxt->swap();

        if(fps != _ctxt->fps()) {
            fps = _ctxt->fps();
            GINFO("FPS " << std::to_string(fps));
        }

        _ctxt->events();
    }
}

void GameScreenImpl::deactivate() {
    _shouldClose = true;
}

void GameScreenImpl::render() {
    auto b = _match.board().begin();
    auto e = _match.board().end();
    std::array<gamespace::BoardTile*, 3> selected;
    uint8_t len = 0;

    // render tiles and units
    for(; b != e; ++b) {
        if(isLookedAtTile(b->centerPos(), _cam.camera->getPlaneCoord())) {
            selected[len++] = &*b;
        } else {
            renderTile(&*b, glm::vec3(0, 0, 0));
        }
    }

    _selection.hovering = nullptr;
    if(len == 1) { // render one selected
        _selection.hovering = selected[0];
        renderTile(selected[0], glm::vec3(0.2, 0.2, 0.2));
    } else if(len != 0){
        for(uint8_t i = 0; i < len; ++i) renderTile(selected[i], glm::vec3(0, 0, 0));
    }

    // render text
    _ctxt->textcontext();
    _strings.gamename.render();
    _strings.help1.render();
    _strings.help2.text() = _strings.help2_tmp + _match.currentPlayer().name();
    _strings.help2.render();
    _ctxt->meshcontext();
}

void GameScreenImpl::renderTile(gamespace::BoardTile *b, glm::vec3 ho) {
    glm::vec2 p = b->centerPos();
    if(b->marked(ACTION_LAYER) || b->marked(MOVE_LAYER)) {
        if(_selection.type == SelectionState::Type::SEL_NONE) {
            _match.board().clearMarker(ACTION_LAYER);
            _match.board().clearMarker(MOVE_LAYER);
        }
        else ho += glm::vec3(0.2, 0.2, 0.2);
    }
    glm::mat4 m = glm::translate(glm::vec3(p.x, -0.25, p.y));
    glm::mat4 mvp = _cam.camera->matrices().pv * m;
    qe::Cache::objv2->use();
    qe::Cache::objv2->setUniform<qe::UNIMVP>(mvp);
    qe::Cache::objv2->setUniform<qe::UNIM>(m);
    qe::Cache::objv2->setUniform<qe::UNICOLOR>(ho);
    _tile->render();
    // render unit
    if(b->unit() != nullptr) {
        m = glm::translate(glm::vec3(p.x, 0, p.y));
        mvp = _cam.camera->matrices().pv * m;
        b->unit()->render(*b, mvp, m);
    }
}
