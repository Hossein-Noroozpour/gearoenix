#include "rnd-pip-pipeline.hpp"
#include "../../core/asset/cr-asset-manager.hpp"
#include "../../core/cr-end-caller.hpp"
#include "../../system/sys-app.hpp"
#include "../../system/sys-log.hpp"
#include "../rnd-engine.hpp"

gearoenix::render::pipeline::Pipeline::Pipeline(core::Id sid, Engine* eng, std::shared_ptr<core::EndCaller> call)
    : eng(eng)
    , shd(eng->get_system_application()->get_asset_manager()->get_shader(sid, call))
{
}

gearoenix::render::pipeline::Pipeline::~Pipeline()
{
}

gearoenix::render::shader::Shader* gearoenix::render::pipeline::Pipeline::get_shader()
{
    return shd.get();
}