#include "gx-tool-ibl-main.hpp"
#include <gearoenix/core/asset/gx-cr-asset-manager.hpp>
#include <gearoenix/core/event/gx-cr-ev-engine.hpp>
#include <gearoenix/core/gx-cr-string.hpp>
#include <gearoenix/math/gx-math-aabb.hpp>
#include <gearoenix/physics/body/gx-phs-bd-rigid.hpp>
#include <gearoenix/physics/collider/gx-phs-cld-sphere.hpp>
#include <gearoenix/render/camera/gx-rnd-cmr-manager.hpp>
#include <gearoenix/render/camera/gx-rnd-cmr-perspective.hpp>
#include <gearoenix/render/camera/gx-rnd-cmr-transformation.hpp>
#include <gearoenix/render/engine/gx-rnd-eng-engine.hpp>
#include <gearoenix/render/graph/tree/gx-rnd-gr-tr-pbr.hpp>
#include <gearoenix/render/light/gx-rnd-lt-directional.hpp>
#include <gearoenix/render/light/gx-rnd-lt-manager.hpp>
#include <gearoenix/render/material/gx-rnd-mat-pbr.hpp>
#include <gearoenix/render/material/gx-rnd-mat-skybox-equirectangular.hpp>
#include <gearoenix/render/mesh/gx-rnd-msh-manager.hpp>
#include <gearoenix/render/mesh/gx-rnd-msh-mesh.hpp>
#include <gearoenix/render/model/gx-rnd-mdl-manager.hpp>
#include <gearoenix/render/model/gx-rnd-mdl-mesh.hpp>
#include <gearoenix/render/model/gx-rnd-mdl-static.hpp>
#include <gearoenix/render/reflection/gx-rnd-rfl-manager.hpp>
#include <gearoenix/render/reflection/gx-rnd-rfl-runtime.hpp>
#include <gearoenix/render/scene/gx-rnd-scn-game.hpp>
#include <gearoenix/render/scene/gx-rnd-scn-manager.hpp>
#include <gearoenix/render/scene/gx-rnd-scn-ui.hpp>
#include <gearoenix/render/skybox/gx-rnd-sky-equirectangular.hpp>
#include <gearoenix/render/skybox/gx-rnd-sky-manager.hpp>
#include <gearoenix/render/texture/gx-rnd-txt-manager.hpp>
#include <gearoenix/render/texture/gx-rnd-txt-texture-2d.hpp>
#include <gearoenix/render/texture/gx-rnd-txt-texture-cube.hpp>
#include <gearoenix/render/widget/gx-rnd-wdg-button.hpp>
#include <gearoenix/render/widget/gx-rnd-wdg-edit.hpp>
#include <gearoenix/render/widget/gx-rnd-wdg-modal.hpp>
#include <gearoenix/render/widget/gx-rnd-wdg-text.hpp>
#include <gearoenix/system/gx-sys-application.hpp>
#include <gearoenix/system/gx-sys-args.hpp>

template <class T>
using GxEndCaller = gearoenix::core::sync::EndCaller<T>;

using GxEndCallerIgnore = gearoenix::core::sync::EndCallerIgnore;
using GxEndCallerIgnored = GxEndCaller<GxEndCallerIgnore>;
using GxMatPbr = gearoenix::render::material::Pbr;
using GxMdMesh = gearoenix::render::model::Mesh;
using GxMesh = gearoenix::render::mesh::Mesh;
using GxDirLight = gearoenix::render::light::Directional;
using GxPersCam = gearoenix::render::camera::Perspective;
using GxTexture = gearoenix::render::texture::Texture;
using GxTexture2D = gearoenix::render::texture::Texture2D;
using GxAabb3 = gearoenix::math::Aabb3;
using GxEventId = gearoenix::core::event::Id;
using GxMouseData = gearoenix::core::event::button::MouseData;
using GxMouseKeyId = gearoenix::core::event::button::MouseKeyId;
using GxMouseActionId = gearoenix::core::event::button::MouseActionId;
using GxTxtSamplerInfo = gearoenix::render::texture::SamplerInfo;
using GxTxtFilter = gearoenix::render::texture::Filter;

void IblBaker::on_open() noexcept
{
    const auto& f = file_location->get_text();
    if (f.empty())
        return;
    environment_file = gearoenix::core::String::to_string(f);
    open_environment();
}

void IblBaker::argument_handling() noexcept
{
    GXLOGD("Going to parsing arguments.")
    const auto* const args = system_application->get_arguments();
    args->get_value("environment-file", environment_file);
    args->get_value("baked-cube-file", baked_cube_file);
    args->get_value("baked-cube-resolution", baked_cube_resolution);
    args->get_value("irradiance-file", irradiance_file);
    args->get_value("irradiance-resolution", irradiance_resolution);
    args->get_value("radiance-file", radiance_file);
    args->get_value("radiance-resolution", radiance_resolution);
    auto& conf = system_application->get_configuration().get_render();
    conf.set_runtime_reflection_environment_resolution(static_cast<std::uint16_t>(baked_cube_resolution));
    conf.set_runtime_reflection_irradiance_resolution(static_cast<std::uint16_t>(irradiance_resolution));
    conf.set_runtime_reflection_radiance_resolution(static_cast<std::uint16_t>(radiance_resolution));
    GXLOGD("Arguments parsed.")
}

IblBaker::IblBaker(gearoenix::system::Application* const sys_app) noexcept
    : gearoenix::core::Application::Application(sys_app)
    , called_from_cli(system_application->get_arguments()->get_has_tokens())
{
    GXLOGD("IBL baker started.")
    if (called_from_cli)
        argument_handling();

    const GxEndCallerIgnored end_call([this] {
        uiscn->set_enabled(true);
        scn->set_enabled(true);
        obj_scn->set_enabled(true);
        auto* const event_engine = system_application->get_event_engine();
        event_engine->add_listener(GxEventId::ButtonMouse, 0.0f, this);
        event_engine->add_listener(GxEventId::MovementMouse, 0.0f, this);
    });

    GxEndCaller<GxUiScene> ui_scn_call([end_call](const std::shared_ptr<GxUiScene>&) {});
    GxEndCaller<GxGameScene> scn_call([end_call](const std::shared_ptr<GxGameScene>&) {});
    GxEndCaller<GxTextWdg> txw_call([end_call](const std::shared_ptr<GxTextWdg>&) {});
    GxEndCaller<GxEditWdg> edt_call([end_call](const std::shared_ptr<GxEditWdg>&) {});
    GxEndCaller<GxButton> btn_call([end_call](const std::shared_ptr<GxButton>&) {});
    GxEndCaller<GxSkyEqrect> sky_call([end_call](const std::shared_ptr<GxSkyEqrect>&) {});
    GxEndCaller<GxRtReflect> rtr_call([end_call](const std::shared_ptr<GxRtReflect>&) {});
    GxEndCaller<GxStaticModel> mdl_call([end_call](const std::shared_ptr<GxStaticModel>&) {});
    GxEndCaller<GxMesh> msh_call([end_call](const std::shared_ptr<GxMesh>&) {});

    render_tree = std::make_unique<GxGrPbr>(render_engine, end_call);
    render_engine->set_render_tree(render_tree.get());

    auto* const ast_mgr = sys_app->get_asset_manager();
    auto* const msh_mgr = ast_mgr->get_mesh_manager();
    auto* const mdl_mgr = ast_mgr->get_model_manager();
    auto* const sky_mgr = ast_mgr->get_skybox_manager();
    auto* const rfl_mgr = ast_mgr->get_reflection_manager();
    auto* const txt_mgr = ast_mgr->get_texture_manager();

    uiscn = ast_mgr->get_scene_manager()->create<GxUiScene>("ui", ui_scn_call);
    scn = ast_mgr->get_scene_manager()->create<GxGameScene>("scn", scn_call);

    cam = ast_mgr->get_camera_manager()->create<GxPersCam>("pers");
    cam_trn = dynamic_cast<GxCamTran*>(cam->get_transformation());
    cam_trn->look_at(GxVec3(25.0, 0.0, 0.0), GxVec3(0.0, 0.0, 0.0), GxVec3(0.0, 0.0, 1.0));
    scn->add_camera(cam);

    sky = sky_mgr->create<GxSkyEqrect>("sky", sky_call);
    scn->add_skybox(sky);

    rtr = rfl_mgr->create<GxRtReflect>("rtr", rtr_call);
    scn->add_reflection(rtr);

    auto tmp_txt = mdl_mgr->create<GxTextWdg>("title", txw_call);
    auto* tmp_tran = tmp_txt->get_transformation();
    tmp_tran->local_scale(0.04f);
    tmp_tran->set_location(GxVec3(0.0, 0.85, 0.1));
    tmp_txt->set_text(L"IBL baker for Gearoenix game engine", end_call);
    uiscn->add_model(tmp_txt);

    tmp_txt = mdl_mgr->create<GxTextWdg>("hdr loc", txw_call);
    tmp_tran = tmp_txt->get_transformation();
    tmp_tran->local_scale(0.03f);
    tmp_tran->set_location(GxVec3(-0.75, 0.75, 0.1));
    tmp_txt->set_text(L"HDR file location:", end_call);
    uiscn->add_model(tmp_txt);

    auto open_button = mdl_mgr->create<GxButton>("open button", btn_call);
    tmp_tran = open_button->get_transformation();
    tmp_tran->local_scale(0.04f);
    tmp_tran->local_x_scale(4.0f);
    tmp_tran->set_location(GxVec3(0.75, 0.75, 0.1));
    open_button->set_text(L"Open File", end_call);
    if (!called_from_cli)
        open_button->set_on_click([this] { on_open(); });
    uiscn->add_model(open_button);

    file_location = mdl_mgr->create<GxEditWdg>("file", edt_call);
    tmp_tran = file_location->get_transformation();
    tmp_tran->local_scale(0.04);
    tmp_tran->local_x_scale(14.0);
    tmp_tran->set_location(GxVec3(0.025, 0.75, 0.1));
    file_location->set_hint_text(L"<Fill it with 'file location'>", end_call);
    uiscn->add_model(file_location);

    obj_scn = ast_mgr->get_scene_manager()->create<GxGameScene>("obj scene", scn_call);
    obj_scn->set_layer((scn->get_layer() + uiscn->get_layer()) * 0.5);

    obj_cam = ast_mgr->get_camera_manager()->create<GxPersCam>("obj pers");
    obj_cam_trn = dynamic_cast<GxCamTran*>(obj_cam->get_transformation());
    obj_cam_trn->look_at(GxVec3(25.0, 0.0, 0.0), GxVec3(0.0, 0.0, 0.0), GxVec3(0.0, 0.0, 1.0));
    obj_scn->add_camera(obj_cam);

    //    {
    //        const auto cube_mesh = msh_mgr->create_cube(msh_call);
    //        const std::shared_ptr<GxMatPbr> mat(new GxMatPbr(render_engine, end_call));
    //        mat->set_roughness_factor(0.5f);
    //        mat->set_metallic_factor(0.5f);
    //        const auto mdl = mdl_mgr->create<GxStaticModel>(mdl_call);
    //        mdl->add_mesh(std::make_shared<GxMdMesh>(cube_mesh, mat));
    //        mdl->get_transformation()->local_scale(10.0);
    //        obj_scn->add_model(mdl);
    //        mdl->set_hooked_reflection(rtr);
    //    }

    //        {
    //            const auto sphere_mesh = msh_mgr->create_icosphere(msh_call);
    //            const std::shared_ptr<GxMatPbr> mat(new GxMatPbr(render_engine, end_call));
    //            mat->set_roughness_factor(0.999f);
    //            mat->set_metallic_factor(0.5f);
    //            const auto mdl = mdl_mgr->create<GxStaticModel>(mdl_call);
    //            mdl->add_mesh(std::make_shared<GxMdMesh>(sphere_mesh, mat));
    //            mdl->get_transformation()->local_scale(10.0);
    //            obj_scn->add_model(mdl);
    //            mdl->set_hooked_reflection(rtr);
    //        }

    const auto obj_msh = msh_mgr->create_icosphere(msh_call);

    for (float y = -10.0f, roughness = 0.1f; y < 10.1f; y += 2.5f, roughness += 0.1f) {
        for (float x = -10.0f, metallic = 0.1f; x < 10.1f; x += 2.5f, metallic += 0.1f) {
            const std::shared_ptr<GxMatPbr> mat(new GxMatPbr(render_engine, end_call));
            mat->set_roughness_factor(roughness);
            mat->set_metallic_factor(metallic);
            mat->set_color(1.0f, 0.0f, 0.0f, end_call);
            const auto mdl = mdl_mgr->create<GxStaticModel>(
                "model-" + std::to_string(metallic) + "-" + std::to_string(roughness),
                mdl_call);
            mdl->add_mesh(std::make_shared<GxMdMesh>(obj_msh, mat));
            mdl->get_transformation()->set_location(GxVec3(x, y, 0.0f));
            obj_scn->add_model(mdl);
            mdl->set_hooked_reflection(rtr);
        }
    }

    if (called_from_cli) {
        GxEndCaller<GxTexture2D> txt_call([end_call](const std::shared_ptr<GxTexture2D>&) {});
        GxTxtSamplerInfo smp;
        smp.min_filter = GxTxtFilter::Linear;
        sky->get_mat_equ()->set_color(txt_mgr->create_2d_f("env", environment_file, txt_call, smp, false));

        rtr->set_on_rendered([this]() {
            GXLOGD("Runtime reflection rendered.")

            GxEndCallerIgnored termination_call([this] {
                system_application->quit();
            });

            static_cast<gearoenix::render::texture::Texture*>(rtr->get_environment().get())->write_gx3d(baked_cube_file, termination_call);
            static_cast<gearoenix::render::texture::Texture*>(rtr->get_radiance().get())->write_gx3d(radiance_file, termination_call);
            static_cast<gearoenix::render::texture::Texture*>(rtr->get_irradiance().get())->write_gx3d(irradiance_file, termination_call);
        });
    }
    GXLOGD("Initialization ended.")
}

IblBaker::~IblBaker() noexcept
{
    terminate();
}

void IblBaker::update() noexcept
{
}

void IblBaker::terminate() noexcept
{
}

bool IblBaker::on_event(const gearoenix::core::event::Data& d) noexcept
{
    switch (d.get_source()) {
    case GxEventId::ButtonMouse: {
        const auto& data = std::get<GxMouseData>(d.get_data());
        if (data.get_key() == GxMouseKeyId::Left)
            camera_rotation_enabled = data.get_action() == GxMouseActionId::Press;
        break;
    }
    case GxEventId::MovementMouse: {
        if (camera_rotation_enabled) {
            const auto& data = std::get<gearoenix::core::event::movement::Base2D>(d.get_data());
            const auto rot_x = data.get_point().get_delta_previous_position().y;
            const auto rot_z = data.get_point().get_delta_previous_position().x;
            cam_trn->local_rotate(rot_z, GxVec3(0.0, 0.0, 1.0));
            cam_trn->local_x_rotate(rot_x);
            obj_cam_trn->global_rotate(rot_z, GxVec3(0.0, 0.0, 1.0), GxVec3(0.0, 0.0, 0.0));
            obj_cam_trn->global_rotate(rot_x, obj_cam_trn->get_x_axis(), GxVec3(0.0, 0.0, 0.0));
        }
        break;
    }
    default:
        GX_UNEXPECTED
    }
    return false;
}

void IblBaker::open_environment() noexcept
{
    auto* const ast_mgr = system_application->get_asset_manager();
    auto* const txt_mgr = ast_mgr->get_texture_manager();
    GxEndCaller<GxTexture2D> txt_call([this](const std::shared_ptr<GxTexture2D>& t) {
        sky->get_mat_equ()->set_color(t);
        rtr->set_pending_to_start(true);
        rtr->set_on_rendered([this]() {
            static_cast<gearoenix::render::texture::Texture*>(rtr->get_radiance().get())->write_gx3d(radiance_file, GX_DEFAULT_IGNORED_END_CALLER);
            static_cast<gearoenix::render::texture::Texture*>(rtr->get_irradiance().get())->write_gx3d(irradiance_file, GX_DEFAULT_IGNORED_END_CALLER);
        });
    });
    GxTxtSamplerInfo smp;
    smp.min_filter = GxTxtFilter::Linear;
    /// TODO failure management
    static int env_i = 0;
    ++env_i;
    (void)txt_mgr->create_2d_f("env-" + std::to_string(env_i), environment_file, txt_call, smp, false);
}

GEAROENIX_START(IblBaker)
