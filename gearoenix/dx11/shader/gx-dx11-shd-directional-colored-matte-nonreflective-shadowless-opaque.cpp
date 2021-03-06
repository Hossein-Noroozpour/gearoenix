//#include "gx-gles2-shd-directional-colored-matte-nonreflective-shadowless-opaque.hpp"
//#ifdef USE_DIRECTX11
//#include "../../system/gx-sys-log.hpp"
//#include "../buffer/gx-gles2-buf-uniform.hpp"
//#include "../gx-gles2-engine.hpp"
//#include "../pipeline/gx-gles2-pip-pipeline.hpp"
//
//gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::Resources::Resources(Engine* e, pipeline::Pipeline* pip, buffer::Uniform* u)
//    : render::material::DirectionalColoredMatteNonreflectiveShadowlessOpaque::Resources(e, pip, u)
//{
//}
//
//void gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::Resources::bind()
//{
//    render::material::DirectionalColoredMatteNonreflectiveShadowlessOpaque::Uniform* data = static_cast<render::material::DirectionalColoredMatteNonreflectiveShadowlessOpaque::Uniform*>(u->get_data());
//    DirectionalColoredMatteNonreflectiveShadowlessOpaque* shd = static_cast<DirectionalColoredMatteNonreflectiveShadowlessOpaque*>(pip->get_shader());
//    shd->use();
//    shd->set_ambl_color(data->ambl_color.data());
//    shd->set_m(data->m.data());
//    shd->set_mvp(data->mvp.data());
//    shd->set_sun(data->sun.data());
//    shd->set_sun_color(data->sun_color.data());
//}
//
//gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::DirectionalColoredMatteNonreflectiveShadowlessOpaque(Engine* eng, std::shared_ptr<core::sync::EndCaller> end)
//    : Shader(eng, end)
//{
//    eng->add_load_function([this, end] {
//        create_program();
//        std::string pvs = "precision highp sampler2D;\n"
//                          "precision highp float;\n"
//                          "attribute vec3 vertex;\n"
//                          "attribute vec3 normal;\n"
//                          "varying float out_diffuse;\n"
//                          "uniform mat4 mvp;\n"
//                          "uniform mat4 m;\n"
//                          "uniform vec3 sun;\n"
//                          "void main()\n"
//                          "{\n"
//                          "    vec3 world_normal = normalize((m * vec4(normal, 0.0)).xyz);\n"
//                          "    out_diffuse = dot(sun, world_normal);\n"
//                          "    gl_Position = mvp * vec4(vertex, 1.0);\n"
//                          "}\n";
//        std::string pfs = "precision highp sampler2D;\n"
//                          "precision highp float;\n"
//                          "varying float out_diffuse;\n"
//                          "uniform vec3 sun_color;\n"
//                          "uniform vec3 ambl_color;\n"
//                          "void main()\n"
//                          "{\n"
//                          "    float diff = smoothstep(0.2, 0.5, out_diffuse);\n"
//                          "    vec3 final_color = sun_color * diff;\n"
//                          "    final_color += ambl_color;\n"
//                          "    gl_FragColor = vec4(final_color, 1.0);\n"
//                          "}\n";
//        vtx_shd = add_shader_to_program(pvs, GL_VERTEX_SHADER);
//        frg_shd = add_shader_to_program(pfs, GL_FRAGMENT_SHADER);
//        run();
//        vtx_att_ind = glGetAttribLocation(shader_program, "vertex");
//        nrm_att_ind = glGetAttribLocation(shader_program, "normal");
//        m = get_uniform_location("m");
//        mvp = get_uniform_location("mvp");
//        sun = get_uniform_location("sun");
//        sun_color = get_uniform_location("sun_color");
//        ambl_color = get_uniform_location("ambl_color");
//        (void)end;
//    });
//}
//
//gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::~DirectionalColoredMatteNonreflectiveShadowlessOpaque()
//{
//    end_object(vtx_shd);
//    end_object(frg_shd);
//    end_program();
//}
//
//void gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::use()
//{
//    glUseProgram(shader_program);
//    glEnableVertexAttribArray(vtx_att_ind);
//    glEnableVertexAttribArray(nrm_att_ind);
//    //////////////////////////////////////////////////////
//    glVertexAttribPointer(vtx_att_ind, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
//    glVertexAttribPointer(nrm_att_ind, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
//}
//
//const std::vector<gearoenix::render::shader::stage::Id>& gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::get_stages_ids() const
//{
//    return graphic_2_stage;
//}
//
//void gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::set_mvp(const GLfloat* data)
//{
//    glUniformMatrix4fv(mvp, 1, GL_FALSE, data);
//}
//
//void gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::set_m(const GLfloat* data)
//{
//    glUniformMatrix4fv(m, 1, GL_FALSE, data);
//}
//
//void gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::set_sun(const GLfloat* data)
//{
//    glUniform3fv(sun, 1, data);
//}
//
//void gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::set_sun_color(const GLfloat* data)
//{
//    glUniform3fv(sun_color, 1, data);
//}
//
//void gearoenix::gles2::shader::DirectionalColoredMatteNonreflectiveShadowlessOpaque::set_ambl_color(const GLfloat* data)
//{
//    glUniform3fv(ambl_color, 1, data);
//}
//
//#endif
