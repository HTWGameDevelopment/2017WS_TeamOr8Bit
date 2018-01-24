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

    std::function<bool(BoardTile&,unsigned int&)> getEdgeRelation(unsigned int layer, relation _v, bool noground_block);

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
    public:
        Unit(qe::Mesh<qe::OBJV3> *m, Player *p, std::string name, unsigned int h, unsigned int d, unsigned int a, unsigned int r, unsigned int v, unsigned int t, relation are, relation dr, relation tr, relation vre)
            : _mesh(m), _player(p), _name(name), _max_hp(h), _hp(h), _dp(d), _ap(a), _ar(r), _vr(v), _dpt(t), _a(are), _d(dr), _t(tr), _v(vre) {}
        virtual ~Unit() {}
        void render(BoardTile &tile, glm::mat4 &mvp, glm::mat4 &m);
        void markVisibility(BoardTile &tile);
        void markMovement(BoardTile &tile);
        void markAttack(BoardTile &tile);
        void setLastTurnId(unsigned int i) {_last_turn_id = i;};
        unsigned int getLastTurnId(){return _last_turn_id;};
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
