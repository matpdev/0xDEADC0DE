/**
 * @file AnimationSystem.cpp
 * @brief Implementation of animation system using tweeny library
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-30
 */

#include "deadcode/graphics/AnimationSystem.hpp"

#include "deadcode/core/Logger.hpp"

#include <algorithm>

#include <tweeny.h>

namespace deadcode
{

// ============================================================================
// TWEENY-BASED ANIMATION IMPLEMENTATION
// ============================================================================

template <typename T>
class TweenyAnimation : public IAnimation
{
public:
    TweenyAnimation(std::uint32_t id, T& target, tweeny::tween<T> tween,
                    std::function<void()> onComplete)
        : m_id(id), m_target(target), m_tween(tween), m_onComplete(onComplete), m_complete(false)
    {
    }

    bool
    update(float32 deltaTime) override
    {
        if (m_complete)
            return false;

        // tweeny uses milliseconds, convert deltaTime (seconds) to ms
        int32_t stepMs = static_cast<int32_t>(deltaTime * 1000.0f);
        m_tween.step(stepMs);
        m_target = m_tween.peek();

        if (m_tween.progress() >= 1.0f)
        {
            m_complete = true;
            if (m_onComplete)
            {
                m_onComplete();
            }
            return false;
        }

        return true;
    }

    bool
    isComplete() const override
    {
        return m_complete;
    }

    void
    reset() override
    {
        m_complete = false;
        m_tween.seek(0);
        m_target = m_tween.peek();
    }

    std::uint32_t
    getID() const override
    {
        return m_id;
    }

    AnimationProperty
    getProperty() const override
    {
        return AnimationProperty::Custom;
    }

private:
    std::uint32_t m_id;
    T& m_target;
    tweeny::tween<T> m_tween;
    std::function<void()> m_onComplete;
    bool m_complete;
};

// ============================================================================
// EASING FUNCTION IMPLEMENTATIONS (Placeholder - using tweeny's defaults)
// ============================================================================

namespace Easing
{

// For now, these return the t value directly since tweeny handles easing internally
// TODO: Properly integrate with tweeny's easing system which uses different signature
float32
linear(float32 t)
{
    return t;
}

float32
easeInQuad(float32 t)
{
    return t * t;
}

float32
easeOutQuad(float32 t)
{
    return 1.0f - (1.0f - t) * (1.0f - t);
}

float32
easeInOutQuad(float32 t)
{
    return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
}

float32
easeInCubic(float32 t)
{
    return t * t * t;
}

float32
easeOutCubic(float32 t)
{
    return 1.0f - std::pow(1.0f - t, 3.0f);
}

float32
easeInOutCubic(float32 t)
{
    return t < 0.5f ? 4.0f * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
}

float32
easeInQuart(float32 t)
{
    return t * t * t * t;
}

float32
easeOutQuart(float32 t)
{
    return 1.0f - std::pow(1.0f - t, 4.0f);
}

float32
easeInOutQuart(float32 t)
{
    return t < 0.5f ? 8.0f * t * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 4.0f) / 2.0f;
}

float32
easeInQuint(float32 t)
{
    return t * t * t * t * t;
}

float32
easeOutQuint(float32 t)
{
    return 1.0f - std::pow(1.0f - t, 5.0f);
}

float32
easeInOutQuint(float32 t)
{
    return t < 0.5f ? 16.0f * t * t * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 5.0f) / 2.0f;
}

float32
easeInSine(float32 t)
{
    constexpr float32 MY_PI = 3.14159265358979323846f;
    return 1.0f - std::cos((t * MY_PI) / 2.0f);
}

float32
easeOutSine(float32 t)
{
    constexpr float32 MY_PI = 3.14159265358979323846f;
    return std::sin((t * MY_PI) / 2.0f);
}

float32
easeInOutSine(float32 t)
{
    constexpr float32 MY_PI = 3.14159265358979323846f;
    return -(std::cos(MY_PI * t) - 1.0f) / 2.0f;
}

float32
easeInExpo(float32 t)
{
    return t == 0.0f ? 0.0f : std::pow(2.0f, 10.0f * t - 10.0f);
}

float32
easeOutExpo(float32 t)
{
    return t == 1.0f ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t);
}

float32
easeInOutExpo(float32 t)
{
    if (t == 0.0f)
        return 0.0f;
    if (t == 1.0f)
        return 1.0f;
    return t < 0.5f ? std::pow(2.0f, 20.0f * t - 10.0f) / 2.0f
                    : (2.0f - std::pow(2.0f, -20.0f * t + 10.0f)) / 2.0f;
}

float32
easeInCirc(float32 t)
{
    return 1.0f - std::sqrt(1.0f - std::pow(t, 2.0f));
}

float32
easeOutCirc(float32 t)
{
    return std::sqrt(1.0f - std::pow(t - 1.0f, 2.0f));
}

float32
easeInOutCirc(float32 t)
{
    return t < 0.5f ? (1.0f - std::sqrt(1.0f - std::pow(2.0f * t, 2.0f))) / 2.0f
                    : (std::sqrt(1.0f - std::pow(-2.0f * t + 2.0f, 2.0f)) + 1.0f) / 2.0f;
}

float32
easeInElastic(float32 t)
{
    constexpr float32 MY_PI = 3.14159265358979323846f;
    constexpr float32 c4 = (2.0f * MY_PI) / 3.0f;
    if (t == 0.0f)
        return 0.0f;
    if (t == 1.0f)
        return 1.0f;
    return -std::pow(2.0f, 10.0f * t - 10.0f) * std::sin((t * 10.0f - 10.75f) * c4);
}

float32
easeOutElastic(float32 t)
{
    constexpr float32 MY_PI = 3.14159265358979323846f;
    constexpr float32 c4 = (2.0f * MY_PI) / 3.0f;
    if (t == 0.0f)
        return 0.0f;
    if (t == 1.0f)
        return 1.0f;
    return std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * c4) + 1.0f;
}

float32
easeInOutElastic(float32 t)
{
    constexpr float32 MY_PI = 3.14159265358979323846f;
    constexpr float32 c5 = (2.0f * MY_PI) / 4.5f;
    if (t == 0.0f)
        return 0.0f;
    if (t == 1.0f)
        return 1.0f;
    return t < 0.5f
               ? -(std::pow(2.0f, 20.0f * t - 10.0f) * std::sin((20.0f * t - 11.125f) * c5)) / 2.0f
               : (std::pow(2.0f, -20.0f * t + 10.0f) * std::sin((20.0f * t - 11.125f) * c5)) /
                         2.0f +
                     1.0f;
}

float32
easeInBack(float32 t)
{
    constexpr float32 c1 = 1.70158f;
    constexpr float32 c3 = c1 + 1.0f;
    return c3 * t * t * t - c1 * t * t;
}

float32
easeOutBack(float32 t)
{
    constexpr float32 c1 = 1.70158f;
    constexpr float32 c3 = c1 + 1.0f;
    return 1.0f + c3 * std::pow(t - 1.0f, 3.0f) + c1 * std::pow(t - 1.0f, 2.0f);
}

float32
easeInOutBack(float32 t)
{
    constexpr float32 c1 = 1.70158f;
    constexpr float32 c2 = c1 * 1.525f;
    return t < 0.5f
               ? (std::pow(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f
               : (std::pow(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) /
                     2.0f;
}

float32
easeOutBounce(float32 t)
{
    constexpr float32 n1 = 7.5625f;
    constexpr float32 d1 = 2.75f;
    if (t < 1.0f / d1)
    {
        return n1 * t * t;
    }
    else if (t < 2.0f / d1)
    {
        t -= 1.5f / d1;
        return n1 * t * t + 0.75f;
    }
    else if (t < 2.5f / d1)
    {
        t -= 2.25f / d1;
        return n1 * t * t + 0.9375f;
    }
    else
    {
        t -= 2.625f / d1;
        return n1 * t * t + 0.984375f;
    }
}

float32
easeInBounce(float32 t)
{
    return 1.0f - easeOutBounce(1.0f - t);
}

float32
easeInOutBounce(float32 t)
{
    return t < 0.5f ? (1.0f - easeOutBounce(1.0f - 2.0f * t)) / 2.0f
                    : (1.0f + easeOutBounce(2.0f * t - 1.0f)) / 2.0f;
}

}  // namespace Easing

// ============================================================================
// INTERPOLATOR IMPLEMENTATIONS
// ============================================================================

float32
Interpolator<float32>::interpolate(float32 start, float32 end, float32 t,
                                   std::function<float32(float32)> easingFunc)
{
    float32 easedT = easingFunc(t);
    return start + (end - start) * easedT;
}

glm::vec2
Interpolator<glm::vec2>::interpolate(const glm::vec2& start, const glm::vec2& end, float32 t,
                                     std::function<float32(float32)> easingFunc)
{
    float32 easedT = easingFunc(t);
    return start + (end - start) * easedT;
}

glm::vec3
Interpolator<glm::vec3>::interpolate(const glm::vec3& start, const glm::vec3& end, float32 t,
                                     std::function<float32(float32)> easingFunc)
{
    float32 easedT = easingFunc(t);
    return start + (end - start) * easedT;
}

glm::vec4
Interpolator<glm::vec4>::interpolate(const glm::vec4& start, const glm::vec4& end, float32 t,
                                     std::function<float32(float32)> easingFunc)
{
    float32 easedT = easingFunc(t);
    return start + (end - start) * easedT;
}

// ============================================================================
// ANIMATION SYSTEM IMPLEMENTATION
// ============================================================================

AnimationSystem&
AnimationSystem::getInstance()
{
    static AnimationSystem instance;
    return instance;
}

AnimationSystem::AnimationSystem() : m_nextAnimationID(1), m_initialized(false) {}

AnimationSystem::~AnimationSystem()
{
    shutdown();
}

bool
AnimationSystem::initialize()
{
    if (m_initialized)
    {
        Logger::warn("AnimationSystem already initialized");
        return true;
    }

    Logger::info("Initializing AnimationSystem (using tweeny backend)...");

    m_animations.clear();
    m_nextAnimationID = 1;
    m_initialized     = true;

    Logger::info("AnimationSystem initialized successfully");
    return true;
}

void
AnimationSystem::shutdown()
{
    if (!m_initialized)
        return;

    Logger::info("Shutting down AnimationSystem...");

    stopAll();
    m_animations.clear();
    m_initialized = false;

    Logger::info("AnimationSystem shut down");
}

void
AnimationSystem::update(float32 deltaTime)
{
    if (!m_initialized)
        return;

    // Update all animations and remove completed ones
    m_animations.erase(std::remove_if(m_animations.begin(), m_animations.end(),
                                      [deltaTime](const std::unique_ptr<IAnimation>& anim) {
                                          return !anim->update(deltaTime);
                                      }),
                       m_animations.end());
}

std::uint32_t
AnimationSystem::createTween(float32& target, float32 startValue, float32 endValue,
                             float32 duration, std::function<float32(float32)> easingFunc,
                             std::function<void()> onComplete)
{
    if (!m_initialized)
    {
        Logger::error("AnimationSystem not initialized");
        return 0;
    }

    std::uint32_t id   = m_nextAnimationID++;
    int32_t durationMs = static_cast<int32_t>(duration * 1000.0f);
    auto tween         = tweeny::from(startValue).to(endValue).during(durationMs);

    // Note: easing parameter ignored for now, tweeny uses linear by default
    (void) easingFunc;

    m_animations.push_back(
        std::make_unique<TweenyAnimation<float32>>(id, target, tween, onComplete));

    Logger::debug("Created float tween animation ID: {}", id);
    return id;
}

std::uint32_t
AnimationSystem::createTween(glm::vec2& target, glm::vec2 startValue, glm::vec2 endValue,
                             float32 duration, std::function<float32(float32)> easingFunc,
                             std::function<void()> onComplete)
{
    if (!m_initialized)
    {
        Logger::error("AnimationSystem not initialized");
        return 0;
    }

    std::uint32_t id   = m_nextAnimationID++;
    int32_t durationMs = static_cast<int32_t>(duration * 1000.0f);
    auto tweenVec2     = tweeny::from(startValue).to(endValue).during(durationMs);

    // Note: easing parameter ignored for now
    (void) easingFunc;

    m_animations.push_back(
        std::make_unique<TweenyAnimation<glm::vec2>>(id, target, tweenVec2, onComplete));

    Logger::debug("Created vec2 tween animation ID: {}", id);
    return id;
}

std::uint32_t
AnimationSystem::createTween(glm::vec3& target, glm::vec3 startValue, glm::vec3 endValue,
                             float32 duration, std::function<float32(float32)> easingFunc,
                             std::function<void()> onComplete)
{
    if (!m_initialized)
    {
        Logger::error("AnimationSystem not initialized");
        return 0;
    }

    std::uint32_t id   = m_nextAnimationID++;
    int32_t durationMs = static_cast<int32_t>(duration * 1000.0f);
    auto tweenVec3     = tweeny::from(startValue).to(endValue).during(durationMs);

    // Note: easing parameter ignored for now
    (void) easingFunc;

    m_animations.push_back(
        std::make_unique<TweenyAnimation<glm::vec3>>(id, target, tweenVec3, onComplete));

    Logger::debug("Created vec3 tween animation ID: {}", id);
    return id;
}

std::uint32_t
AnimationSystem::createTween(glm::vec4& target, glm::vec4 startValue, glm::vec4 endValue,
                             float32 duration, std::function<float32(float32)> easingFunc,
                             std::function<void()> onComplete)
{
    if (!m_initialized)
    {
        Logger::error("AnimationSystem not initialized");
        return 0;
    }

    std::uint32_t id   = m_nextAnimationID++;
    int32_t durationMs = static_cast<int32_t>(duration * 1000.0f);
    auto tweenVec4     = tweeny::from(startValue).to(endValue).during(durationMs);

    // Note: easing parameter ignored for now
    (void) easingFunc;

    m_animations.push_back(
        std::make_unique<TweenyAnimation<glm::vec4>>(id, target, tweenVec4, onComplete));

    Logger::debug("Created vec4 tween animation ID: {}", id);
    return id;
}

void
AnimationSystem::stopAnimation(std::uint32_t animationID)
{
    m_animations.erase(std::remove_if(m_animations.begin(), m_animations.end(),
                                      [animationID](const std::unique_ptr<IAnimation>& anim) {
                                          return anim->getID() == animationID;
                                      }),
                       m_animations.end());

    Logger::debug("Stopped animation ID: {}", animationID);
}

void
AnimationSystem::stopAll()
{
    std::uint32_t count = static_cast<std::uint32_t>(m_animations.size());
    m_animations.clear();

    Logger::debug("Stopped all {} animations", count);
}

std::uint32_t
AnimationSystem::getActiveAnimationCount() const
{
    return static_cast<std::uint32_t>(m_animations.size());
}

bool
AnimationSystem::isRunning(std::uint32_t animationID) const
{
    return std::any_of(m_animations.begin(), m_animations.end(),
                       [animationID](const std::unique_ptr<IAnimation>& anim) {
                           return anim->getID() == animationID && !anim->isComplete();
                       });
}

}  // namespace deadcode
