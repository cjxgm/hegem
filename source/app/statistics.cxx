#include "../lib/imgui.hxx"
#include "../global/counter.hxx"
#include "statistics.hxx"

namespace hegem::app::view
{
    inline namespace
    {
        using global::counter;
    }

    auto statistics(char const* id) -> void
    {
        ImGui::Columns(4, id);
        if (ImGui::Button("Reset")) counter.reset();
        ImGui::NextColumn();
        ImGui::Text("Total"); ImGui::NextColumn();
        ImGui::Text("Per Pixel"); ImGui::NextColumn();
        ImGui::Text("Per Ray"); ImGui::NextColumn();
        ImGui::Separator();

        auto pixel_count = counter.pixel.load();
        auto ray_count = counter.ray.load();

        auto row = [pixel_count, ray_count] (char const* name, auto& atomic_count, char const* tip={}) {
            auto count = atomic_count.load();
            if (count < 0) {
                count = 0;
                atomic_count = 0;
            }

            ImGui::PushID(name);

            ImGui::Text("%s", name);
            if (tip && ImGui::IsItemHovered())
                ImGui::SetTooltip("%s", tip);
            ImGui::NextColumn();

            ImGui::Text("%ld", count);
            ImGui::NextColumn();

            if (pixel_count == 0) ImGui::Text("N/A");
            else ImGui::Text("%.2f", float(count) / pixel_count);
            ImGui::NextColumn();

            if (ray_count == 0) ImGui::Text("N/A");
            else ImGui::Text("%.2f", float(count) / ray_count);
            ImGui::NextColumn();

            ImGui::PopID();
        };

        row("Pixels", counter.pixel, "Number of pixels raytraced");
        row("Rays", counter.ray, "Number of rays produced");
        row("Rays Refl", counter.ray_refl, "Number of reflection rays produced");
        row("Rays Refr", counter.ray_refr, "Number of refraction rays produced");
        row("Rays Shadow", counter.ray_shadow, "Number of shadow rays produced");
        row("Rays to BVH", counter.ray_bvh_incoming, "Number of rays coming to BVH for intersection tests");
        row("Ix", counter.ix, "Number of intersection tests performed");
        row("Ix BVH", counter.ix_bvh, "Number of intersection tests performed on BVH");
        row("Ix BVH Bounds", counter.ix_bvh_bound, "Number of intersection tests performed on bounds of BVH internal nodes");
        row("Ix BVH Faces", counter.ix_bvh_face, "Number of intersection tests against faces performed on BVH leaf nodes");
        row("Ix Grid", counter.ix_grid, "Number of intersection tests performed on grid");
        row("Ix Grid Faces", counter.ix_grid_face, "Number of intersection tests against faces performed on grid");
    }
}

