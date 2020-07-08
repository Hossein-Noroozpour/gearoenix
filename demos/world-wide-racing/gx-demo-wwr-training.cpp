#include "gx-demo-wwr-training.hpp"
#include "gx-demo-wwr-garage.hpp"
#include "gx-demo-wwr-rotating-button.hpp"
#include "gx-demo-wwr-terrain-manager.hpp"
#include <gearoenix/physics/constraint/phs-cns-manager.hpp>
#include <gearoenix/physics/constraint/phs-cns-window-placer.hpp>
#include <gearoenix/render/scene/rnd-scn-game.hpp>
#include <gearoenix/render/scene/rnd-scn-ui.hpp>
#include <gearoenix/render/camera/rnd-cmr-camera.hpp>
#include <gearoenix/render/camera/rnd-cmr-arc-controller.hpp>
#include <gearoenix/render/camera/rnd-cmr-transformation.hpp>
#include <gearoenix/render/model/rnd-mdl-manager.hpp>
#include <gearoenix/render/engine/rnd-eng-engine.hpp>
#include <gearoenix/physics/collider/phs-cld-collider.hpp>
#include <gearoenix/system/sys-app.hpp>

void gearoenix::demo::wwr::Training::initialize_scenes(
        const std::vector<std::shared_ptr<render::scene::Scene>> &scenes) noexcept {
#ifdef GX_DEBUG_MODE
    if(scenes.size() != 2) GX_UNEXPECTED
#endif
    for(const auto& s: scenes) {
        switch (s->get_scene_type()) {
            case render::scene::Type::Game:
                game_scene = std::dynamic_pointer_cast<render::scene::Game>(s);
                break;
            case render::scene::Type::Ui:
                ui_scene = std::dynamic_pointer_cast<render::scene::Ui>(s);
                break;
            default:
            GX_UNEXPECTED
        }
    }
}

void gearoenix::demo::wwr::Training::initialize_camera() noexcept {
     cam_ctrl = std::make_unique<render::camera::ArcController>(game_scene->get_cameras().begin()->second);
     cam_ctrl->set_max_vertical_angle(0.8);
     cam_ctrl->set_min_vertical_angle(0.2);
     cam_ctrl->set_vertical_angle(0.4);
     cam_ctrl->set_max_distance(6.0);
     cam_ctrl->set_min_distance(4.0);
     cam_ctrl->set_distance(5.0);
     cam_ctrl->set_rotate_z(3.14);
     cam_ctrl->set_target(math::Vec3(0.0, 0.0, 0.0));
}

void gearoenix::demo::wwr::Training::initialize_buttons() noexcept {
     auto *const cns_mgr = ui_scene->get_e()->get_system_application()->get_asset_manager()->get_constraint_manager();

    const auto& accelerator_button = ui_scene->get_model("button-accelerator-pedal");
    const auto& break_button = ui_scene->get_model("button-break-pedal");

    accelerator_button_placer = cns_mgr->create<physics::constraint::WindowPlacer>("accelerator-button");
    accelerator_button_placer->set_width_percentage(0.95);
    accelerator_button_placer->add_affected(accelerator_button);
    accelerator_button_placer->set_height_percentage(0.15);

    break_button_placer = cns_mgr->create<physics::constraint::WindowPlacer>("break-button");
    break_button_placer->set_width_percentage(0.1);
    break_button_placer->add_affected(break_button);
    break_button_placer->set_height_percentage(0.15);
}

void gearoenix::demo::wwr::Training::initialize_cars() noexcept {
    core::sync::EndCaller<render::model::Model> mdl_end([](const std::shared_ptr<render::model::Model>&){});
    car = game_scene->get_e()->get_system_application()->get_asset_manager()->get_model_manager()->get_gx3d("audi-a8", mdl_end);
    game_scene->add_model(car);
    game_scene->get_cameras().begin()->second->get_transformation()->look_at(
        car->get_transformation()->get_location(),
        math::Vec3(0.0, 0.0, 1.0));
}

void gearoenix::demo::wwr::Training::initialize_terrain() noexcept {
    terrain_manager = std::make_unique<TerrainManager>(game_scene);
}

gearoenix::demo::wwr::Training::Training(
        const std::vector <std::shared_ptr<render::scene::Scene>> &scenes,
        Garage * const garage) noexcept {
    initialize_scenes(scenes);
    initialize_camera();
    initialize_buttons();
    initialize_cars();
    initialize_terrain();
    garage->clear();
}

gearoenix::demo::wwr::Training::~Training() noexcept = default;
