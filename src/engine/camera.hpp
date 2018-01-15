// Copyright (c) 2017 Fabian Stiewitz
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#ifndef QE_CAMERA_HPP
#define QE_CAMERA_HPP

#include<glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<engine/camerabounds.hpp>

#include<logger.h>
#undef near
#undef far

namespace qe {

    /**
     * \brief Managing camera state (location, rotation) and matrix generation
     */
    class Camera {
    public:
        /**
         * \brief PV and V matrix
         */
        struct Matrices {
            glm::mat4 pv; //!< P * V
            glm::mat4 v; //!< V
        };
    private:
        Matrices _matrices; //!< Final matrices
        glm::mat4 _p; //!< P matrix
        glm::vec3 _pos; //!< Camera position
        glm::vec3 _dir; //!< lookAt direction
        glm::vec3 _right; //!< Right vector
        glm::vec3 _forward; //!< Forward vector
        glm::vec3 _down; //!< Down vecotr
        glm::vec3 _up; //!< Up vector
        glm::vec2 _angles; //!< View angles
        glm::vec3 _planecoord; //!<viewed plane
        CameraBounds _bounds; //!<Extern Camera Bounds
    public:
        /**
         * Constructs a new camera object
         *
         * \param pos Position of camera
         * \param angles View angles
         * \param resolution Screen resolution
         * \param near Near plane distance
         * \param far Far plane distance
         * \param ar Aspect ratio
         * \param fov Field of view
         */
        Camera(glm::vec3 pos, glm::vec2 angles, float near, float far, float ar, float fov)
            : _pos(pos), _angles(angles) {
            _p = glm::perspective<float>(2 * fov * M_PI / 360.0, ar, near, far);
            regenerate();
        }
        /**
         * \brief Callback for mouse movement
         *
         * \param deltaT time difference since last call
         * \param x X coord of pointer
         * \param y Y coord of pointer
         */
        void mouseMoved(double deltaT, double x, double y) {
            _angles.x -= deltaT * x;
            _angles.y -= deltaT * y;

            if(_angles.y >= M_PI / 2.0) _angles.y = M_PI / 2.0;
            else if(_angles.y <= -M_PI / 2.0) _angles.y = -M_PI / 2.0;

            regenerate();
        }
        /**
         * \brief Regenerate matrices
         */
        inline void regenerate() {
            if(_angles.y >= M_PI / 2.0) _angles.y = M_PI / 2.0;
            else if(_angles.y <= -M_PI / 2.0) _angles.y = -M_PI / 2.0;

            if(_pos.y < _bounds.minHeight) _pos.y = 0;

            _dir = generateDirection();
            _right = glm::vec3(sin(_angles.x - M_PI / 2.0),
                               0,
                               cos(_angles.x - M_PI / 2.0));
            _forward = glm::vec3(_dir.x, 0, _dir.z);
            _down = glm::vec3(0, _dir.y, 0);
            _up = glm::cross(_right, _dir);
            _right = glm::normalize(_right);
            _forward = glm::normalize(_forward);
            _down = glm::normalize(_down);
            _up = glm::normalize(_up);
            _matrices.v = glm::lookAt(_pos, _pos + _dir, _up);
            _matrices.pv = _p * _matrices.v;
            _planecoord = getPlaneCoord();
        }
        inline glm::vec3 generateDirection() {
            return glm::vec3(cos(_angles.y) * sin(_angles.x),
                             sin(_angles.y),
                             cos(_angles.y) * cos(_angles.x));
        }
        glm::mat4 getVP() {
            return _matrices.pv;
        }
        void move(glm::vec3 diff) {
            _pos += diff;
            regenerate();
        }
        glm::vec3 pos() {
            return _pos;
        }
        glm::vec3 getPlaneCoord() {
            float delta;

            if(_dir.y != 0) {
                delta = - _pos.y / _dir.y;
            } else delta = 0;

            return glm::vec3(_pos.x + delta * _dir.x,
                             0,
                             _pos.z + delta * _dir.z);
        }
        Matrices &matrices() {
            return _matrices;
        }
        void moveRight(double deltaT) {
            _pos += (float)deltaT * _right;
            regenerate();
        }
        void moveLeft(double deltaT) {
            _pos -= (float)deltaT * _right;
            regenerate();
        }
        void moveForward(double deltaT) {
            _pos += (float)deltaT * _forward;
            regenerate();
        }
        void moveBackward(double deltaT) {
            _pos -= (float)deltaT * _forward;
            regenerate();
        }
        void moveUp(double deltaT) {
            _pos -= (float)deltaT * _down;
            regenerate();
        }
        void moveDown(double deltaT) {
            _pos += (float)deltaT * _down;
            regenerate();
        }
        void scrollIn(double deltaT) {
            _pos += (float)deltaT * _dir;
            regenerate();
        }
        void scrollOut(double deltaT) {
            _pos -= (float)deltaT * _dir;
            regenerate();
        }
    };

}

#endif
