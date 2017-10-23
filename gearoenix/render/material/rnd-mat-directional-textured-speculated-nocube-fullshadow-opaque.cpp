#include "rnd-mat-directional-textured-speculated-nocube-fullshadow-opaque.hpp"
#include "../../core/asset/cr-asset-manager.hpp"
#include "../../core/cr-end-caller.hpp"
#include "../../system/sys-app.hpp"
#include "../../system/sys-file.hpp"
#include "../buffer/rnd-buf-uniform.hpp"
#include "../model/rnd-mdl-model.hpp"
#include "../pipeline/rnd-pip-pipeline.hpp"
#include "../rnd-engine.hpp"
#include "../texture/rnd-txt-texture-2d.hpp"

void gearoenix::render::material::DirectionalTexturedSpeculatedNocubeFullshadowOpaque::Resources::set_texture(texture::Texture2D* t)
{
    txt = t;
}

gearoenix::render::material::DirectionalTexturedSpeculatedNocubeFullshadowOpaque::DirectionalTexturedSpeculatedNocubeFullshadowOpaque(system::File* f, Engine* e, std::shared_ptr<core::EndCaller> end)
    : Material(shader::DIRECTIONAL_TEXTURED_SPECULATED_NOCUBE_FULLSHADOW_OPAQUE, sizeof(u), e, end)
{
    core::Id texid;
    f->read(texid);
    //    LOGE("location: " << f->tell());
    u.spec_color.read(f);
    f->read(u.spec_factor);
    core::asset::Manager* astmgr = e->get_system_application()->get_asset_manager();
    std::function<void()> fun = [this, end, e] {
        shdrsc = reinterpret_cast<Resources*>(e->create_shader_resources(
            shader::DIRECTIONAL_TEXTURED_SPECULATED_NOCUBE_FULLSHADOW_OPAQUE, pl.get(), end));
        shdrsc->set_texture(t.get());
    };
    unsigned int curloc = f->tell();
    t = std::static_pointer_cast<texture::Texture2D>(astmgr->get_texture(texid, core::EndCaller::create(fun)));
    f->seek(curloc);
}

gearoenix::render::material::DirectionalTexturedSpeculatedNocubeFullshadowOpaque::~DirectionalTexturedSpeculatedNocubeFullshadowOpaque()
{
    delete shdrsc;
}

unsigned int gearoenix::render::material::DirectionalTexturedSpeculatedNocubeFullshadowOpaque::get_vertex_elements_count() const
{
    return shader::Shader::get_vertex_real_count(shader::DIRECTIONAL_TEXTURED_SPECULATED_NOCUBE_FULLSHADOW_OPAQUE);
}

void gearoenix::render::material::DirectionalTexturedSpeculatedNocubeFullshadowOpaque::update(const scene::Scene*, const model::Model* m)
{
    u.mvp = m->get_mvp();
    u.m = m->get_m();
    ub->update(&u, sizeof(Uniform));
}

void gearoenix::render::material::DirectionalTexturedSpeculatedNocubeFullshadowOpaque::bind()
{
    shdrsc->bind();
}
