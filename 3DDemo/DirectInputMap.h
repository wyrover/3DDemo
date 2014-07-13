#ifndef DIRECTINPUTMAP_H
#define DIRECTINPUTMAP_H

// TODO: can this header be removed somehow?
#include <WindowsCommon/InputDevice.h>

std::list<std::pair<float, enum Action>> actions_from_keyboard_state(float ellapsed_milliseconds, const WindowsCommon::Keyboard_state& keyboard_state);

#endif

