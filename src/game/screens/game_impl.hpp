#ifndef GAME_SCREEN_IMPL_HPP
#define GAME_SCREEN_IMPL_HPP

#include<font/font.hpp>

#include<engine/qe.hpp>

#include<game/context/coordinatemenu.hpp>
#include<game/match.hpp>

#include<ui/abstractbox.hpp>
#include<ui/abstracttext.hpp>
#include<ui/definedtext.hpp>
#include<ui/uifactory.hpp>

#include<memory>

namespace gamespace {

    typedef decltype(qe::Cache::glyphlatin) glyphmap;
    typedef qe::Text<std::remove_pointer<glyphmap>::type> text_t;

    class GameScreenImpl {
    private:
        static const char* ground_names[16];
        qe::Context *_ctxt;
        gamespace::Match _match;
        std::shared_ptr<font::Font> _font;
        std::unique_ptr<qe::Mesh<qe::OBJV1>> _cube;
        std::unique_ptr<qe::Mesh<qe::OBJV2>> _tile;
        std::unique_ptr<qe::Mesh<qe::OBJV3>> _tank;
        std::unique_ptr<qe::Mesh<qe::OBJV3>> _ground;
        std::unique_ptr<qe::Program> _terrain_shader;
        std::unique_ptr<qe::Buffer<GL_UNIFORM_BUFFER, uint32_t>> _marker_buffer;
        std::array<qe::subobj_t, 16> _ground_indices;
        struct CameraData {
            std::unique_ptr<qe::Camera> camera;
            bool controlling;
        } _cam;
        struct Textures {
            std::unique_ptr<qe::Texture<qe::PNGRGBA, qe::DIFFTEXBIND_GL>> hextile_grass;
        } _textures;
        std::unique_ptr<ui::DefinedUI> _ui;
        std::unique_ptr<CoordinateMenu> _coordinate_menu;
        struct SelectionState {
            enum Type { SEL_TO_MOVE, SEL_TO_ATTACK, SEL_NONE };
            Type type;
            gamespace::BoardTile *selected;
            gamespace::BoardTile *hovering;
        } _selection;
        bool _shouldClose;
        void render();
        void renderTerrain();
        void renderUnitOf(gamespace::BoardTile *b);
    public:
        GameScreenImpl(gamespace::Match &&match, qe::Context *ctxt, std::shared_ptr<font::Font> font);
        void pre_run();
        void run();
        void deactivate();
        void initializeShaders();
        void initializeBuffers();
        void initializeAssets();
        void initializeMap();
        void initializeSelection();
        void initializeHUD();
        void enableMoveMask();
        void enableAttackMask();
        void inCameraMode(bool mode);
        gamespace::Match &match() {
            return _match;
        }
        qe::Camera *camera() {
            return _cam.camera.get();
        }
        qe::Context *context() {
            return _ctxt;
        }
        CoordinateMenu *coordinatemenu() {
            return _coordinate_menu.get();
        }
        void __introspect(size_t off);
    };
}

#endif
