
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <math.h>
#include <iostream>
#include <SDL2/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif

#ifdef __WIN32__
#include <windows.h>
#include <string>
#include <limits.h>

HWND GetWindowHWND(SDL_Window *wnd)
{
    SDL_SysWMinfo sysInfo;
    SDL_VERSION(&sysInfo.version);
    SDL_GetWindowWMInfo(wnd, &sysInfo);
    return sysInfo.info.win.window;
}
std::string getCurrentDir()
{
    char buff[MAX_PATH];
    GetModuleFileName(NULL, buff, MAX_PATH);
    string::size_type position = string(buff).find_last_of("\\/");
    return string(buff).substr(0, position);
}
#endif
bool isNumber(const std::string &str)
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}
void replace(std::string &str,
             const std::string &oldStr,
             const std::string &newStr)
{
    std::string::size_type pos = 0u;
    while ((pos = str.find(oldStr, pos)) != std::string::npos)
    {
        str.replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
    }
}

int main(int, char **argv)
{
    std::string currentDirectory = "/";
#ifdef __WIN32
    currentDirectory = getCurrentDir();
#else
    currentDirectory = get_current_dir_name();
#endif

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    /* Compatibility */
#if defined(IMGUI_IMPL_OPENGL_ES2)
    const char *glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    const char *glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    const char *glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window *window = SDL_CreateWindow("Intervals math functions", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 250, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Block screenshots
#ifdef __WIN32__
    SetWindowDisplayAffinity(GetWindowHWND(window), WDA_MONITOR);
#endif

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.IniFilename = NULL;
    io.LogFilename = NULL;

    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    ImVec4 *colors = style.Colors;

    colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0, 0, 0, 0.88f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.57f, 0.08f, 0.12f, 0.90f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.57f, 0.08f, 0.12f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.57f, 0.08f, 0.12f, 0.80f);
    colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.57f, 0.08f, 0.12f, 0.50f);
    style.PopupRounding = 3;

    style.WindowPadding = ImVec2(4, 4);
    style.FramePadding = ImVec2(2, 2);
    style.ItemSpacing = ImVec2(6, 2);

    style.ScrollbarSize = 18;

    style.WindowBorderSize = 1;
    style.ChildBorderSize = 1;
    style.PopupBorderSize = 1;

    style.WindowRounding = 0;
    style.ChildRounding = 3;
    style.FrameRounding = 3;
    style.ScrollbarRounding = 2;
    style.ScrollbarSize = 3;
    style.GrabRounding = 3;

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVector<ImWchar> ranges;
    ImFontGlyphRangesBuilder builder;
    builder.AddRanges(io.Fonts->GetGlyphRangesDefault()); // Add one of the default ranges
    builder.AddText("≤ ∞ π √ ∈");
    builder.BuildRanges(&ranges);

    io.Fonts->AddFontFromFileTTF(std::string(currentDirectory + "/font.ttf").c_str(), 24.0f, NULL, ranges.Data);
    ImFont *scriptfont = io.Fonts->AddFontFromFileTTF(std::string(currentDirectory + "/fontscript.ttf").c_str(), 24.0f, NULL, ranges.Data);
    ImFont *bigFont = io.Fonts->AddFontFromFileTTF(std::string(currentDirectory + "/font.ttf").c_str(), 36.0f, NULL, ranges.Data);
    ImFont *littleFont = io.Fonts->AddFontFromFileTTF(std::string(currentDirectory + "/font.ttf").c_str(), 16.0f, NULL, ranges.Data);

    ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 1.00f);

    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        int w, h = 0;
        SDL_GetWindowSize(window, &w, &h);

        ImGui::SetNextWindowSize(ImVec2(w, h));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Interval", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

        static char firstnum[11] = {'0'};
        static char secnum[11] = {'0'};

        const char *items[] = {"No", "≤", "<"};

        static int firstsymbol = 0;
        static int secondsymbol = 0;

        static int focused = 0;

        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(5, 5), ImVec2((w / 2) - 10, h - 10), IM_COL32(33, 33, 33, 255), 3.f);
        ImGui::SetCursorPos(ImVec2(10, 10));
        ImGui::BeginChild("Inputs", ImVec2((w / 2) - 20, h - 20));
        if (firstsymbol != 0)
        {
            ImGui::SetNextItemWidth(35);
            ImGui::InputText("##firstnum", firstnum, IM_ARRAYSIZE(firstnum));
            if (ImGui::IsItemFocused())
                focused = 0;
            ImGui::SameLine();
        }
        ImGui::SetNextItemWidth(60);
        ImGui::Combo("##firstnumcombo", &firstsymbol, items, IM_ARRAYSIZE(items));
        ImGui::SameLine();
        ImGui::PushFont(scriptfont);
        ImGui::Text("x");
        ImGui::PopFont();
        ImGui::SameLine();
        ImGui::SetNextItemWidth(60);
        ImGui::Combo("##secnumcombo", &secondsymbol, items, IM_ARRAYSIZE(items));
        if (secondsymbol != 0)
        {
            ImGui::SameLine();
            ImGui::SetNextItemWidth(35);
            ImGui::InputText("##secnum", secnum, IM_ARRAYSIZE(secnum));
            if (ImGui::IsItemFocused())
                focused = 1;
        }
        ImGui::Dummy(ImVec2(w, 25));
        ImGui::Text("Keyboard");

        if (ImGui::Button("π", ImVec2(40, 25)))
        {
            if (focused == 0)
            {
                sprintf(firstnum, "π");
            }
            else
            {
                sprintf(secnum, "π");
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("√", ImVec2(40, 25)))
        {
            if (focused == 0)
            {
                sprintf(firstnum, "√");
            }
            else
            {
                sprintf(secnum, "√");
            }
        }
        ImGui::EndChild();
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2((w / 2) + 5, 5), ImVec2(w - 10, h - 10), IM_COL32(33, 33, 33, 255), 3.f);
        ImGui::SetCursorPos(ImVec2((w / 2) + 10, 10));
        ImGui::BeginChild("Result", ImVec2((w / 2) - 20, h - 20));

        ImGui::PushFont(scriptfont);
        ImGui::Text("x");
        ImGui::PopFont();
        ImGui::SameLine();

        std::stringstream ss;
        ss << " ∈ ";
        if (firstsymbol != 0)
            ss << (firstsymbol == 1 ? "[" : "]") << firstnum;
        else
            ss << "]-∞";
        ss << ";";
        if (secondsymbol != 0)
            ss << secnum << (secondsymbol == 1 ? "]" : "[");
        else
            ss << "+∞[";

        ImGui::Text(ss.str().c_str());
        float windowwidth = (w / 2) - 50;
        float xpos = (w / 2) + 10;
        float width1 = (windowwidth / 2) / 4;
        float xpos2 = (windowwidth / 2) + (w / 2) + 10;

        ImGui::GetWindowDrawList()->AddLine(ImVec2(xpos, 100), ImVec2(xpos + windowwidth, 100), IM_COL32(255, 255, 255, 255), 1.f);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(xpos + (windowwidth / 2), 90), ImVec2(xpos + (windowwidth / 2), 110), IM_COL32(255, 255, 255, 255), 1.f);

        ImGui::GetWindowDrawList()->AddLine(ImVec2(xpos, 96), ImVec2(xpos, 104), IM_COL32(255, 255, 255, 255), 1.f);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(xpos + width1 * 1, 96), ImVec2(xpos + width1 * 1, 104), IM_COL32(255, 255, 255, 255), 1.f);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(xpos + width1 * 2, 96), ImVec2(xpos + width1 * 2, 104), IM_COL32(255, 255, 255, 255), 1.f);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(xpos + width1 * 3, 96), ImVec2(xpos + width1 * 3, 104), IM_COL32(255, 255, 255, 255), 1.f);

        ImGui::GetWindowDrawList()->AddLine(ImVec2(xpos2 + width1 * 1, 96), ImVec2(xpos2 + width1 * 1, 104), IM_COL32(255, 255, 255, 255), 1.f);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(xpos2 + width1 * 2, 96), ImVec2(xpos2 + width1 * 2, 104), IM_COL32(255, 255, 255, 255), 1.f);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(xpos2 + width1 * 3, 96), ImVec2(xpos2 + width1 * 3, 104), IM_COL32(255, 255, 255, 255), 1.f);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(xpos2 + width1 * 4, 96), ImVec2(xpos2 + width1 * 4, 104), IM_COL32(255, 255, 255, 255), 1.f);

        std::string sfirst(firstnum);
        std::string ssecond(secnum);

        replace(sfirst, "π", "3.1415926535897");
        replace(ssecond, "π", "3.1415926535897");
        if (sfirst.find("√") != std::string::npos)
        {
            replace(sfirst, "√", "");
            sfirst = std::to_string(sqrt(strtof(sfirst.c_str(), NULL)));
        }

        if (ssecond.find("√") != std::string::npos)
        {
            replace(ssecond, "√", "");
            ssecond = std::to_string(sqrt(strtof(ssecond.c_str(), NULL)));
        }

        float scale = sqrt(pow(strtof(
                                   (((sqrt(pow(strtof(ssecond.c_str(), NULL), 2.f)) < sqrt(pow(strtof(sfirst.c_str(), NULL), 2.f)) && (firstsymbol != 0 && secondsymbol != 0)) ||
                                     (firstsymbol != 0 && secondsymbol == 0))
                                        ? sfirst.c_str()
                                    : ((sqrt(pow(strtof(ssecond.c_str(), NULL), 2.f)) > sqrt(pow(strtof(sfirst.c_str(), NULL), 2.f)) && (firstsymbol != 0 && secondsymbol != 0)) ||
                                       (firstsymbol == 0 && secondsymbol != 0))
                                        ? ssecond.c_str()
                                        : "0.f"),
                                   NULL) /
                                   2,
                               2));

        bool isgoingintwosign = (strtof(ssecond.c_str(), NULL) < 0 && strtof(sfirst.c_str(), NULL) > 0 && (firstsymbol != 0 && secondsymbol != 0)) ||
                                (strtof(ssecond.c_str(), NULL) > 0 && strtof(sfirst.c_str(), NULL) < 0 && (firstsymbol != 0 && secondsymbol != 0)) ||
                                (firstsymbol == 0 && secondsymbol != 0) ||
                                (firstsymbol != 0 && secondsymbol == 0);

        float less = strtof(ssecond.c_str(), NULL) < strtof(sfirst.c_str(), NULL) ? strtof(ssecond.c_str(), NULL) : strtof(ssecond.c_str(), NULL) > strtof(sfirst.c_str(), NULL) ? strtof(sfirst.c_str(), NULL)
                                                                                                                                                                                 : 0.f;

        ImGui::PushFont(bigFont);
        float middle = !isgoingintwosign ? ((strtof(ssecond.c_str(), NULL) - strtof(sfirst.c_str(), NULL)) / 2) + less : 0.f;

        if (secondsymbol != 0)
            ImGui::GetWindowDrawList()->AddText(ImVec2(xpos + (windowwidth / 2) + (((strtof(ssecond.c_str(), NULL) - middle) / scale) * width1) - ImGui::CalcTextSize((secondsymbol == 1 ? "]" : "[")).x / 2, 85), IM_COL32(255, 0, 0, 255), (secondsymbol == 1 ? "]" : "["));
        if (firstsymbol != 0)
            ImGui::GetWindowDrawList()->AddText(ImVec2(xpos + (windowwidth / 2) + (((strtof(sfirst.c_str(), NULL) - middle) / scale) * width1) - ImGui::CalcTextSize((firstsymbol == 1 ? "[" : "]")).x / 2, 85), IM_COL32(255, 0, 0, 255), (firstsymbol == 1 ? "[" : "]"));

        if (secondsymbol != 0 && firstsymbol != 0)
            ImGui::GetWindowDrawList()->AddLine(ImVec2(xpos + (windowwidth / 2) + ((strtof(sfirst.c_str(), NULL) - middle) / scale) * width1, 103), ImVec2(xpos + (windowwidth / 2) + ((strtof(ssecond.c_str(), NULL) - middle) / scale) * width1, 103), IM_COL32(255, 0, 0, 255), 3.f);
        if (secondsymbol != 0 && firstsymbol == 0)
            ImGui::GetWindowDrawList()->AddLine(ImVec2(xpos + (windowwidth / 2) + ((strtof(ssecond.c_str(), NULL) - middle) / scale) * width1, 103), ImVec2(xpos, 103), IM_COL32(255, 0, 0, 255), 3.f);
        if (secondsymbol == 0 && firstsymbol != 0)
            ImGui::GetWindowDrawList()->AddLine(ImVec2(xpos + (windowwidth / 2) + ((strtof(sfirst.c_str(), NULL) - middle) / scale) * width1, 103), ImVec2(xpos + windowwidth, 103), IM_COL32(255, 0, 0, 255), 3.f);

        ImGui::PopFont();

        ImGui::PushFont(littleFont);
        char buffermiddle[11];
        sprintf(buffermiddle, "%.2f", middle);
        ImGui::GetWindowDrawList()->AddText(ImVec2(xpos2 - (ImGui::CalcTextSize(buffermiddle).x / 2), 120), IM_COL32(255, 255, 255, 255), buffermiddle);

        char bufferscale[11];
        sprintf(bufferscale, "%.2f", scale);
        ImGui::GetWindowDrawList()->AddText(ImVec2(xpos, 120), IM_COL32(255, 255, 255, 255), bufferscale);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(xpos, 120), ImVec2(xpos + width1, 120), IM_COL32(255, 255, 255, 255), 1.f);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(xpos, 115), ImVec2(xpos, 120), IM_COL32(255, 255, 255, 255), 1.f);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(xpos + width1, 115), ImVec2(xpos + width1, 120), IM_COL32(255, 255, 255, 255), 1.f);
        ImGui::PopFont();
        ImGui::EndChild();

        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(w - 30, h - 20), ImVec2(w - 20, h), IM_COL32(0, 0, 145, 150), 0);
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(w - 20, h - 20), ImVec2(w - 10, h), IM_COL32(255, 255, 255, 150), 0);
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(w - 10, h - 20), ImVec2(w, h), IM_COL32(255, 0, 15, 150), 0);
        ImGui::PushFont(littleFont);
        std::string github = "https://github.com/DrayNeur";
        ImGui::GetWindowDrawList()->AddText(ImVec2((w - ImGui::CalcTextSize(github.c_str()).x) - 30, (h - ImGui::CalcTextSize(github.c_str()).y) - 3), IM_COL32(255, 255, 255, 150), github.c_str());
        ImGui::PopFont();
        ImGui::End();

        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
