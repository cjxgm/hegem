#!/usr/bin/env perl
use utf8;

my %opt;
@opt{@ARGV} = ();
$::sanitize = exists $opt{"-s"};

@include_dir__third_party = (
    'sanity-check',             # compiler sanity checks
    'gsl/include',              # guideline support library
    'glm',                      # opengl mathematics
    'stb',                      # single header libraries for game-dev
    'tinyobjloader',            # single header library for loading wavefront obj file
    'variant/include',          # variant and optional
    'earcut/include',           # polygon triangulation
    'observer-ptr/include',     # observer_ptr<T>
    'cxxpool/src',              # thread pool
    'cpptoml/include',          # TOML parser
);
@library__pkg_config = qw{
    gtkmm-3.0
    glfw3
    gl
};

$compiler__bin = "g++";
$loader__bin = $compiler__bin;
$build__output_bin = "hegem";

&output__variable__bool('::sanitize');
@compiler__sanitizers = qw[undefined address] if $::sanitize;
$compiler__flags__standard = "c++17";
@compiler__flags__extra = qw[-Wall -Wextra -Werror -Wno-missing-field-initializers -Wno-unused-parameter -g];
@loader__flags__extra = qw[-pthread -lstdc++fs -g];    # FIXME: feature detection on "libstdc++fs" / C++17 filesystem

$makefile__show_commands = exists $opt{"-c"};
@makefile__commands__test = (
    {
        name => '$(BIN)',
        commands => [
            '$(BIN) support/inputs',
        ],
    },
);

