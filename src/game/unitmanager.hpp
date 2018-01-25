#ifndef GAMESPACE_UNITMANAGER_HPP
#define GAMESPACE_UNITMANAGER_HPP

#include<engine/qe.hpp>

#include<game/unit.hpp>

namespace gamespace {

    class UnitManager {
    private:
        struct {
            std::unique_ptr<qe::Mesh<qe::OBJV3>> tank;
        } _meshes;
        struct {
            std::unique_ptr<Unit> tank;
        } _units;
    public:
        UnitManager() {
            _meshes.tank.reset(new qe::Mesh<qe::OBJV3>(qe::Loader<qe::OBJV3>("assets/models/tank.objv3"_p)));
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
        }
        Unit *createTank(Player *p) {
            auto *t = new Unit(*_units.tank);
            t->setPlayer(p);
            return t;
        }
        void __introspect(size_t off) {
            
        }
    };

}

#endif
