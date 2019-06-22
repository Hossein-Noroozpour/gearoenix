#ifndef GEAROENIX_RENDER_LIGHT_DIRECTIONAL_HPP
#define GEAROENIX_RENDER_LIGHT_DIRECTIONAL_HPP
#include "../../core/cr-build-configuration.hpp"
#include "../../core/sync/cr-sync-end-caller.hpp"
#include "../../math/math-matrix.hpp"
#include "../../math/math-vector.hpp"
#include "rnd-lt-light.hpp"
namespace gearoenix::render {
namespace camera {
    class Camera;
}
namespace model {
    class Model;
}
namespace light {
    struct DirectionalUniform {
        math::Vec3 color = math::Vec3(1.0f, 1.0f, 1.0f);
        core::Real strength = 1.0f;
        math::Vec3 direction = math::Vec3(0.0f, 0.0f, -1.0f);
        core::Real reserved_1 = 0.0f;
    };

    class CascadeInfo;
    class Directional : public Light {
    protected:
        DirectionalUniform uniform;

    public:
        Directional(core::Id my_id, system::stream::Stream* f, engine::Engine* e) noexcept;
        Directional(core::Id my_id, engine::Engine* e) noexcept;
        const math::Vec3& get_direction() const noexcept;
        void set_direction(const math::Vec3& dir) noexcept;
        void update_uniform() noexcept final;
    };
}
}
#endif
