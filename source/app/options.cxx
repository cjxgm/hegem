#include "../tool/journal.hxx"
#include "../tool/invoke.hxx"
#include "../tool/filesystem.hxx"
#include "../swing/example.hxx"
#include "options.hxx"
#include <vector>
#include <algorithm>
#include <iterator>

namespace hegem::app
{
    inline namespace
    {
        using tool::journal;

        journal j() { return {"OPT"}; }

        auto populate_scenes(options& opts) -> void
        {
            j() << "scanning scenes\n";
            tool::thunk_invoke(
                &tool::search_folder,
                tool::string::borrow_owner(&opts.scene_basedir),
                [&] (tool::string filename, tool::file_kind kind) -> bool {
                    if (kind == tool::file_kind::plain) {
                        auto path = opts.scene_basedir;  // Copy intentionally.
                        path += '/';
                        path.append(filename.data, filename.length);
                        j() << "discovered scene " << path << "\n";
                        opts.scenes.emplace_back(std::move(path));
                    }
                    return false;
                }
            );
        }
    }

    options::options(int argc, char const* argv[])
    {
        if (argc > 1) scene_basedir = argv[1];

        j() << "making example scene\n";
        scenes.emplace_back(swing::make_example_scene());
        populate_scenes(*this);
    }
}

