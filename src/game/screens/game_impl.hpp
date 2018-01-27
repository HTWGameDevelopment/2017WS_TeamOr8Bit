#ifndef GAME_SCREEN_IMPL_HPP
#define GAME_SCREEN_IMPL_HPP

#include<font/font.hpp>

#include<engine/qe.hpp>

#include<game/unitmanager.hpp>
#include<game/framebuffer_first_pass.hpp>
#include<game/context/coordinatemenu.hpp>
#include<game/match.hpp>

#include<ui/box.hpp>
#include<ui/text.hpp>

#include<memory>

namespace gamespace {

    typedef decltype(qe::Cache::glyphlatin) glyphmap;
    typedef qe::Text<std::remove_pointer<glyphmap>::type> text_t;

    struct SelectionState {
        enum Type { SEL_TO_ACTION, SEL_NONE };
        Type type;
        gamespace::BoardTile *selected;
        gamespace::BoardTile *hovering;
        hextile::hexpoint_t lastLookedAtTile;
        bool from_container;
    };

    class GameScreenImpl {
    private:
        static const char* ground_names[16];
        qe::Context *_ctxt;
        gamespace::Match _match;
        std::shared_ptr<font::Font> _font;
        std::unique_ptr<qe::Mesh<qe::OBJV1>> _cube;
        std::unique_ptr<qe::Mesh<qe::OBJV2>> _tile;
        UnitManager _unitmanager;
        std::unique_ptr<qe::Mesh<qe::OBJV3>> _ground;
        std::unique_ptr<qe::Program> _terrain_shader;
        std::unique_ptr<qe::Program> _terrain_tileno_shader;
        bool _terrain_render_geometry_pass;
        std::unique_ptr<qe::Buffer<GL_UNIFORM_BUFFER, uint32_t>> _marker_buffer;
        qe::FramebufferFirstPass _ffbo;
        std::array<qe::subobj_t, 16> _ground_indices;
        struct CameraData {
            std::unique_ptr<qe::Camera> camera;
            bool controlling;
        } _cam;
        struct Textures {
            std::unique_ptr<qe::Texture<qe::PNGRGBA, qe::DIFFTEXBIND_GL>> hextile_grass;
        } _textures;
        std::unique_ptr<ui::UI> _ui;
        SelectionState _selection;
        bool _shouldClose;
        void render();
        void renderTerrain();
        void renderUnitOf(gamespace::BoardTile *b);
        hextile::hexpoint_t getLookedAtTile(glm::vec2 pc);
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
        void enableActionMask();
        void inCameraMode(bool mode);
        void createContextForLookAt();
        SelectionState &selection() {
            return _selection;
        }
        gamespace::Match &match() {
            return _match;
        }
        qe::Camera *camera() {
            return _cam.camera.get();
        }
        qe::Context *context() {
            return _ctxt;
        }
        ui::UI *ui() {
            return _ui.get();
        }
        void __introspect(size_t off);
    };
}

#endif
