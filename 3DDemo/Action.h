#pragma once

namespace Demo
{

enum class Action
{
    Move_forward,
    Move_backward,
    Strafe_right,
    Strafe_left,
    Turn_right,
    Turn_left
};

struct Camera apply_actions(const std::vector<std::pair<float, Action>>& actions, const struct Camera& camera);

}

