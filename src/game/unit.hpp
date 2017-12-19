#ifndef OR8BIT_UNIT_HPP
#define OR8BIT_UNIT_HPP

#include<functional>
#include<game/board.hpp>
#include<game/terrain.hpp>

#include<engine/cache.hpp>

#include<glm/mat4x4.hpp>

namespace gamespace {

    class Unit {
    private:
        qe::Mesh<qe::OBJV3> *_mesh;
        unsigned int hp; //!< Health points
        unsigned int dp; //!< Defense points
        unsigned int ap; //!< Attack points
        unsigned int ar; //!< Attack range
        unsigned int vr; //!< Visibility range
        unsigned int dpt; //!< travel distance per turn
        std::function<unsigned int(Terrain &)> _a; //!< attack range relation
        std::function<unsigned int(Terrain &)> _d; //!< attack damage relation
        std::function<unsigned int(Terrain &)> _t; //!< travel distance relation
        std::function<unsigned int(Terrain &)> _v; //!< visibility relation
    public:
        void render(BoardTile &tile, glm::mat4 &mvp, glm::mat4 &m) {
            qe::Cache::objv3->use();
            qe::Cache::objv3->setUniform<qe::UNIMVP>(mvp);
            qe::Cache::objv3->setUniform<qe::UNIM>(m);
            qe::Cache::objv3->setUniform<qe::UNICOLOR>(glm::vec3(0.7, 0.7, 0.7));
            _mesh->render();
        }
    };

}


#endif //OR8BIT_UNIT_HPP
