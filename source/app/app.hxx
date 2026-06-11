#include "../util/task-manager.hxx"
#include "options.hxx"
#include <vector>

namespace hegem::app
{
    using task_type = util::task_type<util::possibly_canceled_job>;
    using task_group_type = std::vector<task_type>;

    void run_once(options opts);
    auto schedule_tasks(task_group_type tasks) -> util::task_io;
}

