#include "../lib/std/filesystem.hxx"
#include "../util/journal.hxx"
#include "../swing/example.hxx"
#include "options.hxx"
#include <vector>
#include <algorithm>
#include <iterator>

namespace rt::app
{
    namespace
    {
        using util::journal;
        namespace fs = lib::filesystem;

        journal j() { return {"OPT"}; }

        void populate_scenes(options& opts)
        {
            j() << "scanning scenes\n";
            std::vector<std::string> filenames;
            for (auto& entry: fs::recursive_directory_iterator{opts.scene_basedir}) {
                if (is_regular_file(entry.path())) {
                    j() << "discovered scene " << entry.path().u8string() << "\n";
                    filenames.emplace_back(entry.path().u8string());
                }
            }
            std::sort(begin(filenames), end(filenames));
            std::move(begin(filenames), end(filenames), std::back_inserter(opts.scenes));
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

