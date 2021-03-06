#include "gx-glc3-pip-shadow-mapper-resource-set.hpp"
#ifdef GX_USE_OPENGL_CLASS_3
#include "../../gl/gx-gl-loader.hpp"
#include "../../render/buffer/gx-rnd-buf-uniform.hpp"
#include "../../render/camera/gx-rnd-cmr-uniform.hpp"
#include "../../render/graph/node/gx-rnd-gr-nd-shadow-mapper.hpp"
#include "../../render/light/gx-rnd-lt-directional.hpp"
#include "../../render/mesh/gx-rnd-msh-mesh.hpp"
#include "../buffer/gx-glc3-buf-index.hpp"
#include "../buffer/gx-glc3-buf-vertex.hpp"
#include "../shader/gx-glc3-shd-shadow-mapper.hpp"
#include "../texture/gx-glc3-txt-2d.hpp"
#include "gx-glc3-pip-resource-set.hpp"
#include "gx-glc3-pip-shadow-mapper.hpp"

gearoenix::glc3::pipeline::ShadowMapperResourceSet::ShadowMapperResourceSet(const std::shared_ptr<shader::ShadowMapper>& shd, std::shared_ptr<ShadowMapper const> pip) noexcept
    : render::pipeline::ShadowMapperResourceSet(std::move(pip))
    , base(new glc3::pipeline::ResourceSet(shd))
{
}

gearoenix::glc3::pipeline::ShadowMapperResourceSet::~ShadowMapperResourceSet() noexcept = default;

void gearoenix::glc3::pipeline::ShadowMapperResourceSet::bind_final(gl::uint& bound_shader_program) const noexcept
{
    GX_GLC3_PIP_RES_START_DRAWING_MESH
    GX_GLC3_PIP_RES_START_SHADER(ShadowMapper, shd)
    GX_GLC3_PIP_RES_SET_TXT_2D(material_color, color)
    const auto* const node = node_uniform_buffer->get_ptr<render::graph::node::ShadowMapperUniform>();
    GX_GLC3_PIP_RES_SET_UNIFORM(effect_mvp, node->mvp.data[0][0])
    GX_GLC3_PIP_RES_SET_UNIFORM(effect_alpha, node->alpha)
    GX_GLC3_PIP_RES_SET_UNIFORM(effect_alpha_cutoff, node->alpha_cutoff)
    GX_GLC3_PIP_RES_END_DRAWING_MESH
}
#endif