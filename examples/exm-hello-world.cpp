#include "exm-hello-world.hpp"
#include <gearoenix/core/asset/cr-asset-manager.hpp>
#include <gearoenix/core/event/cr-ev-bt-mouse.hpp>
#include <gearoenix/core/event/cr-ev-mv-mouse.hpp>
#include <gearoenix/core/event/cr-ev-sys-system.hpp>
#include <gearoenix/core/event/cr-ev-ui-ui.hpp>
#include <gearoenix/physics/body/phs-bd-rigid.hpp>
#include <gearoenix/physics/constraint/phs-cns-tracker-spring-joint-spring.hpp>
#include <gearoenix/render/camera/rnd-cmr-camera.hpp>
#include <gearoenix/render/camera/rnd-cmr-manager.hpp>
#include <gearoenix/render/engine/rnd-eng-engine.hpp>
#include <gearoenix/render/graph/tree/rnd-gr-tr-pbr.hpp>
#include <gearoenix/render/light/rnd-lt-light.hpp>
#include <gearoenix/render/model/rnd-mdl-model.hpp>
#include <gearoenix/render/scene/rnd-scn-manager.hpp>
#include <gearoenix/render/scene/rnd-scn-scene.hpp>
#include <gearoenix/system/sys-app.hpp>
#include <gearoenix/system/sys-log.hpp>

GameApp::GameApp(const std::shared_ptr<gearoenix::system::Application>& sys_app)
    : gearoenix::core::Application::Application(sys_app)
{
    rnd_eng->set_render_tree(
		std::shared_ptr<gearoenix::render::graph::tree::Tree>(
			new gearoenix::render::graph::tree::Pbr(
				rnd_eng, 
				gearoenix::core::sync::EndCaller<gearoenix::core::sync::EndCallerIgnore>([] {}))));
    sys_app->get_asset_manager()->get_scene_manager()->get_gx3d(
		1024, 
		gearoenix::core::sync::EndCaller<gearoenix::render::scene::Scene>(
			[this](std::shared_ptr<gearoenix::render::scene::Scene> s) {
				scn = s;
				s->get_light(1027)->enable_shadowing();
			}));
}

GameApp::~GameApp() {}

void GameApp::update()
{
}

void GameApp::terminate()
{
    gearoenix::core::Application::terminate();
    scn = nullptr;
    cam = nullptr;
    mdl = nullptr;
}

//void GameApp::on_event(const gearoenix::core::event::Event& e)
//{
//    switch (e.get_type()) {
//    case gearoenix::core::event::Event::From::SYSTEM: {
//        const gearoenix::core::event::system::System& se = e.to_system();
//        switch (se.get_action()) {
//        case gearoenix::core::event::system::System::Action::RELOAD:
//            rndeng->load_scene(0, [this]() -> void {
//                cam = rndeng->get_scene(0)->get_current_camera();
//                rndeng->load_scene(1, [this]() -> void {});
//            });
//            break;
//        default:
//            break;
//        }
//        break;
//    }
//    case gearoenix::core::event::Event::From::BUTTON: {
//        const gearoenix::core::event::button::Button& be = e.to_button();
//        switch (be.get_type()) {
//        case gearoenix::core::event::button::Button::MOUSE: {
//            switch (be.get_key()) {
//            case gearoenix::core::event::button::Button::LEFT:
//                switch (be.get_action()) {
//                case gearoenix::core::event::button::Button::PRESS:
//                    if (be.is_taken())
//                        break;
//                    left_mouse_down = true;
//                    break;
//                case gearoenix::core::event::button::Button::RELEASE:
//                    left_mouse_down = false;
//                    break;
//                default:
//                    break;
//                }
//                break;
//            default:
//                break;
//            }
//            break;
//        }
//        case gearoenix::core::event::button::Button::KEYBOARD:
//            switch (be.get_key()) {
//            case gearoenix::core::event::button::Button::LEFT:
//                mdl->local_z_rotate(0.1f);
//                break;
//            case gearoenix::core::event::button::Button::RIGHT:
//                mdl->local_z_rotate(-0.1f);
//                break;
//            case gearoenix::core::event::button::Button::UP:
//                mdl->local_y_translate(-0.1f);
//                break;
//            default:
//                break;
//            }
//            break;
//        default:
//            break;
//        }
//        break;
//    }
//    case gearoenix::core::event::Event::From::MOVEMENT: {
//        const gearoenix::core::event::movement::Movement& me = e.to_movement();
//        switch (me.get_type()) {
//        case gearoenix::core::event::movement::Movement::MOUSE: {
//            const gearoenix::core::event::movement::Mouse& mme = me.to_mouse();
//            if (left_mouse_down) {
//                cam->local_x_rotate(mme.get_dy() * 0.1f);
//                cam->global_rotate(mme.get_dx() * 0.1f, gearoenix::math::Vec3::Z);
//            }
//            break;
//        }
//        default:
//            break;
//        }
//        break;
//    }
//    case gearoenix::core::event::Event::From::UI: {
//        const gearoenix::core::event::ui::Ui& ue = e.to_ui();
//        if (ue.get_action() == gearoenix::core::event::ui::Ui::ActionType::CLICKED) {
//            if (ue.get_widget() == 19) {
//                rndeng->load_scene(0, [this]() -> void {
//                    const auto& scene = rndeng->get_scene(0);
//                    cam = scene->get_current_camera();
//                    scene->add_model(1);
//                    rndeng->delete_scene(1);
//                    rndeng->delete_scene(2);
//                    const std::shared_ptr<gearoenix::core::asset::Manager>& astmgr = rndeng->get_system_application()->get_asset_manager();
//                    scene->add_constraint(
//                        std::shared_ptr<gearoenix::physics::constraint::TrackerSpringJointSpring>(
//                            new gearoenix::physics::constraint::TrackerSpringJointSpring(
//                                astmgr->create_id(),
//                                std::shared_ptr<gearoenix::physics::body::Rigid>(
//                                    new gearoenix::physics::body::Rigid(astmgr->create_id(), mdl)),
//                                std::shared_ptr<gearoenix::physics::body::Rigid>(
//                                    new gearoenix::physics::body::Rigid(astmgr->create_id(), cam)),
//                                1.0f, gearoenix::math::Vec3(0.0f, -0.5f, 0.4f).normalized(), 1.0f, 2.0f)));
//                    state = State::GAME;
//                });
//                //rndeng->load_scene(1, [this]() -> void {});
//            }
//        }
//        break;
//    }
//    default:
//        break;
//    }
//}
//
GEAROENIX_START(GameApp)
