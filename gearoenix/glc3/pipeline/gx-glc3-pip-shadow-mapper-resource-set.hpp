#ifndef GEAROENIX_GLC3_PIPELINE_SHADOW_MAPPER_RESOURCE_SET_HPP
#define GEAROENIX_GLC3_PIPELINE_SHADOW_MAPPER_RESOURCE_SET_HPP
#include "../../core/gx-cr-build-configuration.hpp"
#ifdef GX_USE_OPENGL_CLASS_3
#include "../../gl/gx-gl-types.hpp"
#include "../../render/pipeline/gx-rnd-pip-shadow-mapper-resource-set.hpp"

namespace gearoenix::glc3::shader {
class ShadowMapper;
}

namespace gearoenix::glc3::pipeline {
class ShadowMapper;
class ResourceSet;
class ShadowMapperResourceSet final : public render::pipeline::ShadowMapperResourceSet {
    GX_GET_UCPTR_PRV(glc3::pipeline::ResourceSet, base)
public:
    ShadowMapperResourceSet(const std::shared_ptr<shader::ShadowMapper>& shd, std::shared_ptr<ShadowMapper const> pip) noexcept;
    ~ShadowMapperResourceSet() noexcept final;
    void bind_final(gl::uint& bound_shader_program) const noexcept;
};
}

#endif
#endif
