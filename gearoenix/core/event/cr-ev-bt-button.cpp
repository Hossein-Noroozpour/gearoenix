#include "cr-ev-bt-button.hpp"
#include "cr-ev-bt-mouse.hpp"
#include "../../system/sys-log.hpp"

gearoenix::core::event::button::Button::Button(ButtonType t, KeyType k, ActionType a)
	: Event(EventType::BUTTON)
	, button_type(t)
	, key_type(k)
	, action_type(a)
{
}

gearoenix::core::event::button::Button::~Button() {}

gearoenix::core::event::button::Button::ButtonType gearoenix::core::event::button::Button::get_type() const
{
	return button_type;
}

gearoenix::core::event::button::Button::KeyType gearoenix::core::event::button::Button::get_key() const
{
	return key_type;
}

gearoenix::core::event::button::Button::ActionType gearoenix::core::event::button::Button::get_action() const
{
	return action_type;
}

const gearoenix::core::event::button::Mouse & gearoenix::core::event::button::Button::to_mouse() const
{
#ifdef DEBUG_MODE
	if (MOUSE != button_type)
		UNEXPECTED;
#endif
	return *reinterpret_cast<const Mouse*>(this);
}