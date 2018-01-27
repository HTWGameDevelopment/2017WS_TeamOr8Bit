#ifndef OR8BIT_UNIT_HPP
#define OR8BIT_UNIT_HPP

#include<functional>
#include<game/gameboard.hpp>
#include<game/player.hpp>
#include<game/terrain.hpp>

#include<engine/cache.hpp>

#include<glm/mat4x4.hpp>

namespace gamespace {

    unsigned int defaultFalloff(BoardTile &t);

    typedef std::function<unsigned int(BoardTile &)> relation;

    std::function<bool(BoardTile&,unsigned int&)> getEdgeRelation(Unit *u, unsigned int layer, relation _v, Player *player, bool noground_block, bool unit_block);

    struct cb_t {
        unsigned int id;
        std::function<void(bool)> f;
    };

    struct saction_t {
        std::string name;
        std::function<void(Unit*)> f;
    };

    class Unit {
    private:
        qe::Mesh<qe::OBJV3> *_mesh;
        Player *_player;
        std::string _name;
        unsigned int _max_hp; //!< Maximum HP
        unsigned int _hp; //!< Health points
        unsigned int _dp; //!< Defense points
        unsigned int _ap; //!< Attack points
        unsigned int _ar; //!< Attack range
        unsigned int _vr; //!< Visibility range
        unsigned int _dpt; //!< travel distance per turn
        unsigned int _last_turn_id;
        relation _a; //!< attack range relation
        relation _d; //!< attack damage relation
        relation _t; //!< travel distance relation
        relation _v; //!< visibility relation
        BoardTile *_tile; //!< Board tile
        bool _consider_noground;
        bool _moveable;
        unsigned int _lid;
        std::vector<cb_t> _on_change;
        std::vector<saction_t> _actions;
        std::vector<const char*> _container_mask;
        Unit *_container;
    public:
        Unit(qe::Mesh<qe::OBJV3> *m, Player *p, std::string name, bool consider_noground, bool moveable, unsigned int h, unsigned int d, unsigned int a, unsigned int r, unsigned int v, unsigned int t, relation are, relation dr, relation tr, relation vre)
            : _mesh(m), _player(p), _name(name)
            , _max_hp(h), _hp(h), _dp(d), _ap(a), _ar(r), _vr(v), _dpt(t), _a(are), _d(dr), _t(tr), _v(vre)
            , _consider_noground(consider_noground), _moveable(moveable), _container(nullptr) {}
        virtual ~Unit() {
            if(_container) delete _container;
            emit_change(false);
        }
        void render(BoardTile &tile, glm::mat4 &mvp, glm::mat4 &m);
        void markVisibility(BoardTile &tile);
        void markMovement(BoardTile &tile);
        void markAttack(BoardTile &tile);
        bool considersNoground() {
            return _consider_noground;
        }
        bool isMoveable() {
            return _moveable;
        }
        void setContainerMask(std::initializer_list<const char*> l) {
            _container_mask = l;
        }
        bool containerMatchesType(std::string name) {
            for(auto *i : _container_mask) {
                if(name == i) return true;
            }
            return false;
        }
        Unit *&container() {
            return _container;
        }
        template<typename S, typename F>
        void setSpecialAction(S name, F &&f) {
            _actions.push_back(saction_t {name, f});
        }
        std::vector<saction_t> &getSpecialActions() {
            return _actions;
        }
        void setLastTurnId(unsigned int i) {_last_turn_id = i;};
        unsigned int getLastTurnId(){return _last_turn_id;};
        template<typename F>
        unsigned int on_change(F &&f) {
            _on_change.push_back(cb_t {_lid, f});
            return _lid++;
        }
        void emit_change(bool alive = true) {
            if(_container && alive == true) _container->emit_change(alive);
            for(auto &f : _on_change) {
                f.f(alive);
            }
        }
        void on_change_r(unsigned int id) {
            for(auto b = _on_change.begin(); b != _on_change.end(); ++b) {
                if(b->id == id) {
                    _on_change.erase(b);
                    return;
                }
            }
        }
        void setPlayer(Player *player) {
            _player = player;
        }
        Player &player() {
            return *_player;
        }
        BoardTile *&tile() {
            return _tile;
        }
        void attackedWith(Unit &o) {
            unsigned int off = o._ap;
            if(_hp < off) {
                _hp = 0;
                return;
            }
            _hp -= off;
        }
        bool dead() {
            return _hp == 0;
        }
        void __introspect(size_t off);
        unsigned int max_hp() {return _max_hp;}
        unsigned int hp() {return _hp;}
        unsigned int dp() {return _dp;}
        unsigned int ap() {return _ap;}
        unsigned int ar() {return _ar;}
        unsigned int vr() {return _vr;}
        unsigned int dpt() {return _dpt;}
        std::string name() {return _name;}
    };

}

#endif //OR8BIT_UNIT_HPP
