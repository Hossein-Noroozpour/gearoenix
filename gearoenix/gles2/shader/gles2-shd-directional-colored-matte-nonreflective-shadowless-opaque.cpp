#include "gles2-shd-directional-colored-matte-nonreflective-shadowless-opaque.hpp"
#ifdef USE_OPENGL_ES2
#include "../../system/sys-log.hpp"
#include "../buffer/gles2-buf-uniform.hpp"
#include "../gles2-engine.hpp"
#include "../pipeline/gles2-pip-pipeline.hpp"

gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::Resources::Resources(Engine* e, pipeline::Pipeline* pip, buffer::Uniform* u)
    : render::material::DirectionalColoredMatteNonreflectiveShadowlessOpaque::Resources(e, pip, u)
{
}

void gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::Resources::bind()
{
    render::material::DirectionalColoredMatteNonreflectiveShadowlessOpaque::Uniform* data = reinterpret_cast<render::material::DirectionalColoredMatteNonreflectiveShadowlessOpaque::Uniform*>(u->get_data());
    DirectionalColoredMatteNonreflectiveShadowlessOpaque* shd = reinterpret_cast<DirectionalColoredMatteNonreflectiveShadowlessOpaque*>(pip->get_shader());
    shd->use();
    shd->set_ambl_color(data->ambl_color.data());
    shd->set_color(data->color.data());
    shd->set_m(data->m.data());
    shd->set_mvp(data->mvp.data());
    shd->set_sun(data->sun.data());
    shd->set_sun_color(data->sun_color.data());
}

gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::DirectionalColoredMatteNonreflectiveShadowlessOpaque(Engine* eng, std::shared_ptr<core::EndCaller> end)
    : Shader(eng, end)
{
  eng->add_load_function([this, end] {
      create_program();
    std::string pvs = "precision highp sampler2D;\n"
                      "precision highp float;\n"
                      "attribute vec3 vertex;\n"
                      "attribute vec3 normal;\n"
                      "varying vec3 pos;\n"
                      "varying vec3 nrm;\n"
                      "uniform mat4 mvp;\n"
                      "uniform mat4 m;\n"
                      "void main()\n"
                      "{\n"
                      "    pos = (m * vec4(vertex, 1.0)).xyz;\n"
                      "    nrm = normalize((m * vec4(normal, 0.0)).xyz);\n"
                      "    gl_Position = mvp * vec4(vertex, 1.0);\n"
                      "}\n";
    std::string pfs = "precision highp sampler2D;\n"
                      "precision highp float;\n"
                      "varying vec3 pos;\n"
                      "varying vec3 nrm;\n"
                      "uniform vec3 color;\n"
                      "uniform vec3 sun;\n"
                      "uniform vec3 sun_color;\n"
                      "uniform vec3 ambl_color;\n"
                      "void main()\n"
                      "{\n"
                      "    float diff = -dot(nrm, sun);\n"
                      "    diff = smoothstep(0.0, 0.3, diff) * 0.5;\n"
                      "    vec3 final_color = sun_color * color * diff;\n"
                      "    final_color += ambl_color * color;\n"
                      "    gl_FragColor = vec4(final_color, 1.0);\n"
                      "}\n";
    vtx_shd = add_shader_to_program(pvs, GL_VERTEX_SHADER);
    frg_shd = add_shader_to_program(pfs, GL_FRAGMENT_SHADER);
    run();
    vtx_att_ind = glGetAttribLocation(shader_program, "vertex");
    nrm_att_ind = glGetAttribLocation(shader_program, "normal");
    mvp = get_uniform_location("mvp");
    m = get_uniform_location("m");
    sun = get_uniform_location("sun");
    sun_color = get_uniform_location("sun_color");
    color = get_uniform_location("color");
    ambl_color = get_uniform_location("ambl_color");
    (void)end;
});
}

gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::~DirectionalColoredMatteNonreflectiveShadowlessOpaque()
{
    end_object(vtx_shd);
    end_object(frg_shd);
    end_program();
}

void gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::use()
{
    glUseProgram(shader_program);
    glEnableVertexAttribArray(vtx_att_ind);
    glEnableVertexAttribArray(nrm_att_ind);
    //////////////////////////////////////////////////////
    glVertexAttribPointer(vtx_att_ind, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glVertexAttribPointer(nrm_att_ind, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
}

const std::vector<gearoenix::render::shader::stage::Id>& gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::get_stages_ids() const
{
    return graphic_2_stage;
}

void gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::set_mvp(const GLfloat* data)
{
    glUniformMatrix4fv(mvp, 1, GL_FALSE, data);
}

void gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::set_m(const GLfloat* data)
{
    glUniformMatrix4fv(m, 1, GL_FALSE, data);
}

void gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::set_sun(const GLfloat* data)
{
    glUniform3fv(sun, 1, data);
}

void gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::set_sun_color(const GLfloat* data)
{
    glUniform3fv(sun_color, 1, data);
}

void gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::set_color(const GLfloat* data)
{
    glUniform3fv(color, 1, data);
}

void gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::set_ambl_color(const GLfloat* data)
{
    glUniform3fv(ambl_color, 1, data);
}

#endif