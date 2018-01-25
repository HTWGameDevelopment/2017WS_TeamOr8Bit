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
        } _meshes;
        struct {
            std::unique_ptr<Unit> tank;
            std::unique_ptr<Unit> theli;
        } _units;
    public:
        UnitManager() {
            _meshes.tank.reset(new qe::Mesh<qe::OBJV3>(qe::Loader<qe::OBJV3>("assets/models/tank.objv3"_p)));
            _meshes.theli.reset(new qe::Mesh<qe::OBJV3>(qe::Loader<qe::OBJV3>("assets/models/theli.objv3"_p)));
            _units.tank.reset(new gamespace::Unit(_meshes.tank.get(), nullptr, "Tank",
                100,
                50,
                50,
                3,
                3,
                2,
                gamespace::defaultFalloff,
                gamespace::defaultFalloff,
                gamespace::defaultFalloff,
                gamespace::defaultFalloff));
            _units.theli.reset(new gamespace::Unit(_meshes.theli.get(), nullptr, "Transport Helicopter",
                70,
                20,
                80,
                5,
                7,
                4,
                gamespace::defaultFalloff,
                gamespace::defaultFalloff,
                gamespace::defaultFalloff,
                gamespace::defaultFalloff));
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
        void __introspect(size_t off) {

        }
    };

}

#endif
