#!/usr/bin/env perl
use utf8;

@include_dir__third_party = (
    'sanity-check',             # compiler sanity checks
    'gsl/include',              # guideline support library
    'glm',                      # opengl mathematics
    'stb',                      # single header libraries for game-dev
    'variant/include',          # variant and optional
    'observer-ptr/include',     # observer_ptr<T>
    'cxxpool/src',              # thread pool
);
@library__pkg_config = qw{
    glfw3
    gl
};

$compiler__bin = "clang++";
$loader__bin = $compiler__bin;
$build__output_bin = "raytracer";

$compiler__flags__standard = "c++1z";
@compiler__flags__extra = qw[-g];
@loader__flags__extra = qw[-pthread -lstdc++fs -g];    # FIXME: feature detection on "libstdc++fs" / C++17 filesystem

@makefile__commands__test = (
    {
        name => '$(BIN)',
        commands => [
            '$(BIN) support/inputs',
        ],
    },
);

