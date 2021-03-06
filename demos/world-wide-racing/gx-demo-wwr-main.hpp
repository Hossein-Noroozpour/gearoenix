#ifndef GX_DEMO_WWR_MAIN_HPP
#define GX_DEMO_WWR_MAIN_HPP

#include <gearoenix/core/gx-cr-application.hpp>
#include <vector>

namespace gearoenix::render::scene {
class Logo;
class Scene;
}

namespace gearoenix::render::graph::tree {
class Pbr;
}

namespace gearoenix::demo::wwr {
class Garage;
class Main final : public core::Application {
private:
    std::unique_ptr<render::graph::tree::Pbr> tree;
    std::shared_ptr<render::scene::Logo> logo_scene;
    std::unique_ptr<Garage> garage;

public:
    explicit Main(system::Application* sys_app) noexcept;
};
}

#endif