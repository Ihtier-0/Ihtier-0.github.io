#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif
#include <cmath>

struct AppState {
    SDL_Window* window = nullptr;
    SDL_GLContext gl_context = nullptr;
    float speed = 1.0f;
    float radius = 80.0f;
    int num_circles = 6;
    float color1[3] = {0.2f, 0.6f, 1.0f};
    float color2[3] = {1.0f, 0.3f, 0.5f};
    ImVec4 bg_color = ImVec4(0.06f, 0.07f, 0.09f, 1.0f);
    float time = 0.0f;
    int counter = 0;
};

static AppState g;

void MainLoop(void*) {
    ImGuiIO& io = ImGui::GetIO();

#ifdef __EMSCRIPTEN__
    double cssW, cssH;
    emscripten_get_element_css_size("#canvas", &cssW, &cssH);
    int w = (int)cssW, h = (int)cssH;
    emscripten_set_canvas_element_size("#canvas", w, h);
    SDL_SetWindowSize(g.window, w, h);
#endif

    SDL_Event event;
    while (SDL_PollEvent(&event))
        ImGui_ImplSDL2_ProcessEvent(&event);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    g.time += io.DeltaTime * g.speed;

    // Controls
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(260, 0), ImGuiCond_FirstUseEver);
    ImGui::Begin("Controls");
    ImGui::SliderFloat("Speed", &g.speed, 0.0f, 5.0f);
    ImGui::SliderFloat("Radius", &g.radius, 10.0f, 200.0f);
    ImGui::SliderInt("Circles", &g.num_circles, 1, 20);
    ImGui::ColorEdit3("Color 1", g.color1);
    ImGui::ColorEdit3("Color 2", g.color2);
    ImGui::Separator();
    if (ImGui::Button("Click me"))
        g.counter++;
    ImGui::SameLine();
    ImGui::Text("count = %d", g.counter);
    ImGui::Text("%.1f FPS", io.Framerate);
    ImGui::End();

    // Visualization
    ImGui::SetNextWindowPos(ImVec2(280, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x - 290, io.DisplaySize.y - 20), ImGuiCond_FirstUseEver);
    ImGui::Begin("Visualization", nullptr, ImGuiWindowFlags_NoScrollbar);

    ImVec2 p0 = ImGui::GetCursorScreenPos();
    ImVec2 sz = ImGui::GetContentRegionAvail();
    ImVec2 center = ImVec2(p0.x + sz.x * 0.5f, p0.y + sz.y * 0.5f);
    ImDrawList* dl = ImGui::GetWindowDrawList();

    for (int i = 0; i < g.num_circles; i++) {
        float angle = g.time + (float)i * (6.28318f / (float)g.num_circles);
        float x = center.x + cosf(angle) * g.radius;
        float y = center.y + sinf(angle) * g.radius;
        float t = (float)i / fmaxf((float)g.num_circles, 1.0f);

        ImU32 color = ImGui::ColorConvertFloat4ToU32(ImVec4(
            g.color1[0] * (1 - t) + g.color2[0] * t,
            g.color1[1] * (1 - t) + g.color2[1] * t,
            g.color1[2] * (1 - t) + g.color2[2] * t,
            0.85f
        ));

        float r = 12.0f + 8.0f * sinf(g.time * 2.0f + (float)i);
        dl->AddCircleFilled(ImVec2(x, y), r, color, 32);
    }

    ImGui::End();

    // Render
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(g.bg_color.x, g.bg_color.y, g.bg_color.z, g.bg_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(g.window);
}

int main(int, char**) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    g.window = SDL_CreateWindow(
        "ImGui Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 450,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );
    g.gl_context = SDL_GL_CreateContext(g.window);
    SDL_GL_MakeCurrent(g.window, g.gl_context);
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.IniFilename = nullptr;

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;

    ImGui_ImplSDL2_InitForOpenGL(g.window, g.gl_context);
    ImGui_ImplOpenGL3_Init("#version 300 es");

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(MainLoop, nullptr, 0, true);
#endif

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(g.gl_context);
    SDL_DestroyWindow(g.window);
    SDL_Quit();
    return 0;
}
