// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "Misc/Build.h"

/**
 * @brief Compile-time build features: switches that decide whether a piece of code exists in this build at all.
 *
 * One feature is one pair:
 * - `REM_WITH_<FEATURE>` - a 0/1 switch, defaulting from an engine build define;
 * - `REM_<FEATURE>_ONLY(...)` - the block that switch guards, empty when the switch is off.
 *
 * A feature whose code disappears must not change what the remaining code does, so the guarded block holds
 * the work that only that feature needs (building payloads, spawning components, subscribing sources) - not a
 * branch that leaves a half-initialised object behind. Reach for a derived value with `#if` when the feature
 * is a precondition of the surrounding block rather than a wrapper around one.
 *
 * Prefer a compile-time switch over a runtime net-mode check when the answer cannot change during a session:
 * a listen server is a client build and keeps client-only code, so `IsNetMode(NM_DedicatedServer)` is not a
 * substitute for `UE_SERVER` as the gate.
 */

/** code that only exists outside a shipping build */
#ifndef REM_WITH_DEVELOPMENT_ONLY_CODE
#define REM_WITH_DEVELOPMENT_ONLY_CODE !UE_BUILD_SHIPPING
#endif

/** debug drawing, on top of the development-only gate */
#ifndef REM_DRAW_DEBUG_CODE
#define REM_DRAW_DEBUG_CODE REM_WITH_DEVELOPMENT_ONLY_CODE && UE_ENABLE_DEBUG_DRAWING
#endif

/**
 * code that only exists where something can be seen: a dedicated-server build compiles it out
 *
 * UE_SERVER is the compile-time half of "no local screen"; a listen server is a client build and keeps its
 * cosmetics, which is why a runtime IsNetMode(NM_DedicatedServer) check is the wrong gate here
 */
#ifndef REM_WITH_COSMETIC
#define REM_WITH_COSMETIC (!UE_SERVER)
#endif

#if REM_WITH_COSMETIC
#define REM_COSMETIC_ONLY(...) __VA_ARGS__
#else
#define REM_COSMETIC_ONLY(...)
#endif
