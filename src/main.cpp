/*
 * This file is part of imguiplot_demo
 * Copyright (c) 2020 Malte Kießling
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "messagebox.h"
#include "plotdemo.h"
#include "shared.h"

int main(int, char**)
{
    int sdlRes = SDL_Init(SDL_INIT_VIDEO);
    if (sdlRes != 0) {
        messageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR", "Could not Init SDL: {}", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    const char* glsl_version = "#version 130";

    //windowpos is doing shitstuff NOLINTNEXTLINE
    auto* window = SDL_CreateWindow("LAA", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        messageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR", "Could not create window: {}", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    auto* context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
        messageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR", "Could not create OpenGL Context: {}", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_GL_MakeCurrent(window, context);
    SDL_GL_SetSwapInterval(1);

    // glew has a slightly different signature, but they are compatible NOLINTNEXTLINE
    auto gl3wres = gl3wInit2(reinterpret_cast<GL3WGetProcAddressProc>(&SDL_GL_GetProcAddress));
    if (gl3wres != GL3W_OK) {
        messageBox(SDL_MESSAGEBOX_WARNING, "GL Error", "OpenGL version not supported. App might just crash");
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    bool running = true;
    Uint64 timeAtStart = SDL_GetPerformanceCounter();
    auto ticksPerSecond = static_cast<double>(SDL_GetPerformanceFrequency());

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (ImGui_ImplSDL2_ProcessEvent(&e)) {
                continue;
            }
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }

        glClearColor(0.0F, 0.0F, 0.0F, 1.0F); // NOLINT
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // NOLINT

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::SetNextWindowBgAlpha(0.0f);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Dock Windows Here", nullptr, window_flags);
        ImGui::PopStyleVar(3);

        ImGuiID dockspace_id = ImGui::GetID("Dock windows here");
        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_AutoHideTabBar;
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        ImGui::End();

        auto time = static_cast<double>(SDL_GetPerformanceCounter() - timeAtStart) / ticksPerSecond;
        PlotDemo(time);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(window, context);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}