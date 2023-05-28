#pragma once

#include <unordered_map>
#include <unordered_set>

#include "Mod.hpp"

namespace vrmod {
class CameraDuplicator : public Mod::Component {
public:
    void on_pre_application_entry(void* entry, const char* name, size_t hash) override;
    void on_application_entry(void* entry, const char* name, size_t hash) override;
    void on_draw_ui() override;

private:
    void clone_camera();
    void find_new_camera();
    void copy_camera_properties();

    sdk::SystemArray* get_all_cameras();

    bool m_copy_camera{true};
    bool m_called_activate{false};
    RECamera* m_old_camera{nullptr};
    RECamera* m_new_camera{nullptr};
    
    struct GetterSetter {
        sdk::REMethodDefinition* getter{nullptr};
        sdk::REMethodDefinition* setter{nullptr};
    };

    std::unordered_map<sdk::RETypeDefinition*, std::unordered_map<std::string, GetterSetter>> m_getter_setters{};

    struct WantedComponent {
        std::string name{};
        std::unordered_set<size_t> ignored_properties{};
        bool allowed{true}; // used for debug imgui checkbox
        bool immediate{false}; // used to set which property must be set every frame
    };

    // Components that we want to copy from the old camera to the new camera
    std::vector<WantedComponent> m_wanted_components {
        
        /* the first three components have manual properties setting every frame, so the setters/getters must be immediate */
        WantedComponent{ "via.render.ToneMapping", { "set_InjectingGameObject"_fnv }, true, true }, 
        WantedComponent{ "via.render.LDRPostProcess", {}, true, true },
        WantedComponent{ "via.render.SoftBloom", {}, true, true },

        WantedComponent{"via.render.Outline", {}, true, true},
        WantedComponent{ "via.render.Retrofilm", {}, true, true},
        WantedComponent{"via.render.SSRControl", {}, true, true},
        WantedComponent{ "via.render.SSAOControl", {}, true, true },
        WantedComponent{"via.render.SSSSSControl", {}, true, true},
        WantedComponent{ "via.render.DepthOfField", {}, true, true },
        WantedComponent{"via.render.LightShaftFilterControl", {}, true, true},
        WantedComponent{ "via.render.MotionBlur", {}, true, true },
        WantedComponent{ "via.render.FFTBloom", {}, true, true },
        WantedComponent{ "via.render.Echo", {}, true, true },
        WantedComponent{ "via.render.OverlayFXAA", {}, true, true },
        WantedComponent{ "via.render.TessellationFactor", {}, true, true },
        WantedComponent{ "via.render.ShadowQualityControl", {}, true, true},
        WantedComponent{ "via.render.Fog", { "set_FSSunPosGameObject"_fnv }, true, true },
        WantedComponent{ "via.render.VolumetricFog", { }, true, true },
        WantedComponent{ "via.render.VolumetricFogControl", { }, true, true },
        WantedComponent{ "via.render.GodRay", { }, true, true },
        WantedComponent{ "via.render.GeometryAOControl", { }, true, true },

        WantedComponent{ "via.render.FakeLensflare", { }, true, true },
        //WantedComponent{ "via.render.CustomFilter", { } },
    };

    std::vector<std::function<void()>> m_property_jobs{};
    size_t m_property_jobs_index{0};
};
}