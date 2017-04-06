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
};

$compiler__flags__standard = "c++1z";
$build__output_bin = "raytracer";

my $size = [16*60, 9*60];
my @inputs = (
    # [ input-name, size, depth-range ]
    #   size = [width, height]
    #   depth-range = [min, max]
    [ 'scene-phong-night', $size, [2, 6] ],
    [ 'scene-pbr-night', $size, [2, 6] ],
);
@makefile__commands__test = (
    (map {
        my ($name, $size, $depth_range) = @$_;
        my ($width, $height) = @$size;
        my ($depth_min, $depth_max) = @$depth_range;

        my $output = "\$(BUILD)/output-$name-${width}x$height.png";
        my $normal_output = "\$(BUILD)/output-$name-${width}x$height-normal.png";
        my $depth_output = "\$(BUILD)/output-$name-${width}x$height-depth.png";
        {
            name => $name,
            commands => [
                "\$(BIN) --input support/inputs/$name.txt --width $width --height $height --output $output --normal $normal_output --depth $depth_output --depth-min $depth_min --depth-max $depth_max",
            ],
        }
    } @inputs),
    {
        name => "display result",
        commands => [
            ("feh " . join " ", map {
                my ($name, $size, $depth_range) = @$_;
                my ($width, $height) = @$size;

                my $output = "\$(BUILD)/output-$name-${width}x$height.png";
                my $normal_output = "\$(BUILD)/output-$name-${width}x$height-normal.png";
                my $depth_output = "\$(BUILD)/output-$name-${width}x$height-depth.png";
                ($output, $normal_output, $depth_output)
            } @inputs),
        ],
    },
);

