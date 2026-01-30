# Procedural Animations & Glitch Effect - TODO

## Feature Overview
Implement a flexible AnimationSystem framework supporting switchable animation models (Keyframe or Tween-based) with configurable glitch effects for text rendering, particularly for logo animation with Pixel Operator font.

---

## Core Requirements

### Animation Model Architecture
- [ ] **Choose animation backend**: Keyframe-based OR Tween-based (mutually exclusive, configurable)
- [ ] **AnimationSystem core** manages all animation logic regardless of backend
- [ ] **Backend abstraction layer** allows runtime switching between A (Keyframe) and B (Tween) without recompilation
- [ ] **Configuration flag** in `assets/config/game.json` to select active animation model

### AnimationSystem Framework
- [ ] Create `include/deadcode/graphics/AnimationSystem.hpp`
  - [ ] Base animation classes/interfaces
  - [ ] Easing functions library (linear, ease-in/out, elastic, bounce, etc.)
  - [ ] Animation state container (tracks all active animations)
  - [ ] Property interpolators (float, vec2, vec3, vec4, color)
  - [ ] Time-based update mechanism

- [ ] Create `src/graphics/AnimationSystem.cpp`
  - [ ] Implement easing calculations
  - [ ] Implement interpolation logic
  - [ ] Implement animation lifecycle (start, update, complete, cancel)

- [ ] Create `include/deadcode/graphics/AnimationBackend.hpp`
  - [ ] `IAnimationBackend` interface
  - [ ] `KeyframeBackend` implementation (Option A)
  - [ ] `TweenBackend` implementation (Option B)

- [ ] Create `src/graphics/KeyframeBackend.cpp`
  - [ ] Keyframe track management
  - [ ] Keyframe interpolation logic

- [ ] Create `src/graphics/TweenBackend.cpp`
  - [ ] Tween creation/management
  - [ ] Tween sequencing (parallel, sequential)
  - [ ] Tween callbacks

### Text Animation Support
- [ ] Extend `TextRenderer.hpp` with animation hooks
  - [ ] Per-character animation state storage
  - [ ] Character property accessors (position offset, color mod, scale, rotation)
  - [ ] Methods to bind AnimationSystem to text rendering

- [ ] Modify `TextRenderer.cpp`
  - [ ] Apply per-character transformations during render
  - [ ] Handle animation-driven uniform updates
  - [ ] Maintain performance with potentially many animated characters

### Glitch Effect Implementation
- [ ] Create `include/deadcode/graphics/GlitchEffect.hpp`
  - [ ] GlitchEffect class wrapping glitch configuration & animation
  - [ ] Character displacement controller (horizontal jitter, scanline shifts)
  - [ ] Color distortion (RGB channel separation, color modulation)
  - [ ] Support for vertex-level & character-level effects

- [ ] Create `src/graphics/GlitchEffect.cpp`
  - [ ] Glitch timing/frequency management
  - [ ] Procedural parameter generation (sine waves, noise, random)
  - [ ] Effect state machine (idle, active, cooldown)

- [ ] Create custom glitch shaders
  - [ ] `assets/shaders/text_glitch.vert` - Vertex displacement
  - [ ] `assets/shaders/text_glitch.frag` - RGB/color distortion
  - [ ] Scanline effect (optional enhancement)

### Configuration System
- [ ] Extend `assets/config/game.json` with animation/glitch settings:
  ```json
  {
    "graphics": {
      "animations": {
        "enabled": true,
        "backend": "tween",  // "keyframe" or "tween"
        "default_easing": "ease-out-cubic"
      },
      "glitch": {
        "enabled": true,
        "intensity": 0.8,           // 0.0-1.0
        "frequency": 3.0,           // Hz
        "character_displacement": true,
        "rgb_separation": true,
        "scanline_effect": false,
        "glitch_duration": 0.1,     // seconds per glitch event
        "idle_time": 2.0,           // seconds between glitches
        "character_jitter": 2.0     // pixels max offset
      }
    }
  }
  ```

- [ ] Update Config class to read animation/glitch settings

### Logo Integration
- [ ] Modify `src/ui/StartMenu.cpp`
  - [ ] Replace basic `m_glitchTimer` with GlitchEffect instance
  - [ ] Integrate AnimationSystem for logo animations
  - [ ] Switch font to Pixel Operator for logo rendering
  - [ ] Bind glitch effect to logo text during rendering

- [ ] Update `src/ui/StartMenu.hpp`
  - [ ] Add GlitchEffect member
  - [ ] Add AnimationSystem pointer
  - [ ] Remove old basic glitch timer logic

### CMakeLists.txt Updates
- [ ] Add new source files to `deadcode_engine` target:
  - [ ] `src/graphics/AnimationSystem.cpp`
  - [ ] `src/graphics/KeyframeBackend.cpp`
  - [ ] `src/graphics/TweenBackend.cpp`
  - [ ] `src/graphics/GlitchEffect.cpp`

---

## Nice-to-Have Enhancements

### Advanced Animation Features
- [ ] Animation composition (sequences, parallels, delays)
- [ ] Animation callbacks/events (on start, on complete, on frame)
- [ ] Animator speed multiplier (slow-motion, fast-forward)
- [ ] Animation preview/debug visualization

### Extended Glitch Effects
- [ ] Chromatic aberration (per-channel offset)
- [ ] Block glitch (rectangular corruptions)
- [ ] Wave distortion (sine-wave vertex manipulation)
- [ ] Bit-crush effect (reduce color depth)

### Performance Optimization
- [ ] Object pooling for animation objects
- [ ] Batch uniform updates for GPU
- [ ] LOD system for animation complexity based on character count
- [ ] Animation culling (skip off-screen text animation)

### Tools & Debugging
- [ ] Animation timeline inspector (VS Code extension?)
- [ ] Runtime glitch parameter tweaking (console commands)
- [ ] Animation performance profiler
- [ ] Frame-by-frame animation stepping

---

## Implementation Order (Recommended)

1. **Phase 1: Core Infrastructure**
   - AnimationSystem base classes (easing, interpolation)
   - AnimationBackend interface
   - TweenBackend implementation (simpler to start with)
   - Config system updates

2. **Phase 2: TextRenderer Integration**
   - Extend TextRenderer for per-character animation state
   - Bind AnimationSystem to text rendering
   - Test with simple text animations

3. **Phase 3: Glitch Effect**
   - GlitchEffect class with procedural parameter generation
   - Glitch shader implementation (text_glitch.vert/frag)
   - Integrate with TextRenderer

4. **Phase 4: Logo Animation**
   - Update StartMenu to use Pixel Operator font
   - Apply GlitchEffect to logo
   - Configure via game.json

5. **Phase 5: Polish & Optional**
   - Add KeyframeBackend for future complex animations
   - Implement nice-to-have features
   - Performance optimization

---

## Testing Checklist

- [ ] AnimationSystem easing functions produce expected curves
- [ ] TweenBackend creates/updates/completes tweens correctly
- [ ] Per-character animation state updates correctly
- [ ] Glitch effect parameters animate smoothly
- [ ] Glyph displacement works without visual artifacts
- [ ] RGB separation effect renders correctly
- [ ] Config file settings apply correctly at runtime
- [ ] Logo font switches to Pixel Operator
- [ ] Glitch effect activates on logo during gameplay
- [ ] No performance regression (60 FPS maintained)
- [ ] Memory cleanup on shutdown (no leaks)

---

## Acceptance Criteria

✅ AnimationSystem framework created and functional  
✅ Switchable between Keyframe and Tween backends via config  
✅ GlitchEffect applies to text with configurable intensity/frequency  
✅ Logo uses Pixel Operator font with active glitch animation  
✅ All glitch parameters tunable via `game.json`  
✅ Code follows project style guide and architecture rules  
✅ No performance degradation on 60 FPS target  
✅ Feature is documented in code (Doxygen comments)  
