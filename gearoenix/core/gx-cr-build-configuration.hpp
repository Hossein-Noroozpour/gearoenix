#ifndef GEAROENIX_CORE_BUILD_CONFIGURATION_HPP
#define GEAROENIX_CORE_BUILD_CONFIGURATION_HPP
// Platform definition
#ifdef ANDROID
#define GX_IN_ANDROID
//#define GX_USE_VULKAN
#define GX_USE_OPENGL_ES3
#define GX_USE_OPENGL_ES2
#elif defined(__EMSCRIPTEN__)
#define GX_IN_WEB
#define GX_USE_OPENGL_ES3
#define GX_USE_OPENGL_ES2
//#define GX_USE_SDL
#define GX_THREAD_NOT_SUPPORTED
#elif defined(__unix__)
#define GX_IN_LINUX
//#define GX_USE_VULKAN
#define GX_USE_OPENGL_43
#define GX_USE_OPENGL_33
#define GX_USE_OPENGL_ES3
#define GX_USE_OPENGL_ES2
#define GX_IN_DESKTOP
//#define GX_USE_GLFW
#elif defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
#define NOMINMAX
#define GX_IN_WINDOWS
//#define GX_USE_VULKAN
//#define GX_USE_DIRECTX12
//#define GX_USE_DIRECTX11
#define GX_USE_OPENGL_43
#define GX_USE_OPENGL_33
#define GX_USE_OPENGL_ES3
#define GX_USE_OPENGL_ES2
#define GX_IN_DESKTOP
#elif defined(__APPLE__)
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE
#define GX_IN_IOS
//#define GX_USE_METAL
//#define GX_USE_VULKAN
#define GX_USE_OPENGL_ES3
#define GX_USE_OPENGL_ES2
#elif TARGET_OS_MAC
#define GX_IN_MAC
//#define GX_USE_METAL
//#define GX_USE_VULKAN
#define GX_USE_OPENGL_43
#define GX_USE_OPENGL_33
#define GX_IN_DESKTOP
//#define GX_USE_GLFW
#else
#error "Unknown Apple platform"
#endif // IOS MAC
#else
#error "Not implemetned yet."
#endif
// Application constant definitions
#define GX_ENGINE_NAME "Gearoenix"
#define GX_APP_NAME GX_ENGINE_NAME " Demo App"
#define GX_APP_DATA_NAME "data.gx3d"
// Application compilation mode
#define GX_DEBUG_MODE
// Application features
#define GX_LOG_ENABLED
#ifdef GX_LOG_ENABLED
#endif
/// You can comment this for windowed apps
#ifndef GX_IN_DESKTOP
#define GX_FULLSCREEN
#endif
#ifndef GX_FULLSCREEN
/// Window width in windowed mode
#define GX_DEFAULT_WINDOW_WIDTH 1000
/// Window height in windowed mode
#define GX_DEFAULT_WINDOW_HEIGHT 700
#endif
#define GX_STR_0(a) #a
#define GX_STR_1(a) GX_STR_0(a)
#define GX_STR_2(a) GX_STR_1(a)
#define GX_PROFILING_MODE
#define GX_MAX_DIRECTIONAL_LIGHTS 8
#define GX_MAX_DIRECTIONAL_LIGHTS_STR GX_STR_2(GX_MAX_DIRECTIONAL_LIGHTS)
#define GX_MAX_DIRECTIONAL_LIGHTS_SHADOW_CASTER 1
#define GX_MAX_DIRECTIONAL_LIGHTS_SHADOW_CASTER_STR GX_STR_2(GX_MAX_DIRECTIONAL_LIGHTS_SHADOW_CASTER)
#define GX_MAX_POINT_LIGHTS 8
#define GX_MAX_POINT_LIGHTS_STR GX_STR_2(GX_MAX_POINT_LIGHTS)
#define GX_MAX_CONE_LIGHTS 8
#define GX_MAX_CONE_LIGHTS_STR GX_STR_2(GX_MAX_CONE_LIGHTS)
#define GX_MAX_SHADOW_MAPS 6
#define GX_MAX_SHADOW_MAPS_STR GX_STR_2(GX_MAX_SHADOW_MAPS)
#define GX_MAX_SHADOW_CASCADES 1
#define GX_MAX_SHADOW_CASCADES_STR GX_STR_2(GX_MAX_SHADOW_CASCADES)
#define GX_MAX_DIRECTIONAL_LIGHTS_CASCADES (GX_MAX_SHADOW_CASCADES * GX_MAX_DIRECTIONAL_LIGHTS_SHADOW_CASTER)
#define GX_MAX_DIRECTIONAL_LIGHTS_CASCADES_STR GX_STR_2(GX_MAX_DIRECTIONAL_LIGHTS_CASCADES)
#define GX_DEFAULT_RUNTIME_REFLECTION_ENVIRONMENT_RESOLUTION 512
#define GX_DEFAULT_RUNTIME_REFLECTION_IRRADIANCE_RESOLUTION 32
#define GX_DEFAULT_RUNTIME_REFLECTION_RADIANCE_RESOLUTION 128
#define GX_DEFAULT_BRDFLUT_RESOLUTION 256
#if defined(GX_USE_OPENGL_ES2) || defined(GX_USE_OPENGL_ES3) || defined(GX_USE_OPENGL_33) || defined(GX_USE_OPENGL_43)
#define GX_USE_OPENGL
#endif
#if defined(GX_USE_OPENGL_ES3) || defined(GX_USE_OPENGL_33) || defined(GX_USE_OPENGL_43)
#define GX_USE_OPENGL_CLASS_3
#endif
#if defined(GX_USE_DIRECTX11) || defined(GX_USE_DIRECTX12)
#define GX_USE_DIRECTX
#endif
#if defined(GX_USE_OPENGL) && (defined(GX_USE_DIRECTX) || defined(GX_USE_VULKAN))
#define GX_USE_INSTEAD_OF_OPENGL
#endif
#if defined(GX_USE_OPENGL_ES2) && (defined(GX_USE_OPENGL_ES3) || defined(GX_USE_OPENGL_33) || defined(GX_USE_OPENGL_43) || defined(GX_USE_DIRECTX) || defined(GX_USE_VULKAN))
#define GX_USE_INSTEAD_OF_OPENGL_ES2
#endif
#if defined(GX_USE_OPENGL_ES3) && (defined(GX_USE_OPENGL_ES2) || defined(GX_USE_OPENGL_33) || defined(GX_USE_OPENGL_43) || defined(GX_USE_DIRECTX) || defined(GX_USE_VULKAN))
#define GX_USE_INSTEAD_OF_OPENGL_ES3
#endif
#if defined(GX_USE_OPENGL_33) && (defined(GX_USE_OPENGL_ES2) || defined(GX_USE_OPENGL_ES3) || defined(GX_USE_OPENGL_43) || defined(GX_USE_DIRECTX) || defined(GX_USE_VULKAN))
#define GX_USE_INSTEAD_OF_OPENGL_33
#endif
#if defined(GX_USE_OPENGL_43) && (defined(GX_USE_OPENGL_ES2) || defined(GX_USE_OPENGL_ES3) || defined(GX_USE_OPENGL_33) || defined(GX_USE_DIRECTX) || defined(GX_USE_VULKAN))
#define GX_USE_INSTEAD_OF_OPENGL_43
#endif
#if defined(GX_USE_OPENGL_CLASS_3) && (defined(GX_USE_OPENGL_ES2) || defined(GX_USE_DIRECTX) || defined(GX_USE_VULKAN))
#define GX_USE_INSTEAD_OF_OPENGL_CLASS_3
#endif
#ifdef GX_USE_OPENGL_ES2
#define GX_RUNTIME_USE_OPENGL_ES2_V(v) (v) == gearoenix::render::engine::Type::OpenGLES2 ||
#define GX_RUNTIME_USE_OPENGL_ES2_E(e) GX_RUNTIME_USE_OPENGL_ES2_V((e)->get_engine_type())
#define GX_RUNTIME_USE_OPENGL_ES2 GX_RUNTIME_USE_OPENGL_ES2_E(e)
#else
#define GX_RUNTIME_USE_OPENGL_ES2_V(v)
#define GX_RUNTIME_USE_OPENGL_ES2_E(e)
#define GX_RUNTIME_USE_OPENGL_ES2
#endif
#ifdef GX_USE_OPENGL_ES3
#define GX_RUNTIME_USE_OPENGL_ES3_V(v) (v) == gearoenix::render::engine::Type::OpenGLES3 ||
#define GX_RUNTIME_USE_OPENGL_ES3_E(e) GX_RUNTIME_USE_OPENGL_ES3_V((e)->get_engine_type())
#define GX_RUNTIME_USE_OPENGL_ES3 GX_RUNTIME_USE_OPENGL_ES3_E(e)
#else
#define GX_RUNTIME_USE_OPENGL_ES3_V(v)
#define GX_RUNTIME_USE_OPENGL_ES3_E(e)
#define GX_RUNTIME_USE_OPENGL_ES3
#endif
#ifdef GX_USE_OPENGL_33
#define GX_RUNTIME_USE_OPENGL_33_V(v) (v) == gearoenix::render::engine::Type::OpenGL33 ||
#define GX_RUNTIME_USE_OPENGL_33_E(e) GX_RUNTIME_USE_OPENGL_33_V((e)->get_engine_type_id())
#define GX_RUNTIME_USE_OPENGL_33 GX_RUNTIME_USE_OPENGL_33_E(e)
#else
#define GX_RUNTIME_USE_OPENGL_33_V(v)
#define GX_RUNTIME_USE_OPENGL_33_E(e)
#define GX_RUNTIME_USE_OPENGL_33
#endif
#ifdef GX_USE_OPENGL_43
#define GX_RUNTIME_USE_OPENGL_43_V(v) (v) == gearoenix::render::engine::Type::OpenGL43 ||
#define GX_RUNTIME_USE_OPENGL_43_E(e) GX_RUNTIME_USE_OPENGL_43_V((e)->get_engine_type_id())
#define GX_RUNTIME_USE_OPENGL_43 GX_RUNTIME_USE_OPENGL_43_E(e)
#else
#define GX_RUNTIME_USE_OPENGL_43_V(v)
#define GX_RUNTIME_USE_OPENGL_43_E(e)
#define GX_RUNTIME_USE_OPENGL_43
#endif
#ifdef GX_USE_OPENGL_CLASS_3
#define GX_RUNTIME_USE_OPENGL_CLASS_3_V(v) \
    GX_RUNTIME_USE_OPENGL_43_V(v)          \
    GX_RUNTIME_USE_OPENGL_33_V(v)          \
    GX_RUNTIME_USE_OPENGL_ES3_V(v)         \
    false
#define GX_RUNTIME_USE_OPENGL_CLASS_3_E(e) GX_RUNTIME_USE_OPENGL_CLASS_3_V((e)->get_engine_type())
#define GX_RUNTIME_USE_OPENGL_CLASS_3 GX_RUNTIME_USE_OPENGL_CLASS_3_E(e)
#else
#define GX_RUNTIME_USE_OPENGL_CLASS_3_V(v)
#define GX_RUNTIME_USE_OPENGL_CLASS_3_E(e)
#define GX_RUNTIME_USE_OPENGL_CLASS_3
#endif
#ifdef GX_USE_OPENGL
#define GX_RUNTIME_USE_OPENGL_V(v) \
    GX_RUNTIME_USE_OPENGL_43_V(v)  \
    GX_RUNTIME_USE_OPENGL_33_V(v)  \
    GX_RUNTIME_USE_OPENGL_ES3_V(v) \
    GX_RUNTIME_USE_OPENGL_ES2_V(v) \
    false
#define GX_RUNTIME_USE_OPENGL_E(e) GX_RUNTIME_USE_OPENGL_V((e)->get_engine_type())
#define GX_RUNTIME_USE_OPENGL GX_RUNTIME_USE_OPENGL_E(e)
#else
#define GX_RUNTIME_USE_OPENGL_E(e)
#endif
#endif
