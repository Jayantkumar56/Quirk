
add_library("imgui" STATIC)

target_sources("imgui" PRIVATE 
    "vendor/imgui/imconfig.h"
    "vendor/imgui/imgui.h"
    "vendor/imgui/imgui.cpp"
    "vendor/imgui/imgui_draw.cpp"
    "vendor/imgui/imgui_internal.h"
    "vendor/imgui/imgui_tables.cpp"
    "vendor/imgui/imgui_widgets.cpp"
    "vendor/imgui/imstb_rectpack.h"
    "vendor/imgui/imstb_textedit.h"
    "vendor/imgui/imstb_truetype.h"
    "vendor/imgui/imgui_demo.cpp"
    "vendor/imgui/misc/cpp/imgui_stdlib.h"
    "vendor/imgui/misc/cpp/imgui_stdlib.cpp"
)

target_include_directories("imgui" PUBLIC "vendor/imgui")

add_compiler_flags_for("imgui")
enable_release_optimizations_for("imgui")
