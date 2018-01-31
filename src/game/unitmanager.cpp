#include "unitmanager.hpp"

#include<game/screens/game_impl.hpp>

using namespace gamespace;
using namespace std::literals::string_literals;

UnitManager::UnitManager(GameScreenImpl *impl): _impl(impl) {
    _meshes.tank.reset(new qe::Mesh<qe::OBJV3>(qe::Loader<qe::OBJV3>("assets/models/tank.objv3"_p)));
    _meshes.theli.reset(new qe::Mesh<qe::OBJV3>(qe::Loader<qe::OBJV3>("assets/models/theli.objv3"_p)));
    _meshes.btank.reset(new qe::Mesh<qe::OBJV3>(qe::Loader<qe::OBJV3>("assets/models/btank.objv3"_p)));
    _meshes.gtrans.reset(new qe::Mesh<qe::OBJV3>(qe::Loader<qe::OBJV3>("assets/models/gtrans.objv3"_p)));
    _meshes.infantry.reset(new qe::Mesh<qe::OBJV3>(qe::Loader<qe::OBJV3>("assets/models/infantry.objv3"_p)));
    _meshes.aheli.reset(new qe::Mesh<qe::OBJV3>(qe::Loader<qe::OBJV3>("assets/models/aheli.objv3"_p)));
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
    _units.aheli.reset(new gamespace::Unit(_meshes.aheli.get(), nullptr, "Attack Helicopter", false, true,
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
        120, // Health
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
        140,  // Health
        20,  // Defense Points
        10,  // Attack Points
        4,   // Attack Range
        6,   // Visibility
        4,   // Max move distance
        gamespace::defaultFalloff,
        gamespace::defaultFalloff,
        gamespace::defaultFalloff,
        gamespace::defaultFalloff));
    _units.tank->setSpecialAction([](Unit*){return "Convert to turret"s;}, [this](Unit *u) {
        auto *p = u->tile();
        auto *player = &u->player();
        u->tile()->destroyUnit();
        p->setUnit(createBTank(player));
        p->unit()->setLastTurnId(_impl->match().getTurnId());
    });
    _units.gtrans->setContainerMask({"Infantry"});
    _units.theli->setContainerMask({"Infantry"});
    auto moveoutaction = [this](Unit *u) {
        if(u->container() == nullptr) return;
        if(u->getLastTurnId() == _impl->match().getTurnId()) return;
        // mark map
        u->container()->markMovement(*u->tile());
        u->tile()->board().clearMarker(ACTION_LAYER);
        // tell impl to do stuffer()
        auto &s = _impl->selection();
        s.from_container = true;
        s.selected = u->tile();
        s.type = SelectionState::Type::SEL_TO_ACTION;
        GDBG("set up selection flags");
        u->setLastTurnId(_impl->match().getTurnId());
    };
    auto moveoutstring = [this](Unit *u) -> std::string {
        if(u->container() == nullptr) return "Move Out (unavailable)"s;
        if(u->player() != _impl->match().currentPlayer()) return "Move Out (unavailable)"s;
        if(u->getLastTurnId() == _impl->match().getTurnId()) return "Move Out (unavailable)"s;
        return "Move Out"s;
    };
    _units.gtrans->setSpecialAction(moveoutstring, moveoutaction);
    _units.theli->setSpecialAction(moveoutstring, moveoutaction);
}
