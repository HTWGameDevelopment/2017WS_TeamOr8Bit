#include "game_impl.hpp"

#include<game/uirender.hpp>

#include<game/unitcmove.hpp>
#include<game/unitmove.hpp>
#include<game/unitattack.hpp>

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

hextile::hexpoint_t GameScreenImpl::getLookedAtTile(glm::vec2 uv) {
    qe::rgpixel_t r;
    _ffbo.bindread();
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(uv.x, _ctxt->getResolution().y - uv.y, 1, 1, GL_RG_INTEGER, GL_UNSIGNED_INT, &r);
    GLSERRORCHECK;
    return hextile::hexpoint_t {(int)r.r, (int)r.g};
}

GameScreenImpl::GameScreenImpl(gamespace::Match &&match, qe::Context *ctxt, std::shared_ptr<font::Font> font)
: _match(std::move(match)), _ctxt(ctxt), _font(font), _ffbo(ctxt->getResolution()), _shouldClose(true)
, _unitmanager(this) {
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
    _terrain_tileno_shader.reset(qe::mkProgram("assets/shaders/terrain.vsh"_p, "assets/shaders/terrain_tileno.fsh"_p, as));
}

void GameScreenImpl::initializeSelection() {
    _selection.selected = nullptr;
    _selection.hovering = nullptr;
    _selection.type = SelectionState::Type::SEL_NONE;
    _selection.from_container = false;
}

void GameScreenImpl::initializeHUD() {
    // fixed UI
    _ui.reset(new ui::UI(ui::Point {(float)_ctxt->width(), (float)_ctxt->height()}));

    std::unique_ptr<ui::Box> box(new ui::Box());
    box->orientation() = ui::Box::VERTICAL;
    box->align_x() = ui::Box::BEGINNING;
    box->align_y() = ui::Box::BEGINNING;
    box->expand() = false;

    std::unique_ptr<ui::Box> upper_box(new ui::Box());
    upper_box->expand() = true;
    upper_box->append(new ui::Text());
    upper_box->orientation() = ui::Box::VERTICAL;
    upper_box->align_x() = ui::Box::BEGINNING;
    upper_box->align_y() = ui::Box::END;
    upper_box->expand() = true;
    upper_box->get("1")->dimension() = ui::Point {1, 0.25};

    std::unique_ptr<ui::Box> lower_box(new ui::Box());
    lower_box->append(new ui::Text());
    lower_box->append(new ui::Text());
    lower_box->orientation() = ui::Box::HORIZONTAL;
    lower_box->align_x() = ui::Box::END;
    lower_box->align_y() = ui::Box::BEGINNING;
    lower_box->expand() = false;
    lower_box->dimension() = ui::Point {1, 0.2};
    lower_box->get("1")->dimension() = ui::Point {0.15, 0.1};
    lower_box->get("1")->margin() = ui::Point {0.003, 0.003};
    lower_box->get("2")->dimension() = ui::Point {0.15, 0.1};
    lower_box->get("2")->margin() = ui::Point {0.003, 0.003};

    box->append(lower_box.release());
    box->append(upper_box.release());
    box->convert_coords(_ui->res());
    box->origin() = ui::Point {0, 0};
    box->dimension() = _ui->res();
    box->hoverable() = false;
    _ui->add_layer("top", box.release());

    auto *t = _ui->get(0, "1.1.1");
    auto *t2 = _ui->get(0, "1.2.1");
    auto *t3 = _ui->get(0, "1.2.2");
    auto text_renderer = [](ui::Renderable *t) mutable {render_text(t, glm::vec3(1, 1, 1));};
    auto text_bgrender = [res = _ctxt->getResolution()](ui::Renderable *t) mutable {
        render_rectangle(t->origin() + t->margin(), t->dimension() - (2.0f * t->margin()), glm::vec3(0.3, 0.3, 0.3), res);
        render_text(t, glm::vec3(1, 1, 1));
    };
    t->render_with(text_renderer);
    t2->render_with(text_bgrender);
    t3->render_with(text_bgrender);
    t->payload([](void* t){delete (text_t*)t;});
    t2->payload([](void* t){delete (text_t*)t;});
    t3->payload([](void* t){delete (text_t*)t;});
    _match.observe_player_change([this, t](Player &np) {
        ((ui::Text*)t)->text() = "Or8Bit - (c) 2017-2018 Team Or8Bit\n"s
            + "LMB to move, RMB to attack\n"
            + "Current player (. for ending a turn): " + np.name();
        ((ui::Text*)t)->delete_payload();
    });
    t->payload() = nullptr;
    t2->payload() = nullptr;
    t3->payload() = nullptr;
    ((ui::Text*)t2)->text() = "Trigger Flood";
    ((ui::Text*)t3)->text() = "End Turn";
    t2->on_click([this](ui::Renderable*) {
        if(_match.can_trigger_map_event())
            _match.trigger_map_event();
    });
    t3->on_click([this](ui::Renderable*) {
        _match.endTurn();
    });
}

void GameScreenImpl::initializeAssets() {
    // MODELS
    _cube.reset(new qe::Mesh<qe::OBJV1>(qe::Loader<qe::OBJV1>("assets/models/cube.objv1"_p)));
    _tile.reset(new qe::Mesh<qe::OBJV2>(qe::Loader<qe::OBJV2>("assets/models/hextile.objv2"_p)));
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
    _match.setRenderOffsets(_unitmanager);
}

void GameScreenImpl::enableActionMask() {
    if(_selection.hovering == nullptr) return; // no tile to select
    if(_selection.type == SelectionState::Type::SEL_TO_ACTION) {
        if(_selection.hovering->marked(MOVE_LAYER)
            && _selection.hovering != _selection.selected
            && (_selection.hovering->unit() == nullptr
                || (_selection.hovering->unit()->player() == _selection.selected->unit()->player()
                        && _selection.hovering->unit()->containerMatchesType(_selection.selected->unit()->name())
                        && _selection.hovering->unit()->container() == nullptr))) { // is move action?
            assert(_selection.selected);
            if(_selection.hovering->unit() == nullptr) // normal move
                _match.doMove(new UnitMove(_selection.selected->coord(), _selection.hovering->coord(), &_match, _selection.from_container));
            else // container move
                _match.doMove(new UnitCMove(_selection.selected->coord(), _selection.hovering->coord(), &_match, _selection.from_container));
        } else if(_selection.hovering->marked(ACTION_LAYER) && _selection.hovering != _selection.selected && _selection.hovering->unit() && _selection.hovering->unit()->player() != _match.currentPlayer()) { // is it attack action?
            assert(_selection.selected);
            _match.doMove(new UnitAttack(_selection.selected->coord(), _selection.hovering->coord(), &_match));
        } else {
            GDBG("selected tile out of range for action");
        }
        _selection.from_container = false;
        _selection.type = SelectionState::Type::SEL_NONE;
        _selection.selected = nullptr;
        _selection.hovering = nullptr;
        _match.board().clearMarker(MOVE_LAYER);
        _match.board().clearMarker(ACTION_LAYER);
    } else {
        _selection.from_container = false;
        if(_selection.hovering->unit() == nullptr) return; // no unit to select
        if(_selection.hovering->unit()->player() != _match.currentPlayer()) return; // can only select own units
        if(_selection.hovering->unit()->getLastTurnId() == _match.getTurnId()) return; // unit already moved this turn
        GDBG("enable movement mask on " << _selection.hovering->coord().string());
        if(_selection.hovering->unit()->isMoveable()) _selection.hovering->unit()->markMovement(*_selection.hovering);
        _selection.selected = _selection.hovering;
        _selection.hovering->unit()->markAttack(*_selection.hovering);
        _selection.type = SelectionState::Type::SEL_TO_ACTION;
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
    if(_selection.hovering == nullptr) return;
    auto *u = _selection.hovering->unit();
    if(u && _ui->hasModelMatching([u](std::string name, void *m){return name == "unitctxt" && ((CoordinateMenu*)m)->unit() == u;}) == false)
        CoordinateMenu::createForTile(_selection.hovering, _ui.get(), _ctxt->getResolution());
}

void GameScreenImpl::run() {
    GDBG("activating game screen");
    _shouldClose = false;
    unsigned int fps = 0;
    glm::mat4 m = glm::translate(glm::vec3(0, 0, 0));
    glClearColor(255.0 / 255, 226.0 / 255, 152.0 / 255, 1);

    _textures.hextile_grass->bindTo();

    const auto max_x = _match.board().x();
    const auto max_y = _match.board().y();
    const auto mc = _match.board().marker_count;
    auto *ptr = _marker_buffer->ptr();

    while(!_ctxt->shouldClose() && _shouldClose == false) {
        // calculate lookat tile (of last frame!!!)
        if(_cam.controlling == false) {
            auto pc = getLookedAtTile(_cam.camera->save());
            _selection.lastLookedAtTile = pc;
            if(pc.x < 0 || pc.y < 0 || pc.x >= (int)max_x || pc.y >= (int)max_y) {
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
                ptr[coord.x * max_y * mc + coord.y * mc + 0] = b->marked(0) ? 1 : 0;
                ptr[coord.x * max_y * mc + coord.y * mc + 1] = b->marked(1) && b->unit() && b->unit()->player() != _match.currentPlayer() ? 1 : 0;
                ptr[coord.x * max_y * mc + coord.y * mc + 2] = b->marked(2) ? 1 : 0;
                ptr[coord.x * max_y * mc + coord.y * mc + 3] = b->marked_value(3);
            }
            _marker_buffer->update();
        }
        // render
        _ctxt->start();
        // render terrain tile to fbo
        _ffbo.bind();
        _ffbo.clear();
        _terrain_render_geometry_pass = true;
        renderTerrain();
        _terrain_render_geometry_pass = false;
        _ffbo.unbind();
        // render everything to normal buffer
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
    qe::Program *ts = _terrain_render_geometry_pass ? _terrain_tileno_shader.get() : _terrain_shader.get();
    ts->use();
    ts->setUniform<qe::UNIMVP>(mvp);
    ts->setUniform<qe::UNIM>(m);
    ts->setUniform<qe::UNIV>(_cam.camera->matrices().v);
    if(_terrain_render_geometry_pass == false) {
        if(_cam.controlling) {
            ts->setUniform<qe::UNISEL>(glm::uvec2(9999, 9999));
        } else {
            ts->setUniform<qe::UNISEL>(to_uvec2(_selection.lastLookedAtTile));
        }
        ts->setUniform<qe::UNICOLOR>(glm::vec3(0.8, 0.8, 0.8));
    }
    // render grey areas
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
    if(_terrain_render_geometry_pass == false) ts->setUniform<qe::UNICOLOR>(glm::vec3(0.8, 0.567, 0.305));
    _ground->render_sub(_ground_indices[Ground_Plane_002]);
    // render water
    if(_terrain_render_geometry_pass == false) ts->setUniform<qe::UNICOLOR>(glm::vec3(0.0, 0.551, 0.8));
    _ground->render_sub(_ground_indices[WaterPlane_Plane_003]);
    _ground->render_sub(_ground_indices[WaterBlock_Cube_004]);
}

void GameScreenImpl::renderUnitOf(gamespace::BoardTile *b) {
    glm::vec2 p = b->centerPos();
    // render unit
    glm::mat4 m = glm::translate(glm::vec3(p.x, 0, p.y)) * b->renderData();
    glm::mat4 mvp = _cam.camera->matrices().pv * m;
    b->unit()->render(*b, mvp, m);
}
void GameScreenImpl::__introspect(size_t off) {
    std::cout << std::string(off, ' ') << "GameScreenImpl" << std::endl;
    _match.__introspect(off + 2);
    if(_cube.get()) _cube->__introspect(off + 2);
    _unitmanager.__introspect(off + 2);
    if(_ground.get()) _ground->__introspect(off + 2);
    if(_ui.get()) _ui->__introspect(off + 2);
    if(_terrain_shader.get()) _terrain_shader->__introspect(off + 2);
}
