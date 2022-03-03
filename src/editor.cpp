#include <string>
#include <core/engine.h>
#ifdef DEBUG
#include <input/inputManager.h>
#endif
#include <resource/provider/filesystemResourceProvider.h>
#include <resource/provider/vpkResourceProvider.h>
#include <hook/discordRPC.h>
#include <i18n/translationManager.h>
#include <entity/camera/editorCamera.h>
#include <entity/model/mesh.h>
#include <entity/logic/customEntity.h>
#include <hook/steamAPI.h>
#include <utility/dialogs.h>
#include <resource/fontResource.h>

// necessary to register in material factory
#include <render/materialVTF.h>

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
                Engine::getWindow()->shouldStopAfterThisFrame();
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
    Engine::preInit("settings_puzzlemakerpp.json");
    Resource::addResourceProvider(new FilesystemResourceProvider{"editor"});
    TranslationManager::addTranslationFile("file://i18n/editor");
    TranslationManager::addUniversalFile("file://i18n/editor");

    DiscordRPC::init(TR("editor.discord.application_id"));
    DiscordRPC::setLargeImage("main_logo");
    DiscordRPC::setDetails("https://discord.gg/ASgHFkX");
#ifdef DEBUG
    DiscordRPC::setState(TR("ui.discord.debug_build"));
#else
    DiscordRPC::setState(TR("ui.discord.release_build"));
#endif

    // We are totally P2CE
    Engine::getSettingsLoader()->setValue("engine", "steamworks", true, true, true);
    SteamAPI::generateAppIDFile(440000);
    if (!SteamAPI::Client::initSteam()) {
        dialogPopupError(TR("error.editor.steam_not_running"));
        return EXIT_FAILURE;
    }

#ifdef DEBUG
    InputManager::addCallback(InputKeyButton{Key::ESCAPE, InputKeyEventType::PRESSED, []{
        Engine::getWindow()->shouldStopAfterThisFrame();
    }});
#endif

    // Need to do this after steam
    // todo: load these from p2ce's gameinfo.txt
    Resource::addResourceProvider(new VPKResourceProvider{620, "portal2/pak01_dir.vpk"});
    Resource::addResourceProvider(VPKResourceProvider::getFilesystemResourceProvider(620, "portal2"));
    Resource::addResourceProvider(new VPKResourceProvider{620, "portal2_dlc1/pak01_dir.vpk"});
    Resource::addResourceProvider(VPKResourceProvider::getFilesystemResourceProvider(620, "portal2_dlc1"));
    Resource::addResourceProvider(new VPKResourceProvider{620, "portal2_dlc2/pak01_dir.vpk"});
    Resource::addResourceProvider(VPKResourceProvider::getFilesystemResourceProvider(620, "portal2_dlc2"));
    Resource::addResourceProvider(VPKResourceProvider::getFilesystemResourceProvider(440000, "p2ce"));

    Engine::init([]{
        Engine::getWindow()->setBackgroundColor({0.9098f, 0.9137f, 0.9098f});

        auto camera = new EditorCamera{CameraProjectionMode::PERSPECTIVE};
        Engine::getWindow()->addChild(camera);
        Engine::getWindow()->setCamera(camera);
        EditorCamera::setupKeybinds();

        Engine::getWindow()->addChild(new Mesh{"file://meshes/teapot.json"});

        auto selawk = Resource::getResource<FontResource>("file://fonts/default.json");
        ImGui::GetIO().FontDefault = selawk->getFont();

        Engine::getWindow()->addChild(new CustomEntity{[](glm::mat4){
            setupImGuiStyle();
            // todo: add dock space that respects top bar
            renderMenuBar();
        }});
    });
    Engine::run();
}
