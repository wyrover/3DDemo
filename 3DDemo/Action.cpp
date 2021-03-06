#include "PreCompile.h"
#include "Action.h"
#include "Camera.h"
#include "world.h"

namespace Demo
{

Camera apply_actions(const std::vector<std::pair<float, Action>>& actions, const Camera& camera)
{
    Camera new_camera = camera;
    float new_x = new_camera.m_position.x();
    float new_z = new_camera.m_position.z();

    // These two values can be configurable for gameplay.
    const auto walk_distance_per_tick = 0.045f;
    const auto keyboard_rotational_speed_per_tick = 0.5f;

    const auto radians_per_degree = static_cast<float>(M_PI * 2.0f / 360.0f);

    // Accumulate all movement inputs before application.  This will prevent
    // two contractory button presses (e.g. strafe and forward) from bouncing
    // between each other in, say, a corner.
    // TODO: There still exists an issue, where pushing into a corner, and
    // then moving forward and strafing at the same time still produces movement.
    // Ideally, moving forward would just follow the edge until there is no possible
    // forward motion possible, even while moving laterally.
    std::for_each(actions.cbegin(), actions.cend(), [&](const std::pair<float, Action>& action)
    {
        const auto walk_distance = walk_distance_per_tick * action.first;
        const auto sine = sinf(new_camera.m_degrees * radians_per_degree) * walk_distance;
        const auto cosine = cosf(new_camera.m_degrees * radians_per_degree) * walk_distance;
        const auto rotation_degrees = keyboard_rotational_speed_per_tick * action.first;

        switch(action.second)
        {
            case Action::Move_forward:
            {
                new_x -= sine;
                new_z += cosine;
                break;
            }

            case Action::Move_backward:
            {
                new_x += sine;
                new_z -= cosine;
                break;
            }

            case Action::Strafe_right:
            {
                new_x -= cosine;
                new_z -= sine;
                break;
            }

            case Action::Strafe_left:
            {
                new_x += cosine;
                new_z += sine;
                break;
            }

            case Action::Turn_right:
            {
                new_camera.m_degrees = fmod(new_camera.m_degrees + rotation_degrees, 360.f);
                break;
            }

            case Action::Turn_left:
            {
                new_camera.m_degrees = fmod(new_camera.m_degrees - rotation_degrees + 360.f, 360.f);
                break;
            }
        }
    });

    // Testing each coordinate on its own only is correct because each boundary is axis aligned.
    float updated_x = new_camera.m_position.x();
    if(is_point_in_world(make_vector(new_x, new_camera.m_position.y(), new_camera.m_position.z())))
    {
        updated_x = new_x;
    }
    if(is_point_in_world(make_vector(new_camera.m_position.x(), new_camera.m_position.y(), new_z)))
    {
        new_camera.m_position.z() = new_z;
    }
    new_camera.m_position.x() = updated_x;

    return new_camera;
}

}

