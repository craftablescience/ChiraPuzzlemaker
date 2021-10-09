#include <string>
#include "../../external/ChiraEngine/src/core/engine.h"
#include "../../external/ChiraEngine/src/render/freecam.h"
#include "../../external/ChiraEngine/src/loader/objMeshLoader.h"
#include "../../external/ChiraEngine/src/render/texture2d.h"
#include "../../external/ChiraEngine/src/resource/filesystemResourceProvider.h"
#include "../render/vtfTexture.h"
#include "../../external/ChiraEngine/src/hook/discordRichPresence.h"
#include "../render/vtfMaterial.h"
#include "../../external/ChiraEngine/src/resource/resourceManager.h"
#include "../../external/ChiraEngine/src/wec/componentManager.h"
#include "../../external/ChiraEngine/src/wec/extensibleWorld.h"
#include "../../external/ChiraEngine/src/wec/propEntity.h"

// https://github.com/ocornut/imgui/issues/707#issuecomment-362574409
void setupImGuiStyle() {
    // todo: integrate this into an abstracted ui later, and use push/pop
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

    style->Colors[ImGuiCol_ScrollbarBg] = header;
    style->Colors[ImGuiCol_ScrollbarGrab] = grab;
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = dark;
    style->Colors[ImGuiCol_ScrollbarGrabActive] = darker;
}

void renderMenuBar() {
    // todo(i18n)
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
                engine::stop();
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
    engine::preInit();
    resourceManager::addResourceProvider("file", new filesystemResourceProvider{"file", "resources/editor"});
    engine::getSettingsLoader()->setValue("engine", "title", std::string("Chira Editor"), true, true);

    // decreases the max number of lights, there's no need for as many as the engine default
    engine::getSettingsLoader()->setValue("engine", "maxPointLights", 8, false, false);
    engine::getSettingsLoader()->setValue("engine", "maxDirectionalLights", 1, false, false);
    engine::getSettingsLoader()->setValue("engine", "maxSpotLights", 1, false, false);

#if DEBUG
    engine::addKeybind(keybind(GLFW_KEY_ESCAPE, GLFW_PRESS, []() {
        engine::stop();
    }));
    engine::addKeybind(keybind(GLFW_KEY_GRAVE_ACCENT, GLFW_PRESS, []() {
        engine::showConsole(!engine::getConsole()->getEnabled());
    }));
#endif

    mesh* teapot;

    engine::addInitFunction([&teapot]() {
        teapot = resourceManager::getResource<mesh>(
                "file://meshes/teapot.json",
                resourceManager::getResource<vtfMaterial>("file://materials/test_vtf_material.json"));

        discordRichPresence::init("875568220315205653");
        discordRichPresence::setLargeImage("main_logo");
        discordRichPresence::setDetails("https://discord.gg/ASgHFkX");
#if DEBUG
        // todo(i18n)
        discordRichPresence::setState("Debug build");
#else
        // todo(i18n)
        discordRichPresence::setState("Release build");
#endif
        uuids::uuid worldId = componentManager::addWorld(new extensibleWorld{
                [](double delta){},
                [](double delta){},
                [](){}
        });
        componentManager::getWorld<extensibleWorld>(worldId)->add(
                (new propEntity())->init(new meshComponent(teapot, glm::vec3{}, glm::vec3{})));

        engine::captureMouse(false);
        // todo: make own camera class
        engine::setMainCamera(new freecam{});
        engine::setBackgroundColor(0.9098f, 0.9137f, 0.9098f, 1.0f);

        // Don't release the fontResource when done to keep it cached
        auto* selawk = resourceManager::getResource<fontResource>("file://fonts/default.json");
        ImGui::GetIO().FontDefault = selawk->getFont();
    });
    engine::init();

    engine::addRenderFunction([&teapot]() {
        teapot->getMaterial()->getShader()->setUniform("p", engine::getMainCamera()->getProjectionMatrix());
        teapot->getMaterial()->getShader()->setUniform("v", engine::getMainCamera()->getViewMatrix());
        // todo: add dock space that respects top bar
        setupImGuiStyle();
        renderMenuBar();
    });
    engine::run();
}
