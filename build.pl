#!/usr/bin/env perl
use utf8;

my %opt;
@opt{@ARGV} = ();
$::sanitize = exists $opt{"-s"};

$base_dir__source = "source";

@include_dir__third_party = (
    'sanity-check',             # compiler sanity checks
    'gsl/include',              # guideline support library
    'glm',                      # opengl mathematics
    'stb',                      # single header libraries for game-dev
    'tinyobjloader',            # single header library for loading wavefront obj file
    'variant/include',          # variant, optional, and reference wrapper. The reference wrapper is a must, and that's why we don't use std::variant.
    'earcut/include',           # polygon triangulation
    'cxxpool/src',              # thread pool
    'cpptoml/include',          # TOML parser
);
@library__pkg_config = qw{
    libsystemd
    glfw3
    gl
};

$compiler__bin = "clang++";
$loader__bin = $compiler__bin;
$build__output_bin = "hegem";
$build__main_impl = "main.cxx";

&output__variable__bool('::sanitize');
@compiler__sanitizers = qw[undefined address] if $::sanitize;
$compiler__flags__standard = "c++17";
$compiler__flags__optimization_level = "0" if exists $opt{"-g"};
$compiler__flags__architecture = "ivybridge";
@compiler__flags__extra = qw[-Wall -Wextra -Wno-missing-field-initializers -Wno-unused-parameter -ggdb -Wno-missing-braces];
@loader__flags__extra = qw[-pthread -lstdc++fs -ggdb];    # FIXME: feature detection on "libstdc++fs" / C++17 filesystem

$makefile__show_commands = exists $opt{"-c"};
@makefile__commands__test = (
    {
        name => '$(BIN)',
        commands => [
            '$(BIN) support/inputs',
        ],
    },
);

