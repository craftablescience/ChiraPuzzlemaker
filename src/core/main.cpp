#include <string>
#include "../../external/ChiraEngine/src/core/engine.h"
#include "../../external/ChiraEngine/src/render/freecam.h"
#include "../../external/ChiraEngine/src/loader/objMeshLoader.h"
#include "../../external/ChiraEngine/src/render/unlitMaterial.h"
#include "../../external/ChiraEngine/src/render/texture2d.h"

// https://github.com/ocornut/imgui/issues/707#issuecomment-362574409
void setupImGuiStyle() {
    ImGuiStyle* style = &ImGui::GetStyle();
    float hspacing = 8.0;
    float vspacing = 6.0;
    style->DisplaySafeAreaPadding = ImVec2(0, 0);
    style->WindowPadding = ImVec2(hspacing/2, vspacing);
    style->FramePadding = ImVec2(hspacing, vspacing);
    style->ItemSpacing = ImVec2(hspacing, vspacing);
    style->ItemInnerSpacing = ImVec2(hspacing, vspacing);
    style->IndentSpacing = 20.0f;

    style->WindowRounding = 0.0f;
    style->FrameRounding = 0.0f;

    style->WindowBorderSize = 0.0f;
    style->FrameBorderSize = 1.0f;
    style->PopupBorderSize = 1.0f;

    style->ScrollbarSize = 20.0f;
    style->ScrollbarRounding = 0.0f;
    style->GrabMinSize = 5.0f;
    style->GrabRounding = 0.0f;

    ImVec4 white = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    ImVec4 transparent = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    ImVec4 dark = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
    ImVec4 darker = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);

    ImVec4 background = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    ImVec4 text = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    ImVec4 border = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    ImVec4 grab = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
    ImVec4 header = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    ImVec4 active = ImVec4(0.00f, 0.47f, 0.84f, 1.00f);
    ImVec4 hover = ImVec4(0.00f, 0.47f, 0.84f, 0.20f);

    style->Colors[ImGuiCol_Text] = text;
    style->Colors[ImGuiCol_WindowBg] = background;
    style->Colors[ImGuiCol_ChildBg] = background;
    style->Colors[ImGuiCol_PopupBg] = white;

    style->Colors[ImGuiCol_Border] = border;
    style->Colors[ImGuiCol_BorderShadow] = transparent;

    style->Colors[ImGuiCol_Button] = header;
    style->Colors[ImGuiCol_ButtonHovered] = hover;
    style->Colors[ImGuiCol_ButtonActive] = active;

    style->Colors[ImGuiCol_FrameBg] = white;
    style->Colors[ImGuiCol_FrameBgHovered] = hover;
    style->Colors[ImGuiCol_FrameBgActive] = active;

    style->Colors[ImGuiCol_MenuBarBg] = header;
    style->Colors[ImGuiCol_Header] = header;
    style->Colors[ImGuiCol_HeaderHovered] = hover;
    style->Colors[ImGuiCol_HeaderActive] = active;

    style->Colors[ImGuiCol_CheckMark] = text;
    style->Colors[ImGuiCol_SliderGrab] = grab;
    style->Colors[ImGuiCol_SliderGrabActive] = darker;

    //style->Colors[ImGuiCol_CloseButton] = transparent;
    //style->Colors[ImGuiCol_CloseButtonHovered] = transparent;
    //style->Colors[ImGuiCol_CloseButtonActive] = transparent;

    style->Colors[ImGuiCol_ScrollbarBg] = header;
    style->Colors[ImGuiCol_ScrollbarGrab] = grab;
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = dark;
    style->Colors[ImGuiCol_ScrollbarGrabActive] = darker;
}

void renderMenuBar(engine* e) {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New")) {
                // load default map
            }
            if (ImGui::MenuItem("Open...")) {
                // open map open dialog
            }
            if (ImGui::MenuItem("Save")) {
                // if it's not saved before open save dialog, else save
            }
            if (ImGui::MenuItem("Save as...")) {
                // open map save dialog
            }
            if (ImGui::MenuItem("Settings...")) {
                // open settings window
            }
            if (ImGui::MenuItem("Exit")) {
                // Maybe save the map before exiting, or ask to save
                e->stop();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Properties...")) {
                // open the map properties window
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("Controls")) {
                // show controls window
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

int main() {
    engine engine;
    virtualFileSystem::addResourceDirectory("resources/editor/");

    engine.getSettingsLoader()->setValue("engine", "title", std::string("Chira Editor"), true, true);
    objMeshLoader meshLoader;

#if DEBUG
    engine.addKeybind(keybind(GLFW_KEY_ESCAPE, GLFW_PRESS, [](class engine* e) {
        e->stop();
    }));
    engine.addKeybind(keybind(GLFW_KEY_GRAVE_ACCENT, GLFW_PRESS, [](class engine* e) {
        e->showConsole(!e->getConsole()->getEnabled());
    }));
#endif

    engine::addTexture("teapot", new texture2d{"ui/icon.png", GL_RGBA});
    engine::addShader("unlit", new shader{"unlit.vsh", "unlit.fsh"});
    engine::addMaterial("unlit", new unlitMaterial{"unlit"});
    engine::addMesh("teapot", new mesh{&meshLoader, "teapot.obj", "unlit"});

    engine.addInitFunction([](class engine* e) {
        e->captureMouse(false);
        // todo: make own camera class
        e->setWorld(new world{e, new freecam{e}});
        e->getWorld()->addMesh("teapot");
        engine::setBackgroundColor(0.9098f, 0.9137f, 0.9098f, 1.0f);

        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->Clear();
        ImFont* font = io.Fonts->AddFontFromFileTTF(virtualFileSystem::getGenericPath("fonts/selawik/selawk.ttf").c_str(), 18.0f);
        if (font != nullptr) {
            io.FontDefault = font;
        } else {
            io.Fonts->AddFontDefault();
        }
        io.Fonts->Build();
    });
    engine.init();

    engine.addRenderFunction([](class engine* e) {
        setupImGuiStyle();
        renderMenuBar(e);
    });
    engine.run();
    return 0;
}
