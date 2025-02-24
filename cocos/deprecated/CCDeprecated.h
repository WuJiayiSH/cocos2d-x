/****************************************************************************
 Copyright (c) 2013      cocos2d-x.org
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

/// @cond DO_NOT_SHOW

/** Add deprecated global functions and variables here
 */

#ifndef __COCOS2D_CCDEPRECATED_H__
#define __COCOS2D_CCDEPRECATED_H__

#include <math.h>

#include "2d/CCActionCamera.h"
#include "2d/CCActionCatmullRom.h"
#include "2d/CCActionEase.h"
#include "2d/CCActionGrid.h"
#include "2d/CCActionGrid3D.h"
#include "2d/CCActionInstant.h"
#include "2d/CCActionPageTurn3D.h"
#include "2d/CCActionProgressTimer.h"
#include "2d/CCActionTiledGrid.h"
#include "2d/CCActionTween.h"
#include "2d/CCAnimationCache.h"
#include "2d/CCAtlasNode.h"
#include "2d/CCClippingNode.h"
#include "2d/CCGrid.h"
#include "2d/CCLabelAtlas.h"
#include "2d/CCLabelBMFont.h"
#include "2d/CCLabelTTF.h"
#include "2d/CCLayer.h"
#include "2d/CCMenu.h"
#include "2d/CCMenuItem.h"
#include "2d/CCMotionStreak.h"
#include "2d/CCParallaxNode.h"
#include "2d/CCParticleBatchNode.h"
#include "2d/CCParticleExamples.h"
#include "2d/CCParticleSystemQuad.h"
#include "2d/CCProgressTimer.h"
#include "2d/CCSpriteFrameCache.h"
#include "2d/CCTMXLayer.h"
#include "2d/CCTMXObjectGroup.h"
#include "2d/CCTMXTiledMap.h"
#include "2d/CCTMXXMLParser.h"
#include "2d/CCTextFieldTTF.h"
#include "2d/CCTileMapAtlas.h"
#include "2d/CCTransition.h"
#include "2d/CCTransitionPageTurn.h"
#include "2d/CCTransitionProgress.h"
#include "base/CCConfiguration.h"
#include "base/CCDataVisitor.h"
#include "base/CCEvent.h"
#include "base/CCIMEDelegate.h"
#include "base/CCScheduler.h"
#include "base/CCUserDefault.h"
#include "base/ccTypes.h"
#include "deprecated/CCArray.h"
#include "deprecated/CCBool.h"
#include "deprecated/CCDictionary.h"
#include "deprecated/CCDouble.h"
#include "deprecated/CCFloat.h"
#include "deprecated/CCInteger.h"
#include "deprecated/CCNotificationCenter.h"
#include "deprecated/CCSet.h"
#include "math/CCAffineTransform.h"
#include "math/CCGeometry.h"
#include "platform/CCApplication.h"
#include "platform/CCFileUtils.h"
#include "renderer/CCGLProgram.h"
#include "renderer/CCGLProgramCache.h"
#include "renderer/CCTextureAtlas.h"
#include "renderer/ccGLStateCache.h"

NS_CC_BEGIN

/**
 * @addtogroup data_structures
 * @{
 */

/** Helper macro that creates a Vec2
 @return Vec2
 @since v0.7.2
 */
[[deprecated]] inline Vec2 ccp(float x, float y)
{
    return Vec2(x, y);
}

/** Returns opposite of point.
 @return Vec2
 @since v0.7.2
 @deprecated please use Vec2::-, for example: -v1
 */
[[deprecated]] static inline Vec2
ccpNeg(const Vec2& v)
{
    return -v;
}

/** Calculates sum of two points.
 @return Vec2
 @since v0.7.2
 @deprecated please use Vec2::+, for example: v1 + v2
 */
[[deprecated]] static inline Vec2
ccpAdd(const Vec2& v1, const Vec2& v2)
{
    return v1 + v2;
}

/** Calculates difference of two points.
 @return Vec2
 @since v0.7.2
 @deprecated please use Vec2::-, for example: v1 - v2
 */
[[deprecated]] static inline Vec2
ccpSub(const Vec2& v1, const Vec2& v2)
{
    return v1 - v2;
}

/** Returns point multiplied by given factor.
 @return Vec2
 @since v0.7.2
 @deprecated please use Vec2::*, for example: v1 * v2
 */
[[deprecated]] static inline Vec2
ccpMult(const Vec2& v, const float s)
{
    return v * s;
}

/** Calculates midpoint between two points.
 @return Vec2
 @since v0.7.2
 @deprecated please use it like (v1 + v2) / 2.0f
 */
[[deprecated]] static inline Vec2
ccpMidpoint(const Vec2& v1, const Vec2& v2)
{
    return v1.getMidpoint(v2);
}

/** Calculates dot product of two points.
 @return float
 @since v0.7.2
 */
[[deprecated]] static inline float
ccpDot(const Vec2& v1, const Vec2& v2)
{
    return v1.dot(v2);
}

/** Calculates cross product of two points.
 @return float
 @since v0.7.2
 */
[[deprecated]] static inline float
ccpCross(const Vec2& v1, const Vec2& v2)
{
    return v1.cross(v2);
}

/** Calculates perpendicular of v, rotated 90 degrees counter-clockwise -- cross(v, perp(v)) >= 0
 @return Vec2
 @since v0.7.2
 */
[[deprecated]] static inline Vec2
ccpPerp(const Vec2& v)
{
    return v.getPerp();
}

/** Calculates perpendicular of v, rotated 90 degrees clockwise -- cross(v, rperp(v)) <= 0
 @return Vec2
 @since v0.7.2
 */
[[deprecated]] static inline Vec2
ccpRPerp(const Vec2& v)
{
    return v.getRPerp();
}

/** Calculates the projection of v1 over v2.
 @return Vec2
 @since v0.7.2
 */
[[deprecated]] static inline Vec2
ccpProject(const Vec2& v1, const Vec2& v2)
{
    return v1.project(v2);
}

/** Rotates two points.
 @return Vec2
 @since v0.7.2
 */
[[deprecated]] static inline Vec2
ccpRotate(const Vec2& v1, const Vec2& v2)
{
    return v1.rotate(v2);
}

/** Unrotates two points.
 @return Vec2
 @since v0.7.2
 */
[[deprecated]] static inline Vec2
ccpUnrotate(const Vec2& v1, const Vec2& v2)
{
    return v1.unrotate(v2);
}

/** Calculates the square length of a Vec2 (not calling sqrt() )
 @return float
 @since v0.7.2
 */
[[deprecated]] static inline float
ccpLengthSQ(const Vec2& v)
{
    return v.getLengthSq();
}


/** Calculates the square distance between two points (not calling sqrt() )
 @return float
 @since v1.1
 */
[[deprecated]] static inline float
ccpDistanceSQ(const Vec2 p1, const Vec2 p2)
{
    return (p1 - p2).getLengthSq();
}


/** Calculates distance between point an origin
 @return float
 @since v0.7.2
 */
[[deprecated]] static inline float ccpLength(const Vec2& v)
{
    return v.getLength();
}

/** Calculates the distance between two points
 @return float
 @since v0.7.2
 */
[[deprecated]] static inline float ccpDistance(const Vec2& v1, const Vec2& v2)
{
    return v1.getDistance(v2);
}

/** Returns point multiplied to a length of 1.
 @return Vec2
 @since v0.7.2
 */
[[deprecated]] static inline Vec2 ccpNormalize(const Vec2& v)
{
    return v.getNormalized();
}

/** Converts radians to a normalized vector.
 @return Vec2
 @since v0.7.2
 */
[[deprecated]] static inline Vec2 ccpForAngle(const float a)
{
    return Vec2::forAngle(a);
}

/** Converts a vector to radians.
 @return float
 @since v0.7.2
 */
[[deprecated]] static inline float ccpToAngle(const Vec2& v)
{
    return v.getAngle();
}


/** Clamp a point between from and to.
 @since v0.99.1
 */
[[deprecated]] static inline Vec2 ccpClamp(const Vec2& p, const Vec2& from, const Vec2& to)
{
    return p.getClampPoint(from, to);
}

/** Quickly convert Size to a Vec2
 @since v0.99.1
 */
[[deprecated]] static inline Vec2 ccpFromSize(const Size& s)
{
    return Vec2(s);
}

/** Run a math operation function on each point component
 * absf, floorf, ceilf, roundf
 * any function that has the signature: float func(float);
 * For example: let's try to take the floor of x,y
 * ccpCompOp(p,floorf);
 @since v0.99.1
 */
[[deprecated]] static inline Vec2 ccpCompOp(const Vec2& p, float (*opFunc)(float))
{
    return p.compOp(opFunc);
}

/** Linear Interpolation between two points a and b
 @returns
 alpha == 0 ? a
 alpha == 1 ? b
 otherwise a value between a..b
 @since v0.99.1
 */
[[deprecated]] static inline Vec2 ccpLerp(const Vec2& a, const Vec2& b, float alpha)
{
    return a.lerp(b, alpha);
}


/** @returns if points have fuzzy equality which means equal with some degree of variance.
 @since v0.99.1
 */
[[deprecated]] static inline bool ccpFuzzyEqual(const Vec2& a, const Vec2& b, float variance)
{
    return a.fuzzyEquals(b, variance);
}


/** Multiplies a and b components, a.x*b.x, a.y*b.y
 @returns a component-wise multiplication
 @since v0.99.1
 */
[[deprecated]] static inline Vec2 ccpCompMult(const Vec2& a, const Vec2& b)
{
    return Vec2(a.x * b.x, a.y * b.y);
}

/** @returns the signed angle in radians between two vector directions
 @since v0.99.1
 */
[[deprecated]] static inline float ccpAngleSigned(const Vec2& a, const Vec2& b)
{
    return a.getAngle(b);
}

/** @returns the angle in radians between two vector directions
 @since v0.99.1
 */
[[deprecated]] static inline float ccpAngle(const Vec2& a, const Vec2& b)
{
    return a.getAngle(b);
}

/** Rotates a point counter clockwise by the angle around a pivot
 @param v is the point to rotate
 @param pivot is the pivot, naturally
 @param angle is the angle of rotation cw in radians
 @returns the rotated point
 @since v0.99.1
 */
[[deprecated]] static inline Vec2 ccpRotateByAngle(const Vec2& v, const Vec2& pivot, float angle)
{
    return v.rotateByAngle(pivot, angle);
}

/** A general line-line intersection test
 @param p1
 is the startpoint for the first line P1 = (p1 - p2)
 @param p2
 is the endpoint for the first line P1 = (p1 - p2)
 @param p3
 is the startpoint for the second line P2 = (p3 - p4)
 @param p4
 is the endpoint for the second line P2 = (p3 - p4)
 @param s
 is the range for a hitpoint in P1 (pa = p1 + s*(p2 - p1))
 @param t
 is the range for a hitpoint in P3 (pa = p2 + t*(p4 - p3))
 @return bool
 indicating successful intersection of a line
 note that to truly test intersection for segments we have to make
 sure that s & t lie within [0..1] and for rays, make sure s & t > 0
 the hit point is        p3 + t * (p4 - p3);
 the hit point also is    p1 + s * (p2 - p1);
 @since v0.99.1
 */
[[deprecated]] static inline bool ccpLineIntersect(const Vec2& p1, const Vec2& p2,
                                                     const Vec2& p3, const Vec2& p4,
                                                     float *s, float *t)
{
    return Vec2::isLineIntersect(p1, p2, p3, p4, s, t);
}

/*
 ccpSegmentIntersect returns true if Segment A-B intersects with segment C-D
 @since v1.0.0
 */
[[deprecated]] static inline bool ccpSegmentIntersect(const Vec2& A, const Vec2& B, const Vec2& C, const Vec2& D)
{
    return Vec2::isSegmentIntersect(A, B, C, D);
}

/*
 ccpIntersectPoint returns the intersection point of line A-B, C-D
 @since v1.0.0
 */
[[deprecated]] static inline Vec2 ccpIntersectPoint(const Vec2& A, const Vec2& B, const Vec2& C, const Vec2& D)
{
    return Vec2::getIntersectPoint(A, B, C, D);
}

[[deprecated]] inline Vec2 CCPointMake(float x, float y)
{
    return Vec2(x, y);
}

[[deprecated]] inline Size CCSizeMake(float width, float height)
{
    return Size(width, height);
}

[[deprecated]] inline Rect CCRectMake(float x, float y, float width, float height)
{
    return Rect(x, y, width, height);
}


[[deprecated]] extern const Vec2 CCPointZero;

/* The "zero" size -- equivalent to Size(0, 0). */
[[deprecated]] extern const Size CCSizeZero;

/* The "zero" rectangle -- equivalent to Rect(0, 0, 0, 0). */
[[deprecated]] extern const Rect CCRectZero;


[[deprecated]] extern const Color3B ccWHITE;
[[deprecated]] extern const Color3B ccYELLOW;
[[deprecated]] extern const Color3B ccGREEN;
[[deprecated]] extern const Color3B ccBLUE;
[[deprecated]] extern const Color3B ccRED;
[[deprecated]] extern const Color3B ccMAGENTA;
[[deprecated]] extern const Color3B ccBLACK;
[[deprecated]] extern const Color3B ccORANGE;
[[deprecated]] extern const Color3B ccGRAY;

[[deprecated]] extern const BlendFunc kCCBlendFuncDisable;

[[deprecated]] static inline Color3B ccc3(GLubyte r, GLubyte g, GLubyte b)
{
    return Color3B(r, g, b);
}

[[deprecated]] static inline bool ccc3BEqual(const Color3B &col1, const Color3B &col2)
{
    return col1.r == col2.r && col1.g == col2.g && col1.b == col2.b;
}

[[deprecated]] static inline Color4B
ccc4(const GLubyte r, const GLubyte g, const GLubyte b, const GLubyte o)
{
    return Color4B(r, g, b, o);
}

[[deprecated]] static inline Color4F ccc4FFromccc3B(Color3B c)
{
    return Color4F(c.r/255.f, c.g/255.f, c.b/255.f, 1.f);
}

[[deprecated]] static inline Color4F
ccc4f(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a)
{
    return Color4F(r, g, b, a);
}

[[deprecated]] static inline Color4F ccc4FFromccc4B(Color4B c)
{
    return Color4F(c.r/255.f, c.g/255.f, c.b/255.f, c.a/255.f);
}

[[deprecated]] static inline Color4B ccc4BFromccc4F(Color4F c)
{
        return Color4B((GLubyte)(c.r*255), (GLubyte)(c.g*255), (GLubyte)(c.b*255), (GLubyte)(c.a*255));
}

[[deprecated]] static inline bool ccc4FEqual(Color4F a, Color4F b)
{
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

[[deprecated]] static inline Vec2 vertex2(const float x, const float y)
{
    Vec2 c(x, y);
    return c;
}

[[deprecated]] static inline Vec3 vertex3(const float x, const float y, const float z)
{
    Vec3 c(x, y, z);
    return c;
}

[[deprecated]] static inline Tex2F tex2(const float u, const float v)
{
    Tex2F t(u , v);
    return t;
}

[[deprecated]] static inline AffineTransform CCAffineTransformMake(float a, float b, float c, float d, float tx, float ty)
{
    return AffineTransformMake(a, b, c, d, tx, ty);
}

[[deprecated]] static inline Vec2 CCPointApplyAffineTransform(const Vec2& point, const AffineTransform& t)
{
    return PointApplyAffineTransform(point, t);
}

[[deprecated]] static inline Size CCSizeApplyAffineTransform(const Size& size, const AffineTransform& t)
{
    return SizeApplyAffineTransform(size, t);
}

[[deprecated]] static inline AffineTransform CCAffineTransformMakeIdentity()
{
    return AffineTransformMakeIdentity();
}

[[deprecated]] static inline Rect CCRectApplyAffineTransform(const Rect& rect, const AffineTransform& anAffineTransform)
{
    return RectApplyAffineTransform(rect, anAffineTransform);
}

[[deprecated]] static inline AffineTransform CCAffineTransformTranslate(const AffineTransform& t, float tx, float ty)
{
    return AffineTransformTranslate(t, tx, ty);
}

[[deprecated]] static inline AffineTransform CCAffineTransformRotate(const AffineTransform& aTransform, float anAngle)
{
    return AffineTransformRotate(aTransform, anAngle);
}

[[deprecated]] static inline AffineTransform CCAffineTransformScale(const AffineTransform& t, float sx, float sy)
{
    return AffineTransformScale(t, sx, sy);
}

[[deprecated]] static inline AffineTransform CCAffineTransformConcat(const AffineTransform& t1, const AffineTransform& t2)
{
    return AffineTransformConcat(t1, t2);
}

[[deprecated]] static inline bool CCAffineTransformEqualToTransform(const AffineTransform& t1, const AffineTransform& t2)
{
    return AffineTransformEqualToTransform(t1, t2);
}

[[deprecated]] static inline AffineTransform CCAffineTransformInvert(const AffineTransform& t)
{
    return AffineTransformInvert(t);
}

[[deprecated]] static inline AffineTransform CCAffineTransformIdentity()
{
    return AffineTransformMakeIdentity();
}

// CC prefix compatibility
[[deprecated]] typedef Ref CCObject;
[[deprecated]] typedef Ref Object;
[[deprecated]] typedef Event CCEvent;
[[deprecated]] typedef DataVisitor CCDataVisitor;
[[deprecated]] typedef PrettyPrinter CCPrettyPrinter;
[[deprecated]] typedef Acceleration CCAcceleration;
[[deprecated]] typedef TextureAtlas CCTextureAtlas;
[[deprecated]] typedef Configuration CCConfiguration;
[[deprecated]] typedef PointArray CCPointArray;
[[deprecated]] typedef RemoveSelf CCRemoveSelf;
[[deprecated]] typedef IMEDelegate CCIMEDelegate;
[[deprecated]] typedef IMEKeyboardNotificationInfo CCIMEKeyboardNotificationInfo;
[[deprecated]] typedef TextFieldDelegate CCTextFieldDelegate;
[[deprecated]] typedef TextFieldTTF CCTextFieldTTF;
[[deprecated]] typedef __NotificationCenter CCNotificationCenter;
[[deprecated]] typedef __NotificationCenter NotificationCenter;
//[[deprecated]] typedef TargetedTouchDelegate CCTargetedTouchDelegate;
//[[deprecated]] typedef StandardTouchDelegate CCStandardTouchDelegate;
//[[deprecated]] typedef TouchDelegate CCTouchDelegate;
[[deprecated]] typedef Image CCImage;
[[deprecated]] typedef UserDefault CCUserDefault;

[[deprecated]] typedef Action CCAction;
[[deprecated]] typedef FiniteTimeAction CCFiniteTimeAction;
[[deprecated]] typedef Speed CCSpeed;
[[deprecated]] typedef Follow CCFollow;
[[deprecated]] typedef GLProgram CCGLProgram;
[[deprecated]] typedef Touch CCTouch;
[[deprecated]] typedef Texture2D CCTexture2D;
[[deprecated]] typedef Node CCNode;
[[deprecated]] typedef Node CCNodeRGBA;
[[deprecated]] typedef Node CCRGBAProtocol;
[[deprecated]] typedef SpriteFrame CCSpriteFrame;
[[deprecated]] typedef AnimationFrame CCAnimationFrame;
[[deprecated]] typedef Animation CCAnimation;
[[deprecated]] typedef ActionInterval CCActionInterval;
[[deprecated]] typedef Sequence CCSequence;
[[deprecated]] typedef Repeat CCRepeat;
[[deprecated]] typedef RepeatForever CCRepeatForever;
[[deprecated]] typedef Spawn CCSpawn;
[[deprecated]] typedef RotateTo CCRotateTo;
[[deprecated]] typedef RotateBy CCRotateBy;
[[deprecated]] typedef MoveBy CCMoveBy;
[[deprecated]] typedef MoveTo CCMoveTo;
[[deprecated]] typedef SkewTo CCSkewTo;
[[deprecated]] typedef SkewBy CCSkewBy;
[[deprecated]] typedef JumpBy CCJumpBy;
[[deprecated]] typedef JumpTo CCJumpTo;
[[deprecated]] typedef BezierBy CCBezierBy;
[[deprecated]] typedef BezierTo CCBezierTo;
[[deprecated]] typedef ScaleTo CCScaleTo;
[[deprecated]] typedef ScaleBy CCScaleBy;
[[deprecated]] typedef Blink CCBlink;
[[deprecated]] typedef FadeIn CCFadeIn;
[[deprecated]] typedef FadeOut CCFadeOut;
[[deprecated]] typedef FadeTo CCFadeTo;
[[deprecated]] typedef TintTo CCTintTo;
[[deprecated]] typedef TintBy CCTintBy;
[[deprecated]] typedef DelayTime CCDelayTime;
[[deprecated]] typedef Animate CCAnimate;
[[deprecated]] typedef TargetedAction CCTargetedAction;
[[deprecated]] typedef ActionCamera CCActionCamera;
[[deprecated]] typedef OrbitCamera CCOrbitCamera;
[[deprecated]] typedef ActionManager CCActionManager;
[[deprecated]] typedef ActionEase CCActionEase;
[[deprecated]] typedef EaseRateAction CCEaseRateAction;
[[deprecated]] typedef EaseIn CCEaseIn;
[[deprecated]] typedef EaseOut CCEaseOut;
[[deprecated]] typedef EaseInOut CCEaseInOut;
[[deprecated]] typedef EaseExponentialIn CCEaseExponentialIn;
[[deprecated]] typedef EaseExponentialOut CCEaseExponentialOut;
[[deprecated]] typedef EaseExponentialInOut CCEaseExponentialInOut;
[[deprecated]] typedef EaseSineIn CCEaseSineIn;
[[deprecated]] typedef EaseSineOut CCEaseSineOut;
[[deprecated]] typedef EaseSineInOut CCEaseSineInOut;
[[deprecated]] typedef EaseElastic CCEaseElastic;
[[deprecated]] typedef EaseElasticIn CCEaseElasticIn;
[[deprecated]] typedef EaseElasticOut CCEaseElasticOut;
[[deprecated]] typedef EaseElasticInOut CCEaseElasticInOut;
[[deprecated]] typedef EaseBounce CCEaseBounce;
[[deprecated]] typedef EaseBounceIn CCEaseBounceIn;
[[deprecated]] typedef EaseBounceOut CCEaseBounceOut;
[[deprecated]] typedef EaseBounceInOut CCEaseBounceInOut;
[[deprecated]] typedef EaseBackIn CCEaseBackIn;
[[deprecated]] typedef EaseBackOut CCEaseBackOut;
[[deprecated]] typedef EaseBackInOut CCEaseBackInOut;
[[deprecated]] typedef ActionInstant CCActionInstant;
[[deprecated]] typedef Show CCShow;
[[deprecated]] typedef Hide CCHide;
[[deprecated]] typedef ToggleVisibility CCToggleVisibility;
[[deprecated]] typedef FlipX CCFlipX;
[[deprecated]] typedef FlipY CCFlipY;
[[deprecated]] typedef Place CCPlace;
[[deprecated]] typedef CallFunc CCCallFunc;
[[deprecated]] typedef CallFuncN CCCallFuncN;
[[deprecated]] typedef __CCCallFuncND CCCallFuncND;
[[deprecated]] typedef __CCCallFuncO CCCallFuncO;
[[deprecated]] typedef GridAction CCGridAction;
[[deprecated]] typedef Grid3DAction CCGrid3DAction;
[[deprecated]] typedef TiledGrid3DAction CCTiledGrid3DAction;
[[deprecated]] typedef StopGrid CCStopGrid;
[[deprecated]] typedef ReuseGrid CCReuseGrid;
[[deprecated]] typedef Waves3D CCWaves3D;
[[deprecated]] typedef FlipX3D CCFlipX3D;
[[deprecated]] typedef FlipY3D CCFlipY3D;
[[deprecated]] typedef Lens3D CCLens3D;
[[deprecated]] typedef Ripple3D CCRipple3D;
[[deprecated]] typedef Shaky3D CCShaky3D;
[[deprecated]] typedef Liquid CCLiquid;
[[deprecated]] typedef Waves CCWaves;
[[deprecated]] typedef Twirl CCTwirl;
[[deprecated]] typedef PageTurn3D CCPageTurn3D;
[[deprecated]] typedef ProgressTo CCProgressTo;
[[deprecated]] typedef ProgressFromTo CCProgressFromTo;
[[deprecated]] typedef ShakyTiles3D CCShakyTiles3D;
[[deprecated]] typedef ShatteredTiles3D CCShatteredTiles3D;
[[deprecated]] typedef ShuffleTiles CCShuffleTiles;
[[deprecated]] typedef FadeOutTRTiles CCFadeOutTRTiles;
[[deprecated]] typedef FadeOutBLTiles CCFadeOutBLTiles;
[[deprecated]] typedef FadeOutUpTiles CCFadeOutUpTiles;
[[deprecated]] typedef FadeOutDownTiles CCFadeOutDownTiles;
[[deprecated]] typedef TurnOffTiles CCTurnOffTiles;
[[deprecated]] typedef WavesTiles3D CCWavesTiles3D;
[[deprecated]] typedef JumpTiles3D CCJumpTiles3D;
[[deprecated]] typedef SplitRows CCSplitRows;
[[deprecated]] typedef SplitCols CCSplitCols;
[[deprecated]] typedef ActionTween CCActionTween;
[[deprecated]] typedef CardinalSplineTo CCCardinalSplineTo;
[[deprecated]] typedef CardinalSplineBy CCCardinalSplineBy;
[[deprecated]] typedef CatmullRomTo CCCatmullRomTo;
[[deprecated]] typedef CatmullRomBy CCCatmullRomBy;
[[deprecated]] typedef AtlasNode CCAtlasNode;
[[deprecated]] typedef TextureProtocol CCTextureProtocol;
[[deprecated]] typedef BlendProtocol CCBlendProtocol;
[[deprecated]] typedef DrawNode CCDrawNode;
[[deprecated]] typedef Camera CCCamera;
[[deprecated]] typedef LabelAtlas CCLabelAtlas;
[[deprecated]] typedef LabelProtocol CCLabelProtocol;
[[deprecated]] typedef Director CCDirector;
[[deprecated]] typedef GridBase CCGridBase;
[[deprecated]] typedef Grid3D CCGrid3D;
[[deprecated]] typedef TiledGrid3D CCTiledGrid3D;
[[deprecated]] typedef Sprite CCSprite;
#define CCLabelTTF LabelTTF
[[deprecated]] typedef SpriteBatchNode CCSpriteBatchNode;
#define CCLabelBMFont LabelBMFont
[[deprecated]] typedef Layer CCLayer;
//[[deprecated]] typedef KeypadDelegate CCKeypadDelegate;
[[deprecated]] typedef Layer CCLayerRGBA;
[[deprecated]] typedef LayerColor CCLayerColor;
[[deprecated]] typedef LayerGradient CCLayerGradient;
[[deprecated]] typedef LayerMultiplex CCLayerMultiplex;
[[deprecated]] typedef Scene CCScene;
[[deprecated]] typedef TransitionEaseScene CCTransitionEaseScene;
[[deprecated]] typedef TransitionScene CCTransitionScene;
[[deprecated]] typedef TransitionSceneOriented CCTransitionSceneOriented;
[[deprecated]] typedef TransitionRotoZoom CCTransitionRotoZoom;
[[deprecated]] typedef TransitionJumpZoom CCTransitionJumpZoom;
[[deprecated]] typedef TransitionMoveInL CCTransitionMoveInL;
[[deprecated]] typedef TransitionMoveInR CCTransitionMoveInR;
[[deprecated]] typedef TransitionMoveInT CCTransitionMoveInT;
[[deprecated]] typedef TransitionMoveInB CCTransitionMoveInB;
[[deprecated]] typedef TransitionSlideInL CCTransitionSlideInL;
[[deprecated]] typedef TransitionSlideInR CCTransitionSlideInR;
[[deprecated]] typedef TransitionSlideInB CCTransitionSlideInB;
[[deprecated]] typedef TransitionSlideInT CCTransitionSlideInT;
[[deprecated]] typedef TransitionShrinkGrow CCTransitionShrinkGrow;
[[deprecated]] typedef TransitionFlipX CCTransitionFlipX;
[[deprecated]] typedef TransitionFlipY CCTransitionFlipY;
[[deprecated]] typedef TransitionFlipAngular CCTransitionFlipAngular;
[[deprecated]] typedef TransitionZoomFlipX CCTransitionZoomFlipX;
[[deprecated]] typedef TransitionZoomFlipY CCTransitionZoomFlipY;
[[deprecated]] typedef TransitionZoomFlipAngular CCTransitionZoomFlipAngular;
[[deprecated]] typedef TransitionFade CCTransitionFade;
[[deprecated]] typedef TransitionCrossFade CCTransitionCrossFade;
[[deprecated]] typedef TransitionTurnOffTiles CCTransitionTurnOffTiles;
[[deprecated]] typedef TransitionSplitCols CCTransitionSplitCols;
[[deprecated]] typedef TransitionSplitRows CCTransitionSplitRows;
[[deprecated]] typedef TransitionFadeTR CCTransitionFadeTR;
[[deprecated]] typedef TransitionFadeBL CCTransitionFadeBL;
[[deprecated]] typedef TransitionFadeUp CCTransitionFadeUp;
[[deprecated]] typedef TransitionFadeDown CCTransitionFadeDown;
[[deprecated]] typedef TransitionPageTurn CCTransitionPageTurn;
[[deprecated]] typedef TransitionProgress CCTransitionProgress;
[[deprecated]] typedef TransitionProgressRadialCCW CCTransitionProgressRadialCCW;
[[deprecated]] typedef TransitionProgressRadialCW CCTransitionProgressRadialCW;
[[deprecated]] typedef TransitionProgressHorizontal CCTransitionProgressHorizontal;
[[deprecated]] typedef TransitionProgressVertical CCTransitionProgressVertical;
[[deprecated]] typedef TransitionProgressInOut CCTransitionProgressInOut;
[[deprecated]] typedef TransitionProgressOutIn CCTransitionProgressOutIn;
[[deprecated]] typedef MenuItem CCMenuItem;
[[deprecated]] typedef MenuItemLabel CCMenuItemLabel;
[[deprecated]] typedef MenuItemAtlasFont CCMenuItemAtlasFont;
[[deprecated]] typedef MenuItemFont CCMenuItemFont;
[[deprecated]] typedef MenuItemSprite CCMenuItemSprite;
[[deprecated]] typedef MenuItemImage CCMenuItemImage;
[[deprecated]] typedef MenuItemToggle CCMenuItemToggle;
[[deprecated]] typedef Menu CCMenu;
[[deprecated]] typedef ClippingNode CCClippingNode;
[[deprecated]] typedef MotionStreak CCMotionStreak;
[[deprecated]] typedef ProgressTimer CCProgressTimer;
[[deprecated]] typedef RenderTexture CCRenderTexture;
[[deprecated]] typedef ParticleBatchNode CCParticleBatchNode;
[[deprecated]] typedef ParticleSystem CCParticleSystem;
[[deprecated]] typedef ParticleSystemQuad CCParticleSystemQuad;
[[deprecated]] typedef ParticleFire CCParticleFire;
[[deprecated]] typedef ParticleFireworks CCParticleFireworks;
[[deprecated]] typedef ParticleSun CCParticleSun;
[[deprecated]] typedef ParticleGalaxy CCParticleGalaxy;
[[deprecated]] typedef ParticleFlower CCParticleFlower;
[[deprecated]] typedef ParticleMeteor CCParticleMeteor;
[[deprecated]] typedef ParticleSpiral CCParticleSpiral;
[[deprecated]] typedef ParticleExplosion CCParticleExplosion;
[[deprecated]] typedef ParticleSmoke CCParticleSmoke;
[[deprecated]] typedef ParticleSnow CCParticleSnow;
[[deprecated]] typedef ParticleRain CCParticleRain;
[[deprecated]] typedef FileUtils CCFileUtils;
[[deprecated]] typedef Application CCApplication;
[[deprecated]] typedef GLProgramCache CCShaderCache;
[[deprecated]] typedef GLProgramCache ShaderCache;
[[deprecated]] typedef AnimationCache CCAnimationCache;
[[deprecated]] typedef SpriteFrameCache CCSpriteFrameCache;
[[deprecated]] typedef TextureCache CCTextureCache;
[[deprecated]] typedef ParallaxNode CCParallaxNode;
[[deprecated]] typedef TMXObjectGroup CCTMXObjectGroup;
[[deprecated]] typedef TMXLayerInfo CCTMXLayerInfo;
[[deprecated]] typedef TMXTilesetInfo CCTMXTilesetInfo;
[[deprecated]] typedef TMXMapInfo CCTMXMapInfo;
[[deprecated]] typedef TMXLayer CCTMXLayer;
[[deprecated]] typedef TMXTiledMap CCTMXTiledMap;
[[deprecated]] typedef TileMapAtlas CCTileMapAtlas;
[[deprecated]] typedef Timer CCTimer;
[[deprecated]] typedef Scheduler CCScheduler;
[[deprecated]] typedef GLView CCEGLView;

[[deprecated]] typedef Component CCComponent;
[[deprecated]] typedef AffineTransform CCAffineTransform;
[[deprecated]] typedef Vec2 CCPoint;
[[deprecated]] typedef Size CCSize;
[[deprecated]] typedef Rect CCRect;
[[deprecated]] typedef Color3B ccColor3B;
[[deprecated]] typedef Color4F ccColor4F;
[[deprecated]] typedef Color4B ccColor4B;
[[deprecated]] typedef Vec2 ccVertex2F;
[[deprecated]] typedef Vec2 Vertex2F;
[[deprecated]] typedef Vec3 ccVertex3F;
[[deprecated]] typedef Vec3 Vertex3F;
[[deprecated]] typedef Tex2F ccTex2F;
[[deprecated]] typedef PointSprite ccPointSprite;
[[deprecated]] typedef Quad2 ccQuad2;
[[deprecated]] typedef Quad3 ccQuad3;
[[deprecated]] typedef V2F_C4B_T2F ccV2F_C4B_T2F;
[[deprecated]] typedef V2F_C4F_T2F ccV2F_C4F_T2F;
[[deprecated]] typedef V3F_C4B_T2F ccV3F_C4B_T2F;
[[deprecated]] typedef V2F_C4B_T2F_Triangle ccV2F_C4B_T2F_Triangle;
[[deprecated]] typedef V2F_C4B_T2F_Quad ccV2F_C4B_T2F_Quad;
[[deprecated]] typedef V3F_C4B_T2F_Quad ccV3F_C4B_T2F_Quad;
[[deprecated]] typedef V2F_C4F_T2F_Quad ccV2F_C4F_T2F_Quad;
[[deprecated]] typedef BlendFunc ccBlendFunc;
[[deprecated]] typedef T2F_Quad ccT2F_Quad;
[[deprecated]] typedef AnimationFrameData ccAnimationFrameData;
[[deprecated]] typedef FontShadow ccFontShadow;
[[deprecated]] typedef FontStroke ccFontStroke;
[[deprecated]] typedef FontDefinition ccFontDefinition;

[[deprecated]] typedef TextVAlignment CCVerticalTextAlignment;
[[deprecated]] typedef TextHAlignment CCTextAlignment;
[[deprecated]] typedef ProgressTimer::Type CCProgressTimerType;

[[deprecated]] typedef void* CCZone;

[[deprecated]] const int kCCVertexAttrib_Position = GLProgram::VERTEX_ATTRIB_POSITION;
[[deprecated]] const int kCCVertexAttrib_Color = GLProgram::VERTEX_ATTRIB_COLOR;
[[deprecated]] const int kCCVertexAttrib_TexCoords = GLProgram::VERTEX_ATTRIB_TEX_COORD;
[[deprecated]] const int kCCVertexAttrib_MAX = GLProgram::VERTEX_ATTRIB_MAX;

[[deprecated]] const int kCCUniformPMatrix = GLProgram::UNIFORM_P_MATRIX;
[[deprecated]] const int kCCUniformMVMatrix = GLProgram::UNIFORM_MV_MATRIX;
[[deprecated]] const int kCCUniformMVPMatrix = GLProgram::UNIFORM_MVP_MATRIX;
[[deprecated]] const int kCCUniformTime = GLProgram::UNIFORM_TIME;
[[deprecated]] const int kCCUniformSinTime = GLProgram::UNIFORM_SIN_TIME;
[[deprecated]] const int kCCUniformCosTime = GLProgram::UNIFORM_COS_TIME;
[[deprecated]] const int kCCUniformRandom01 = GLProgram::UNIFORM_RANDOM01;
[[deprecated]] const int kCCUniformSampler = GLProgram::UNIFORM_SAMPLER0;
[[deprecated]] const int kCCUniform_MAX = GLProgram::UNIFORM_MAX;

[[deprecated]] extern const char* kCCShader_PositionTextureColor;
[[deprecated]] extern const char* kCCShader_PositionTextureColorAlphaTest;
[[deprecated]] extern const char* kCCShader_PositionColor;
[[deprecated]] extern const char* kCCShader_PositionTexture;
[[deprecated]] extern const char* kCCShader_PositionTexture_uColor;
[[deprecated]] extern const char* kCCShader_PositionTextureA8Color;
[[deprecated]] extern const char* kCCShader_Position_uColor;
[[deprecated]] extern const char* kCCShader_PositionLengthTexureColor;

// uniform names
[[deprecated]] extern const char* kCCUniformPMatrix_s;
[[deprecated]] extern const char* kCCUniformMVMatrix_s;
[[deprecated]] extern const char* kCCUniformMVPMatrix_s;
[[deprecated]] extern const char* kCCUniformTime_s;
[[deprecated]] extern const char* kCCUniformSinTime_s;
[[deprecated]] extern const char* kCCUniformCosTime_s;
[[deprecated]] extern const char* kCCUniformRandom01_s;
[[deprecated]] extern const char* kCCUniformSampler_s;
[[deprecated]] extern const char* kCCUniformAlphaTestValue;

// Attribute names
[[deprecated]] extern const char*    kCCAttributeNameColor;
[[deprecated]] extern const char*    kCCAttributeNamePosition;
[[deprecated]] extern const char*    kCCAttributeNameTexCoord;

[[deprecated]] const int kCCVertexAttribFlag_None = GL::VERTEX_ATTRIB_FLAG_NONE;
[[deprecated]] const int kCCVertexAttribFlag_Position = GL::VERTEX_ATTRIB_FLAG_POSITION;
[[deprecated]] const int kCCVertexAttribFlag_Color = GL::VERTEX_ATTRIB_FLAG_COLOR;
[[deprecated]] const int kCCVertexAttribFlag_TexCoords = GL::VERTEX_ATTRIB_FLAG_TEX_COORD;
[[deprecated]] const int kCCVertexAttribFlag_PosColorTex = GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX;

[[deprecated]] const ProgressTimer::Type kCCProgressTimerTypeRadial = ProgressTimer::Type::RADIAL;
[[deprecated]] const ProgressTimer::Type kCCProgressTimerTypeBar = ProgressTimer::Type::BAR;
[[deprecated]] typedef ProgressTimer::Type ProgressTimerType;

[[deprecated]] const Director::Projection kCCDirectorProjection2D = Director::Projection::_2D;
[[deprecated]] const Director::Projection kCCDirectorProjection3D = Director::Projection::_3D;
[[deprecated]] const Director::Projection kCCDirectorProjectionCustom = Director::Projection::CUSTOM;
[[deprecated]] const Director::Projection kCCDirectorProjectionDefault = Director::Projection::DEFAULT;
[[deprecated]] typedef Director::Projection ccDirectorProjection;

[[deprecated]] const TextVAlignment kCCVerticalTextAlignmentTop = TextVAlignment::TOP;
[[deprecated]] const TextVAlignment kCCVerticalTextAlignmentCenter = TextVAlignment::CENTER;
[[deprecated]] const TextVAlignment kCCVerticalTextAlignmentBottom = TextVAlignment::BOTTOM;

[[deprecated]] const TextHAlignment kCCTextAlignmentLeft = TextHAlignment::LEFT;
[[deprecated]] const TextHAlignment kCCTextAlignmentCenter = TextHAlignment::CENTER;
[[deprecated]] const TextHAlignment kCCTextAlignmentRight = TextHAlignment::RIGHT;

[[deprecated]] const Texture2D::PixelFormat kCCTexture2DPixelFormat_RGBA8888 = Texture2D::PixelFormat::RGBA8888;
[[deprecated]] const Texture2D::PixelFormat kCCTexture2DPixelFormat_RGB888 = Texture2D::PixelFormat::RGB888;
[[deprecated]] const Texture2D::PixelFormat kCCTexture2DPixelFormat_RGB565 = Texture2D::PixelFormat::RGB565;
[[deprecated]] const Texture2D::PixelFormat kCCTexture2DPixelFormat_A8 = Texture2D::PixelFormat::A8;
[[deprecated]] const Texture2D::PixelFormat kCCTexture2DPixelFormat_I8 = Texture2D::PixelFormat::I8;
[[deprecated]] const Texture2D::PixelFormat kCCTexture2DPixelFormat_AI88 = Texture2D::PixelFormat::AI88;
[[deprecated]] const Texture2D::PixelFormat kCCTexture2DPixelFormat_RGBA4444 = Texture2D::PixelFormat::RGBA4444;
[[deprecated]] const Texture2D::PixelFormat kCCTexture2DPixelFormat_RGB5A1 = Texture2D::PixelFormat::RGB5A1;
[[deprecated]] const Texture2D::PixelFormat kCCTexture2DPixelFormat_PVRTC4 = Texture2D::PixelFormat::PVRTC4;
[[deprecated]] const Texture2D::PixelFormat kCCTexture2DPixelFormat_PVRTC2 = Texture2D::PixelFormat::PVRTC2;
[[deprecated]] const Texture2D::PixelFormat kCCTexture2DPixelFormat_Default = Texture2D::PixelFormat::DEFAULT;

[[deprecated]] typedef Texture2D::PixelFormat CCTexture2DPixelFormat;


[[deprecated]] const int kCCParticleDurationInfinity = ParticleSystem::DURATION_INFINITY;
[[deprecated]] const int kCCParticleStartSizeEqualToEndSize = ParticleSystem::START_SIZE_EQUAL_TO_END_SIZE;
[[deprecated]] const int kCCParticleStartRadiusEqualToEndRadius = ParticleSystem::START_RADIUS_EQUAL_TO_END_RADIUS;

[[deprecated]] const int kParticleDurationInfinity = ParticleSystem::DURATION_INFINITY;
[[deprecated]] const int kParticleStartSizeEqualToEndSize = ParticleSystem::START_SIZE_EQUAL_TO_END_SIZE;
[[deprecated]] const int kParticleStartRadiusEqualToEndRadius = ParticleSystem::START_RADIUS_EQUAL_TO_END_RADIUS;

[[deprecated]] const ParticleSystem::Mode kCCParticleModeGravity = ParticleSystem::Mode::GRAVITY;
[[deprecated]] const ParticleSystem::Mode kCCParticleModeRadius = ParticleSystem::Mode::RADIUS;
[[deprecated]] const int kCCParticleDefaultCapacity = kParticleDefaultCapacity;

[[deprecated]] const ParticleSystem::PositionType kCCPositionTypeFree =  ParticleSystem::PositionType::FREE;
[[deprecated]] const ParticleSystem::PositionType kCCPositionTypeRelative =  ParticleSystem::PositionType::RELATIVE;
[[deprecated]] const ParticleSystem::PositionType kCCPositionTypeGrouped =  ParticleSystem::PositionType::GROUPED;

[[deprecated]] typedef ParticleSystem::PositionType tPositionType;

#define kCCLabelAutomaticWidth  kLabelAutomaticWidth


[[deprecated]] const Menu::State kCCMenuStateWaiting = Menu::State::WAITING;
[[deprecated]] const Menu::State kCCMenuStateTrackingTouch = Menu::State::TRACKING_TOUCH;

[[deprecated]] typedef Menu::State tMenuState;

[[deprecated]] const Touch::DispatchMode kCCTouchesOneByOne = Touch::DispatchMode::ONE_BY_ONE;
[[deprecated]] const Touch::DispatchMode kCCTouchesAllAtOnce = Touch::DispatchMode::ALL_AT_ONCE;

[[deprecated]] typedef Touch::DispatchMode ccTouchesMode;

[[deprecated]] const Image::Format kCCImageFormatPNG = Image::Format::PNG;
[[deprecated]] const Image::Format kCCImageFormatJPEG = Image::Format::JPG;

[[deprecated]] typedef Image::Format tImageFormat;

[[deprecated]] const TransitionScene::Orientation kCCTransitionOrientationLeftOver = TransitionScene::Orientation::LEFT_OVER;
[[deprecated]] const TransitionScene::Orientation kCCTransitionOrientationRightOver = TransitionScene::Orientation::RIGHT_OVER;
[[deprecated]] const TransitionScene::Orientation kCCTransitionOrientationUpOver = TransitionScene::Orientation::UP_OVER;
[[deprecated]] const TransitionScene::Orientation kCCTransitionOrientationDownOver = TransitionScene::Orientation::DOWN_OVER;

[[deprecated]] typedef TransitionScene::Orientation tOrientation;

[[deprecated]] const int kCCPrioritySystem = Scheduler::PRIORITY_SYSTEM;
[[deprecated]] const int kCCPriorityNonSystemMin = Scheduler::PRIORITY_NON_SYSTEM_MIN;

[[deprecated]] const int kCCActionTagInvalid = Action::INVALID_TAG;
[[deprecated]] const int kCCNodeTagInvalid = Node::INVALID_TAG;

[[deprecated]] const int kCCNodeOnEnter = kNodeOnEnter;
[[deprecated]] const int kCCNodeOnExit = kNodeOnExit;
[[deprecated]] const int kCCNodeOnEnterTransitionDidFinish = kNodeOnEnterTransitionDidFinish;
[[deprecated]] const int kCCNodeOnExitTransitionDidStart = kNodeOnExitTransitionDidStart;
[[deprecated]] const int kCCNodeOnCleanup = kNodeOnCleanup;


[[deprecated]] const LanguageType kLanguageEnglish     = LanguageType::ENGLISH;
[[deprecated]] const LanguageType kLanguageChinese     = LanguageType::CHINESE;
[[deprecated]] const LanguageType kLanguageFrench      = LanguageType::FRENCH;
[[deprecated]] const LanguageType kLanguageItalian     = LanguageType::ITALIAN;
[[deprecated]] const LanguageType kLanguageGerman      = LanguageType::GERMAN;
[[deprecated]] const LanguageType kLanguageSpanish     = LanguageType::SPANISH;
[[deprecated]] const LanguageType kLanguageRussian     = LanguageType::RUSSIAN;
[[deprecated]] const LanguageType kLanguageKorean      = LanguageType::KOREAN;
[[deprecated]] const LanguageType kLanguageJapanese    = LanguageType::JAPANESE;
[[deprecated]] const LanguageType kLanguageHungarian   = LanguageType::HUNGARIAN;
[[deprecated]] const LanguageType kLanguagePortuguese  = LanguageType::PORTUGUESE;
[[deprecated]] const LanguageType kLanguageArabic      = LanguageType::ARABIC;
[[deprecated]] const LanguageType kLanguageNorwegian   = LanguageType::NORWEGIAN;
[[deprecated]] const LanguageType kLanguagePolish      = LanguageType::POLISH;
[[deprecated]] typedef LanguageType ccLanguageType;


[[deprecated]] const Application::Platform kTargetWindows      = Application::Platform::OS_WINDOWS;
[[deprecated]] const Application::Platform kTargetLinux        = Application::Platform::OS_LINUX;
[[deprecated]] const Application::Platform kTargetMacOS        = Application::Platform::OS_MAC;
[[deprecated]] const Application::Platform kTargetAndroid      = Application::Platform::OS_ANDROID;
[[deprecated]] const Application::Platform kTargetIphone       = Application::Platform::OS_IPHONE;
[[deprecated]] const Application::Platform kTargetIpad         = Application::Platform::OS_IPAD;
[[deprecated]] const Application::Platform kTargetBlackBerry   = Application::Platform::OS_BLACKBERRY;
[[deprecated]] const Application::Platform kTargetNaCl         = Application::Platform::OS_NACL;
[[deprecated]] const Application::Platform kTargetEmscripten   = Application::Platform::OS_EMSCRIPTEN;
[[deprecated]] const Application::Platform kTargetTizen        = Application::Platform::OS_TIZEN;
[[deprecated]] const Application::Platform kTargetWinRT        = Application::Platform::OS_WINRT;
[[deprecated]] const Application::Platform kTargetWP8          = Application::Platform::OS_WP8;
[[deprecated]] typedef Application::Platform TargetPlatform;

[[deprecated]] const ResolutionPolicy kResolutionExactFit      = ResolutionPolicy::EXACT_FIT;
[[deprecated]] const ResolutionPolicy kResolutionNoBorder      = ResolutionPolicy::NO_BORDER;
[[deprecated]] const ResolutionPolicy kResolutionShowAll       = ResolutionPolicy::SHOW_ALL;
[[deprecated]] const ResolutionPolicy kResolutionFixedHeight   = ResolutionPolicy::FIXED_HEIGHT;
[[deprecated]] const ResolutionPolicy kResolutionFixedWidth    = ResolutionPolicy::FIXED_WIDTH;
[[deprecated]] const ResolutionPolicy kResolutionUnKnown       = ResolutionPolicy::UNKNOWN;


#define kCCTMXTileHorizontalFlag      kTMXTileHorizontalFlag
#define kCCTMXTileVerticalFlag        kTMXTileVerticalFlag
#define kCCTMXTileDiagonalFlag        kTMXTileDiagonalFlag
#define kCCFlipedAll                  kFlipedAll
#define kCCFlippedMask                kTMXFlippedMask


/** use log() instead */
[[deprecated]] void CC_DLL CCLog(const char * pszFormat, ...) CC_FORMAT_PRINTF(1, 2);

[[deprecated]] void CC_DLL ccDrawInit();
[[deprecated]] void CC_DLL ccDrawFree();
[[deprecated]] void CC_DLL ccDrawPoint( const Vec2& point );
[[deprecated]] void CC_DLL ccDrawPoints( const Vec2 *points, unsigned int numberOfPoints );
[[deprecated]] void CC_DLL ccDrawLine( const Vec2& origin, const Vec2& destination );
[[deprecated]] void CC_DLL ccDrawRect( Vec2 origin, Vec2 destination );
[[deprecated]] void CC_DLL ccDrawSolidRect( Vec2 origin, Vec2 destination, Color4F color );
[[deprecated]] void CC_DLL ccDrawPoly( const Vec2 *vertices, unsigned int numOfVertices, bool closePolygon );
[[deprecated]] void CC_DLL ccDrawSolidPoly( const Vec2 *poli, unsigned int numberOfPoints, Color4F color );
[[deprecated]] void CC_DLL ccDrawCircle( const Vec2& center, float radius, float angle, unsigned int segments, bool drawLineToCenter, float scaleX, float scaleY);
[[deprecated]] void CC_DLL ccDrawCircle( const Vec2& center, float radius, float angle, unsigned int segments, bool drawLineToCenter);
[[deprecated]] void CC_DLL ccDrawSolidCircle( const Vec2& center, float radius, float angle, unsigned int segments, float scaleX, float scaleY);
[[deprecated]] void CC_DLL ccDrawSolidCircle( const Vec2& center, float radius, float angle, unsigned int segments);
[[deprecated]] void CC_DLL ccDrawQuadBezier(const Vec2& origin, const Vec2& control, const Vec2& destination, unsigned int segments);
[[deprecated]] void CC_DLL ccDrawCubicBezier(const Vec2& origin, const Vec2& control1, const Vec2& control2, const Vec2& destination, unsigned int segments);
[[deprecated]] void CC_DLL ccDrawCatmullRom( PointArray *arrayOfControlPoints, unsigned int segments );
[[deprecated]] void CC_DLL ccDrawCardinalSpline( PointArray *config, float tension,  unsigned int segments );
[[deprecated]] void CC_DLL ccDrawColor4B( GLubyte r, GLubyte g, GLubyte b, GLubyte a );
[[deprecated]] void CC_DLL ccDrawColor4F( GLfloat r, GLfloat g, GLfloat b, GLfloat a );
[[deprecated]] void CC_DLL ccPointSize( GLfloat pointSize );


[[deprecated]] inline void CC_DLL ccGLInvalidateStateCache() { GL::invalidateStateCache(); }
[[deprecated]] inline void CC_DLL ccGLUseProgram(GLuint program) { GL::useProgram(program); }
[[deprecated]] inline void CC_DLL ccGLDeleteProgram(GLuint program){ GL::deleteProgram(program); }
[[deprecated]] inline void CC_DLL ccGLBlendFunc(GLenum sfactor, GLenum dfactor) { GL::blendFunc(sfactor, dfactor); }
[[deprecated]] inline void CC_DLL ccGLBlendResetToCache() { GL::blendResetToCache(); }
[[deprecated]] inline void CC_DLL ccSetProjectionMatrixDirty() { GL::setProjectionMatrixDirty(); }
[[deprecated]] inline void CC_DLL ccGLEnableVertexAttribs(unsigned int flags) { GL::enableVertexAttribs(flags); }
[[deprecated]] inline void CC_DLL ccGLBindTexture2D(GLuint textureId) { GL::bindTexture2D(textureId); }
[[deprecated]] inline void CC_DLL ccGLBindTexture2DN(GLuint textureUnit, GLuint textureId) { GL::bindTexture2DN(textureUnit, textureId); }
[[deprecated]] inline void CC_DLL ccGLDeleteTexture(GLuint textureId) { GL::deleteTexture(textureId); }
[[deprecated]] inline void CC_DLL ccGLDeleteTextureN(GLuint textureUnit, GLuint textureId) { GL::deleteTexture(textureId); }
[[deprecated]] inline void CC_DLL ccGLBindVAO(GLuint vaoId) { GL::bindVAO(vaoId); }
[[deprecated]] inline void CC_DLL ccGLEnable( int flags ) { /* ignore */ }
[[deprecated]] typedef int ccGLServerState;

[[deprecated]] typedef Data CCData;
[[deprecated]] typedef __Set CCSet;
[[deprecated]] typedef __SetIterator CCSetIterator;
[[deprecated]] typedef __Set Set;
[[deprecated]] typedef __SetIterator SetIterator;

[[deprecated]] typedef __Array CCArray;
[[deprecated]] typedef __Array Array;

[[deprecated]] typedef __Dictionary Dictionary;
[[deprecated]] typedef __Dictionary CCDictionary;

[[deprecated]] typedef __Double Double;
[[deprecated]] typedef __Double CCDouble;
[[deprecated]] typedef __Float Float;
[[deprecated]] typedef __Float CCFloat;
[[deprecated]] typedef __Integer Integer;
[[deprecated]] typedef __Integer CCInteger;
[[deprecated]] typedef __Bool Bool;
[[deprecated]] typedef __Bool CCBool;
[[deprecated]] typedef __String CCString;
[[deprecated]] typedef __String String;

[[deprecated]] typedef __RGBAProtocol RGBAProtocol;
[[deprecated]] typedef __NodeRGBA NodeRGBA;
[[deprecated]] typedef __LayerRGBA LayerRGBA;

//deprecated attributes and methods for kazMath
[[deprecated]] typedef float kmScalar;

//kmMat4 and kmMat4 stack
[[deprecated]] typedef Mat4 kmMat4;
[[deprecated]] const unsigned int KM_GL_MODELVIEW = 0x1700;
[[deprecated]] const unsigned int KM_GL_PROJECTION = 0x1701;
[[deprecated]] const unsigned int KM_GL_TEXTURE = 0x1702;

[[deprecated]] void CC_DLL kmGLFreeAll();
[[deprecated]] void CC_DLL kmGLPushMatrix();
[[deprecated]] void CC_DLL kmGLPopMatrix();
[[deprecated]] void CC_DLL kmGLMatrixMode(unsigned int mode);
[[deprecated]] void CC_DLL kmGLLoadIdentity();
[[deprecated]] void CC_DLL kmGLLoadMatrix(const Mat4* pIn);
[[deprecated]] void CC_DLL kmGLMultMatrix(const Mat4* pIn);
[[deprecated]] void CC_DLL kmGLTranslatef(float x, float y, float z);
[[deprecated]] void CC_DLL kmGLRotatef(float angle, float x, float y, float z);
[[deprecated]] void CC_DLL kmGLScalef(float x, float y, float z);
[[deprecated]] void CC_DLL kmGLGetMatrix(unsigned int mode, Mat4* pOut);

[[deprecated]] CC_DLL Mat4* kmMat4Fill(Mat4* pOut, const float* pMat);
[[deprecated]] CC_DLL Mat4* kmMat4Assign(Mat4* pOut, const Mat4* pIn);
[[deprecated]] CC_DLL Mat4* kmMat4Identity(Mat4* pOut);
[[deprecated]] CC_DLL Mat4* kmMat4Inverse(Mat4* pOut, const Mat4* pM);
[[deprecated]] CC_DLL Mat4* kmMat4Transpose(Mat4* pOut, const Mat4* pIn);
[[deprecated]] CC_DLL Mat4* kmMat4Multiply(Mat4* pOut, const Mat4* pM1, const Mat4* pM2);
[[deprecated]] CC_DLL Mat4* kmMat4Translation(Mat4* pOut, const float x, const float y, const float z);
[[deprecated]] CC_DLL Mat4* kmMat4RotationX(Mat4* pOut, const float radians);
[[deprecated]] CC_DLL Mat4* kmMat4RotationY(Mat4* pOut, const float radians);
[[deprecated]] CC_DLL Mat4* kmMat4RotationZ(Mat4* pOut, const float radians);
[[deprecated]] CC_DLL Mat4* kmMat4RotationAxisAngle(Mat4* pOut, const Vec3* axis, float radians);
[[deprecated]] CC_DLL Mat4* kmMat4Scaling(Mat4* pOut, const float x, const float y, const float z);

[[deprecated]] CC_DLL Mat4* kmMat4PerspectiveProjection(Mat4* pOut, float fovY, float aspect, float zNear, float zFar);
[[deprecated]] CC_DLL Mat4* kmMat4OrthographicProjection(Mat4* pOut, float left, float right, float bottom, float top, float nearVal, float farVal);
[[deprecated]] CC_DLL Mat4* kmMat4LookAt(Mat4* pOut, const Vec3* pEye, const Vec3* pCenter, const Vec3* pUp);

//kmVec3
[[deprecated]] typedef Vec3 kmVec3;
[[deprecated]] CC_DLL Vec3* kmVec3Fill(Vec3* pOut, float x, float y, float z);
[[deprecated]] CC_DLL float kmVec3Length(const Vec3* pIn);
[[deprecated]] CC_DLL float kmVec3LengthSq(const Vec3* pIn);
[[deprecated]] CC_DLL Vec3* kmVec3Lerp(Vec3* pOut, const Vec3* pV1, const Vec3* pV2, float t);
[[deprecated]] CC_DLL Vec3* kmVec3Normalize(Vec3* pOut, const Vec3* pIn);
[[deprecated]] CC_DLL Vec3* kmVec3Cross(Vec3* pOut, const Vec3* pV1, const Vec3* pV2);
[[deprecated]] CC_DLL float kmVec3Dot(const Vec3* pV1, const Vec3* pV2);
[[deprecated]] CC_DLL Vec3* kmVec3Add(Vec3* pOut, const Vec3* pV1, const Vec3* pV2);
[[deprecated]] CC_DLL Vec3* kmVec3Subtract(Vec3* pOut, const Vec3* pV1, const Vec3* pV2);

[[deprecated]] CC_DLL Vec3* kmVec3Transform(Vec3* pOut, const Vec3* pV1, const Mat4* pM);
[[deprecated]] CC_DLL Vec3* kmVec3TransformNormal(Vec3* pOut, const Vec3* pV, const Mat4* pM);
[[deprecated]] CC_DLL Vec3* kmVec3TransformCoord(Vec3* pOut, const Vec3* pV, const Mat4* pM);
[[deprecated]] CC_DLL Vec3* kmVec3Scale(Vec3* pOut, const Vec3* pIn, const float s);
[[deprecated]] CC_DLL Vec3* kmVec3Assign(Vec3* pOut, const Vec3* pIn);
[[deprecated]] CC_DLL Vec3* kmVec3Zero(Vec3* pOut);

[[deprecated]] extern const Vec3 KM_VEC3_NEG_Z;
[[deprecated]] extern const Vec3 KM_VEC3_POS_Z;
[[deprecated]] extern const Vec3 KM_VEC3_POS_Y;
[[deprecated]] extern const Vec3 KM_VEC3_NEG_Y;
[[deprecated]] extern const Vec3 KM_VEC3_NEG_X;
[[deprecated]] extern const Vec3 KM_VEC3_POS_X;
[[deprecated]] extern const Vec3 KM_VEC3_ZERO;

//kmVec2
[[deprecated]] typedef Vec2 kmVec2;
[[deprecated]] CC_DLL Vec2* kmVec2Fill(Vec2* pOut, float x, float y);
[[deprecated]] CC_DLL float kmVec2Length(const Vec2* pIn);
[[deprecated]] CC_DLL float kmVec2LengthSq(const Vec2* pIn);
[[deprecated]] CC_DLL Vec2* kmVec2Normalize(Vec2* pOut, const Vec2* pIn);
[[deprecated]] CC_DLL Vec2* kmVec2Lerp(Vec2* pOut, const Vec2* pV1, const Vec2* pV2, float t);
[[deprecated]] CC_DLL Vec2* kmVec2Add(Vec2* pOut, const Vec2* pV1, const Vec2* pV2);
[[deprecated]] CC_DLL float kmVec2Dot(const Vec2* pV1, const Vec2* pV2);
[[deprecated]] CC_DLL Vec2* kmVec2Subtract(Vec2* pOut, const Vec2* pV1, const Vec2* pV2);
[[deprecated]] CC_DLL Vec2* kmVec2Scale(Vec2* pOut, const Vec2* pIn, const float s);
[[deprecated]] CC_DLL Vec2* kmVec2Assign(Vec2* pOut, const Vec2* pIn);

[[deprecated]] extern const Vec2 KM_VEC2_POS_Y;
[[deprecated]] extern const Vec2 KM_VEC2_NEG_Y;
[[deprecated]] extern const Vec2 KM_VEC2_NEG_X;
[[deprecated]] extern const Vec2 KM_VEC2_POS_X;
[[deprecated]] extern const Vec2 KM_VEC2_ZERO;

//kmVec4
[[deprecated]] typedef Vec4 kmVec4;
[[deprecated]] CC_DLL Vec4* kmVec4Fill(Vec4* pOut, float x, float y, float z, float w);
[[deprecated]] CC_DLL Vec4* kmVec4Add(Vec4* pOut, const Vec4* pV1, const Vec4* pV2);
[[deprecated]] CC_DLL float kmVec4Dot(const Vec4* pV1, const Vec4* pV2);
[[deprecated]] CC_DLL float kmVec4Length(const Vec4* pIn);
[[deprecated]] CC_DLL float kmVec4LengthSq(const Vec4* pIn);
[[deprecated]] CC_DLL Vec4* kmVec4Lerp(Vec4* pOut, const Vec4* pV1, const Vec4* pV2, float t);
[[deprecated]] CC_DLL Vec4* kmVec4Normalize(Vec4* pOut, const Vec4* pIn);
[[deprecated]] CC_DLL Vec4* kmVec4Scale(Vec4* pOut, const Vec4* pIn, const float s);
[[deprecated]] CC_DLL Vec4* kmVec4Subtract(Vec4* pOut, const Vec4* pV1, const Vec4* pV2);
[[deprecated]] CC_DLL Vec4* kmVec4Assign(Vec4* pOut, const Vec4* pIn);
[[deprecated]] CC_DLL Vec4* kmVec4MultiplyMat4(Vec4* pOut, const Vec4* pV, const Mat4* pM);
[[deprecated]] CC_DLL Vec4* kmVec4Transform(Vec4* pOut, const Vec4* pV, const Mat4* pM);

//end of deprecated attributes and methods for kazMath

NS_CC_END

/// @endcond
#endif // __COCOS2D_CCDEPRECATED_H__
