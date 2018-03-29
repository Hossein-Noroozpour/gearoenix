#ifndef GEAROENIX_CORE_ASSET_MANAGER_HPP
#define GEAROENIX_CORE_ASSET_MANAGER_HPP
#include "../cr-types.hpp"
#include "../sync/cr-sync-end-caller.hpp"
#include <atomic>
#include <memory>
#include <string>
namespace gearoenix {
namespace audio {
    class Audio;
}
namespace physics {
    namespace constraint {
        class Constraint;
    }
}
namespace render {
    class Engine;
    namespace camera {
        class Camera;
    }
    namespace font {
        class Font;
    }
    namespace light {
        class Light;
    }
    namespace mesh {
        class Mesh;
    }
    namespace model {
        class Model;
    }
    namespace skybox {
        class Skybox;
    }
    namespace scene {
        class Scene;
    }
    namespace shader {
        class Shader;
    }
    namespace texture {
        class Texture;
    }
}
namespace system {
    class Application;
    namespace stream {
        class Asset;
    }
}
namespace core {
    namespace cache {
        class Cacher;
        namespace file {
            class File;
            class Sparse;
        }
    }
    namespace asset {
        class Manager {
        private:
            system::Application* sys_app = nullptr;
            render::Engine* render_engine = nullptr;
            system::stream::Asset* file = nullptr;
            cache::Cacher* shaders = nullptr;
            cache::file::File* cameras = nullptr;
            cache::file::File* audios = nullptr;
            cache::file::File* lights = nullptr;
            cache::file::File* textures = nullptr;
            cache::file::File* fonts = nullptr;
            cache::file::File* meshes = nullptr;
            cache::file::File* models = nullptr;
            cache::file::File* skyboxes = nullptr;
            cache::file::File* constraints = nullptr;
            cache::file::File* scenes = nullptr;
            std::atomic<Id> last_id;

        public:
            Manager(system::Application* sys_app, const std::string& file);
            ~Manager();
            void initialize();
            void set_render_engine(render::Engine* rndeng);
            system::stream::Asset* get_file();
            std::shared_ptr<render::shader::Shader> get_shader(Id id, sync::EndCaller<render::shader::Shader> end);
            std::shared_ptr<render::shader::Shader> get_cached_shader(Id id) const;
            std::shared_ptr<render::camera::Camera> get_camera(Id id);
            std::shared_ptr<render::camera::Camera> get_cached_camera(Id id) const;
            std::shared_ptr<audio::Audio> get_audio(Id id);
            std::shared_ptr<audio::Audio> get_cached_audio(Id id);
            std::shared_ptr<render::light::Light> get_light(Id id);
            std::shared_ptr<render::light::Light> get_cached_light(Id id) const;
            std::shared_ptr<render::texture::Texture> get_texture(Id id, sync::EndCaller<render::texture::Texture> end);
            std::shared_ptr<render::texture::Texture> get_cached_texture(Id id) const;
            std::shared_ptr<render::font::Font> get_font(Id id, sync::EndCaller<render::font::Font> end);
            std::shared_ptr<render::font::Font> get_cached_font(Id id) const;
            std::shared_ptr<render::mesh::Mesh> get_mesh(Id id, sync::EndCaller<render::mesh::Mesh> e);
            std::shared_ptr<render::mesh::Mesh> get_cached_mesh(Id id) const;
            std::shared_ptr<render::model::Model> get_model(Id id, sync::EndCaller<render::model::Model> e);
            std::shared_ptr<render::model::Model> get_cached_model(Id id) const;
            std::shared_ptr<render::skybox::Skybox> get_skybox(Id id, sync::EndCaller<render::skybox::Skybox> e);
            std::shared_ptr<render::skybox::Skybox> get_cached_skybox(Id id) const;
            std::shared_ptr<physics::constraint::Constraint> get_constriants(Id id, sync::EndCaller<physics::constraint::Constraint> e);
            std::shared_ptr<physics::constraint::Constraint> get_cached_constraints(Id id) const;
            std::shared_ptr<render::scene::Scene> get_scene(Id id, sync::EndCaller<render::scene::Scene> e);
            std::shared_ptr<render::scene::Scene> get_cached_scene(Id id) const;
            Id create_id();
        };
    } // namespace asset
} // namespace core
} // namespace gearoenix
#endif // GEAROENIX_CORE_ASSET_MANAGER_HPP
