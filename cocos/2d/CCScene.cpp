/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2016 Chukong Technologies Inc.
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

#include "2d/CCScene.h"
#include "base/CCDirector.h"
#include "2d/CCCamera.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerCustom.h"
#include "base/ccUTF8.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCFrameBuffer.h"
#include "platform/CCDataManager.h"
#include "2d/CCLight.h"

#if CC_USE_PHYSICS
#include "physics/CCPhysicsWorld.h"
#endif

#if CC_USE_3D_PHYSICS && CC_ENABLE_BULLET_INTEGRATION
#include "physics3d/CCPhysics3DWorld.h"
#include "physics3d/CCPhysics3DComponent.h"
#endif

#if CC_USE_NAVMESH
#include "navmesh/CCNavMesh.h"
#endif

NS_CC_BEGIN

namespace
{
    static bool lightCompareCastShadow(BaseLight* a, BaseLight* b)
    {
        return a->getCastShadow() > b->getCastShadow();
    }
}

Scene::Scene()
: _defaultCamera(Camera::create())
, _event(Director::getInstance()->getEventDispatcher()->addCustomEventListener(Director::EVENT_PROJECTION_CHANGED, std::bind(&Scene::onProjectionChanged, this, std::placeholders::_1)))
{
    _ignoreAnchorPointForPosition = true;
    setAnchorPoint(Vec2(0.5f, 0.5f));
    
    //create default camera

    addChild(_defaultCamera);
    
    _event->retain();
    
    Camera::_visitingCamera = nullptr;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    DataManager::onSceneLoaderBegin();
#endif
}

void Scene::onEnter()
{
    Node::onEnter();
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    DataManager::onSceneLoaderEnd();
#endif
}

Scene::~Scene()
{
#if CC_USE_3D_PHYSICS && CC_ENABLE_BULLET_INTEGRATION
    CC_SAFE_RELEASE(_physics3DWorld);
    CC_SAFE_RELEASE(_physics3dDebugCamera);
#endif
#if CC_USE_NAVMESH
    CC_SAFE_RELEASE(_navMesh);
#endif
    Director::getInstance()->getEventDispatcher()->removeEventListener(_event);
    CC_SAFE_RELEASE(_event);
    
#if CC_USE_PHYSICS
    delete _physicsWorld;
#endif
    
#if CC_ENABLE_GC_FOR_NATIVE_OBJECTS
    auto sEngine = ScriptEngineManager::getInstance()->getScriptEngine();
    if (sEngine)
    {
        sEngine->releaseAllChildrenRecursive(this);
    }
#endif // CC_ENABLE_GC_FOR_NATIVE_OBJECTS
}

#if CC_USE_NAVMESH
void Scene::setNavMesh(NavMesh* navMesh)
{
    if (_navMesh != navMesh)
    {
        CC_SAFE_RETAIN(navMesh);
        CC_SAFE_RELEASE(_navMesh);
        _navMesh = navMesh;
    }
}
#endif

bool Scene::init()
{
    auto size = Director::getInstance()->getWinSize();
    return initWithSize(size);
}

bool Scene::initWithSize(const Size& size)
{
    setContentSize(size);
    return true;
}

Scene* Scene::create()
{
    Scene *ret = new (std::nothrow) Scene();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

Scene* Scene::createWithSize(const Size& size)
{
    Scene *ret = new (std::nothrow) Scene();
    if (ret && ret->initWithSize(size))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

std::string Scene::getDescription() const
{
    return StringUtils::format("<Scene | tag = %d>", _tag);
}

void Scene::onProjectionChanged(EventCustom* /*event*/)
{
    if (_defaultCamera)
    {
        _defaultCamera->initDefault();
    }
}

static bool camera_cmp(const Camera* a, const Camera* b)
{
    return a->getRenderOrder() < b->getRenderOrder();
}

const std::vector<Camera*>& Scene::getCameras()
{
    if (_cameraOrderDirty)
    {
        stable_sort(_cameras.begin(), _cameras.end(), camera_cmp);
        _cameraOrderDirty = false;
    }
    return _cameras;
}

void Scene::render(Renderer* renderer, const Mat4& eyeTransform, const Mat4* eyeProjection)
{
    render(renderer, &eyeTransform, eyeProjection, 1);
}

void Scene::render(Renderer* renderer, const Mat4* eyeTransforms, const Mat4* eyeProjections, unsigned int multiViewCount)
{
    auto director = Director::getInstance();
    Camera* defaultCamera = nullptr;
    const auto& transform = getNodeToParentTransform();

    // start to update lights for this frame
    if (_lightOrderDirty)
    {
        sort(_lights.begin(), _lights.end(), lightCompareCastShadow);
        _lightOrderDirty = false;
    }

    for (BaseLight* light : getLights())
    {
        switch(light->getLightType())
        {
            case LightType::DIRECTIONAL:
                static_cast<DirectionLight*>(light)->updateShadowCamera();
                break;
            case LightType::SPOT:
                static_cast<SpotLight*>(light)->updateShadowCamera();
                break;
            default:
                break;
        }
    }
    // done updating lights

    for (const auto& camera : getCameras())
    {
        if (!camera->isVisible())
            continue;

        for (BaseLight* light : getLights())
        {
            if (((unsigned short)camera->getCameraFlag() & light->getCameraMask()) == 0)
                continue;

            if (!light->isEnabled() || !light->getCastShadow())
                continue;

            Camera* shadowCamera = nullptr;
            switch(light->getLightType())
            {
                case LightType::DIRECTIONAL:
					shadowCamera = static_cast<DirectionLight*>(light)->_shadowCamera;
					break;
                case LightType::SPOT:
                    shadowCamera = static_cast<SpotLight*>(light)->_shadowCamera;
					break;
                default:
                    break;
            }
            
            if (shadowCamera)
            {
                shadowCamera->setCameraFlag(camera->getCameraFlag());
                shadowCamera->setNodeToParentTransform(light->getNodeToWorldTransform());

                Camera::_visitingCamera = shadowCamera;

                for (unsigned int i = 0; i < multiViewCount; ++i) {
                    if (eyeProjections)
                        shadowCamera->setAdditionalProjection(eyeProjections[i] * shadowCamera->getProjectionMatrix().getInversed());
                    if (eyeTransforms)
                        shadowCamera->setAdditionalTransform(eyeTransforms[i].getInversed());
                    director->pushProjectionMatrix(i);
                    director->loadProjectionMatrix(shadowCamera->getViewProjectionMatrix(), i);
                }

                shadowCamera->apply();
                shadowCamera->clearBackground();
                visit(renderer, transform, 0);

                renderer->render();
                shadowCamera->restore();

                for (unsigned int i = 0; i < multiViewCount; ++i)
                    director->popProjectionMatrix(i);
            }
        }

        Camera::_visitingCamera = camera;
        if (Camera::_visitingCamera->getCameraFlag() == CameraFlag::DEFAULT)
        {
            defaultCamera = Camera::_visitingCamera;
        }

        // There are two ways to modify the "default camera" with the eye Transform:
        // a) modify the "nodeToParentTransform" matrix
        // b) modify the "additional transform" matrix
        // both alternatives are correct, if the user manually modifies the camera with a camera->setPosition()
        // then the "nodeToParent transform" will be lost.
        // And it is important that the change is "permanent", because the matrix might be used for calculate
        // culling and other stuff.
        for (unsigned int i = 0; i < multiViewCount; ++i) {
            if (eyeProjections)
                camera->setAdditionalProjection(eyeProjections[i] * camera->getProjectionMatrix().getInversed());
            if (eyeTransforms)
                camera->setAdditionalTransform(eyeTransforms[i].getInversed());
            director->pushProjectionMatrix(i);
            director->loadProjectionMatrix(Camera::_visitingCamera->getViewProjectionMatrix(), i);
        }

        camera->apply();
        //clear background with max depth
        camera->clearBackground();
        //visit the scene
        visit(renderer, transform, 0);
#if CC_USE_NAVMESH
        if (_navMesh && _navMeshDebugCamera == camera)
        {
            _navMesh->debugDraw(renderer);
        }
#endif

        renderer->render();
        camera->restore();

        for (unsigned int i = 0; i < multiViewCount; ++i)
            director->popProjectionMatrix(i);

        // we shouldn't restore the transform matrix since it could be used
        // from "update" or other parts of the game to calculate culling or something else.
//        camera->setNodeToParentTransform(eyeCopy);
    }

#if CC_USE_3D_PHYSICS && CC_ENABLE_BULLET_INTEGRATION
    if (_physics3DWorld && _physics3DWorld->isDebugDrawEnabled())
    {
        Camera *physics3dDebugCamera = _physics3dDebugCamera != nullptr ? _physics3dDebugCamera: defaultCamera;
        
        for (unsigned int i = 0; i < multiViewCount; ++i) {
            if (eyeProjections)
                physics3dDebugCamera->setAdditionalProjection(eyeProjections[i] * physics3dDebugCamera->getProjectionMatrix().getInversed());
            if (eyeTransforms)
                physics3dDebugCamera->setAdditionalTransform(eyeTransforms[i].getInversed());
            director->pushProjectionMatrix(i);
            director->loadProjectionMatrix(physics3dDebugCamera->getViewProjectionMatrix(), i);
        }
        
        physics3dDebugCamera->apply();
        physics3dDebugCamera->clearBackground();

        _physics3DWorld->debugDraw(renderer);
        renderer->render();
        
        physics3dDebugCamera->restore();

        for (unsigned int i = 0; i < multiViewCount; ++i)
            director->popProjectionMatrix(i);
    }
#endif

    Camera::_visitingCamera = nullptr;
//    experimental::FrameBuffer::applyDefaultFBO();
}

void Scene::removeAllChildren()
{
    if (_defaultCamera)
        _defaultCamera->retain();

    Node::removeAllChildren();

    if (_defaultCamera)
    {
        addChild(_defaultCamera);
        _defaultCamera->release();
    }
}

#if CC_USE_3D_PHYSICS && CC_ENABLE_BULLET_INTEGRATION
void Scene::setPhysics3DDebugCamera(Camera* camera)
{
    CC_SAFE_RETAIN(camera);
    CC_SAFE_RELEASE(_physics3dDebugCamera);
    _physics3dDebugCamera = camera;
}
#endif

#if CC_USE_NAVMESH
void Scene::setNavMeshDebugCamera(Camera *camera)
{
    CC_SAFE_RETAIN(camera);
    CC_SAFE_RELEASE(_navMeshDebugCamera);
    _navMeshDebugCamera = camera;
}

#endif

#if (CC_USE_PHYSICS || (CC_USE_3D_PHYSICS && CC_ENABLE_BULLET_INTEGRATION))

Scene* Scene::createWithPhysics()
{
    Scene *ret = new (std::nothrow) Scene();
    if (ret && ret->initWithPhysics())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool Scene::initWithPhysics()
{
#if CC_USE_PHYSICS
    _physicsWorld = PhysicsWorld::construct(this);
#endif

    bool ret = false;
    do
    {
        Director * director;
        CC_BREAK_IF( ! (director = Director::getInstance()) );

        this->setContentSize(director->getWinSize());

#if CC_USE_3D_PHYSICS && CC_ENABLE_BULLET_INTEGRATION
        Physics3DWorldDes info;
        CC_BREAK_IF(! (_physics3DWorld = Physics3DWorld::create(&info)));
        _physics3DWorld->retain();
#endif

        // success
        ret = true;
    } while (0);
    return ret;
}

#endif

#if (CC_USE_PHYSICS || (CC_USE_3D_PHYSICS && CC_ENABLE_BULLET_INTEGRATION) || CC_USE_NAVMESH)
void Scene::stepPhysicsAndNavigation(float deltaTime)
{
#if CC_USE_PHYSICS
    if (_physicsWorld && _physicsWorld->isAutoStep())
        _physicsWorld->update(deltaTime);
#endif

#if CC_USE_3D_PHYSICS && CC_ENABLE_BULLET_INTEGRATION
    if (_physics3DWorld)
    {
        _physics3DWorld->stepSimulate(deltaTime);
    }
#endif
#if CC_USE_NAVMESH
    if (_navMesh)
    {
        _navMesh->update(deltaTime);
    }
#endif
}
#endif

NS_CC_END
