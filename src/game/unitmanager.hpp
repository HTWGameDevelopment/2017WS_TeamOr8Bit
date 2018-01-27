#ifndef GAMESPACE_UNITMANAGER_HPP
#define GAMESPACE_UNITMANAGER_HPP

#include<engine/qe.hpp>

#include<game/unit.hpp>

namespace gamespace {

    class UnitManager {
    private:
        struct {
            std::unique_ptr<qe::Mesh<qe::OBJV3>> tank;
            std::unique_ptr<qe::Mesh<qe::OBJV3>> theli;
            std::unique_ptr<qe::Mesh<qe::OBJV3>> btank;
            std::unique_ptr<qe::Mesh<qe::OBJV3>> gtrans;
            std::unique_ptr<qe::Mesh<qe::OBJV3>> infantry;
        } _meshes;
        struct {
            std::unique_ptr<Unit> tank;
            std::unique_ptr<Unit> theli;
            std::unique_ptr<Unit> btank;
            std::unique_ptr<Unit> gtrans;
            std::unique_ptr<Unit> infantry;
        } _units;
    public:
        UnitManager() {
            _meshes.tank.reset(new qe::Mesh<qe::OBJV3>(qe::Loader<qe::OBJV3>("assets/models/tank.objv3"_p)));
            _meshes.theli.reset(new qe::Mesh<qe::OBJV3>(qe::Loader<qe::OBJV3>("assets/models/theli.objv3"_p)));
            _meshes.btank.reset(new qe::Mesh<qe::OBJV3>(qe::Loader<qe::OBJV3>("assets/models/btank.objv3"_p)));
            _meshes.gtrans.reset(new qe::Mesh<qe::OBJV3>(qe::Loader<qe::OBJV3>("assets/models/gtrans.objv3"_p)));
            _meshes.infantry.reset(new qe::Mesh<qe::OBJV3>(qe::Loader<qe::OBJV3>("assets/models/infantry.objv3"_p)));
            _units.tank.reset(new gamespace::Unit(_meshes.tank.get(), nullptr, "Tank", true, true,
                100, // Health
                50,  // Defense Points
                50,  // Attack Points
                2,   // Attack Range
                3,   // Visibility
                2,   // Max move distance
                gamespace::defaultFalloff,
                gamespace::defaultFalloff,
                gamespace::defaultFalloff,
                gamespace::defaultFalloff));
            _units.infantry.reset(new gamespace::Unit(_meshes.infantry.get(), nullptr, "Infantry", true, true,
                50, // Health
                20,  // Defense Points
                20,  // Attack Points
                1,   // Attack Range
                2,   // Visibility
                1,   // Max move distance
                gamespace::defaultFalloff,
                gamespace::defaultFalloff,
                gamespace::defaultFalloff,
                gamespace::defaultFalloff));
            _units.btank.reset(new gamespace::Unit(_meshes.btank.get(), nullptr, "Turret", true, false,
                200, // Health
                80,  // Defense Points
                70,  // Attack Points
                3,   // Attack Range
                3,   // Visibility
                2,   // Max move distance
                gamespace::defaultFalloff,
                gamespace::defaultFalloff,
                gamespace::defaultFalloff,
                gamespace::defaultFalloff));
            _units.gtrans.reset(new gamespace::Unit(_meshes.gtrans.get(), nullptr, "Ground Transporter", true, true,
                60, // Health
                80,  // Defense Points
                20,  // Attack Points
                1,   // Attack Range
                2,   // Visibility
                3,   // Max move distance
                gamespace::defaultFalloff,
                gamespace::defaultFalloff,
                gamespace::defaultFalloff,
                gamespace::defaultFalloff));
            _units.theli.reset(new gamespace::Unit(_meshes.theli.get(), nullptr, "Transport Helicopter", false, true,
                70,  // Health
                20,  // Defense Points
                80,  // Attack Points
                4,   // Attack Range
                6,   // Visibility
                4,   // Max move distance
                gamespace::defaultFalloff,
                gamespace::defaultFalloff,
                gamespace::defaultFalloff,
                gamespace::defaultFalloff));
            _units.tank->setSpecialAction("Convert to turret", [this](Unit *u) {
                auto *p = u->tile();
                auto *player = &u->player();
                u->tile()->destroyUnit();
                p->setUnit(createBTank(player));
            });
            _units.gtrans->setContainerMask({"Infantry"});
            _units.gtrans->setSpecialAction("Move out", [this](Unit *u) {
                // mark map
                // tell impl to do stuff
            });
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

        }
    };

}

#endif
