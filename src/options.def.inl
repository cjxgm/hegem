// no #pragma once intentionally
// Define a list of command line options with metadata.

#ifndef OPTION
#   error "You must #define OPTION(...) before including this file."
#endif

#ifndef BEGIN_OPTIONS
#   define BEGIN_OPTIONS
#endif

#ifndef END_OPTIONS
#   define END_OPTIONS
#endif

#ifndef MORE_OPTIONS
#   define MORE_OPTIONS
#endif

// OPTION(type, variable_name, short, long, descriptive, default, check, check_args...)
//
// - short:
//      Short form of the command line option.
//      Can be either NO_ABBR or ABBR(x).
//
// - long:
//      Long form of the command line option.
//
// - descriptive:
//      Descriptive name for the command line option.
//
// - default:
//      Default value if user does not specify the argument.
//      Can be either NO_DEFAULT or DEFAULT(string).
//
// - check, args...:
//      Constraint the input. Available checks are:
//
//      * NO_CHECK: Always pass the check.
//      * CHECK_EMPTY: Ensures the string input won't be empty.
//      * CHECK_MIN, min: Ensures the numeric input won't be less than "min".

BEGIN_OPTIONS
OPTION(std::string, input_path         , ABBR(i), input        , "input filename"                  , NO_DEFAULT            , CHECK_EMPTY)   MORE_OPTIONS
OPTION(std::string, output_path        , ABBR(o), output       , "shaded composite output png filename",NO_DEFAULT         , NO_CHECK)      MORE_OPTIONS
OPTION(std::string, diffuse_path       , NO_ABBR, diffuse      , "diffuse map output png filename" , NO_DEFAULT            , NO_CHECK)      MORE_OPTIONS
OPTION(std::string, depth_path         , NO_ABBR, depth        , "depth map output png filename"   , NO_DEFAULT            , NO_CHECK)      MORE_OPTIONS
OPTION(float      , depth_min          , NO_ABBR, depth-min    , "minimum depth value to output"   , DEFAULT("-1")         , NO_CHECK)      MORE_OPTIONS
OPTION(float      , depth_max          , NO_ABBR, depth-max    , "maximum depth value to output"   , DEFAULT("1")          , NO_CHECK)      MORE_OPTIONS
OPTION(int        , max_reflection_count,ABBR(r), reflections  , "maximum number of samples per reflecting point",DEFAULT("4"),CHECK_MIN,1) MORE_OPTIONS
OPTION(int        , max_bounce_count   , ABBR(b), bounces      , "maximum number of bounces"       , DEFAULT("4")          , CHECK_MIN, 0)  MORE_OPTIONS
OPTION(int        , width              , ABBR(W), width        , "width of the output image"       , DEFAULT("256")        , CHECK_MIN, 16) MORE_OPTIONS
OPTION(int        , height             , ABBR(H), height       , "height of the output image"      , DEFAULT("256")        , CHECK_MIN, 16) MORE_OPTIONS
OPTION(int        , anti_aliasing_level, ABBR(a), anti-aliasing, "anti-aliasing level"             , DEFAULT("0")          , CHECK_MIN, 0)
END_OPTIONS

#ifndef NO_UNDEF
#   undef OPTION
#   undef BEGIN_OPTIONS
#   undef END_OPTIONS
#   undef MORE_OPTIONS
#
#   ifdef ABBR
#       undef ABBR
#   endif
#   ifdef DEFAULT
#       undef DEFAULT
#   endif
#   ifdef CHECK_EMPTY
#       undef CHECK_EMPTY
#   endif
#   ifdef CHECK_MIN
#       undef CHECK_MIN
#   endif
#
#   ifdef NO_ABBR
#       undef NO_ABBR
#   endif
#   ifdef NO_DEFAULT
#       undef NO_DEFAULT
#   endif
#   ifdef NO_CHECK
#       undef NO_CHECK
#   endif
#endif

