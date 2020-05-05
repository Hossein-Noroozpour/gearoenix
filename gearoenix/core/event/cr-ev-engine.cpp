#include "cr-ev-engine.hpp"
#include "cr-ev-listener.hpp"
#include <functional>

constexpr double CLICK_THRESHOLD = 0.2f;
constexpr double MOUSE_DRAG_DISTANCE_THRESHOLD = 0.1f;

void gearoenix::core::event::Engine::loop() noexcept
{
    state = State::Running;
#define GX_CHECK                 \
    if (state != State::Running) \
    break
    while (state == State::Running) {
        signaler.lock_until(std::chrono::system_clock::now() + std::chrono::milliseconds(500));
        GX_CHECK;
        decltype(events) es;
        {
            std::lock_guard<std::mutex> _l(events_guard);
            std::swap(events, es);
        }
        GX_CHECK;
        std::lock_guard<std::mutex> _l(listeners_guard);
        for (const auto& e : es) {
            GX_CHECK;
            if (update_window_size_state(e))
                continue;
            auto& ps = events_id_priority_listeners[e.source];
            GX_CHECK;
            for (auto& p : ps) {
                GX_CHECK;
                auto& ls = p.second;
                GX_CHECK;
                for (auto& l : ls) {
                    GX_CHECK;
                    if (l->on_event(e)) {
                        GX_CHECK;
                        goto event_processed;
                    }
                    GX_CHECK;
                }
                GX_CHECK;
            }
        event_processed:;
            GX_CHECK;
        }
        check_window_size_state_timeout();
        GX_CHECK;
    }
#undef GX_CHECK
    state = State::Terminated;
}

bool gearoenix::core::event::Engine::update_window_size_state(const Data& event_data) noexcept
{
    if (event_data.source == Id::InternalSystemWindowSizeChange) {
        previous_window_size_update = std::chrono::high_resolution_clock::now();
        return true;
    }
    return false;
}

void gearoenix::core::event::Engine::check_window_size_state_timeout() noexcept
{
    if (window_width == previous_window_width && window_height == previous_window_height)
        return;
    if (std::chrono::high_resolution_clock::now() - previous_window_size_update < std::chrono::milliseconds(500))
        return;
    broadcast(Data {
        .source = Id::SystemWindowSizeChange,
        .data = system::WindowSizeChangeData {
            .previous_width = previous_window_width,
            .previous_height = previous_window_height,
            .previous_reversed_half_width = previous_window_reversed_half_width,
            .previous_reversed_half_height = previous_window_reversed_half_height,
            .previous_ratio = previous_window_ratio,
            .current_width = window_width,
            .current_height = window_height,
            .current_reversed_half_width = window_reversed_half_width,
            .current_reversed_half_height = window_reversed_half_height,
            .current_ratio = window_ratio,
            .delta_width = window_width - previous_window_width,
            .delta_height = window_height - previous_window_height,
            .delta_reversed_half_width = window_reversed_half_width - previous_window_reversed_half_width,
            .delta_reversed_half_height = window_reversed_half_height - previous_window_reversed_half_height,
            .delta_ratio = window_ratio - previous_window_ratio,
        },
    });
    set_previous_window_size();
}

void gearoenix::core::event::Engine::set_window_size(std::size_t w, std::size_t h) noexcept
{
    window_width = w;
    window_height = h;
    window_reversed_half_width = 2.0 / static_cast<double>(w);
    window_reversed_half_height = 2.0 / static_cast<double>(h);
    window_ratio = window_reversed_half_height / window_reversed_half_width;
}

void gearoenix::core::event::Engine::set_previous_window_size() noexcept
{
    previous_window_width = window_width;
    previous_window_height = window_height;
    previous_window_reversed_half_width = window_reversed_half_width;
    previous_window_reversed_half_height = window_reversed_half_height;
    previous_window_ratio = window_ratio;
}

double gearoenix::core::event::Engine::convert_raw_x(const int x) const noexcept
{
    return static_cast<double>(x) * window_reversed_half_height - window_ratio;
}

double gearoenix::core::event::Engine::convert_raw_y(const int y) const noexcept
{
    return static_cast<double>(y) * window_reversed_half_height - 1.0;
}

gearoenix::math::Vec2<double> gearoenix::core::event::Engine::convert_raw(const int x, const int y) const noexcept
{
    return math::Vec2(convert_raw_x(x), convert_raw_y(y));
}

gearoenix::core::event::Engine::Engine() noexcept
    : state(State::Running)
    , event_thread([this] { loop(); })
{
}

gearoenix::core::event::Engine::~Engine() noexcept
{
    state = State::Terminating;
    while (state != State::Terminated)
        signaler.release();
    event_thread.join();
}

void gearoenix::core::event::Engine::add_listener(Id event_id, double priority, Listener* listener) noexcept
{
    std::lock_guard<std::mutex> _l(listeners_guard);
    events_id_priority_listeners[event_id][priority].insert(listener);
}

void gearoenix::core::event::Engine::remove_listener(Id event_id, double priority, Listener* listener) noexcept
{
    std::lock_guard<std::mutex> _l(listeners_guard);
    events_id_priority_listeners[event_id][priority].erase(listener);
}

void gearoenix::core::event::Engine::remove_listener(Id event_id, Listener* listener) noexcept
{
    std::lock_guard<std::mutex> _l(listeners_guard);
    auto& e = events_id_priority_listeners[event_id];
    for (auto& p : e)
        p.second.erase(listener);
}

void gearoenix::core::event::Engine::remove_listener(Listener* listener) noexcept
{
    std::lock_guard<std::mutex> _l(listeners_guard);
    for (auto& e : events_id_priority_listeners) {
        auto& ps = e.second;
        for (auto& p : ps)
            p.second.erase(listener);
    }
}

void gearoenix::core::event::Engine::broadcast(const Data& event_data) noexcept
{
    if (event_data.source == Id::ButtonKeyboard) {
        const auto& ke = std::get<core::event::button::KeyboardData>(event_data.data);
        if (ke.action == button::KeyboardActionId::Press) {
            pressed_keyboard_buttons.insert(ke.key);
        } else if (ke.action == button::KeyboardActionId::Release) {
            pressed_keyboard_buttons.erase(ke.key);
        }
    }

    std::lock_guard<std::mutex> _l(events_guard);
    events.push_back(event_data);
    signaler.release();
}

void gearoenix::core::event::Engine::initialize_mouse_position(const int x, const int y) noexcept
{
    mouse_movement = movement::Base2D({x, y}, convert_raw(x, y));
}

void gearoenix::core::event::Engine::update_mouse_position(const int x, const int y) noexcept
{
    const math::Vec2<int> rp(x, y);
    const auto cp = convert_raw(x, y);
    mouse_movement.update(rp, cp);
    Data d;
    d.source = Id::MovementMouse;
    d.data = mouse_movement;
    broadcast(d);

    for (auto& ap : pressed_mouse_buttons_state) {
        auto& p = ap.second;
        p.update(rp, cp);
        const auto dt = mouse_movement.get_point().get_delta_start_time();
        if (dt > CLICK_THRESHOLD || mouse_movement.get_point().get_delta_start_position().length() > MOUSE_DRAG_DISTANCE_THRESHOLD) {
            d.source = Id::GestureDrag2D;
            gesture::Drag2D drag(p);
            if (ap.first == button::MouseKeyId::Left) {
                d.source = Id::GestureDrag2D;
                d.data = drag;
                broadcast(d);
            }
            broadcast(Data {
                .source = Id::GestureMouseDrag,
                .data = gesture::MouseDrag(drag, ap.first),
            });
        }
    }
}

void gearoenix::core::event::Engine::mouse_button(const button::MouseKeyId k, const button::MouseActionId a) noexcept
{
    broadcast(Data {
            .source = core::event::Id::ButtonMouse,
            .data = button::MouseData(
                    a, k,
            mouse_movement.get_point().get_current_position(),
            mouse_movement.get_point().get_raw_current_position()),
    });

    if (a == button::MouseActionId::Press) {
        pressed_mouse_buttons_state.emplace(std::make_pair(
                k, Point2D(
                        mouse_movement.get_point().get_raw_current_position(),
                        mouse_movement.get_point().get_current_position())));
    } else if (a == button::MouseActionId::Release) {
        auto pi = pressed_mouse_buttons_state.find(k);
        if (pi == pressed_mouse_buttons_state.begin()) {
            auto now = std::chrono::high_resolution_clock::now();
            auto& p = pi->second;
            const std::chrono::duration<double> dt = now - p.get_current_time();
            if (dt.count() < CLICK_THRESHOLD) {
                broadcast(Data {
                        .source = core::event::Id::ButtonMouse,
                        .data = button::MouseData(
                                button::MouseActionId::Click, k,
                                mouse_movement.get_point().get_current_position(),
                                mouse_movement.get_point().get_raw_current_position()),
                });
            }
            pressed_mouse_buttons_state.erase(k);
        }
    }
}

bool gearoenix::core::event::Engine::is_pressed(gearoenix::core::event::button::KeyboardKeyId k) const noexcept
{
    return pressed_keyboard_buttons.find(k) != pressed_keyboard_buttons.end();
}

void gearoenix::core::event::Engine::initialize_window_size(std::size_t w, std::size_t h) noexcept
{
    set_window_size(w, h);
    set_previous_window_size();
}

void gearoenix::core::event::Engine::update_window_size(const std::size_t w, const std::size_t h) noexcept
{
    set_window_size(w, h);
    broadcast(Data {
        .source = Id::InternalSystemWindowSizeChange,
        .data = 0,
    });
}

void gearoenix::core::event::Engine::touch_down(touch::FingerId finger_id, int x, int y) noexcept
{
    touch_states.emplace(std::make_pair(finger_id, touch::State(finger_id, math::Vec2(x, y), convert_raw(x, y))));
    if (touch_states.size() == 1) {
    }
}

void gearoenix::core::event::Engine::touch_move(touch::FingerId finger_id, int x, int y) noexcept
{
    auto ts = touch_states.find(finger_id);
    ts->second.update(math::Vec2(x, y), convert_raw(x, y));
}

void gearoenix::core::event::Engine::touch_up(touch::FingerId finger_id, int x, int y) noexcept
{
    touch_states.erase(finger_id);
}

void gearoenix::core::event::Engine::touch_cancel(touch::FingerId finger_id, int x, int y) noexcept
{
    touch_states.erase(finger_id);
}
