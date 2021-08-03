#include <string>
#include "../external/ChiraEngine/src/core/engine.h"
#include "../external/ChiraEngine/src/render/freecam.h"
#include "../external/ChiraEngine/src/loader/objMeshLoader.h"
#include "../external/ChiraEngine/src/render/unlitMaterial.h"
#include "../external/ChiraEngine/src/render/texture2d.h"
#include "loader/vtfImage.h"

int main() {
    engine engine;
    virtualFileSystem::addResourceDirectory("resources/editor/");

    engine.getSettingsLoader()->setValue("engine", "title", std::string("Chira Editor"), true, true);
    objMeshLoader meshLoader;

#if DEBUG
    engine.addKeybind(keybind(GLFW_KEY_ESCAPE, GLFW_PRESS, [](class engine* e) {
        e->stop();
    }));
#endif

    engine::addTexture("teapot", new texture2d{"ui/icon.png", GL_RGBA});
    engine::addShader("unlit", new shader{"unlit.vsh", "unlit.fsh"});
    engine::addMaterial("unlit", new unlitMaterial{"unlit"});
    engine::addMesh("teapot", new mesh{&meshLoader, "teapot.obj", "unlit"});

    engine.addInitFunction([](class engine* e) {
        // todo: make own camera class
        e->setWorld(new world{e, new freecam{e}});
        e->getWorld()->addMesh("teapot");
        engine::setBackgroundColor(0.9098f, 0.9137f, 0.9098f, 1.0f);
        e->captureMouse(false);
    });
    engine.init();

    engine.addRenderFunction([](class engine* e) {
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
    });
    engine.run();
    return 0;
}
