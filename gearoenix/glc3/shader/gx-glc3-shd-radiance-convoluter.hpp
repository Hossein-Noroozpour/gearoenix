#ifndef GEAROENIX_GLC3_SHADER_RADIANCE_CONVOLUTER_HPP
#define GEAROENIX_GLC3_SHADER_RADIANCE_CONVOLUTER_HPP
#include "../../core/gx-cr-build-configuration.hpp"
#ifdef GX_USE_OPENGL_CLASS_3
#include "../../core/gx-cr-static.hpp"
#include "../../core/sync/gx-cr-sync-end-caller.hpp"
#include "../gx-glc3.hpp"
#include "gx-glc3-shd-shader.hpp"
#include <string>

namespace gearoenix::glc3::engine {
class Engine;
}

namespace gearoenix::glc3::shader {
class RadianceConvoluter final : public Shader {
    GX_GLC3_UNIFORM_FLOAT(roughness, 1)
    GX_GLC3_UNIFORM_FLOAT(roughness_p_4, 1)
    GX_GLC3_UNIFORM_FLOAT(sa_texel, 1)
    GX_GLC3_UNIFORM_TEXTURE(environment)
public:
    RadianceConvoluter(engine::Engine* e, const core::sync::EndCaller<core::sync::EndCallerIgnore>& c) noexcept;
    ~RadianceConvoluter() noexcept final;
    void bind() const noexcept final;
};
}

#endif
#endif
