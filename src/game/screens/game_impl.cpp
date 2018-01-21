#include "game_impl.hpp"

using namespace gamespace;
using namespace std::string_literals;

const char* GameScreenImpl::ground_names[16] = {
    "Cylinder.002_Cylinder.007",
    "Cylinder.001_Cylinder.006",
    "Cylinder_Cylinder.003",
    "WaterPlane_Plane.003",
    "WaterBlock_Cube.004",
    "DamBaseBottom.002_Cube.003",
    "DamBaseMiddle.002_Cylinder.002",
    "DamBaseTop.002_Cone.002",
    "DamBaseBottom.001_Cube.002",
    "DamBaseMiddle.001_Cylinder.001",
    "DamBaseTop.001_Cone.001",
    "DamBaseTop_Cone",
    "DamBaseMiddle_Cylinder",
    "DamBaseBottom_Cube.001",
    "DamMain_CUBezierCurve",
    "Ground_Plane.002"
};

#define Cylinder_002_Cylinder_007 0
#define Cylinder_001_Cylinder_006 1
#define Cylinder_Cylinder_003 2
#define WaterPlane_Plane_003 3
#define WaterBlock_Cube_004 4
#define DamBaseBottom_002_Cube_003 5
#define DamBaseMiddle_002_Cylinder_002 6
#define DamBaseTop_002_Cone_002 7
#define DamBaseBottom_001_Cube_002 8
#define DamBaseMiddle_001_Cylinder_001 9
#define DamBaseTop_001_Cone_001 10
#define DamBaseTop_Cone 11
#define DamBaseMiddle_Cylinder 12
#define DamBaseBottom_Cube_001 13
#define DamMain_CUBezierCurve 14
#define Ground_Plane_002 15

inline glm::uvec2 to_uvec2(hextile::hexpoint_t t) {
    return glm::uvec2(t.x, t.y);
}

hextile::hexpoint_t getLookedAtTile(glm::vec2 pc) {
    float yval1 = floor(pc.y / 1.5);
    float xval1 = floor((pc.x + (((int)yval1) % 2) * 0.5f * 2.0f * 0.866f) / (2.0f * 0.866f));
    glm::vec4 xval = glm::vec4(0, 1, 0, 1) + xval1;
    glm::vec4 yval = glm::vec4(0, 0, 1, 1) + yval1;

    glm::vec4 xpoints(2.0f * 0.866f * xval.x - ((int)yval.x % 2) * 0.5f * 2.0f * 0.866f,
        2.0f * 0.866f * xval.y - ((int)yval.y % 2) * 0.5f * 2.0f * 0.866f,
        2.0f * 0.866f * xval.z - ((int)yval.z % 2) * 0.5f * 2.0f * 0.866f,
        2.0f * 0.866f * xval.w - ((int)yval.w % 2) * 0.5f * 2.0f * 0.866f);
    glm::vec4 ypoints(yval * 1.5f);

    glm::vec4 distances(glm::distance(pc, glm::vec2(xpoints.x, ypoints.x)),
        glm::distance(pc, glm::vec2(xpoints.y, ypoints.y)),
        glm::distance(pc, glm::vec2(xpoints.z, ypoints.z)),
        glm::distance(pc, glm::vec2(xpoints.w, ypoints.w)));

    float d = std::min(distances.x, std::min(distances.y, std::min(distances.z, distances.w)));
    if(d == distances.x) return hextile::hexpoint_t {(int)xval.x, (int)yval.x};
    else if(d == distances.y) return hextile::hexpoint_t {(int)xval.y, (int)yval.y};
    else if(d == distances.z) return hextile::hexpoint_t {(int)xval.z, (int)yval.z};
    else if(d == distances.w) return hextile::hexpoint_t {(int)xval.w, (int)yval.w};
}

hextile::hexpoint_t getLookedAtTile(glm::vec3 pc) {
    return getLookedAtTile(glm::vec2(pc.x, pc.z));
}

GameScreenImpl::GameScreenImpl(gamespace::Match &&match, qe::Context *ctxt, std::shared_ptr<font::Font> font)
: _match(std::move(match)), _ctxt(ctxt), _font(font), _shouldClose(true) {
    initializeBuffers();
    initializeShaders();
    initializeSelection();
    initializeHUD();
    initializeAssets();
    initializeMap();
}

void GameScreenImpl::initializeBuffers() {
    _marker_buffer.reset(new qe::Buffer<GL_UNIFORM_BUFFER, uint32_t>());
    _marker_buffer->data<GL_DYNAMIC_DRAW>(nullptr, _match.board().x() * _match.board().y() * _match.board().marker_count * 4, true);
    _marker_buffer->bindTo(1);
}

void GameScreenImpl::initializeShaders() {
    qe::ShaderAssignments as;
    as.assign("MARKERCOUNT", std::to_string(_match.board().marker_count));
    as.assign("BOARDSIZE_X", std::to_string(_match.board().x()));
    as.assign("BOARDSIZE_Y", std::to_string(_match.board().y()));
    _terrain_shader.reset(qe::mkProgram("assets/shaders/terrain.vsh"_p, "assets/shaders/terrain.fsh"_p, as));
    _terrain_shader->use();
    _terrain_shader->setUniform<qe::UNIL>(glm::vec3(0.1, -1, 0.1));
    _terrain_shader->bindUniformBlockBinding(_terrain_shader->getUniformBlockIndex("MarkerBlock"), 1);
}

void GameScreenImpl::initializeSelection() {
    _selection.selected = nullptr;
    _selection.hovering = nullptr;
    _selection.type = SelectionState::Type::SEL_NONE;
}

void GameScreenImpl::initializeHUD() {
    // fixed UI
    _ui.reset(new ui::UI(ui::Point {_ctxt->width(), _ctxt->height()}));
    std::unique_ptr<ui::Box> box(new ui::Box());
    std::unique_ptr<ui::Text> text(new ui::Text());
    text->dimension() = ui::Point {1, 0.25};
    text->margin() = ui::Point {0.02, 0.02};

    box->orientation() = ui::Box::VERTICAL;
    box->expand() = false;
    box->align_x() = ui::Box::BEGINNING;
    box->align_y() = ui::Box::END;

    box->append(text.release());
    box->convert_coords(_ui->res());
    _ui->set_container(box.release());

    auto *t = _ui->get("1.1");
    // TODO Various text rendering issues
    auto text_renderer = [this](ui::Renderable *t) mutable {
        if(t->payload() == nullptr) {
            t->payload() = new text_t(
                ((ui::Text*)t)->text(),
                qe::Cache::glyphlatin,
                // to_ivec2(t->origin() + t->margin() + t->padding() + ui::absp_t {0, 0.5} * (t->dimension() - t->margin() - t->padding())),
                to_ivec2(t->origin() + t->margin() + t->padding()),
                (int)(0.5 * (t->dimension().y - t->margin().y - t->padding().y)),
                (int)(t->dimension().x - t->margin().x - t->padding().x));
        }
        text_t *pl = (text_t*)t->payload();
        pl->foreground() = glm::vec3(0, 0, 0);
        pl->render();
    };
    t->render_with([this](ui::Renderable *t) mutable {
        if(t->payload() == nullptr) {
            t->payload() = new text_t(
                ((ui::Text*)t)->text(),
                qe::Cache::glyphlatin,
                to_ivec2(t->origin() + t->margin() + t->padding() + ui::Point {0, 0.5} * (t->dimension() - t->margin() - t->padding())),
                (int)(0.5 * (t->dimension().y - t->margin().y - t->padding().y)),
                (int)(t->dimension().x - t->margin().x - t->padding().x));
        }
        text_t *pl = (text_t*)t->payload();
        pl->foreground() = glm::vec3(1, 1, 1);
        pl->render();
    });
    t->payload([](void* t){delete (text_t*)t;});
    _match.observe_player_change([this, t](auto np) {
        ((ui::Text*)t)->text() = "Or8Bit - (c) 2017-2018 Team Or8Bit\n"s
            + "LMB to move, RMB to attack\n"
            + "Current player (. for ending a turn): " + np.name();
        ((ui::Text*)t)->delete_payload();
    });
    t->payload() = nullptr;
}

void GameScreenImpl::initializeAssets() {
    // MODELS
    _cube.reset(new qe::Mesh<qe::OBJV1>(qe::Loader<qe::OBJV1>("assets/models/cube.objv1"_p)));
    _tile.reset(new qe::Mesh<qe::OBJV2>(qe::Loader<qe::OBJV2>("assets/models/hextile.objv2"_p)));
    _tank.reset(new qe::Mesh<qe::OBJV3>(qe::Loader<qe::OBJV3>("assets/models/tank.objv3"_p)));
    _ground.reset(new qe::Mesh<qe::OBJV3>(qe::Loader<qe::OBJV3>("assets/models/map.objv3"_p)));
    // RESOLVE SUBOBJS
        const char* names[] = {
            "Cylinder.002_Cylinder.007",
            "Cylinder.001_Cylinder.006",
            "Cylinder_Cylinder.003",
            "WaterPlane_Plane.003",
            "WaterBlock_Cube.004",
            "DamBaseBottom.002_Cube.003",
            "DamBaseMiddle.002_Cylinder.002",
            "DamBaseTop.002_Cone.002",
            "DamBaseBottom.001_Cube.002",
            "DamBaseMiddle.001_Cylinder.001",
            "DamBaseTop.001_Cone.001",
            "DamBaseTop_Cone",
            "DamBaseMiddle_Cylinder",
            "DamBaseBottom_Cube.001",
            "DamMain_CUBezierCurve",
            "Ground_Plane.002"
        };
    for(size_t i = 0; i < 16; ++i) {
        _ground_indices[i] = _ground->get_object(names[i]);
    }
    // TEXTURES
    _textures.hextile_grass.reset(new qe::Texture<qe::PNGRGBA, qe::DIFFTEXBIND_GL>(qe::Loader<qe::PNGRGBA>("assets/textures/hextile-grass.png"_p)));
    _cam.controlling = false;
    _cam.camera.reset(new qe::Camera(
                          glm::vec3(4, 4, 4),
                          glm::vec2(-45, -45),
                          _ctxt->getResolution(),
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
    auto *u1 = new gamespace::Unit(_tank.get(), &_match.player1(), "Tank",
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
    auto *u2 = new gamespace::Unit(_tank.get(), &_match.player2(), "Tank",
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
    _match.board().synchronize();
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
        _match.board().clearMarker(ACTION_LAYER);
    } else {
        _match.board().clearMarker(ACTION_LAYER);
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
        _match.board().clearMarker(MOVE_LAYER);
    } else {
        _match.board().clearMarker(MOVE_LAYER);
        if(_selection.hovering->unit() == nullptr) return; // no unit to select
        if(_selection.hovering->unit()->player() != _match.currentPlayer()) return; // can only select own units
        _selection.hovering->unit()->markAttack(*_selection.hovering);
        _selection.selected = _selection.hovering;
        _selection.type = SelectionState::Type::SEL_TO_ATTACK;
    }
}

void GameScreenImpl::inCameraMode(bool mode) {
    _cam.controlling = mode;
}

void GameScreenImpl::pre_run() {
    //_ctxt->hideCursor();
    _ctxt->events();
}

void GameScreenImpl::createContextForLookAt() {
    if(_selection.hovering && _selection.hovering->unit())
        CoordinateMenu::createForTile(_selection.hovering, _ui.get(), _ctxt->getResolution());
}

void GameScreenImpl::run() {
    GDBG("activating game screen");
    _shouldClose = false;
    unsigned int fps = 0;
    glm::mat4 m = glm::translate(glm::vec3(0, 0, 0));
    glClearColor(0.3, 0.3, 0.3, 1);

    _textures.hextile_grass->bindTo();

    const auto max_x = _match.board().x();
    const auto max_y = _match.board().y();
    const auto mc = _match.board().marker_count;
    auto *ptr = _marker_buffer->ptr();

    while(!_ctxt->shouldClose() && _shouldClose == false) {
        // calculate lookat tile
        if(_cam.controlling == false) {
            auto pc = getLookedAtTile(_cam.camera->getPlaneCoord());
            if(pc.x < 0 || pc.y < 0 || pc.x >= max_x || pc.y >= max_y) {
                _selection.hovering = nullptr;
            } else {
                _selection.hovering = &_match.board().get(pc);
            }
        } else {
            _selection.hovering = nullptr;
        }
        // set matrices
        glm::mat4 mvp = _cam.camera->matrices().pv * m;
        qe::Cache::objv2->use();
        qe::Cache::objv2->setUniform<qe::UNIMVP>(mvp);
        qe::Cache::objv2->setUniform<qe::UNIM>(m);
        qe::Cache::objv2->setUniform<qe::UNIV>(_cam.camera->matrices().v);
        qe::Cache::objv3->use();
        qe::Cache::objv3->setUniform<qe::UNIV>(_cam.camera->matrices().v);
        // update marker buffer
        {
            auto b = _match.board().begin();
            auto e = _match.board().end();
            for(; b != e; ++b) {
                auto coord = b->coord();
                for(size_t i = 0; i < mc; ++i) {
                    ptr[coord.x * max_y * mc + coord.y * mc + i] = b->marked(i) ? 1 : 0;
                }
            }
            _marker_buffer->update();
        }
        // render
        _ctxt->start();
        render();
        _ctxt->swap();
        // event handling

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

    // render tiles and units
    for(; b != e; ++b) {
        if(b->unit())
            renderUnitOf(&*b);
    }

    // render terrain
    renderTerrain();

    // render text
    _ctxt->textcontext();
    _ui->render();
    _ctxt->meshcontext();
}

void GameScreenImpl::renderTerrain() {
    glm::mat4 m = glm::translate(glm::vec3(18.099, 0, 10.963) + glm::vec3(-2 * 0.866, 0, -1.0));
    glm::mat4 mvp = _cam.camera->matrices().pv * m;
    _terrain_shader->use();
    _terrain_shader->setUniform<qe::UNIMVP>(mvp);
    _terrain_shader->setUniform<qe::UNIM>(m);
    _terrain_shader->setUniform<qe::UNIV>(_cam.camera->matrices().v);
    if(_cam.controlling)
        _terrain_shader->setUniform<qe::UNISEL>(glm::uvec2(9999, 9999));
    else
        _terrain_shader->setUniform<qe::UNISEL>(to_uvec2(getLookedAtTile(_cam.camera->getPlaneCoord())));
    // qe::Cache::objv3->setUniform<qe::UNICOLOR>(glm::vec3(0.800, 0.567, 0.305));
    // render grey areas
    _terrain_shader->setUniform<qe::UNICOLOR>(glm::vec3(0.8, 0.8, 0.8));
    _ground->render_sub(_ground_indices[DamMain_CUBezierCurve]);
    _ground->render_sub(_ground_indices[DamBaseBottom_Cube_001]);
    _ground->render_sub(_ground_indices[DamBaseBottom_001_Cube_002]);
    _ground->render_sub(_ground_indices[DamBaseBottom_002_Cube_003]);
    _ground->render_sub(_ground_indices[Cylinder_Cylinder_003]);
    _ground->render_sub(_ground_indices[Cylinder_001_Cylinder_006]);
    _ground->render_sub(_ground_indices[Cylinder_002_Cylinder_007]);
    // TODO render faction colors
    _ground->render_sub(_ground_indices[DamBaseMiddle_Cylinder]);
    _ground->render_sub(_ground_indices[DamBaseMiddle_001_Cylinder_001]);
    _ground->render_sub(_ground_indices[DamBaseMiddle_002_Cylinder_002]);
    _ground->render_sub(_ground_indices[DamBaseTop_Cone]);
    _ground->render_sub(_ground_indices[DamBaseTop_001_Cone_001]);
    _ground->render_sub(_ground_indices[DamBaseTop_002_Cone_002]);
    // render ground areas
    _terrain_shader->setUniform<qe::UNICOLOR>(glm::vec3(0.8, 0.567, 0.305));
    _ground->render_sub(_ground_indices[Ground_Plane_002]);
    // render water
    _terrain_shader->setUniform<qe::UNICOLOR>(glm::vec3(0.0, 0.551, 0.8));
    _ground->render_sub(_ground_indices[WaterPlane_Plane_003]);
    _ground->render_sub(_ground_indices[WaterBlock_Cube_004]);
}

void GameScreenImpl::renderUnitOf(gamespace::BoardTile *b) {
    glm::vec2 p = b->centerPos();
    // render unit
    glm::mat4 m = glm::translate(glm::vec3(p.x, b->renderData(), p.y));
    glm::mat4 mvp = _cam.camera->matrices().pv * m;
    b->unit()->render(*b, mvp, m);
}
void GameScreenImpl::__introspect(size_t off) {
    std::cout << std::string(off, ' ') << "GameScreenImpl" << std::endl;
    _match.__introspect(off + 2);
    if(_cube.get()) _cube->__introspect(off + 2);
    if(_tank.get()) _tank->__introspect(off + 2);
    if(_ground.get()) _ground->__introspect(off + 2);
    if(_ui.get()) _ui->__introspect(off + 2);
    if(_terrain_shader.get()) _terrain_shader->__introspect(off + 2);
}
