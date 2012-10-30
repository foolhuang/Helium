#pragma once

#include "Platform/Platform.h"  // Always make sure Platform.h gets included first.

#if HELIUM_SHARED
# ifdef HELIUM_SCENE_GRAPH_EXPORTS
#  define HELIUM_SCENE_GRAPH_API HELIUM_API_EXPORT
# else
#  define HELIUM_SCENE_GRAPH_API HELIUM_API_IMPORT
# endif
#else
#define HELIUM_SCENE_GRAPH_API
#endif

// profiling for SCENE systems
//#define SCENE_GRAPH_PROFILE

#if defined(PROFILE_INSTRUMENT_ALL) || defined (SCENE_GRAPH_PROFILE)
# define SCENE_GRAPH_SCOPE_TIMER(__Str) PROFILE_SCOPE_TIMER(__Str)
#else
# define SCENE_GRAPH_SCOPE_TIMER(__Str)
#endif

// profiling for SCENE EVALUATE systems
//#define SCENE_GRAPH_PROFILE_EVALUATE

#if defined(PROFILE_INSTRUMENT_ALL) || defined (SCENE_GRAPH_PROFILE_EVALUATE)
# define SCENE_GRAPH_EVALUATE_SCOPE_TIMER(__Str) PROFILE_SCOPE_TIMER(__Str)
#else
# define SCENE_GRAPH_EVALUATE_SCOPE_TIMER(__Str)
#endif

// profiling for SCENE DRAW systems
//#define SCENE_GRAPH_PROFILE_RENDER

#if defined(PROFILE_INSTRUMENT_ALL) || defined (SCENE_GRAPH_PROFILE_RENDER)
# define SCENE_GRAPH_RENDER_SCOPE_TIMER(__Str) PROFILE_SCOPE_TIMER(__Str)
#else
# define SCENE_GRAPH_RENDER_SCOPE_TIMER(__Str)
#endif
