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
            glm::mat4 op; //!< orthogonal perspective
            glm::mat4 ov; //!< orthogonal view
            glm::mat4 opv; //!< orthogonal P * V
            glm::mat4 ip; //!< inverse of P
            glm::mat4 iv; //!< inverse of V
            glm::mat4 pv; //!< P * V
            glm::mat4 v; //!< V
        };
    private:
        enum CameraMode { PERSPECTIVE, ORTHO } _mode;
        Matrices _matrices; //!< Final matrices
        glm::mat4 _p; //!< P matrix
        glm::vec3 _pos; //!< Camera position
        glm::vec3 _dir; //!< lookAt direction
        glm::vec3 _right; //!< Right vector
        glm::vec3 _up; //!< Up vector
        glm::vec2 _angles; //!< View angles
        glm::vec2 _res; //!< Screen resolution
        glm::vec3 _planecoord; //!<viewed plane
        CameraBounds _bounds; //!<Extern Camera Bounds
        bool _locked; //!< Matrices locked?
        glm::vec2 _save; //!< Holds mouse position for movement
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
        Camera(glm::vec3 pos, glm::vec2 angles, glm::vec2 res, float near, float far, float ar, float fov)
            : _mode(PERSPECTIVE), _pos(pos), _angles(angles), _res(res), _locked(false), _save(0, 0) {
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
        void mouseMoved(double deltaT, double x, double y, bool unlocked_matrix = true) {
            _locked = !unlocked_matrix;
            _save.x = x;
            _save.y = y;
            if(unlocked_matrix) {
                _angles.x -= deltaT * x;
                _angles.y -= deltaT * y;

                if(_angles.y >= M_PI / 2.0) _angles.y = M_PI / 2.0;
                else if(_angles.y <= -M_PI / 2.0) _angles.y = -M_PI / 2.0;

                regenerate();
            } else {
                glm::vec2 rf = (glm::vec2(x, _res.y - y) - (_res / 2.0f)) / (_res / 2.0f);
                glm::vec4 r = _matrices.ip * glm::vec4(rf.x, rf.y, -1, 1);
                r = _matrices.iv * glm::vec4(r.x, r.y, -1, 1);
                _dir = glm::normalize(glm::vec3(r.x, r.y, r.z) - _pos);
                _planecoord = getPlaneCoord();
            }
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
            _up = glm::cross(_right, _dir);
            _right = glm::normalize(_right);
            _up = glm::normalize(_up);
            _matrices.v = glm::lookAt(_pos, _pos + _dir, _up);
            _matrices.pv = _p * _matrices.v;
            _matrices.ip = glm::inverse(_p);
            _matrices.iv = glm::inverse(_matrices.v);
            _planecoord = getPlaneCoord();
            if(_locked) mouseMoved(0, _save.x, _save.y, false);
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
            _pos += (float)deltaT * _dir;
            regenerate();
        }
        void moveBackward(double deltaT) {
            _pos -= (float)deltaT * _dir;
            regenerate();
        }
        void moveUp(double deltaT) {
            _pos += (float)deltaT * _up;
            regenerate();
        }
        void moveDown(double deltaT) {
            _pos -= (float)deltaT * _up;
            regenerate();
        }
        void setOrtho(glm::vec3 o, glm::vec3 d) {
            _matrices.op = _matrices.opv = glm::ortho(o.x, o.x + d.x,
                                                      o.y, o.y + d.y,
                                                      o.z, o.z + d.z);
            _matrices.v = glm::translate(glm::vec3(0, 0, 0));
        }
        void setToOrtho() {
            // TODO
        }
        void setToPerspective() {
            // TODO
        }
    };

}

#endif
