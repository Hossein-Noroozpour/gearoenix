#include "rnd-mat-shadeless-d2-matte-nonreflective-shadowless-opaque.hpp"
#include "../../core/asset/cr-asset-manager.hpp"
#include "../../core/cr-end-caller.hpp"
#include "../../system/sys-app.hpp"
#include "../../system/sys-file.hpp"
#include "../buffer/rnd-buf-uniform.hpp"
#include "../camera/rnd-cmr-camera.hpp"
#include "../light/rnd-lt-sun.hpp"
#include "../model/rnd-mdl-model.hpp"
#include "../pipeline/rnd-pip-pipeline.hpp"
#include "../rnd-engine.hpp"
#include "../scene/rnd-scn-scene.hpp"
#include "../texture/rnd-txt-texture-2d.hpp"

const gearoenix::core::Id gearoenix::render::material::ShadelessD2MatteNonreflectiveShadowlessOpaque::SHADER_ID = gearoenix::render::shader::SHADELESS_D2_MATTE_NONREFLECTIVE_SHADOWLESS_OPAQUE;

gearoenix::render::material::ShadelessD2MatteNonreflectiveShadowlessOpaque::Resources::Resources(Engine* e, pipeline::Pipeline* pip, buffer::Uniform* u)
    : shader::Resources(e, pip, u)
{
}

void gearoenix::render::material::ShadelessD2MatteNonreflectiveShadowlessOpaque::Resources::set_texture_2d(texture::Texture2D* t2d)
{
    txt2d = t2d;
}

gearoenix::render::material::ShadelessD2MatteNonreflectiveShadowlessOpaque::ShadelessD2MatteNonreflectiveShadowlessOpaque(system::File* f, Engine* e, std::shared_ptr<core::EndCaller> end)
    : Material(SHADER_ID, sizeof(u), e, end)
{
    core::Id texid;
    f->read(texid);
    core::asset::Manager* astmgr = e->get_system_application()->get_asset_manager();
    std::function<void()> fun = [this, end, e] {
        shdrsc = reinterpret_cast<Resources*>(e->create_shader_resources(SHADER_ID, pl.get(), ub, end));
        shdrsc->set_texture_2d(txt2d.get());
    };
    unsigned int curloc = f->tell();
    txt2d = std::static_pointer_cast<texture::Texture2D>(astmgr->get_texture(texid, core::EndCaller::create(fun)));
    f->seek(curloc);
}

gearoenix::render::material::ShadelessD2MatteNonreflectiveShadowlessOpaque::~ShadelessD2MatteNonreflectiveShadowlessOpaque()
{
    delete shdrsc;
}

unsigned int gearoenix::render::material::ShadelessD2MatteNonreflectiveShadowlessOpaque::get_vertex_elements_count() const
{
    return shader::Shader::get_vertex_real_count(SHADER_ID);
}

gearoenix::core::Id gearoenix::render::material::ShadelessD2MatteNonreflectiveShadowlessOpaque::get_shader_id() const
{
    return SHADER_ID;
}

void gearoenix::render::material::ShadelessD2MatteNonreflectiveShadowlessOpaque::update(const scene::Scene*, const model::Model* m)
{
    u.mvp = m->get_mvp();
    ub->update(&u, sizeof(Uniform));
}

void gearoenix::render::material::ShadelessD2MatteNonreflectiveShadowlessOpaque::bind(texture::Texture2D*)
{
    shdrsc->bind();
}