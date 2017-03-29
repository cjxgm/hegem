// no #pragma once intentionally

#define GLM_ENABLE_EXPERIMENTAL // in order to use experimental extensions
#define GLM_FORCE_CXX14         // force c++14
#define GLM_FORCE_SIZE_FUNC     // use ".size()" instead of ".length()"
#define GLM_FORCE_RADIANS       // use radians everywhere
#define GLM_FORCE_SWIZZLE       // enable swizzles, e.g. `foo.xyy = bar.rgb`

#ifndef NO_FORWARD
#   include <glm/fwd.hpp>
#endif

#ifdef WANT_EVERYTHING
#   include <glm/glm.hpp>
#else
#   if defined(WANT_ALL) || defined(WANT_VEC) || defined(WANT_VEC2)
#       include <glm/vec2.hpp>
#       include <glm/vector_relational.hpp>
#   endif
#   if defined(WANT_ALL) || defined(WANT_VEC) || defined(WANT_VEC3)
#       include <glm/vec3.hpp>
#       include <glm/vector_relational.hpp>
#   endif
#   if defined(WANT_ALL) || defined(WANT_VEC) || defined(WANT_VEC4)
#       include <glm/vec4.hpp>
#       include <glm/vector_relational.hpp>
#   endif
#   if defined(WANT_ALL) || defined(WANT_MAT) || defined(WANT_MAT2)
#       include <glm/mat2x2.hpp>
#   endif
#   if defined(WANT_ALL) || defined(WANT_MAT) || defined(WANT_MAT3)
#       include <glm/mat3x3.hpp>
#   endif
#   if defined(WANT_ALL) || defined(WANT_MAT) || defined(WANT_MAT4)
#       include <glm/mat4x4.hpp>
#   endif
#   if defined(WANT_ALL) || defined(WANT_OP) || defined(WANT_COMMON_OP)
#       include <glm/common.hpp>
#   endif
#   if defined(WANT_ALL) || defined(WANT_OP) || defined(WANT_GEOM_OP)
#       include <glm/geometric.hpp>
#   endif
#   if defined(WANT_ALL) || defined(WANT_OP) || defined(WANT_EXP_OP)
#       include <glm/exponential.hpp>
#   endif
#   if defined(WANT_ALL) || defined(WANT_OP) || defined(WANT_TRIG_OP)
#       include <glm/trigonometric.hpp>
#   endif
#   if defined(WANT_ALL) || defined(WANT_OP) || defined(WANT_MAT_OP)
#       include <glm/gtc/matrix_transform.hpp>
#   endif
#   if defined(WANT_ALL) || defined(WANT_OP) || defined(WANT_INTERSECT_OP)
#       include <glm/gtx/intersect.hpp>
#   endif
#   if defined(WANT_ALL) || defined(WANT_UTIL) || defined(WANT_RANGE_UTIL)
#       include <glm/gtx/range.hpp>
#   endif
#   if defined(WANT_ALL) || defined(WANT_UTIL) || defined(WANT_STRING_CAST_UTIL)
#       include <glm/gtx/string_cast.hpp>
#   endif
#endif

