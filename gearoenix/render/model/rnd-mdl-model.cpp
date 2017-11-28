#include "rnd-mdl-model.hpp"
#include "../../core/asset/cr-asset-manager.hpp"
#include "../../system/sys-app.hpp"
#include "../../system/sys-file.hpp"
#include "../camera/rnd-cmr-camera.hpp"
#include "../camera/rnd-cmr-orthographic.hpp"
#include "../material/rnd-mat-depth.hpp"
#include "../material/rnd-mat-material.hpp"
#include "../mesh/rnd-msh-mesh.hpp"
#include "../rnd-engine.hpp"
#include "../scene/rnd-scn-scene.hpp"

gearoenix::render::model::Model::Model(system::File* f, Engine* e, std::shared_ptr<core::EndCaller> c)
{
    m.read(f);
    occloc.read(f);
    f->read(occrds);
    core::Count mesh_count = 0;
    f->read(mesh_count);
    std::vector<core::Id> mesh_ids(mesh_count);
    std::map<core::Id, std::tuple<std::shared_ptr<material::Material>, std::shared_ptr<material::Depth>>> materials;
    for (core::Count i = 0; i < mesh_count; ++i) {
        std::shared_ptr<material::Material> mat(material::Material::read(f, e, c));
        f->read(mesh_ids[i]);
        if (shader::Shader::is_shadow_caster(mat->get_shader_id())) {
            std::shared_ptr<material::Depth> dp(new material::Depth(
                shader::Shader::get_shadow_caster_shader_id(mat->get_shader_id()),
                e, c));
            materials[mesh_ids[i]] = std::make_tuple(mat, dp);
        } else {
            materials[mesh_ids[i]] = std::make_tuple(mat, nullptr);
        }
    }
    core::Count children_count = 0;
    f->read(children_count);
    std::vector<core::Id> model_children(children_count);
    for (core::Count i = 0; i < children_count; ++i) {
        f->read(model_children[i]);
    }
    core::asset::Manager* astmgr = e->get_system_application()->get_asset_manager();
    for (core::Id mesh_id : mesh_ids) {
        const std::tuple<std::shared_ptr<material::Material>, std::shared_ptr<material::Depth>>& mat = materials[mesh_id];
        meshes[mesh_id] = std::make_tuple(astmgr->get_mesh(mesh_id, c), std::get<0>(mat), std::get<1>(mat));
    }
    for (core::Id model_id : model_children) {
        children[model_id] = astmgr->get_model(model_id, c);
    }
}

gearoenix::render::model::Model::~Model() {}

void gearoenix::render::model::Model::commit(const scene::Scene* s)
{
    const render::camera::Camera* camera = s->get_current_camera();
    const render::light::Sun* sun = scene->get_sun();
    const render::camera::Orthographic* suncam = sun->get_camera();
    bool moccloc_not_initialized = true;
    math::Vec3 moccloc;
    if (camera->get_changed() || changed) {
        moccloc = m * occloc;
        moccloc_not_initialized = false;
        is_in_camera = camera->in_sight(moccloc, occrds);
        if (is_in_camera) {
            if (needs_mvp) {
                mvp = camera->get_view_projection() * m;
            }
            if (has_transparent) {
                distcam = (camera->get_location() - moccloc).square_length();
            }
            for (std::pair<const core::Id, std::tuple<std::shared_ptr<mesh::Mesh>, std::shared_ptr<material::Material>, std::shared_ptr<material::Depth>>>& mshmtr : meshes) {
                std::get<1>(mshmtr.second)->update(s, this);
            }
        }
    }
    if (has_shadow_caster && (suncam->get_changed() || moved)) {
        if (moccloc_not_initialized) {
            moccloc = m * occloc;
        }
        is_in_sun = suncam->in_sight(moccloc, occrds);
        if (is_in_sun) {
            if (needs_dbm) {
                dbm = sun->get_bias() * m;
            }
            for (std::pair<const core::Id, std::tuple<std::shared_ptr<mesh::Mesh>, std::shared_ptr<material::Material>, std::shared_ptr<material::Depth>>>& mshmtr : meshes) {
                std::shared_ptr<material::Depth>& dp = std::get<2>(mshmtr.second);
                if (nullptr != dp) {
                    dp->update(s, this);
                }
            }
        }
    }
}

void gearoenix::render::model::Model::draw(core::Id mesh_id, texture::Texture2D* shadow_texture)
{
    std::tuple<std::shared_ptr<mesh::Mesh>, std::shared_ptr<material::Material>, std::shared_ptr<material::Depth>>& mshmtr = meshes[mesh_id];
    std::get<0>(mshmtr)->bind();
    std::get<1>(mshmtr)->bind(shadow_texture);
    std::get<0>(mshmtr)->draw();
}

void gearoenix::render::model::Model::cast_shadow(core::Id mesh_id)
{
    std::tuple<std::shared_ptr<mesh::Mesh>, std::shared_ptr<material::Material>, std::shared_ptr<material::Depth>>& mshmtr = meshes[mesh_id];
    std::get<0>(mshmtr)->bind();
    std::get<2>(mshmtr)->bind(nullptr);
    std::get<0>(mshmtr)->draw();
}

const std::map<gearoenix::core::Id, std::shared_ptr<gearoenix::render::model::Model>>& gearoenix::render::model::Model::get_children() const
{
    return children;
}

const std::map<gearoenix::core::Id,
    std::tuple<std::shared_ptr<gearoenix::render::mesh::Mesh>,
        std::shared_ptr<gearoenix::render::material::Material>,
        std::shared_ptr<gearoenix::render::material::Depth>>>&
gearoenix::render::model::Model::get_meshes() const
{
    return meshes;
}

const gearoenix::math::Mat4x4& gearoenix::render::model::Model::get_m() const
{
    return m;
}

const gearoenix::math::Mat4x4& gearoenix::render::model::Model::get_mvp() const
{
    return mvp;
}
