#!/usr/bin/env perl
use utf8;

@include_dir__third_party = (
    'sanity-check',             # compiler sanity checks
    'gsl/include',              # guideline support library
    'glm',                      # opengl mathematics
    'stb',                      # single header libraries for game-dev
    'cxxopts/include',          # command-line argument parser
    'variant/include',          # variant and optional
);
@library__pkg_config = qw{
};

$compiler__flags__standard = "c++1z";
$build__output_bin = "raytracer";

my $size = [200, 200];
my @inputs = (
    # [ input-name, size, depth-range ]
    #   size = [width, height]
    #   depth-range = [min, max]
    [ 'scene1_01', $size, [9, 10] ],
    [ 'scene1_02', $size, [8, 12] ],
    [ 'scene1_03', $size, [8, 12] ],
    [ 'scene1_04', $size, [12, 17] ],
    [ 'scene1_05', $size, [14.5, 19.5] ],
    [ 'scene1_06', $size, [3, 7] ],
    [ 'scene1_07', $size, [-2, 2] ],
);
@makefile__commands__test = (
    (map {
        my ($name, $size, $depth_range) = @$_;
        my ($width, $height) = @$size;
        my ($depth_min, $depth_max) = @$depth_range;

        my $diffuse_output = "\$(BUILD)/output-$name-${width}x$height-diffuse.png";
        my $depth_output = "\$(BUILD)/output-$name-${width}x$height-depth.png";
        {
            name => $name,
            commands => [
                "\$(BIN) --input support/inputs/$name.txt --width $width --height $height --diffuse $diffuse_output --depth $depth_output --depth-min $depth_min --depth-max $depth_max",
            ],
        }
    } @inputs),
    {
        name => "display result",
        commands => [
            ("feh " . join " ", map {
                my ($name, $size, $depth_range) = @$_;
                my ($width, $height) = @$size;

                my $diffuse_output = "\$(BUILD)/output-$name-${width}x$height-diffuse.png";
                my $depth_output = "\$(BUILD)/output-$name-${width}x$height-depth.png";
                ($diffuse_output, $depth_output)
            } @inputs),
        ],
    },
);

