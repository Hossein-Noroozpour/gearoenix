#include "gles2-pip-resource-set.hpp"
#include "../../render/mesh/rnd-msh-mesh.hpp"
#include "../buffer/gles2-buf-index.hpp"
#include "../buffer/gles2-buf-vertex.hpp"
#include "../shader/gles2-shd-shader.hpp"

gearoenix::gles2::pipeline::ResourceSet::ResourceSet(const std::shared_ptr<shader::Shader>& shd)
    : shd(shd)
{
}

gearoenix::gles2::pipeline::ResourceSet::~ResourceSet() {}

void gearoenix::gles2::pipeline::ResourceSet::bind(gl::uint& bound_shader_program) const
{
    //if (shd->get_shader_program() != bound_shader_program) {
    bound_shader_program = shd->get_shader_program();
    shd->bind();
    //}
}
