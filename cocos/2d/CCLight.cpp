/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "2d/CCLight.h"
#include <cmath>
#include "2d/CCScene.h"
#include "base/ccUtils.h"

NS_CC_BEGIN

namespace
{
    experimental::FrameBuffer* createFrameBufferObject(unsigned int width, unsigned int height)
    {
        using namespace experimental;
        if (RenderTarget* depthStencilTarget = RenderTarget::create(width, height, Texture2D::PixelFormat::D24S8))
        {
            if (FrameBuffer* fbo = FrameBuffer::create(1, width, height))
            {
                fbo->attachDepthStencilTarget(depthStencilTarget);
                return fbo;
            }
        }

		return nullptr;
    }
}

void BaseLight::setIntensity(float intensity)
{
    CC_ASSERT(intensity >= 0);
    _intensity = intensity;
}

void BaseLight::onEnter()
{
    auto scene = getScene();
    if (scene)
    {
        auto &lights = scene->_lights;
        auto iter = std::find(lights.begin(), lights.end(), this);
        if (iter == lights.end())
            lights.push_back(this);

        scene->setLightOrderDirty();
    }
    Node::onEnter();
}
void BaseLight::onExit()
{
    auto scene = getScene();
    if (scene)
    {
        auto &lights = scene->_lights;
        auto iter = std::find(lights.begin(), lights.end(), this);
        if (iter != lights.end())
            lights.erase(iter);
    }
    Node::onExit();
}

void BaseLight::setCastShadow(bool castShadow)
{
    if (getCastShadow() != castShadow)
    {
        Node::setCastShadow(castShadow);

        Scene* scene = getScene();
        if (scene)
        {
            scene->setLightOrderDirty();
        }
    }
}

void BaseLight::setRotationFromDirection( const Vec3 &direction )
{
    float projLen = std::sqrt(direction.x * direction.x + direction.z * direction.z);
    float rotY = CC_RADIANS_TO_DEGREES(atan2f(-direction.x, -direction.z));
    float rotX = -CC_RADIANS_TO_DEGREES(atan2f(-direction.y, projLen));
    setRotation3D(Vec3(rotX, rotY, 0.0f));
}

BaseLight::BaseLight()
: _intensity(1.0f)
, _lightFlag(LightFlag::LIGHT0)
, _enabled(true)
, _shadowMapSize(ShadowSize::High_1024x1024)
, _shadowBias(0.0f)
{
    
}
BaseLight::~BaseLight()
{
    
}


////////////////////////////////////////////////////////////////////
DirectionLight* DirectionLight::create(const Vec3 &direction, const Color3B &color)
{
    auto light = new (std::nothrow) DirectionLight();
    light->setRotationFromDirection(direction);
    light->setColor(color);
    light->autorelease();
    return light;
}

void DirectionLight::setDirection(const Vec3 &dir)
{
    setRotationFromDirection(dir);
}
Vec3 DirectionLight::getDirection() const
{
    Mat4 mat = getNodeToParentTransform();
    return Vec3(-mat.m[8], -mat.m[9], -mat.m[10]);
}
Vec3 DirectionLight::getDirectionInWorld() const
{
    Mat4 mat = getNodeToWorldTransform();
    return Vec3(-mat.m[8], -mat.m[9], -mat.m[10]);
}
DirectionLight::DirectionLight()
{
    
}
DirectionLight::~DirectionLight()
{
    
}

void DirectionLight::updateShadowCamera()
{
    if (isEnabled() && getCastShadow())
    {
        const unsigned int size = static_cast<unsigned int>(_shadowMapSize);
        if (!_shadowCamera || _shadowCamera->getFrameBufferObject()->getWidth() != size)
        {
            _shadowCamera = nullptr; // avoid holding 2 fbo at the same time
            if (experimental::FrameBuffer* fbo = createFrameBufferObject(size, size))
            {
                if (_shadowCamera = Camera::createOrthographicOffCenter(-500.0f, 500.0f, -500.0f, 500.0f, 0.0f, 50000.0f))
                {
                    _shadowCamera->setFrameBufferObject(fbo);
                    _shadowCamera->setCastShadow(true);
                }
            }
        }
    }
    else
    {
        _shadowCamera = nullptr;
    }
}

//////////////////////////////////////////////////////////////////
PointLight* PointLight::create(const Vec3 &position, const Color3B &color, float range)
{
    auto light = new (std::nothrow) PointLight();
    light->setPosition3D(position);
    light->setColor(color);
    light->_range = range;
    light->autorelease();
    return light;
}

PointLight::PointLight()
{
    
}
PointLight::~PointLight()
{
    
}

//////////////////////////////////////////////////////////////
SpotLight* SpotLight::create(const Vec3 &direction, const Vec3 &position, const Color3B &color, float innerAngle, float outerAngle, float range)
{
    auto light = new (std::nothrow) SpotLight();
    light->setRotationFromDirection(direction);
    light->setPosition3D(position);
    light->setColor(color);
    light->setInnerAngle(innerAngle);
    light->setOuterAngle(outerAngle);
    light->_range = range;
    light->autorelease();
    return light;
}

void SpotLight::setDirection(const Vec3 &dir)
{
    setRotationFromDirection(dir);
}

Vec3 SpotLight::getDirection() const
{
    Mat4 mat = getNodeToParentTransform();
    return Vec3(-mat.m[8], -mat.m[9], -mat.m[10]);
}

Vec3 SpotLight::getDirectionInWorld() const
{
    Mat4 mat = getNodeToWorldTransform();
    return Vec3(-mat.m[8], -mat.m[9], -mat.m[10]);
}

void SpotLight::setRange(float range)
{
    _range = range;
    _shadowCameraDirty = true;
}

void SpotLight::setInnerAngle(float angle)
{
    _innerAngle = angle;
    _cosInnerAngle = cosf(angle);
}

void SpotLight::setOuterAngle(float angle)
{
    _outerAngle = angle;
    _cosOuterAngle = cosf(angle);
    _shadowCameraDirty = true;
}

SpotLight::SpotLight()
{

}

SpotLight::~SpotLight()
{

}

void SpotLight::updateShadowCamera()
{
    if (isEnabled() && getCastShadow())
    {
        const unsigned int size = static_cast<unsigned int>(_shadowMapSize);
        if (_shadowCameraDirty || !_shadowCamera || _shadowCamera->getFrameBufferObject()->getWidth() != size)
        {
            _shadowCamera = nullptr; // avoid holding 2 fbo at the same time
            if (experimental::FrameBuffer* fbo = createFrameBufferObject(size, size))
            {
                const float fov = CC_RADIANS_TO_DEGREES(_outerAngle * 2.0f);
                if (_shadowCamera = Camera::createPerspective(fov, 1.0f, 1.0f, _range))
                {
                    _shadowCamera->setFrameBufferObject(fbo);
                    _shadowCamera->setCastShadow(true);
                    _shadowCameraDirty = false;
                }
            }
        }
    }
    else
    {
        _shadowCamera = nullptr;
    }
}
/////////////////////////////////////////////////////////////

AmbientLight* AmbientLight::create( const Color3B &color )
{
    auto light = new (std::nothrow) AmbientLight();
    light->setColor(color);
    light->autorelease();
    return light;
}

AmbientLight::AmbientLight()
{

}

AmbientLight::~AmbientLight()
{

}

NS_CC_END
