# Objective

- Remove discrete small allocations in the scene and BVH representation.
  - Remove mapbox::variant by the way.
  - Remove scene object level polymorphism. Instead, make systems/containers polymorphic.
  - More flat structures, improve memory performance (both speed and size).
  - Arenas.

# Modernize codebase

- Now that we have C++20, we can start
  - removing template headings and just auto in the parameter
  - requires () fun!
- We should enable -fno-exceptions and -fno-rtti.
- Should also check if makepkg builds with the 2026 toolchain.

Maybe later:
- Update Dear IMGUI to the latest version.

# Task Cancellation

- Not just cancel not-yet-started jobs, but also stops running jobs.
  - There are limits in this as the old system cannot manage lifetimes properly.
- New task system, new channels design and internalize thread pool (remove cxxpool).

# Pathtracing Noise

Even though we have multiple importance sampling, it's still quite noisy.
We hate machine-learning based methods, so no denoisers.

- There could be numerical errors in ray intersection (or computation of next ray sample), we have refraction rays for passing-through-out-of-object-surface ray samples, when the scene is completely opaque.
- ReSTIR or techniques of even more bleeding edge.

# Fat pointers and new Execution Engine

- New way to write X-macros.
- Do not use std::any. Use a fat pointer (hegem::any), stored in the arena of current execution.
- The sk nodes should be considered meta programs, that generate "Program". Each editable program has its own arena. The runtime environment is a separate arena.
- At least, pool + coarse events (e.g. actively broadcast object deletion intentions by the one who triggers the deletion. never reactively by the pool).
- Maybe we need chunky collections, i.e. tables, arrays (pools), queues, stacks that are based on non-contiguous chunks of items. They can reside in arenas.

# Silo

- Support saving project.
  - Maybe inside the skein files? And rename them as "Hegemonic Multimedia Pack (*.hmp)", which should be the Jim format.
  - Should probably also convert all scene into skein nodes to make everything fit in the same format.
- Support export animation.
- Support toggle gizmo visibility.

# Jim

Coined from "Just Ini but also binary Marshalling".

- Flat ini. No trees. Only sections of key-value pairs.
- Stream of primitives. Not data structures. It also means, orders matter, and duplicated keys (and section names) will be everywhere.
- Comments. Parsers should spit out comments to the callers. Comments are not just comments but may be verifiable if desired. Commemts can be considered as some kind of empty-keyed values with a different syntax.
- Binary format. That is, strict as hell. Whitespaces (and their numbers) are strict. Texts are encoded with UTF-8 into byte sequence. May contain literal raw bytes.
- Zero-copy escaping. Unescaping can be done without copying or modifying the original data.
- Slightly human readable. You can understand most things but just not raw bytes, which, even if encoded as texts, are not understandable anyway.
- Human editable if it contains no raw bytes.
- Keys and section names have strict format requirements, so that you store arbitrary data in values, not in keys or section names. And yes, this encourages duplicate keys and section names.
- Blob is the only value type.
- Large blobs can be skipped in O(1) without reading or scanning them.
- Won't be too ugly or too misleading, even if interleaved with raw bytes.
- Round-trip exact. A jim data goes into the jim parser and then dumps out, is bit-by-bit exact as the original.

# C++20

We want (at least) "auto" in parameters and guaranteed copy elision, and ~std::string.starts_with()~ just use tool::string.starts_with(). tool::string needs arena to better serve a complete circle.

Mapbox variant is showing its age.
- The replacement is hegem::any, a fat pointer `struct any { void* pointer; u16 base_type; u8 indirection_count; ...? };`
- Which in turn, requires hegem::arena, a chain of buckets. (And shall design the entire Chunky Collections Initiative collections, checkpoints, pools, guards (?), et al).
- The any also requires a type registry. We may do "struct type_serial<Compiler_String identity, typename Type> {};" or? This should really be static data. I don't want a global (or singleton) object called Registry. Do not rely on ODR (should work across arbitrary DSO boundary). Write an introspection compiler?

# Virtual Filesystem

- Either support more locations to open/locate stock assets (so that dev and packages can all locate their data) or support virtual filesystems (where config files is able to "mount" directories, then we can have static code + dynamic config on where to find stock data).
- In the future, we may support mounting zip files.
- Path canonicalization and things like that. So that path are always separated by "/" and there are no "drives". "/" are not leading sigils for each segment but separators. "/foo" should be considered empty "/" foo, where "empty" represents root.

# Experimenting Innovative Graphics API Design

- An RHI-free API design, inversed control: The backend expose only a single "execute(Command_Buffer commands) -> void" asynchronous function.
- Or, the [No Graphics](https://www.sebastianaaltonen.com/blog/no-graphics-api)-style API?

# Skein-centered Unified Experience

- Automatically convert scene description into sk nodes?
- Hegem should work just like Neovim: You see nothing but the text area. There is no sidebars, no terminals, no tabs, no menus. Just the text with a status bar, fully utilizing your screen real-estate. You can dynamically split into multiple areas. Splits live INSIDE tabs (unlike those so called "Modern" editors, where tabs live inside splits). But 1 important difference: Hegem must be mouse-centered. If something cannot be done by solely the mouse, it should not exist.
- Not so many dialogs. Just different kind of "buffers" (as in Neovim).

# Alternative Experience Unification

- Instead of unifying experience around sk, how about unifying around "notebooks"? e.g. Jupyter Notebook.

