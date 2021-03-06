#ifndef GEAROENIX_GLES2_SHADER_SHADOW_MAPPER_HPP
#define GEAROENIX_GLES2_SHADER_SHADOW_MAPPER_HPP
#include "gx-gles2-shd-shader.hpp"
#ifdef GX_USE_OPENGL_ES2

namespace gearoenix::gles2::engine {
class Engine;
}

namespace gearoenix::gles2::shader {
class ShadowMapper final : public Shader {
    GX_GLES2_UNIFORM_TEXTURE(material_color)
    GX_GLES2_UNIFORM_FLOAT(effect_alpha, 1)
    GX_GLES2_UNIFORM_FLOAT(effect_alpha_cutoff, 1)
    GX_GLES2_UNIFORM_MATRIX(effect_mvp, 4, 1)
public:
    ShadowMapper(engine::Engine* e, const core::sync::EndCaller<core::sync::EndCallerIgnore>& c) noexcept;
    ~ShadowMapper() noexcept final;
    void bind() const noexcept final;
};
}

#endif
#endif
