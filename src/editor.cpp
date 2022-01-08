#include <string>
#include <core/engine.h>
#include <entity/imgui/console/console.h>
#include <resource/provider/filesystemResourceProvider.h>
#include <hook/discordRPC.h>
#include <i18n/translationManager.h>
#include <entity/3d/camera/editorCamera3d.h>
#include <entity/3d/model/mesh3d.h>

#include "render/materialVTF.h" // necessary to register in material factory

using namespace chira;

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
        if (ImGui::BeginMenu(TRC("ui.menubar.file"))) {
            if (ImGui::MenuItem(TRC("ui.menubar.new"))) {
                // load default map
            }
            if (ImGui::MenuItem(TRC("ui.menubar.open"))) {
                // open map open dialog
            }
            if (ImGui::MenuItem(TRC("ui.menubar.save"))) {
                // if it's not saved before open save dialog, else save
            }
            if (ImGui::MenuItem(TRC("ui.menubar.save_as"))) {
                // open map save dialog
            }
            if (ImGui::MenuItem("Settings...")) {
                // open settings window
            }
            if (ImGui::MenuItem(TRC("ui.menubar.exit"))) {
                // Maybe save the map before exiting, or ask to save
                Engine::stop();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(TRC("ui.menubar.edit"))) {
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
    Engine::preInit();
    Resource::addResourceProvider(new FilesystemResourceProvider{"editor"});
    TranslationManager::addTranslationFile("file://i18n/editor");
    TranslationManager::addUniversalFile("file://i18n/editor");
    Engine::getSettingsLoader()->setValue("engine", "title", std::string("Chira Editor"), true, true);

    // Decreases the max number of lights, there's no need for as many as the engine default
    Engine::getSettingsLoader()->setValue("engine", "maxPointLights", 8, false, false);
    Engine::getSettingsLoader()->setValue("engine", "maxDirectionalLights", 1, false, false);
    Engine::getSettingsLoader()->setValue("engine", "maxSpotLights", 1, false, false);

#if DEBUG
    Engine::addKeybind(Keybind(GLFW_KEY_ESCAPE, GLFW_PRESS, []() {
        Engine::stop();
    }));
    Engine::addKeybind(Keybind(GLFW_KEY_GRAVE_ACCENT, GLFW_PRESS, []() {
        Engine::getConsole()->setVisible(!Engine::getConsole()->isVisible());
    }));
#endif

    Engine::addInitFunction([]() {
        DiscordRPC::init(TR("editor.discord.application_id"));
        DiscordRPC::setLargeImage("main_logo");
        DiscordRPC::setDetails("https://discord.gg/ASgHFkX");
#if DEBUG
        DiscordRPC::setState(TR("ui.discord.debug_build"));
#else
        DiscordRPC::setState(TR("ui.discord.release_build"));
#endif
        Engine::setBackgroundColor(ColorRGB{0.9098f, 0.9137f, 0.9098f});

        auto camera = new EditorCamera3d{CameraProjectionMode::PERSPECTIVE};
        Engine::getRoot()->addChild(camera);
        Engine::getRoot()->setMainCamera(camera);

        auto teapot = Resource::getResource<MeshResource>("file://meshes/teapot.json");
        Engine::getRoot()->addChild(new Mesh3d{teapot});

        auto selawk = Resource::getResource<FontResource>("file://fonts/default.json");
        ImGui::GetIO().FontDefault = selawk->getFont();
    });
    Engine::init();

    Engine::addRenderFunction([]() {
        setupImGuiStyle();
        // todo: add dock space that respects top bar
        renderMenuBar();
    });
    Engine::run();
}
