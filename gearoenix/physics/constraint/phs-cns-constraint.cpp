#include "phs-cns-constraint.hpp"
#include "../../render/model/rnd-mdl-model.hpp"

gearoenix::physics::constraint::Constraint::Constraint(const core::Id id, const Type t, const bool a) noexcept
    : core::asset::Asset(id, core::asset::Type::Constraint)
    , constraint_type(t)
    , active(a)
{
}

gearoenix::physics::constraint::Constraint::~Constraint() = default;

void gearoenix::physics::constraint::Constraint::add_affected(const std::shared_ptr<render::model::Model>& m) noexcept
{
#ifdef GX_DEBUG_MODE
    if (m->get_model_type() == render::model::Type::Static || affected_models.find(m->get_asset_id()) != affected_models.end())
        GX_UNEXPECTED
#endif
    affected_models.emplace(m->get_asset_id(), m);
}