#ifndef OR8BIT_UNIT_HPP
#define OR8BIT_UNIT_HPP

#include<functional>
#include<game/board.hpp>
#include<game/terrain.hpp>

#include<engine/cache.hpp>

#include<glm/mat4x4.hpp>

namespace gamespace {

    unsigned int defaultFalloff(Terrain &t) {
        return 1;
    }

    typedef std::function<unsigned int(Terrain &)> relation;
    class Unit {
    private:
        qe::Mesh<qe::OBJV3> *_mesh;
        unsigned int hp; //!< Health points
        unsigned int dp; //!< Defense points
        unsigned int ap; //!< Attack points
        unsigned int ar; //!< Attack range
        unsigned int vr; //!< Visibility range
        unsigned int dpt; //!< travel distance per turn
        relation _a; //!< attack range relation
        relation _d; //!< attack damage relation
        relation _t; //!< travel distance relation
        relation _v; //!< visibility relation
    public:
        Unit(qe::Mesh<qe::OBJV3> *m, unsigned int h, unsigned int d, unsigned int a, unsigned int r, unsigned int v, unsigned int t, relation are, relation dr, relation tr, relation vre)
            : _mesh(m), hp(h), dp(d), ap(a), ar(r), vr(v), dpt(t), _a(are), _d(dr), _t(tr), _v(vre) {}
        void render(BoardTile &tile, glm::mat4 &mvp, glm::mat4 &m) {
            qe::Cache::objv3->use();
            qe::Cache::objv3->setUniform<qe::UNIMVP>(mvp);
            qe::Cache::objv3->setUniform<qe::UNIM>(m);
            qe::Cache::objv3->setUniform<qe::UNICOLOR>(glm::vec3(0.448, 0.844, 1));
            _mesh->render();
        }
    };

}

#endif //OR8BIT_UNIT_HPP
