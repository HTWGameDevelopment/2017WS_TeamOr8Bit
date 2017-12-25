#ifndef GAME_SCREEN_IMPL_HPP
#define GAME_SCREEN_IMPL_HPP

#include<font/font.hpp>

#include<engine/qe.hpp>

#include<game/match.hpp>

#include<memory>

namespace gamespace {
    class GameScreenImpl {
    private:
        qe::Context *_ctxt;
        gamespace::Match _match;
        std::shared_ptr<font::Font> _font;
        std::unique_ptr<qe::Mesh<qe::OBJV1>> _cube;
        std::unique_ptr<qe::Mesh<qe::OBJV2>> _tile;
        std::unique_ptr<qe::Mesh<qe::OBJV3>> _tank;
        struct CameraData {
            std::unique_ptr<qe::Camera> camera;
        } _cam;
        struct Textures {
            std::unique_ptr<qe::Texture<qe::PNGRGBA, qe::DIFFTEXBIND_GL>> hextile_grass;
        } _textures;
        struct Text {
            qe::Text<qe::GlyphmapLatin> gamename;
            qe::Text<qe::GlyphmapLatin> help1;
            qe::Text<qe::GlyphmapLatin> help2;
            std::string help2_tmp;
        } _strings;
        struct SelectionState {
            enum Type { SEL_TO_MOVE, SEL_TO_ATTACK, SEL_NONE };
            Type type;
            gamespace::BoardTile *selected;
            gamespace::BoardTile *hovering;
        } _selection;
        bool _shouldClose;
        void render();
        void renderTile(gamespace::BoardTile *b, glm::vec3 ho);
    public:
        GameScreenImpl(gamespace::Match &&match, qe::Context *ctxt, std::shared_ptr<font::Font> font);
        void run();
        void deactivate();
        void initializeAssets();
        void initializeMap();
        void initializeSelection();
        void initializeHUD();
        void enableMoveMask();
        void enableAttackMask();
        gamespace::Match &match() {
            return _match;
        }
        qe::Camera *camera() {
            return _cam.camera.get();
        }
        qe::Context *context() {
            return _ctxt;
        }
    };
}

#endif
