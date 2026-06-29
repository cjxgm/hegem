#pragma once

namespace hegem::tool
{
    template <typename Function, typename... Target_Arguments>
    struct thunker;

    template <typename Visitor_Result, typename Target_Result, typename... Visitor_Arguments, typename... Target_Arguments>
    struct thunker<auto (auto (*) (void* context, Visitor_Arguments...) -> Visitor_Result, void* context) -> Target_Result, Target_Arguments...>
    {
        using visitor_type = auto (void* context, Visitor_Arguments...) -> Visitor_Result;
        using target_type = auto (Target_Arguments..., visitor_type* visit, void* context) -> Target_Result;

        template <typename Visitor>
        static auto invoke(target_type* invoke_target, Target_Arguments... target_arguments, Visitor const& visit) -> Target_Result
        {
            constexpr auto thunk_visit = +[] (void* context, Visitor_Arguments... visitor_arguments) -> Visitor_Result {
                auto visit = (Visitor const*) context;
                return visit[0](static_cast<Visitor_Arguments&&>(visitor_arguments)...);
            };
            return invoke_target(static_cast<Target_Arguments>(target_arguments)..., thunk_visit, (void*) &visit);
        }
    };

    template <typename Result, typename Argument1, typename Argument2, typename Argument3, typename... Arguments, typename... Target_Arguments>
    struct thunker<auto (Argument1, Argument2, Argument3, Arguments...) -> Result, Target_Arguments...>
    : thunker<auto (Argument2, Argument3, Arguments...) -> Result, Target_Arguments..., Argument1>
    {};

    // Given using some_callback = auto (void* context, char const*) -> unsigned;
    // and auto foo(int, float, some_callback* some, void* context) -> Result;
    // you may call invoke(&foo, int{}, float{}, [&] (char const*) -> unsigned {});
    template <typename Function, typename... Arguments>
    auto thunk_invoke(Function* target, Arguments&&... arguments) -> decltype(auto)
    {
        return thunker<Function>::invoke(target, static_cast<decltype(arguments)&&>(arguments)...);
    }
}

