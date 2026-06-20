#include "../tool/task-manager.hxx"
#include "options.hxx"
#include <vector>

namespace hegem::app
{
    using task_type = tool::task_type<tool::possibly_canceled_job>;
    using task_group_type = std::vector<task_type>;

    auto run_once(options opts) -> void;
    auto schedule_tasks(task_group_type tasks) -> tool::task_io;
}

