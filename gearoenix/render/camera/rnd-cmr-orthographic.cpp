#include "rnd-cmr-orthographic.hpp"
#include "../../core/event/cr-ev-event.hpp"
#include "../../system/sys-file.hpp"
#include <cmath>

void gearoenix::render::camera::Orthographic::on_ratio_change()
{
    c_width = aspects_size * screen_ratio;
    p = math::Mat4x4::orthographic(c_width * 2.0f, c_height * 2.0f, start, end);
    vp = p * v;
}

gearoenix::render::camera::Orthographic::Orthographic(system::File* f, system::Application* sysapp)
    : Camera(f, sysapp)
{
    f->read(aspects_size);
    c_height = aspects_size;
    on_ratio_change();
}

bool gearoenix::render::camera::Orthographic::in_sight(const math::Vec3& location, const core::Real radius) const
{
    math::Vec3 eye = location - l;
    core::Real eye_z = -(eye.dot(z));
    if (eye_z < 0.0f)
        return eye.square_length() < (radius * radius);
    if (eye_z - radius > end)
        return false;
    math::Vec3 eye_on_z_plane = eye - (z * (-eye_z));
    core::Real eye_on_x = std::abs(eye_on_z_plane.dot(x));
    if (eye_on_x - radius > c_width)
        return false;
    core::Real eye_on_y = std::abs(eye_on_z_plane.dot(y));
    if (eye_on_y - radius > c_height)
        return false;
    return true;
}

void gearoenix::render::camera::Orthographic::on_event(const core::event::Event& e)
{
    Camera::on_event(e);
	if (e.get_type() == core::event::Event::EventType::WINDOW_RESIZE)
		on_ratio_change();
}

gearoenix::math::Ray3 gearoenix::render::camera::Orthographic::create_ray3(const core::Real x, const core::Real y) const
{
	math::Vec3 dir = -(z * start);
	math::Vec3 origin = (this->x * x) + (this->y * y) + dir + l;
	dir.normalize();
	origin.print();
	dir.print();
	return math::Ray3(origin, dir);
}