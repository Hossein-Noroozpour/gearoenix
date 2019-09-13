#ifndef GEAROENIX_RENDER_SCENE_UI_HPP
#define GEAROENIX_RENDER_SCENE_UI_HPP
#include "../../core/event/cr-ev-listner.hpp"
#include "rnd-scn-scene.hpp"
#include <chrono>

namespace gearoenix::render::scene {
class Ui : public Scene, public core::event::Listner {
private:
    void init() noexcept;

public:
    Ui(
        const core::Id my_id, system::stream::Stream* f, engine::Engine* e,
        const core::sync::EndCaller<core::sync::EndCallerIgnore>& c) noexcept;
    Ui(
        const core::Id my_id, engine::Engine* e,
        const core::sync::EndCaller<core::sync::EndCallerIgnore>& c) noexcept;
    bool on_event(const core::event::Data& d) noexcept final;
};
}
#endif
