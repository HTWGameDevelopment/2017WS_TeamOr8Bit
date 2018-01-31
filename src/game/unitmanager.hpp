#ifndef GAMESPACE_UNITMANAGER_HPP
#define GAMESPACE_UNITMANAGER_HPP

#include<engine/qe.hpp>

#include<game/unit.hpp>

namespace gamespace {

    class GameScreenImpl;

    class UnitManager {
    private:
        struct {
            std::unique_ptr<qe::Mesh<qe::OBJV3>> tank;
            std::unique_ptr<qe::Mesh<qe::OBJV3>> theli;
            std::unique_ptr<qe::Mesh<qe::OBJV3>> btank;
            std::unique_ptr<qe::Mesh<qe::OBJV3>> gtrans;
            std::unique_ptr<qe::Mesh<qe::OBJV3>> infantry;
            std::unique_ptr<qe::Mesh<qe::OBJV3>> aheli;
        } _meshes;
        struct {
            std::unique_ptr<Unit> tank;
            std::unique_ptr<Unit> theli;
            std::unique_ptr<Unit> btank;
            std::unique_ptr<Unit> gtrans;
            std::unique_ptr<Unit> infantry;
            std::unique_ptr<Unit> aheli;
        } _units;
        GameScreenImpl *_impl;
    public:
        UnitManager(GameScreenImpl *impl);
        Unit *createAHeli(Player *p) {
            auto *t = new Unit(*_units.aheli);
            t->setPlayer(p);
            return t;
        }
        Unit *createTank(Player *p) {
            auto *t = new Unit(*_units.tank);
            t->setPlayer(p);
            return t;
        }
        Unit *createTHeli(Player *p) {
            auto *t = new Unit(*_units.theli);
            t->setPlayer(p);
            return t;
        }
        Unit *createBTank(Player *p) {
            auto *t = new Unit(*_units.btank);
            t->setPlayer(p);
            return t;
        }
        Unit *createGTrans(Player *p) {
            auto *t = new Unit(*_units.gtrans);
            t->setPlayer(p);
            return t;
        }
        Unit *createInfantry(Player *p) {
            auto *t = new Unit(*_units.infantry);
            t->setPlayer(p);
            return t;
        }
        void __introspect(size_t off) {
            (void)off;
        }
    };

}

#endif
