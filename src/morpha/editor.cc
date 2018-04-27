#include "editor.hh"

namespace rt::morpha
{
    struct editor::temporary_state
    {
    };

    editor::editor(): tmp{std::make_unique<temporary_state>()} {}
    editor::~editor() = default;

    void editor::draw()
    {
    }
}

