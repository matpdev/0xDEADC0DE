/**
 * @file AnimationSystem.hpp
 * @brief Main system for animations in whole game
 *
 * Beautiful animations with full control using tweeny library
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-29
 */

#pragma once

#include "deadcode/core/Types.hpp"

#include <glm/glm.hpp>

#include <functional>
#include <map>
#include <memory>
#include <vector>

namespace deadcode
{
namespace Easing
{
float32 linear(float32 t);

// Quadratic easing
float32 easeInQuad(float32 t);
float32 easeOutQuad(float32 t);
float32 easeInOutQuad(float32 t);

// Cubic easing
float32 easeInCubic(float32 t);
float32 easeOutCubic(float32 t);
float32 easeInOutCubic(float32 t);

// Quartic easing
float32 easeInQuart(float32 t);
float32 easeOutQuart(float32 t);
float32 easeInOutQuart(float32 t);

// Quintic easing
float32 easeInQuint(float32 t);
float32 easeOutQuint(float32 t);
float32 easeInOutQuint(float32 t);

// Sinusoidal easing
float32 easeInSine(float32 t);
float32 easeOutSine(float32 t);
float32 easeInOutSine(float32 t);

// Exponential easing
float32 easeInExpo(float32 t);
float32 easeOutExpo(float32 t);
float32 easeInOutExpo(float32 t);

// Circular easing
float32 easeInCirc(float32 t);
float32 easeOutCirc(float32 t);
float32 easeInOutCirc(float32 t);

// Elastic easing
float32 easeInElastic(float32 t);
float32 easeOutElastic(float32 t);
float32 easeInOutElastic(float32 t);

// Back easing
float32 easeInBack(float32 t);
float32 easeOutBack(float32 t);
float32 easeInOutBack(float32 t);

// Bounce easing
float32 easeOutBounce(float32 t);
float32 easeInBounce(float32 t);
float32 easeInOutBounce(float32 t);
}  // namespace Easing

// ============================================================================
// INTERPOLATION TEMPLATE
// ============================================================================

template <typename T>
class Interpolator
{
public:
    /// Interpolate between start and end values with easing
    static T interpolate(const T& start, const T& end, float32 t,
                         std::function<float32(float32)> easingFunc = Easing::linear);
};

// Specializations for common types
template <>
class Interpolator<float32>
{
public:
    static float32 interpolate(float32 start, float32 end, float32 t,
                               std::function<float32(float32)> easingFunc = Easing::linear);
};

template <>
class Interpolator<glm::vec2>
{
public:
    static glm::vec2 interpolate(const glm::vec2& start, const glm::vec2& end, float32 t,
                                 std::function<float32(float32)> easingFunc = Easing::linear);
};

template <>
class Interpolator<glm::vec3>
{
public:
    static glm::vec3 interpolate(const glm::vec3& start, const glm::vec3& end, float32 t,
                                 std::function<float32(float32)> easingFunc = Easing::linear);
};

template <>
class Interpolator<glm::vec4>
{
public:
    static glm::vec4 interpolate(const glm::vec4& start, const glm::vec4& end, float32 t,
                                 std::function<float32(float32)> easingFunc = Easing::linear);
};

// ============================================================================
// ANIMATION PROPERTY ENUM
// ============================================================================

enum class AnimationProperty
{
    PositionX,
    PositionY,
    PositionZ,
    Position2D,
    Position3D,
    ScaleX,
    ScaleY,
    ScaleZ,
    Scale2D,
    Scale3D,
    RotationX,
    RotationY,
    RotationZ,
    Rotation3D,
    ColorR,
    ColorG,
    ColorB,
    ColorA,
    Color3,
    Color4,
    Opacity,
    Custom
};

// ============================================================================
// BASE ANIMATION INTERFACE
// ============================================================================

class IAnimation
{
public:
    virtual ~IAnimation() = default;

    /// Update animation by deltaTime, return true if still active
    virtual bool update(float32 deltaTime) = 0;

    /// Check if animation is complete
    virtual bool isComplete() const = 0;

    /// Reset animation to start
    virtual void reset() = 0;

    /// Get animation unique ID
    virtual uint32 getID() const = 0;

    /// Get animation property type
    virtual AnimationProperty getProperty() const = 0;
};

// ============================================================================
// ANIMATION SYSTEM (SINGLETON)
// ============================================================================

class AnimationSystem
{
public:
    /// Get singleton instance
    static AnimationSystem& getInstance();

    /// Initialize animation system
    bool initialize();

    /// Shutdown animation system
    void shutdown();

    /// Update all active animations
    void update(float32 deltaTime);

    /// Create a new tween animation for float
    uint32 createTween(float32& target, float32 startValue, float32 endValue, float32 duration,
                       std::function<float32(float32)> easingFunc = Easing::linear,
                       std::function<void()> onComplete           = nullptr);

    /// Create a new tween animation for vec2
    uint32 createTween(glm::vec2& target, glm::vec2 startValue, glm::vec2 endValue,
                       float32 duration,
                       std::function<float32(float32)> easingFunc = Easing::linear,
                       std::function<void()> onComplete           = nullptr);

    /// Create a new tween animation for vec3
    uint32 createTween(glm::vec3& target, glm::vec3 startValue, glm::vec3 endValue,
                       float32 duration,
                       std::function<float32(float32)> easingFunc = Easing::linear,
                       std::function<void()> onComplete           = nullptr);

    /// Create a new tween animation for vec4
    uint32 createTween(glm::vec4& target, glm::vec4 startValue, glm::vec4 endValue,
                       float32 duration,
                       std::function<float32(float32)> easingFunc = Easing::linear,
                       std::function<void()> onComplete           = nullptr);

    /// Stop animation by ID
    void stopAnimation(uint32 animationID);

    /// Stop all animations
    void stopAll();

    /// Get number of active animations
    uint32 getActiveAnimationCount() const;

    /// Check if animation is still running
    bool isRunning(uint32 animationID) const;

private:
    AnimationSystem();
    ~AnimationSystem();

    // Prevent copy/move
    AnimationSystem(const AnimationSystem&)            = delete;
    AnimationSystem& operator=(const AnimationSystem&) = delete;

    std::vector<std::unique_ptr<IAnimation>> m_animations;
    uint32 m_nextAnimationID;
    bool m_initialized;
};

}  // namespace deadcode
