#include "PreCompile.h"
#include "app.h"
#include "world.h"
#include "render.h"
#include "particle.h"
#include "Camera.h"
#include "Action.h"
#include "Bitmap.h"
#include "Clock.h"
#include "DirectInputMap.h"
#include "HRException.h"
#include "InputDevice.h"
#include "LinearAlgebra.h"
#include "ThreadAffinity.h"
#include "Tracing.h"
#include "WindowsGL.h"

static bool s_fWindowed = true;

static UINT_PTR game_message_loop(WindowsCommon::Clock& clock, const WindowsCommon::Input_device& keyboard, const std::vector<Graphics::Polygon>& polys)
{
    Camera camera(make_vector(0.0f, 0.0f, 1.0f), 0.0f);
    Emitter emitter(make_vector(-3.0f, 0.0f, -10.5f));

    MSG message;
    for(;;)
    {
        if(!WindowsCommon::dispatch_all_windows_messages(&message))
        {
            // Renderer and window handle were destroyed in WM_DESTROY.
//            assert(nullptr == m_renderer);
            break;
        }

        const float elapsed_milliseconds = clock.ellapsed_milliseconds();
        WindowsCommon::dprintf("QPC: %f\r\n", elapsed_milliseconds);

        WindowsCommon::Keyboard_state keyboard_state;
        keyboard.get_input(&keyboard_state);

        std::list<std::pair<float, Action>> actions = actions_from_keyboard_state(elapsed_milliseconds, keyboard_state);
        camera = apply_actions(actions, camera);
        emitter.update(elapsed_milliseconds);

        draw_list(polys, emitter, camera);

        const HDC device_context = wglGetCurrentDC();
        SwapBuffers(device_context);
    }

    return message.wParam;
}

class App_window : public WindowsCommon::OpenGL_window
{
public:
    App_window(_In_ HINSTANCE instance, bool windowed) :
        OpenGL_window(TEXT("3D Demo 1999 (Updated for C++11)"), instance, windowed)
    {
    }

protected:
    LRESULT window_proc(_In_ HWND window, UINT message, WPARAM w_param, LPARAM l_param) NOEXCEPT
    {
        LRESULT return_value = OpenGL_window::window_proc(window, message, w_param, l_param);

        if(message == WM_DESTROY)
        {
            PostQuitMessage(0);
        }

        return return_value;
    }
};

void app_run(HINSTANCE instance, int show_command)
{
    try
    {
        // Start load first, to kick off async reads.
        std::vector<Bitmap> texture_list;
        std::vector<Graphics::Polygon> polys;
        std::vector<Vector3f> vertices;
        std::vector<Vector2f> texture_coords;
        start_load("polydefs.txt", &texture_list, &polys, &vertices, &texture_coords);

        App_window app(instance, true);

        // TODO: 2014: does this need to be reinitialized if the video engine is reinitialized?
        initialize_gl_constants();
        initialize_gl_world_data(texture_list, vertices, texture_coords);

        // Set thread affinity to the first available processor, so that QPC
        // will always be done on the same processor.
        WindowsCommon::lock_thread_to_first_processor();
        WindowsCommon::Clock clock;

        WindowsCommon::Input_device keyboard(instance, app.m_state.window);

        ShowWindow(app.m_state.window, show_command);
        UpdateWindow(app.m_state.window);

        auto return_code = game_message_loop(clock, keyboard, polys);

        // _tWinMain return code is an int type.
        assert(INT_MAX > return_code);

        // Window procedure doesn't currently have a reason to return non-zero.
        assert(0 == return_code);

        // This would be the legal part of the return code.
        //static_cast<int>(return_code);
        UNREFERENCED_PARAMETER(return_code);

        assert((nullptr == app.m_state.window) && "window handle was never released.");
    }
    // Convert exceptions to a OS native error type for later display.
    catch(const WindowsCommon::HRESULT_exception& ex)
    {
        UNREFERENCED_PARAMETER(ex);

        throw;
    }
    catch(const std::bad_alloc& ex)
    {
        UNREFERENCED_PARAMETER(ex);

        WindowsCommon::throw_hr(E_OUTOFMEMORY);
    }
    catch(...)
    {
        // TODO: uninitialize isn't always the correct text.
        //MessageBox(nullptr, TEXT("Unable to initialize engine."), TEXT("Exiting"), MB_OK);
        WindowsCommon::throw_hr(E_FAIL);
    }
}

