#!/usr/bin/env perl
use utf8;

@include_dir__third_party = (
    'sanity-check',             # compiler sanity checks
    'gsl/include',              # guideline support library
    'glm',                      # opengl mathematics
    'stb',                      # single header libraries for game-dev
    'cxxopts/include',          # command-line argument parser
    'variant/include',          # variant and optional
    'observer-ptr/include',     # observer_ptr<T>
);
@library__pkg_config = qw{
    glfw3
    gl
};

$compiler__flags__standard = "c++1z";
$build__output_bin = "raytracer";
@loader__flags__extra = qw[-pthread];

my $size = [16*60, 9*60];
my @inputs = (
    [ 'scene-phong-night', $size ],
    [ 'scene-pbr-night', $size ],
    [ 'scene-phong-day', $size ],
    [ 'scene-pbr-day', $size ],
    [ 'scene-sunny', $size ],
    [ 'scene-triple-moon', $size ],
);
@makefile__commands__test = (
    (map {
        my ($name, $size) = @$_;
        my ($width, $height) = @$size;

        my $output = "\$(BUILD)/output-$name-${width}x$height.png";
        {
            name => $name,
            commands => [
                "\$(BIN) --input support/inputs/$name.txt --width $width --height $height --output $output",
            ],
        }
    } @inputs),
    {
        name => "display result",
        commands => [
            ("feh " . join " ", map {
                my ($name, $size) = @$_;
                my ($width, $height) = @$size;

                my $output = "\$(BUILD)/output-$name-${width}x$height.png";
                ($output);
            } @inputs),
        ],
    },
);

