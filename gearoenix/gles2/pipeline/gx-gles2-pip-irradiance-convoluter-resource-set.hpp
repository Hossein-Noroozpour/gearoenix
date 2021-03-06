#ifndef GEAROENIX_GLES2_PIPELINE_IRRADIANCE_CONVOLUTER_RESOURCE_SET_HPP
#define GEAROENIX_GLES2_PIPELINE_IRRADIANCE_CONVOLUTER_RESOURCE_SET_HPP
#include "../../core/gx-cr-build-configuration.hpp"
#ifdef GX_USE_OPENGL_ES2
#include "../../core/sync/gx-cr-sync-end-caller.hpp"
#include "../../gl/gx-gl-types.hpp"
#include "../../render/pipeline/gx-rnd-pip-irradiance-convoluter-resource-set.hpp"

namespace gearoenix::gles2::shader {
class IrradianceConvoluter;
}

namespace gearoenix::gles2::pipeline {
class IrradianceConvoluter;
class ResourceSet;
class IrradianceConvoluterResourceSet final : public render::pipeline::IrradianceConvoluterResourceSet {
    GX_GET_UCPTR_PRV(gles2::pipeline::ResourceSet, base)
public:
    IrradianceConvoluterResourceSet(const std::shared_ptr<shader::IrradianceConvoluter>& shd, std::shared_ptr<IrradianceConvoluter const> pip) noexcept;
    ~IrradianceConvoluterResourceSet() noexcept final;
    void bind_final(gl::uint& bound_shader_program) const noexcept;
};
}

#endif
#endif
