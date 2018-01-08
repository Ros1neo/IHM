
// Copyright (C) 2008 Gael Guennebaud <gael.guennebaud@inria.fr>
//
// Eigen is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// Alternatively, you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// Eigen is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License or the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License and a copy of the GNU General Public License along with
// Eigen. If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include "eigen3/Eigen/LU"
#include "camera.h"

using namespace Eigen;

Camera::Camera()
    : mViewIsUptodate(false), mProjIsUptodate(false)
{
    mViewMatrix.setIdentity();

    mFovY = M_PI/3.;
    mNearDist = 0.1;
    mFarDist = 50000.;

    mVpX = 0;
    mVpY = 0;

//    lookAt(Vector3f::Constant(100.), Vector3f::Zero(), Vector3f::UnitZ());
//    lookAt(Vector3f(0, 0, 0), Vector3f(0, 0, 1), Vector3f::UnitY());
}

Camera& Camera::operator=(const Camera& other)
{
    mViewIsUptodate = false;
    mProjIsUptodate = false;

    mVpX = other.mVpX;
    mVpY = other.mVpY;
    mVpWidth = other.mVpWidth;
    mVpHeight = other.mVpHeight;

    mTarget = other.mTarget;
    mFovY = other.mFovY;
    mNearDist = other.mNearDist;
    mFarDist = other.mFarDist;

    mViewMatrix = other.mViewMatrix;
    mProjectionMatrix = other.mProjectionMatrix;

    return *this;
}

Camera::Camera(const Camera& other)
{
    *this = other;
}

Camera::~Camera()
{
}

void Camera::setPerspective(float fovY, float near, float far)
{
    mFovY = fovY;
    mNearDist = near;
    mFarDist = far;
    mProjIsUptodate = false;
}

void Camera::setViewport(uint offsetx, uint offsety, uint width, uint height)
{
    mVpX = offsetx;
    mVpY = offsety;
    mVpWidth = width;
    mVpHeight = height;

    mProjIsUptodate = false;
}

void Camera::setViewport(uint width, uint height)
{
    mVpWidth = width;
    mVpHeight = height;

    mProjIsUptodate = false;
}

void Camera::setFovY(float value)
{
    mFovY = value;
    mProjIsUptodate = false;
}

Vector3f Camera::direction() const
{
    updateViewMatrix();
    return -mViewMatrix.linear().row(2);
}
Vector3f Camera::up() const
{
    updateViewMatrix();
    return mViewMatrix.linear().row(1);
}
Vector3f Camera::right() const
{
    updateViewMatrix();
    return mViewMatrix.linear().row(0);
}

void Camera::lookAt(const Eigen::Vector3f& position, const Eigen::Vector3f& target, const Eigen::Vector3f& up)
{
    mTarget = target;
    mFrame.position = position;
    Matrix3f R;
    R.col(2) = (position-target).normalized();
    R.col(0) = up.cross(R.col(2)).normalized();
    R.col(1) = R.col(2).cross(R.col(0));
    setOrientation(Quaternionf(R));
    mViewIsUptodate = false;
}

void Camera::setPosition(const Vector3f& p)
{
    mFrame.position = p;
    mViewIsUptodate = false;
//    cout << "cam pos : " << p.transpose() << endl;
}

void Camera::setTarget(const Vector3f &target)
{
    mTarget = target;
//    cout << "cam target : " << target.transpose() << endl;
}

void Camera::setOrientation(const Quaternionf& q)
{
    mFrame.orientation = q;
    mViewIsUptodate = false;
//    cout << "cam orientation : " << q.matrix() << endl;
}

void Camera::setFrame(const Frame& f)
{
    mFrame = f;
    mViewIsUptodate = false;
}

void Camera::rotateAroundTarget(const Quaternionf& q)
{
    Matrix4f mrot, mt, mtm;

    // update the transform matrix
    updateViewMatrix();
    Vector3f t = mViewMatrix * mTarget;

    mViewMatrix = Translation3f(t)
            * q
            * Translation3f(-t)
            * mViewMatrix;

    Quaternionf qa(mViewMatrix.linear());
    qa = qa.conjugate();
    setOrientation(qa);
    setPosition(- (qa * mViewMatrix.translation()) );

    mViewIsUptodate = true;
}

void Camera::localRotate(const Quaternionf& q)
{
    float dist = (position() - mTarget).norm();
    setOrientation(orientation() * q);
    mTarget = position() + dist * direction();
    mViewIsUptodate = false;
}

void Camera::zoom(float d)
{
    float dist = (position() - mTarget).norm();
    if(dist > d)
    {
        setPosition(position() + direction() * d);
        mViewIsUptodate = false;
    }
}

void Camera::localTranslate(const Vector3f& t)
{
    Vector3f trans = orientation() * t;
    setPosition( position() + trans );
    mTarget += trans;

    mViewIsUptodate = false;
}

void Camera::updateViewMatrix() const
{
    if(!mViewIsUptodate)
    {
        Quaternionf q = orientation().conjugate();
        mViewMatrix.linear() = q.toRotationMatrix();
        mViewMatrix.translation() = - (mViewMatrix.linear() * position());

        mViewIsUptodate = true;
    }
}

const Affine3f& Camera::viewMatrix() const
{
    updateViewMatrix();
    return mViewMatrix;
}

void Camera::updateProjectionMatrix() const
{

    if(!mProjIsUptodate)
    {
        mProjectionMatrix.setIdentity();
        float aspect = float(mVpWidth)/float(mVpHeight);
        float theta = mFovY*0.5;
        float range = mFarDist - mNearDist;
        float invtan = 1./tan(theta);

        mProjectionMatrix(0,0) = invtan / aspect;
        mProjectionMatrix(1,1) = invtan;
        mProjectionMatrix(2,2) = -(mNearDist + mFarDist) / range;
        mProjectionMatrix(3,2) = -1;
        mProjectionMatrix(2,3) = -2 * mNearDist * mFarDist / range;
        mProjectionMatrix(3,3) = 0;
        mProjIsUptodate = true;
    }
}

const Matrix4f& Camera::projectionMatrix() const
{
    updateProjectionMatrix();
    return mProjectionMatrix;
}

Vector3f Camera::unProject(const Vector2f& uv, float depth) const
{
    Matrix4f inv = mViewMatrix.inverse().matrix();
    return unProject(uv, depth, inv);
}

Vector3f Camera::unProject(const Vector2f& uv, float depth, const Matrix4f& invModelview) const
{
    updateViewMatrix();
    updateProjectionMatrix();

    Vector3f a(2.*uv.x()/float(mVpWidth)-1., 2.*uv.y()/float(mVpHeight)-1., 1.);
    a.x() *= depth/mProjectionMatrix(0,0);
    a.y() *= depth/mProjectionMatrix(1,1);
    a.z() = -depth;
    //Vector4f b = invModelview * Vector4f(a.x(), a.y(), a.z(), 1.);
    //Vector3f b = mViewMatrix.linear().transpose() * a + position();//Vector4f(a.x(), a.y(), a.z(), 1.);

    Vector3f b = a.x() * right() + a.y() * up() - a.z() * direction() + position();

    return Vector3f(b.x(), b.y(), b.z());
}
