# Hegem: The Hegemonic Multimedia Creator

## Synopsis

Create 3D models via visual scripting, render with pathtracing, morph between images, design particle effects -- Anything about content creation are the mission of the project.

## Pronounciation

Hegem is pronounced like `heh gem`, like in `hegemony`.

## Repository Branches

- `master`: The general bleeding-edge development branch. It constantly breaks and force-pushes all the day.
- `next`: The matured development branch. It contains curated commits from the master branch that is going to be released in the next version. It should never break. It cannot force-push.
- `release`: The set-in-stone branch. It contains commits from previous releases.
- `version/<version>`: A branch for a certain released version.
- `topic/<topic>`: A branch where development on a certain `<topic>` happens.
- `attic/<topic>`: An abandoned branch on development of a certain `<topic>`.

## Build and Run (For Developers)

Checkout a stable branch first. You don't want to resolve bleeding edge issues.

```sh
git worktree add ../hegem-build origin/next
cd ../hegem-build
```

The first-time build requires a manual configure. You need Perl and a C++ compiler.

```sh
./configure
```

Then it can be build via GNU Make.

```sh
make           # Single-threaded build.
make -j        # Multi-threaded build.
make -j test   # Multi-threaded build and run.
```

A `build.flags` file will be generated. It contains canonical build flags for all files.

## Build, Install, and Run (For End-users)

Checkout a stable branch first. Or even better a released branch. You don't want to resolve bleeding edge issues.

```sh
git worktree add ../hegem-build origin/release
cd ../hegem-build
```

For Arch Linux users, you may build and install via makepkg. Other distros are not officially supported.

```sh
cd support/packaging/archlinux
makepkg -si
```

Run hegem from the command line.

```sh
hegem
```

