#pragma once
#define LEGION_LOG_DEBUG

#if defined(NDEBUG)
#define LEGION_KEEP_CONSOLE
#endif

#include <core/core.hpp>
#include <application/application.hpp>
#include <rendering/rendering.hpp>
#include <audio/audio.hpp>
#include <rendering/debugrendering.hpp>
