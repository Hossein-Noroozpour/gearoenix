#include "cr-asset-manager.hpp"
#include "../../audio/au-audio.hpp"
#include "../../render/camera/rnd-cmr-camera.hpp"
#include "../../render/light/rnd-lt-light.hpp"
#include "../../render/model/rnd-mdl-model.hpp"
#include "../../render/rnd-engine.hpp"
#include "../../render/scene/rnd-scn-scene.hpp"
#include "../../render/shader/rnd-shd-shader.hpp"
#include "../../render/texture/rnd-txt-texture.hpp"
#include "../../system/sys-app.hpp"
#include "../cache/file/cr-cache-file-sparse.hpp"
#include "../cache/file/cr-cache-file.hpp"
gearoenix::core::asset::Manager::Manager(system::Application* sys_app, const std::string& name)
    : sys_app(sys_app)
    , render_engine(sys_app->get_render_engine())
    , file(new system::File(sys_app, name))
    , shaders(new cache::file::Sparse(file))
    , cameras(new cache::file::File(file))
    , audios(new cache::file::File(file))
    , lights(new cache::file::File(file))
    , textures(new cache::file::File(file))
    , models(new cache::file::File(file))
    , scenes(new cache::file::File(file))
{
}

void gearoenix::core::asset::Manager::initialize()
{
    shaders->read_offsets();
    cameras->read_offsets();
    audios->read_offsets();
    lights->read_offsets();
    textures->read_offsets();
    models->read_offsets();
    scenes->read_offsets();
}

std::shared_ptr<gearoenix::render::shader::Shader> gearoenix::core::asset::Manager::get_shader(Id id)
{
    std::function<std::shared_ptr<render::shader::Shader>()> fn_new = [this] {
        return render::shader::Shader::read(file, render_engine);
    };
    return shaders->get<render::shader::Shader>(id, fn_new);
}

std::shared_ptr<gearoenix::render::shader::Shader> gearoenix::core::asset::Manager::get_cached_shader(Id id) const
{
    return shaders->get<gearoenix::render::shader::Shader>(id);
}

std::shared_ptr<gearoenix::render::camera::Camera> gearoenix::core::asset::Manager::get_camera(Id id)
{
    std::function<std::shared_ptr<render::camera::Camera>()> fn_new = [this] {
        return std::shared_ptr<render::camera::Camera>(render::camera::Camera::read(file, sys_app));
    };
    return cameras->get(id, fn_new);
}

std::shared_ptr<gearoenix::render::camera::Camera> gearoenix::core::asset::Manager::get_cached_camera(Id id) const
{
    return cameras->get<gearoenix::render::camera::Camera>(id);
}

std::shared_ptr<gearoenix::audio::Audio> gearoenix::core::asset::Manager::get_audio(Id id)
{
    std::function<std::shared_ptr<audio::Audio>()> fn_new = [this] {
        return std::shared_ptr<gearoenix::audio::Audio>(gearoenix::audio::Audio::read(file));
    };
    return audios->get(id, fn_new);
}

std::shared_ptr<gearoenix::audio::Audio> gearoenix::core::asset::Manager::get_cached_audio(Id id)
{
    return audios->get<audio::Audio>(id);
}

std::shared_ptr<gearoenix::render::light::Light> gearoenix::core::asset::Manager::get_light(Id id)
{
    std::function<std::shared_ptr<render::light::Light>()> fn_new = [this] {
        return std::shared_ptr<render::light::Light>(render::light::Light::read(file));
    };
    return lights->get<render::light::Light>(id, fn_new);
}

std::shared_ptr<gearoenix::render::light::Light> gearoenix::core::asset::Manager::get_cached_light(Id id) const
{
    return lights->get<render::light::Light>(id);
}

std::shared_ptr<gearoenix::render::texture::Texture> gearoenix::core::asset::Manager::get_texture(Id id)
{
    std::function<std::shared_ptr<render::texture::Texture>()> fn_new = [this] {
        LOGF("Not implemented.");
        return nullptr;
    };
    return textures->get<render::texture::Texture>(id, fn_new);
}

std::shared_ptr<gearoenix::render::texture::Texture> gearoenix::core::asset::Manager::get_cached_texture(Id id) const
{
    LOGF("Not implemented.");
    return textures->get<render::texture::Texture>(id);
}

std::shared_ptr<gearoenix::render::model::Model> gearoenix::core::asset::Manager::get_model(Id id)
{
    std::function<std::shared_ptr<render::model::Model>()> fn_new = [this] {
        LOGF("Not implemented.");
        return nullptr;
    };
    return models->get<render::model::Model>(id, fn_new);
}

std::shared_ptr<gearoenix::render::model::Model> gearoenix::core::asset::Manager::get_cached_model(Id id) const
{
    LOGF("Not implemented.");
    return models->get<render::model::Model>(id);
}

std::shared_ptr<gearoenix::render::scene::Scene> gearoenix::core::asset::Manager::get_scene(Id id)
{
    std::function<std::shared_ptr<render::scene::Scene>()> fn_new = [this] {
        LOGF("Not implemented.");
        return nullptr;
    };
    return scenes->get<render::scene::Scene>(id, fn_new);
}

std::shared_ptr<gearoenix::render::scene::Scene> gearoenix::core::asset::Manager::get_cached_scene(Id id) const
{
    LOGF("Not implemented.");
    return scenes->get<render::scene::Scene>(id);
}
