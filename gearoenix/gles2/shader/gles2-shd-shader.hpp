#ifndef GEAROENIX_GLES2_SHADER_SHADER_HPP
#define GEAROENIX_GLES2_SHADER_SHADER_HPP
#include "../../core/cr-build-configuration.hpp"
#ifdef GX_USE_OPENGL_ES2
#include "../../core/cr-static.hpp"
#include "../../core/cr-types.hpp"
#include "../../core/sync/cr-sync-end-caller.hpp"
#include "../../gl/gl-constants.hpp"
#include "../../gl/gl-loader.hpp"
#include "../../gl/gl-types.hpp"
#include "../gles2.hpp"
#include <string>

#define GX_GLES2_UNIFORM_FAILED -1

#ifdef GX_DEBUG_GLES2
#define GX_DEBUG_GLES2_GLSL
#endif

#define GX_GLES2_GET_UNIFORM(shd, uniform) uniform = shd->get_uniform_location(#uniform)

#define GX_GLES2_GET_UNIFORM_F(shd, uniform)                 \
    GX_GLES2_GET_UNIFORM(shd, uniform);                      \
    if (GX_GLES2_UNIFORM_FAILED == uniform) {                \
        GXLOGF("Failed to locate the uniform " << #uniform); \
    }

#define GX_GLES2_THIS_GET_UNIFORM(uniform) GX_GLES2_GET_UNIFORM(this, uniform)

#define GX_GLES2_THIS_GET_UNIFORM_F(uniform) GX_GLES2_GET_UNIFORM_F(this, uniform)

#define GX_GLES2_SHADER_SET_DATA_FUNCTION_V(x, t, n) \
    GX_GETTER_BUILDER(x)                             \
    void set_##x##_data(const float* data) const     \
    {                                                \
        gl::Loader::uniform##t##fv(x, n, data);      \
    }

#define GX_GLES2_SHADER_SET_DATA_FUNCTION_F(x)   \
    GX_GETTER_BUILDER(x)                         \
    void set_##x##_data(const float* data) const \
    {                                            \
        gl::Loader::uniform1f(x, *data);         \
    }

#define GX_GLES2_SHADER_SET_DATA_FUNCTION_M(x, t, n)             \
    GX_GETTER_BUILDER(x)                                         \
    void set_##x##_data(const float* data) const                 \
    {                                                            \
        gl::Loader::uniform_matrix##t##fv(x, n, GL_FALSE, data); \
    }

#define GX_GLES2_SHADER_SET_TEXTURE_INDEX_STARTING gl::sint texture_index = 0;

#define GX_GLES2_SHADER_SET_TEXTURE_INDEX(x) \
    x##_index = texture_index;               \
    ++texture_index;

#define GX_GLES2_SHADER_SET_TEXTURE_INDEX_UNIFORM(x) \
    if (x != GX_GLES2_UNIFORM_FAILED)                \
        gl::Loader::uniform1i(x, x##_index);

#define GX_GLES2_SHADER_SRC_DEFAULT_PERCISION \
    "precision highp float;\n"                \
    "precision highp sampler2D;\n"            \
    "precision highp samplerCube;\n"          \
    "#define GXPI 3.14159265359\n"

#define GX_GLES2_SHADER_SRC_DEFAULT_ATTRIBUTES \
    "attribute vec3 position;\n"               \
    "attribute vec3 normal;\n"                 \
    "attribute vec4 tangent;\n"                \
    "attribute vec2 uv;\n"

#define GX_GLES2_SHADER_SRC_DEFAULT_VERTEX_STARTING GX_GLES2_SHADER_SRC_DEFAULT_PERCISION GX_GLES2_SHADER_SRC_DEFAULT_ATTRIBUTES

#define GX_GLES2_SHADER_SRC_DEFAULT_FRAGMENT_STARTING GX_GLES2_SHADER_SRC_DEFAULT_PERCISION

#define GX_GLES2_SHADER_SRC_MATERIAL_UNIFORMS          \
    "uniform float     material_alpha;\n"              \
    "uniform float     material_alpha_cutoff;\n"       \
    "uniform float     material_metallic_factor;\n"    \
    "uniform float     material_normal_scale;\n"       \
    "uniform float     material_occlusion_strength;\n" \
    "uniform float     material_roughness_factor;\n"

#define GX_GLES2_SHADER_SRC_MATERIAL_TEXTURES          \
    "uniform sampler2D material_base_color;\n"         \
    "uniform sampler2D material_metallic_roughness;\n" \
    "uniform sampler2D material_normal;\n"             \
    "uniform sampler2D material_emissive;\n"

#define GX_GLES2_SHADER_SRC_MATERIAL_RESOURCES GX_GLES2_SHADER_SRC_MATERIAL_UNIFORMS GX_GLES2_SHADER_SRC_MATERIAL_TEXTURES

#define GX_GLES2_SHADER_MATERIAL_UNIFORMS_LOCATIONS                       \
    gl::sint material_alpha = GX_GLES2_UNIFORM_FAILED;                    \
    gl::sint material_alpha_cutoff = GX_GLES2_UNIFORM_FAILED;             \
    gl::sint material_base_color = GX_GLES2_UNIFORM_FAILED;               \
    gl::sint material_emissive = GX_GLES2_UNIFORM_FAILED;                 \
    gl::sint material_metallic_factor = GX_GLES2_UNIFORM_FAILED;          \
    gl::sint material_metallic_roughness = GX_GLES2_UNIFORM_FAILED;       \
    gl::sint material_normal = GX_GLES2_UNIFORM_FAILED;                   \
    gl::sint material_normal_scale = GX_GLES2_UNIFORM_FAILED;             \
    gl::sint material_occlusion_strength = GX_GLES2_UNIFORM_FAILED;       \
    gl::sint material_roughness_factor = GX_GLES2_UNIFORM_FAILED;         \
    gl::sint material_base_color_index = GX_GLES2_UNIFORM_FAILED;         \
    gl::sint material_metallic_roughness_index = GX_GLES2_UNIFORM_FAILED; \
    gl::sint material_normal_index = GX_GLES2_UNIFORM_FAILED;             \
    gl::sint material_emissive_index = GX_GLES2_UNIFORM_FAILED;

#define GX_GLES2_SHADER_MATERIAL_FUNCTION_DECLARATIONS               \
    GX_GETTER_BUILDER(material_base_color)                           \
    GX_GETTER_BUILDER(material_emissive)                             \
    GX_GETTER_BUILDER(material_metallic_roughness)                   \
    GX_GETTER_BUILDER(material_normal)                               \
    GX_GETTER_BUILDER(material_base_color_index)                     \
    GX_GETTER_BUILDER(material_metallic_roughness_index)             \
    GX_GETTER_BUILDER(material_normal_index)                         \
    GX_GETTER_BUILDER(material_emissive_index)                       \
    GX_GLES2_SHADER_SET_DATA_FUNCTION_F(material_alpha)              \
    GX_GLES2_SHADER_SET_DATA_FUNCTION_F(material_alpha_cutoff)       \
    GX_GLES2_SHADER_SET_DATA_FUNCTION_F(material_metallic_factor)    \
    GX_GLES2_SHADER_SET_DATA_FUNCTION_F(material_normal_scale)       \
    GX_GLES2_SHADER_SET_DATA_FUNCTION_F(material_occlusion_strength) \
    GX_GLES2_SHADER_SET_DATA_FUNCTION_F(material_roughness_factor)

#define GX_GLES2_SHADER_MATERIAL_GET_UNIFORM_LOCATIONS      \
    GX_GLES2_THIS_GET_UNIFORM(material_alpha);              \
    GX_GLES2_THIS_GET_UNIFORM(material_alpha_cutoff);       \
    GX_GLES2_THIS_GET_UNIFORM(material_base_color);         \
    GX_GLES2_THIS_GET_UNIFORM(material_emissive);           \
    GX_GLES2_THIS_GET_UNIFORM(material_metallic_factor);    \
    GX_GLES2_THIS_GET_UNIFORM(material_metallic_roughness); \
    GX_GLES2_THIS_GET_UNIFORM(material_normal);             \
    GX_GLES2_THIS_GET_UNIFORM(material_normal_scale);       \
    GX_GLES2_THIS_GET_UNIFORM(material_occlusion_strength); \
    GX_GLES2_THIS_GET_UNIFORM(material_roughness_factor);

#define GX_GLES2_SHADER_MATERIAL_SET_TEXTURE_INDEX                 \
    GX_GLES2_SHADER_SET_TEXTURE_INDEX(material_base_color)         \
    GX_GLES2_SHADER_SET_TEXTURE_INDEX(material_metallic_roughness) \
    GX_GLES2_SHADER_SET_TEXTURE_INDEX(material_normal)             \
    GX_GLES2_SHADER_SET_TEXTURE_INDEX(material_emissive)

#define GX_GLES2_SHADER_MATERIAL_SET_TEXTURE_INDEX_UNIFORM                 \
    GX_GLES2_SHADER_SET_TEXTURE_INDEX_UNIFORM(material_base_color)         \
    GX_GLES2_SHADER_SET_TEXTURE_INDEX_UNIFORM(material_metallic_roughness) \
    GX_GLES2_SHADER_SET_TEXTURE_INDEX_UNIFORM(material_normal)             \
    GX_GLES2_SHADER_SET_TEXTURE_INDEX_UNIFORM(material_emissive)

namespace gearoenix {
namespace gles2 {
    namespace engine {
        class Engine;
    }
    namespace shader {
        class Shader {
        protected:
            const std::shared_ptr<engine::Engine> e;
            gl::uint shader_program = 0;
            gl::uint vertex_object = 0;
            gl::uint fragment_object = 0;
            gl::sint position_attribute_location = -1;
            gl::sint normal_attribute_location = -1;
            gl::sint tangent_attribute_location = -1;
            gl::sint uv_attribute_location = -1;
            void create_program();
            void run();
            void link();
            void validate();
            gl::uint add_shader_to_program(const std::string& shd, const gl::enumerated shader_type);
            gl::uint set_vertex_shader(const std::string& shd);
            gl::uint set_fragment_shader(const std::string& shd);
            static void end_program(const gl::uint shader_program);
            static void end_object(const gl::uint shader_object);

        public:
            Shader(const std::shared_ptr<engine::Engine>& e, const core::sync::EndCaller<core::sync::EndCallerIgnore>& c);
            virtual ~Shader();
            /// On not found returns GX_SHADER_UNIFORM_FAILED
            gl::sint get_uniform_location(const std::string& name) const;
            gl::uint get_shader_program() const;
            virtual void bind() const;
        };
    }
}
}
#endif
#endif