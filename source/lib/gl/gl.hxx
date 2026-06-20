#pragma once
#include <cstddef>
#include <cstdint>

namespace gl
{
    namespace enums
    {
        enum enum_type: unsigned int
        {
            alpha                            = 0x1906,
            always                           = 0x0207,
            and_                             = 0x1501,
            and_inverted                     = 0x1504,
            and_reverse                      = 0x1502,
            back                             = 0x0405,
            back_left                        = 0x0402,
            back_right                       = 0x0403,
            blend                            = 0x0be2,
            blend_dst                        = 0x0be0,
            blend_src                        = 0x0be1,
            blue                             = 0x1905,
            byte                             = 0x1400,
            ccw                              = 0x0901,
            clear                            = 0x1500,
            color                            = 0x1800,
            color_buffer_bit                 = 0x00004000,
            color_clear_value                = 0x0c22,
            color_logic_op                   = 0x0bf2,
            color_writemask                  = 0x0c23,
            copy                             = 0x1503,
            copy_inverted                    = 0x150c,
            cull_face                        = 0x0b44,
            cull_face_mode                   = 0x0b45,
            cw                               = 0x0900,
            decr                             = 0x1e03,
            depth                            = 0x1801,
            depth_buffer_bit                 = 0x00000100,
            depth_clear_value                = 0x0b73,
            depth_component                  = 0x1902,
            depth_func                       = 0x0b74,
            depth_range                      = 0x0b70,
            depth_test                       = 0x0b71,
            depth_writemask                  = 0x0b72,
            dither                           = 0x0bd0,
            dont_care                        = 0x1100,
            double_                          = 0x140a,
            doublebuffer                     = 0x0c32,
            draw_buffer                      = 0x0c01,
            dst_alpha                        = 0x0304,
            dst_color                        = 0x0306,
            equal                            = 0x0202,
            equiv                            = 0x1509,
            extensions                       = 0x1f03,
            false_                           = 0,
            fastest                          = 0x1101,
            fill                             = 0x1b02,
            float_                           = 0x1406,
            front                            = 0x0404,
            front_and_back                   = 0x0408,
            front_face                       = 0x0b46,
            front_left                       = 0x0400,
            front_right                      = 0x0401,
            gequal                           = 0x0206,
            greater                          = 0x0204,
            green                            = 0x1904,
            incr                             = 0x1e02,
            int_                             = 0x1404,
            invalid_enum                     = 0x0500,
            invalid_operation                = 0x0502,
            invalid_value                    = 0x0501,
            invert                           = 0x150a,
            keep                             = 0x1e00,
            left                             = 0x0406,
            lequal                           = 0x0203,
            less                             = 0x0201,
            line                             = 0x1b01,
            linear                           = 0x2601,
            linear_mipmap_linear             = 0x2703,
            linear_mipmap_nearest            = 0x2701,
            lines                            = 0x0001,
            line_loop                        = 0x0002,
            line_smooth                      = 0x0b20,
            line_smooth_hint                 = 0x0c52,
            line_strip                       = 0x0003,
            line_width                       = 0x0b21,
            line_width_granularity           = 0x0b23,
            line_width_range                 = 0x0b22,
            logic_op_mode                    = 0x0bf0,
            max_texture_size                 = 0x0d33,
            max_viewport_dims                = 0x0d3a,
            nand                             = 0x150e,
            nearest                          = 0x2600,
            nearest_mipmap_linear            = 0x2702,
            nearest_mipmap_nearest           = 0x2700,
            never                            = 0x0200,
            nicest                           = 0x1102,
            none                             = 0,
            noop                             = 0x1505,
            nor                              = 0x1508,
            notequal                         = 0x0205,
            no_error                         = 0,
            one                              = 1,
            one_minus_dst_alpha              = 0x0305,
            one_minus_dst_color              = 0x0307,
            one_minus_src_alpha              = 0x0303,
            one_minus_src_color              = 0x0301,
            or_                              = 0x1507,
            or_inverted                      = 0x150d,
            or_reverse                       = 0x150b,
            out_of_memory                    = 0x0505,
            pack_alignment                   = 0x0d05,
            pack_lsb_first                   = 0x0d01,
            pack_row_length                  = 0x0d02,
            pack_skip_pixels                 = 0x0d04,
            pack_skip_rows                   = 0x0d03,
            pack_swap_bytes                  = 0x0d00,
            point                            = 0x1b00,
            points                           = 0x0000,
            point_size                       = 0x0b11,
            point_size_granularity           = 0x0b13,
            point_size_range                 = 0x0b12,
            polygon_mode                     = 0x0b40,
            polygon_offset_factor            = 0x8038,
            polygon_offset_fill              = 0x8037,
            polygon_offset_line              = 0x2a02,
            polygon_offset_point             = 0x2a01,
            polygon_offset_units             = 0x2a00,
            polygon_smooth                   = 0x0b41,
            polygon_smooth_hint              = 0x0c53,
            proxy_texture_1d                 = 0x8063,
            proxy_texture_2d                 = 0x8064,
            quads                            = 0x0007,
            r3_g3_b2                         = 0x2a10,
            read_buffer                      = 0x0c02,
            red                              = 0x1903,
            renderer                         = 0x1f01,
            repeat                           = 0x2901,
            replace                          = 0x1e01,
            rgb                              = 0x1907,
            rgb10                            = 0x8052,
            rgb10_a2                         = 0x8059,
            rgb12                            = 0x8053,
            rgb16                            = 0x8054,
            rgb4                             = 0x804f,
            rgb5                             = 0x8050,
            rgb5_a1                          = 0x8057,
            rgb8                             = 0x8051,
            rgba                             = 0x1908,
            rgba12                           = 0x805a,
            rgba16                           = 0x805b,
            rgba2                            = 0x8055,
            rgba4                            = 0x8056,
            rgba8                            = 0x8058,
            right                            = 0x0407,
            scissor_box                      = 0x0c10,
            scissor_test                     = 0x0c11,
            set                              = 0x150f,
            short_                           = 0x1402,
            src_alpha                        = 0x0302,
            src_alpha_saturate               = 0x0308,
            src_color                        = 0x0300,
            stack_overflow                   = 0x0503,
            stack_underflow                  = 0x0504,
            stencil                          = 0x1802,
            stencil_buffer_bit               = 0x00000400,
            stencil_clear_value              = 0x0b91,
            stencil_fail                     = 0x0b94,
            stencil_func                     = 0x0b92,
            stencil_index                    = 0x1901,
            stencil_pass_depth_fail          = 0x0b95,
            stencil_pass_depth_pass          = 0x0b96,
            stencil_ref                      = 0x0b97,
            stencil_test                     = 0x0b90,
            stencil_value_mask               = 0x0b93,
            stencil_writemask                = 0x0b98,
            stereo                           = 0x0c33,
            subpixel_bits                    = 0x0d50,
            texture                          = 0x1702,
            texture_1d                       = 0x0de0,
            texture_2d                       = 0x0de1,
            texture_alpha_size               = 0x805f,
            texture_binding_1d               = 0x8068,
            texture_binding_2d               = 0x8069,
            texture_blue_size                = 0x805e,
            texture_border_color             = 0x1004,
            texture_green_size               = 0x805d,
            texture_height                   = 0x1001,
            texture_internal_format          = 0x1003,
            texture_mag_filter               = 0x2800,
            texture_min_filter               = 0x2801,
            texture_red_size                 = 0x805c,
            texture_width                    = 0x1000,
            texture_wrap_s                   = 0x2802,
            texture_wrap_t                   = 0x2803,
            triangles                        = 0x0004,
            triangle_fan                     = 0x0006,
            triangle_strip                   = 0x0005,
            true_                            = 1,
            unpack_alignment                 = 0x0cf5,
            unpack_lsb_first                 = 0x0cf1,
            unpack_row_length                = 0x0cf2,
            unpack_skip_pixels               = 0x0cf4,
            unpack_skip_rows                 = 0x0cf3,
            unpack_swap_bytes                = 0x0cf0,
            unsigned_byte                    = 0x1401,
            unsigned_int                     = 0x1405,
            unsigned_short                   = 0x1403,
            vendor                           = 0x1f00,
            version                          = 0x1f02,
            vertex_array                     = 0x8074,
            viewport                         = 0x0ba2,
            xor_                             = 0x1506,
            zero                             = 0,

            aliased_line_width_range         = 0x846e,
            bgr                              = 0x80e0,
            bgra                             = 0x80e1,
            clamp_to_edge                    = 0x812f,
            max_3d_texture_size              = 0x8073,
            max_elements_indices             = 0x80e9,
            max_elements_vertices            = 0x80e8,
            pack_image_height                = 0x806c,
            pack_skip_images                 = 0x806b,
            proxy_texture_3d                 = 0x8070,
            smooth_line_width_granularity    = 0x0b23,
            smooth_line_width_range          = 0x0b22,
            smooth_point_size_granularity    = 0x0b13,
            smooth_point_size_range          = 0x0b12,
            texture_3d                       = 0x806f,
            texture_base_level               = 0x813c,
            texture_binding_3d               = 0x806a,
            texture_depth                    = 0x8071,
            texture_max_level                = 0x813d,
            texture_max_lod                  = 0x813b,
            texture_min_lod                  = 0x813a,
            texture_wrap_r                   = 0x8072,
            unpack_image_height              = 0x806e,
            unpack_skip_images               = 0x806d,
            unsigned_byte_2_3_3_rev          = 0x8362,
            unsigned_byte_3_3_2              = 0x8032,
            unsigned_int_10_10_10_2          = 0x8036,
            unsigned_int_2_10_10_10_rev      = 0x8368,
            unsigned_int_8_8_8_8             = 0x8035,
            unsigned_int_8_8_8_8_rev         = 0x8367,
            unsigned_short_1_5_5_5_rev       = 0x8366,
            unsigned_short_4_4_4_4           = 0x8033,
            unsigned_short_4_4_4_4_rev       = 0x8365,
            unsigned_short_5_5_5_1           = 0x8034,
            unsigned_short_5_6_5             = 0x8363,
            unsigned_short_5_6_5_rev         = 0x8364,

            active_texture                   = 0x84e0,
            clamp_to_border                  = 0x812d,
            compressed_rgb                   = 0x84ed,
            compressed_rgba                  = 0x84ee,
            compressed_texture_formats       = 0x86a3,
            max_cube_map_texture_size        = 0x851c,
            multisample                      = 0x809d,
            num_compressed_texture_formats   = 0x86a2,
            proxy_texture_cube_map           = 0x851b,
            samples                          = 0x80a9,
            sample_alpha_to_coverage         = 0x809e,
            sample_alpha_to_one              = 0x809f,
            sample_buffers                   = 0x80a8,
            sample_coverage                  = 0x80a0,
            sample_coverage_invert           = 0x80ab,
            sample_coverage_value            = 0x80aa,
            texture0                         = 0x84c0,
            texture1                         = 0x84c1,
            texture10                        = 0x84ca,
            texture11                        = 0x84cb,
            texture12                        = 0x84cc,
            texture13                        = 0x84cd,
            texture14                        = 0x84ce,
            texture15                        = 0x84cf,
            texture16                        = 0x84d0,
            texture17                        = 0x84d1,
            texture18                        = 0x84d2,
            texture19                        = 0x84d3,
            texture2                         = 0x84c2,
            texture20                        = 0x84d4,
            texture21                        = 0x84d5,
            texture22                        = 0x84d6,
            texture23                        = 0x84d7,
            texture24                        = 0x84d8,
            texture25                        = 0x84d9,
            texture26                        = 0x84da,
            texture27                        = 0x84db,
            texture28                        = 0x84dc,
            texture29                        = 0x84dd,
            texture3                         = 0x84c3,
            texture30                        = 0x84de,
            texture31                        = 0x84df,
            texture4                         = 0x84c4,
            texture5                         = 0x84c5,
            texture6                         = 0x84c6,
            texture7                         = 0x84c7,
            texture8                         = 0x84c8,
            texture9                         = 0x84c9,
            texture_binding_cube_map         = 0x8514,
            texture_compressed               = 0x86a1,
            texture_compressed_image_size    = 0x86a0,
            texture_compression_hint         = 0x84ef,
            texture_cube_map                 = 0x8513,
            texture_cube_map_negative_x      = 0x8516,
            texture_cube_map_negative_y      = 0x8518,
            texture_cube_map_negative_z      = 0x851a,
            texture_cube_map_positive_x      = 0x8515,
            texture_cube_map_positive_y      = 0x8517,
            texture_cube_map_positive_z      = 0x8519,

            blend_color                      = 0x8005,
            blend_dst_alpha                  = 0x80ca,
            blend_dst_rgb                    = 0x80c8,
            blend_src_alpha                  = 0x80cb,
            blend_src_rgb                    = 0x80c9,
            constant_alpha                   = 0x8003,
            constant_color                   = 0x8001,
            decr_wrap                        = 0x8508,
            depth_component16                = 0x81a5,
            depth_component24                = 0x81a6,
            depth_component32                = 0x81a7,
            func_add                         = 0x8006,
            func_reverse_subtract            = 0x800b,
            func_subtract                    = 0x800a,
            incr_wrap                        = 0x8507,
            max                              = 0x8008,
            max_texture_lod_bias             = 0x84fd,
            min                              = 0x8007,
            mirrored_repeat                  = 0x8370,
            one_minus_constant_alpha         = 0x8004,
            one_minus_constant_color         = 0x8002,
            point_fade_threshold_size        = 0x8128,
            texture_compare_func             = 0x884d,
            texture_compare_mode             = 0x884c,
            texture_depth_size               = 0x884a,
            texture_lod_bias                 = 0x8501,

            array_buffer                     = 0x8892,
            array_buffer_binding             = 0x8894,
            buffer_access                    = 0x88bb,
            buffer_mapped                    = 0x88bc,
            buffer_map_pointer               = 0x88bd,
            buffer_size                      = 0x8764,
            buffer_usage                     = 0x8765,
            current_query                    = 0x8865,
            dynamic_copy                     = 0x88ea,
            dynamic_draw                     = 0x88e8,
            dynamic_read                     = 0x88e9,
            element_array_buffer             = 0x8893,
            element_array_buffer_binding     = 0x8895,
            query_counter_bits               = 0x8864,
            query_result                     = 0x8866,
            query_result_available           = 0x8867,
            read_only                        = 0x88b8,
            read_write                       = 0x88ba,
            samples_passed                   = 0x8914,
            src1_alpha                       = 0x8589,
            static_copy                      = 0x88e6,
            static_draw                      = 0x88e4,
            static_read                      = 0x88e5,
            stream_copy                      = 0x88e2,
            stream_draw                      = 0x88e0,
            stream_read                      = 0x88e1,
            vertex_attrib_array_buffer_binding = 0x889f,
            write_only                       = 0x88b9,

            active_attributes                = 0x8b89,
            active_attribute_max_length      = 0x8b8a,
            active_uniforms                  = 0x8b86,
            active_uniform_max_length        = 0x8b87,
            attached_shaders                 = 0x8b85,
            blend_equation_alpha             = 0x883d,
            blend_equation_rgb               = 0x8009,
            bool_                            = 0x8b56,
            bool_vec2                        = 0x8b57,
            bool_vec3                        = 0x8b58,
            bool_vec4                        = 0x8b59,
            compile_status                   = 0x8b81,
            current_program                  = 0x8b8d,
            current_vertex_attrib            = 0x8626,
            delete_status                    = 0x8b80,
            draw_buffer0                     = 0x8825,
            draw_buffer1                     = 0x8826,
            draw_buffer10                    = 0x882f,
            draw_buffer11                    = 0x8830,
            draw_buffer12                    = 0x8831,
            draw_buffer13                    = 0x8832,
            draw_buffer14                    = 0x8833,
            draw_buffer15                    = 0x8834,
            draw_buffer2                     = 0x8827,
            draw_buffer3                     = 0x8828,
            draw_buffer4                     = 0x8829,
            draw_buffer5                     = 0x882a,
            draw_buffer6                     = 0x882b,
            draw_buffer7                     = 0x882c,
            draw_buffer8                     = 0x882d,
            draw_buffer9                     = 0x882e,
            float_mat2                       = 0x8b5a,
            float_mat3                       = 0x8b5b,
            float_mat4                       = 0x8b5c,
            float_vec2                       = 0x8b50,
            float_vec3                       = 0x8b51,
            float_vec4                       = 0x8b52,
            fragment_shader                  = 0x8b30,
            fragment_shader_derivative_hint  = 0x8b8b,
            info_log_length                  = 0x8b84,
            int_vec2                         = 0x8b53,
            int_vec3                         = 0x8b54,
            int_vec4                         = 0x8b55,
            link_status                      = 0x8b82,
            lower_left                       = 0x8ca1,
            max_combined_texture_image_units = 0x8b4d,
            max_draw_buffers                 = 0x8824,
            max_fragment_uniform_components  = 0x8b49,
            max_texture_image_units          = 0x8872,
            max_varying_floats               = 0x8b4b,
            max_vertex_attribs               = 0x8869,
            max_vertex_texture_image_units   = 0x8b4c,
            max_vertex_uniform_components    = 0x8b4a,
            point_sprite_coord_origin        = 0x8ca0,
            sampler_1d                       = 0x8b5d,
            sampler_1d_shadow                = 0x8b61,
            sampler_2d                       = 0x8b5e,
            sampler_2d_shadow                = 0x8b62,
            sampler_3d                       = 0x8b5f,
            sampler_cube                     = 0x8b60,
            shader_source_length             = 0x8b88,
            shader_type                      = 0x8b4f,
            shading_language_version         = 0x8b8c,
            stencil_back_fail                = 0x8801,
            stencil_back_func                = 0x8800,
            stencil_back_pass_depth_fail     = 0x8802,
            stencil_back_pass_depth_pass     = 0x8803,
            stencil_back_ref                 = 0x8ca3,
            stencil_back_value_mask          = 0x8ca4,
            stencil_back_writemask           = 0x8ca5,
            upper_left                       = 0x8ca2,
            validate_status                  = 0x8b83,
            vertex_attrib_array_enabled      = 0x8622,
            vertex_attrib_array_normalized   = 0x886a,
            vertex_attrib_array_pointer      = 0x8645,
            vertex_attrib_array_size         = 0x8623,
            vertex_attrib_array_stride       = 0x8624,
            vertex_attrib_array_type         = 0x8625,
            vertex_program_point_size        = 0x8642,
            vertex_shader                    = 0x8b31,

            compressed_srgb                  = 0x8c48,
            compressed_srgb_alpha            = 0x8c49,
            float_mat2x3                     = 0x8b65,
            float_mat2x4                     = 0x8b66,
            float_mat3x2                     = 0x8b67,
            float_mat3x4                     = 0x8b68,
            float_mat4x2                     = 0x8b69,
            float_mat4x3                     = 0x8b6a,
            pixel_pack_buffer                = 0x88eb,
            pixel_pack_buffer_binding        = 0x88ed,
            pixel_unpack_buffer              = 0x88ec,
            pixel_unpack_buffer_binding      = 0x88ef,
            srgb                             = 0x8c40,
            srgb8                            = 0x8c41,
            srgb8_alpha8                     = 0x8c43,
            srgb_alpha                       = 0x8c42,

            bgra_integer                     = 0x8d9b,
            bgr_integer                      = 0x8d9a,
            blue_integer                     = 0x8d96,
            buffer_access_flags              = 0x911f,
            buffer_map_length                = 0x9120,
            buffer_map_offset                = 0x9121,
            clamp_read_color                 = 0x891c,
            clip_distance0                   = 0x3000,
            clip_distance1                   = 0x3001,
            clip_distance2                   = 0x3002,
            clip_distance3                   = 0x3003,
            clip_distance4                   = 0x3004,
            clip_distance5                   = 0x3005,
            clip_distance6                   = 0x3006,
            clip_distance7                   = 0x3007,
            color_attachment0                = 0x8ce0,
            color_attachment1                = 0x8ce1,
            color_attachment10               = 0x8cea,
            color_attachment11               = 0x8ceb,
            color_attachment12               = 0x8cec,
            color_attachment13               = 0x8ced,
            color_attachment14               = 0x8cee,
            color_attachment15               = 0x8cef,
            color_attachment16               = 0x8cf0,
            color_attachment17               = 0x8cf1,
            color_attachment18               = 0x8cf2,
            color_attachment19               = 0x8cf3,
            color_attachment2                = 0x8ce2,
            color_attachment20               = 0x8cf4,
            color_attachment21               = 0x8cf5,
            color_attachment22               = 0x8cf6,
            color_attachment23               = 0x8cf7,
            color_attachment24               = 0x8cf8,
            color_attachment25               = 0x8cf9,
            color_attachment26               = 0x8cfa,
            color_attachment27               = 0x8cfb,
            color_attachment28               = 0x8cfc,
            color_attachment29               = 0x8cfd,
            color_attachment3                = 0x8ce3,
            color_attachment30               = 0x8cfe,
            color_attachment31               = 0x8cff,
            color_attachment4                = 0x8ce4,
            color_attachment5                = 0x8ce5,
            color_attachment6                = 0x8ce6,
            color_attachment7                = 0x8ce7,
            color_attachment8                = 0x8ce8,
            color_attachment9                = 0x8ce9,
            compare_ref_to_texture           = 0x884e,
            compressed_red                   = 0x8225,
            compressed_red_rgtc1             = 0x8dbb,
            compressed_rg                    = 0x8226,
            compressed_rg_rgtc2              = 0x8dbd,
            compressed_signed_red_rgtc1      = 0x8dbc,
            compressed_signed_rg_rgtc2       = 0x8dbe,
            context_flags                    = 0x821e,
            context_flag_forward_compatible_bit = 0x00000001,
            depth24_stencil8                 = 0x88f0,
            depth32f_stencil8                = 0x8cad,
            depth_attachment                 = 0x8d00,
            depth_component32f               = 0x8cac,
            depth_stencil                    = 0x84f9,
            depth_stencil_attachment         = 0x821a,
            draw_framebuffer                 = 0x8ca9,
            draw_framebuffer_binding         = 0x8ca6,
            fixed_only                       = 0x891d,
            float_32_unsigned_int_24_8_rev   = 0x8dad,
            framebuffer                      = 0x8d40,
            framebuffer_attachment_alpha_size = 0x8215,
            framebuffer_attachment_blue_size = 0x8214,
            framebuffer_attachment_color_encoding = 0x8210,
            framebuffer_attachment_component_type = 0x8211,
            framebuffer_attachment_depth_size = 0x8216,
            framebuffer_attachment_green_size = 0x8213,
            framebuffer_attachment_object_name = 0x8cd1,
            framebuffer_attachment_object_type = 0x8cd0,
            framebuffer_attachment_red_size  = 0x8212,
            framebuffer_attachment_stencil_size = 0x8217,
            framebuffer_attachment_texture_cube_map_face = 0x8cd3,
            framebuffer_attachment_texture_layer = 0x8cd4,
            framebuffer_attachment_texture_level = 0x8cd2,
            framebuffer_binding              = 0x8ca6,
            framebuffer_complete             = 0x8cd5,
            framebuffer_default              = 0x8218,
            framebuffer_incomplete_attachment = 0x8cd6,
            framebuffer_incomplete_draw_buffer = 0x8cdb,
            framebuffer_incomplete_missing_attachment = 0x8cd7,
            framebuffer_incomplete_multisample = 0x8d56,
            framebuffer_incomplete_read_buffer = 0x8cdc,
            framebuffer_srgb                 = 0x8db9,
            framebuffer_undefined            = 0x8219,
            framebuffer_unsupported          = 0x8cdd,
            green_integer                    = 0x8d95,
            half_float                       = 0x140b,
            interleaved_attribs              = 0x8c8c,
            int_sampler_1d                   = 0x8dc9,
            int_sampler_1d_array             = 0x8dce,
            int_sampler_2d                   = 0x8dca,
            int_sampler_2d_array             = 0x8dcf,
            int_sampler_3d                   = 0x8dcb,
            int_sampler_cube                 = 0x8dcc,
            invalid_framebuffer_operation    = 0x0506,
            major_version                    = 0x821b,
            map_flush_explicit_bit           = 0x0010,
            map_invalidate_buffer_bit        = 0x0008,
            map_invalidate_range_bit         = 0x0004,
            map_read_bit                     = 0x0001,
            map_unsynchronized_bit           = 0x0020,
            map_write_bit                    = 0x0002,
            max_array_texture_layers         = 0x88ff,
            max_clip_distances               = 0x0d32,
            max_color_attachments            = 0x8cdf,
            max_program_texel_offset         = 0x8905,
            max_renderbuffer_size            = 0x84e8,
            max_samples                      = 0x8d57,
            max_transform_feedback_interleaved_components = 0x8c8a,
            max_transform_feedback_separate_attribs = 0x8c8b,
            max_transform_feedback_separate_components = 0x8c80,
            max_varying_components           = 0x8b4b,
            minor_version                    = 0x821c,
            min_program_texel_offset         = 0x8904,
            num_extensions                   = 0x821d,
            primitives_generated             = 0x8c87,
            proxy_texture_1d_array           = 0x8c19,
            proxy_texture_2d_array           = 0x8c1b,
            query_by_region_no_wait          = 0x8e16,
            query_by_region_wait             = 0x8e15,
            query_no_wait                    = 0x8e14,
            query_wait                       = 0x8e13,
            r11f_g11f_b10f                   = 0x8c3a,
            r16                              = 0x822a,
            r16f                             = 0x822d,
            r16i                             = 0x8233,
            r16ui                            = 0x8234,
            r32f                             = 0x822e,
            r32i                             = 0x8235,
            r32ui                            = 0x8236,
            r8                               = 0x8229,
            r8i                              = 0x8231,
            r8ui                             = 0x8232,
            rasterizer_discard               = 0x8c89,
            read_framebuffer                 = 0x8ca8,
            read_framebuffer_binding         = 0x8caa,
            red_integer                      = 0x8d94,
            renderbuffer                     = 0x8d41,
            renderbuffer_alpha_size          = 0x8d53,
            renderbuffer_binding             = 0x8ca7,
            renderbuffer_blue_size           = 0x8d52,
            renderbuffer_depth_size          = 0x8d54,
            renderbuffer_green_size          = 0x8d51,
            renderbuffer_height              = 0x8d43,
            renderbuffer_internal_format     = 0x8d44,
            renderbuffer_red_size            = 0x8d50,
            renderbuffer_samples             = 0x8cab,
            renderbuffer_stencil_size        = 0x8d55,
            renderbuffer_width               = 0x8d42,
            rg                               = 0x8227,
            rg16                             = 0x822c,
            rg16f                            = 0x822f,
            rg16i                            = 0x8239,
            rg16ui                           = 0x823a,
            rg32f                            = 0x8230,
            rg32i                            = 0x823b,
            rg32ui                           = 0x823c,
            rg8                              = 0x822b,
            rg8i                             = 0x8237,
            rg8ui                            = 0x8238,
            rgb16f                           = 0x881b,
            rgb16i                           = 0x8d89,
            rgb16ui                          = 0x8d77,
            rgb32f                           = 0x8815,
            rgb32i                           = 0x8d83,
            rgb32ui                          = 0x8d71,
            rgb8i                            = 0x8d8f,
            rgb8ui                           = 0x8d7d,
            rgb9_e5                          = 0x8c3d,
            rgba16f                          = 0x881a,
            rgba16i                          = 0x8d88,
            rgba16ui                         = 0x8d76,
            rgba32f                          = 0x8814,
            rgba32i                          = 0x8d82,
            rgba32ui                         = 0x8d70,
            rgba8i                           = 0x8d8e,
            rgba8ui                          = 0x8d7c,
            rgba_integer                     = 0x8d99,
            rgb_integer                      = 0x8d98,
            rg_integer                       = 0x8228,
            sampler_1d_array                 = 0x8dc0,
            sampler_1d_array_shadow          = 0x8dc3,
            sampler_2d_array                 = 0x8dc1,
            sampler_2d_array_shadow          = 0x8dc4,
            sampler_cube_shadow              = 0x8dc5,
            separate_attribs                 = 0x8c8d,
            stencil_attachment               = 0x8d20,
            stencil_index1                   = 0x8d46,
            stencil_index16                  = 0x8d49,
            stencil_index4                   = 0x8d47,
            stencil_index8                   = 0x8d48,
            texture_1d_array                 = 0x8c18,
            texture_2d_array                 = 0x8c1a,
            texture_alpha_type               = 0x8c13,
            texture_binding_1d_array         = 0x8c1c,
            texture_binding_2d_array         = 0x8c1d,
            texture_blue_type                = 0x8c12,
            texture_depth_type               = 0x8c16,
            texture_green_type               = 0x8c11,
            texture_red_type                 = 0x8c10,
            texture_shared_size              = 0x8c3f,
            texture_stencil_size             = 0x88f1,
            transform_feedback_buffer        = 0x8c8e,
            transform_feedback_buffer_binding = 0x8c8f,
            transform_feedback_buffer_mode   = 0x8c7f,
            transform_feedback_buffer_size   = 0x8c85,
            transform_feedback_buffer_start  = 0x8c84,
            transform_feedback_primitives_written = 0x8c88,
            transform_feedback_varyings      = 0x8c83,
            transform_feedback_varying_max_length = 0x8c76,
            unsigned_int_10f_11f_11f_rev     = 0x8c3b,
            unsigned_int_24_8                = 0x84fa,
            unsigned_int_5_9_9_9_rev         = 0x8c3e,
            unsigned_int_sampler_1d          = 0x8dd1,
            unsigned_int_sampler_1d_array    = 0x8dd6,
            unsigned_int_sampler_2d          = 0x8dd2,
            unsigned_int_sampler_2d_array    = 0x8dd7,
            unsigned_int_sampler_3d          = 0x8dd3,
            unsigned_int_sampler_cube        = 0x8dd4,
            unsigned_int_vec2                = 0x8dc6,
            unsigned_int_vec3                = 0x8dc7,
            unsigned_int_vec4                = 0x8dc8,
            unsigned_normalized              = 0x8c17,
            vertex_array_binding             = 0x85b5,
            vertex_attrib_array_integer      = 0x88fd,

            active_uniform_blocks            = 0x8a36,
            active_uniform_block_max_name_length = 0x8a35,
            copy_read_buffer                 = 0x8f36,
            copy_write_buffer                = 0x8f37,
            int_sampler_2d_rect              = 0x8dcd,
            int_sampler_buffer               = 0x8dd0,
            invalid_index                    = 0xffffffff,
            max_combined_fragment_uniform_components = 0x8a33,
            max_combined_geometry_uniform_components = 0x8a32,
            max_combined_uniform_blocks      = 0x8a2e,
            max_combined_vertex_uniform_components = 0x8a31,
            max_fragment_uniform_blocks      = 0x8a2d,
            max_geometry_uniform_blocks      = 0x8a2c,
            max_rectangle_texture_size       = 0x84f8,
            max_texture_buffer_size          = 0x8c2b,
            max_uniform_block_size           = 0x8a30,
            max_uniform_buffer_bindings      = 0x8a2f,
            max_vertex_uniform_blocks        = 0x8a2b,
            primitive_restart                = 0x8f9d,
            primitive_restart_index          = 0x8f9e,
            proxy_texture_rectangle          = 0x84f7,
            r16_snorm                        = 0x8f98,
            r8_snorm                         = 0x8f94,
            rg16_snorm                       = 0x8f99,
            rg8_snorm                        = 0x8f95,
            rgb16_snorm                      = 0x8f9a,
            rgb8_snorm                       = 0x8f96,
            rgba16_snorm                     = 0x8f9b,
            rgba8_snorm                      = 0x8f97,
            sampler_2d_rect                  = 0x8b63,
            sampler_2d_rect_shadow           = 0x8b64,
            sampler_buffer                   = 0x8dc2,
            signed_normalized                = 0x8f9c,
            texture_binding_buffer           = 0x8c2c,
            texture_binding_rectangle        = 0x84f6,
            texture_buffer                   = 0x8c2a,
            texture_buffer_data_store_binding = 0x8c2d,
            texture_rectangle                = 0x84f5,
            uniform_array_stride             = 0x8a3c,
            uniform_block_active_uniforms    = 0x8a42,
            uniform_block_active_uniform_indices = 0x8a43,
            uniform_block_binding            = 0x8a3f,
            uniform_block_data_size          = 0x8a40,
            uniform_block_index              = 0x8a3a,
            uniform_block_name_length        = 0x8a41,
            uniform_block_referenced_by_fragment_shader = 0x8a46,
            uniform_block_referenced_by_geometry_shader = 0x8a45,
            uniform_block_referenced_by_vertex_shader = 0x8a44,
            uniform_buffer                   = 0x8a11,
            uniform_buffer_binding           = 0x8a28,
            uniform_buffer_offset_alignment  = 0x8a34,
            uniform_buffer_size              = 0x8a2a,
            uniform_buffer_start             = 0x8a29,
            uniform_is_row_major             = 0x8a3e,
            uniform_matrix_stride            = 0x8a3d,
            uniform_name_length              = 0x8a39,
            uniform_offset                   = 0x8a3b,
            uniform_size                     = 0x8a38,
            uniform_type                     = 0x8a37,
            unsigned_int_sampler_2d_rect     = 0x8dd5,
            unsigned_int_sampler_buffer      = 0x8dd8,

            already_signaled                 = 0x911a,
            condition_satisfied              = 0x911c,
            context_compatibility_profile_bit = 0x00000002,
            context_core_profile_bit         = 0x00000001,
            context_profile_mask             = 0x9126,
            depth_clamp                      = 0x864f,
            first_vertex_convention          = 0x8e4d,
            framebuffer_attachment_layered   = 0x8da7,
            framebuffer_incomplete_layer_targets = 0x8da8,
            geometry_input_type              = 0x8917,
            geometry_output_type             = 0x8918,
            geometry_shader                  = 0x8dd9,
            geometry_vertices_out            = 0x8916,
            int_sampler_2d_multisample       = 0x9109,
            int_sampler_2d_multisample_array = 0x910c,
            last_vertex_convention           = 0x8e4e,
            lines_adjacency                  = 0x000a,
            line_strip_adjacency             = 0x000b,
            max_color_texture_samples        = 0x910e,
            max_depth_texture_samples        = 0x910f,
            max_fragment_input_components    = 0x9125,
            max_geometry_input_components    = 0x9123,
            max_geometry_output_components   = 0x9124,
            max_geometry_output_vertices     = 0x8de0,
            max_geometry_texture_image_units = 0x8c29,
            max_geometry_total_output_components = 0x8de1,
            max_geometry_uniform_components  = 0x8ddf,
            max_integer_samples              = 0x9110,
            max_sample_mask_words            = 0x8e59,
            max_server_wait_timeout          = 0x9111,
            max_vertex_output_components     = 0x9122,
            object_type                      = 0x9112,
            program_point_size               = 0x8642,
            provoking_vertex                 = 0x8e4f,
            proxy_texture_2d_multisample     = 0x9101,
            proxy_texture_2d_multisample_array = 0x9103,
            quads_follow_provoking_vertex_convention = 0x8e4c,
            sampler_2d_multisample           = 0x9108,
            sampler_2d_multisample_array     = 0x910b,
            sample_mask                      = 0x8e51,
            sample_mask_value                = 0x8e52,
            sample_position                  = 0x8e50,
            signaled                         = 0x9119,
            sync_condition                   = 0x9113,
            sync_fence                       = 0x9116,
            sync_flags                       = 0x9115,
            sync_flush_commands_bit          = 0x00000001,
            sync_gpu_commands_complete       = 0x9117,
            sync_status                      = 0x9114,
            texture_2d_multisample           = 0x9100,
            texture_2d_multisample_array     = 0x9102,
            texture_binding_2d_multisample   = 0x9104,
            texture_binding_2d_multisample_array = 0x9105,
            texture_cube_map_seamless        = 0x884f,
            texture_fixed_sample_locations   = 0x9107,
            texture_samples                  = 0x9106,
            timeout_expired                  = 0x911b,
            triangles_adjacency              = 0x000c,
            triangle_strip_adjacency         = 0x000d,
            unsignaled                       = 0x9118,
            unsigned_int_sampler_2d_multisample = 0x910a,
            unsigned_int_sampler_2d_multisample_array = 0x910d,
            wait_failed                      = 0x911d,

            any_samples_passed               = 0x8c2f,
            int_2_10_10_10_rev               = 0x8d9f,
            max_dual_source_draw_buffers     = 0x88fc,
            one_minus_src1_alpha             = 0x88fb,
            one_minus_src1_color             = 0x88fa,
            rgb10_a2ui                       = 0x906f,
            sampler_binding                  = 0x8919,
            src1_color                       = 0x88f9,
            texture_swizzle_a                = 0x8e45,
            texture_swizzle_b                = 0x8e44,
            texture_swizzle_g                = 0x8e43,
            texture_swizzle_r                = 0x8e42,
            texture_swizzle_rgba             = 0x8e46,
            timestamp                        = 0x8e28,
            time_elapsed                     = 0x88bf,
            vertex_attrib_array_divisor      = 0x88fe,

            active_subroutines               = 0x8de5,
            active_subroutine_max_length     = 0x8e48,
            active_subroutine_uniforms       = 0x8de6,
            active_subroutine_uniform_locations = 0x8e47,
            active_subroutine_uniform_max_length = 0x8e49,
            compatible_subroutines           = 0x8e4b,
            double_mat2                      = 0x8f46,
            double_mat2x3                    = 0x8f49,
            double_mat2x4                    = 0x8f4a,
            double_mat3                      = 0x8f47,
            double_mat3x2                    = 0x8f4b,
            double_mat3x4                    = 0x8f4c,
            double_mat4                      = 0x8f48,
            double_mat4x2                    = 0x8f4d,
            double_mat4x3                    = 0x8f4e,
            double_vec2                      = 0x8ffc,
            double_vec3                      = 0x8ffd,
            double_vec4                      = 0x8ffe,
            draw_indirect_buffer             = 0x8f3f,
            draw_indirect_buffer_binding     = 0x8f43,
            fractional_even                  = 0x8e7c,
            fractional_odd                   = 0x8e7b,
            fragment_interpolation_offset_bits = 0x8e5d,
            geometry_shader_invocations      = 0x887f,
            int_sampler_cube_map_array       = 0x900e,
            isolines                         = 0x8e7a,
            max_combined_tess_control_uniform_components = 0x8e1e,
            max_combined_tess_evaluation_uniform_components = 0x8e1f,
            max_fragment_interpolation_offset = 0x8e5c,
            max_geometry_shader_invocations  = 0x8e5a,
            max_patch_vertices               = 0x8e7d,
            max_program_texture_gather_offset = 0x8e5f,
            max_subroutines                  = 0x8de7,
            max_subroutine_uniform_locations = 0x8de8,
            max_tess_control_input_components = 0x886c,
            max_tess_control_output_components = 0x8e83,
            max_tess_control_texture_image_units = 0x8e81,
            max_tess_control_total_output_components = 0x8e85,
            max_tess_control_uniform_blocks  = 0x8e89,
            max_tess_control_uniform_components = 0x8e7f,
            max_tess_evaluation_input_components = 0x886d,
            max_tess_evaluation_output_components = 0x8e86,
            max_tess_evaluation_texture_image_units = 0x8e82,
            max_tess_evaluation_uniform_blocks = 0x8e8a,
            max_tess_evaluation_uniform_components = 0x8e80,
            max_tess_gen_level               = 0x8e7e,
            max_tess_patch_components        = 0x8e84,
            max_transform_feedback_buffers   = 0x8e70,
            max_vertex_streams               = 0x8e71,
            min_fragment_interpolation_offset = 0x8e5b,
            min_program_texture_gather_offset = 0x8e5e,
            min_sample_shading_value         = 0x8c37,
            num_compatible_subroutines       = 0x8e4a,
            patches                          = 0x000e,
            patch_default_inner_level        = 0x8e73,
            patch_default_outer_level        = 0x8e74,
            patch_vertices                   = 0x8e72,
            proxy_texture_cube_map_array     = 0x900b,
            sampler_cube_map_array           = 0x900c,
            sampler_cube_map_array_shadow    = 0x900d,
            sample_shading                   = 0x8c36,
            tess_control_output_vertices     = 0x8e75,
            tess_control_shader              = 0x8e88,
            tess_evaluation_shader           = 0x8e87,
            tess_gen_mode                    = 0x8e76,
            tess_gen_point_mode              = 0x8e79,
            tess_gen_spacing                 = 0x8e77,
            tess_gen_vertex_order            = 0x8e78,
            texture_binding_cube_map_array   = 0x900a,
            texture_cube_map_array           = 0x9009,
            transform_feedback               = 0x8e22,
            transform_feedback_binding       = 0x8e25,
            transform_feedback_buffer_active = 0x8e24,
            transform_feedback_buffer_paused = 0x8e23,
            uniform_block_referenced_by_tess_control_shader = 0x84f0,
            uniform_block_referenced_by_tess_evaluation_shader = 0x84f1,
            unsigned_int_sampler_cube_map_array = 0x900f,

            active_program                   = 0x8259,
            all_shader_bits                  = 0xffffffff,
            fixed                            = 0x140c,
            fragment_shader_bit              = 0x00000002,
            geometry_shader_bit              = 0x00000004,
            high_float                       = 0x8df2,
            high_int                         = 0x8df5,
            implementation_color_read_format = 0x8b9b,
            implementation_color_read_type   = 0x8b9a,
            layer_provoking_vertex           = 0x825e,
            low_float                        = 0x8df0,
            low_int                          = 0x8df3,
            max_fragment_uniform_vectors     = 0x8dfd,
            max_varying_vectors              = 0x8dfc,
            max_vertex_uniform_vectors       = 0x8dfb,
            max_viewports                    = 0x825b,
            medium_float                     = 0x8df1,
            medium_int                       = 0x8df4,
            num_program_binary_formats       = 0x87fe,
            num_shader_binary_formats        = 0x8df9,
            program_binary_formats           = 0x87ff,
            program_binary_length            = 0x8741,
            program_binary_retrievable_hint  = 0x8257,
            program_pipeline_binding         = 0x825a,
            program_separable                = 0x8258,
            rgb565                           = 0x8d62,
            shader_binary_formats            = 0x8df8,
            shader_compiler                  = 0x8dfa,
            tess_control_shader_bit          = 0x00000008,
            tess_evaluation_shader_bit       = 0x00000010,
            undefined_vertex                 = 0x8260,
            vertex_shader_bit                = 0x00000001,
            viewport_bounds_range            = 0x825d,
            viewport_index_provoking_vertex  = 0x825f,
            viewport_subpixel_bits           = 0x825c,

            active_atomic_counter_buffers    = 0x92d9,
            all_barrier_bits                 = 0xffffffff,
            atomic_counter_barrier_bit       = 0x00001000,
            atomic_counter_buffer            = 0x92c0,
            atomic_counter_buffer_active_atomic_counters = 0x92c5,
            atomic_counter_buffer_active_atomic_counter_indices = 0x92c6,
            atomic_counter_buffer_binding    = 0x92c1,
            atomic_counter_buffer_data_size  = 0x92c4,
            atomic_counter_buffer_referenced_by_fragment_shader = 0x92cb,
            atomic_counter_buffer_referenced_by_geometry_shader = 0x92ca,
            atomic_counter_buffer_referenced_by_tess_control_shader = 0x92c8,
            atomic_counter_buffer_referenced_by_tess_evaluation_shader = 0x92c9,
            atomic_counter_buffer_referenced_by_vertex_shader = 0x92c7,
            atomic_counter_buffer_size       = 0x92c3,
            atomic_counter_buffer_start      = 0x92c2,
            buffer_update_barrier_bit        = 0x00000200,
            command_barrier_bit              = 0x00000040,
            compressed_rgba_bptc_unorm       = 0x8e8c,
            compressed_rgb_bptc_signed_float = 0x8e8e,
            compressed_rgb_bptc_unsigned_float = 0x8e8f,
            compressed_srgb_alpha_bptc_unorm = 0x8e8d,
            copy_read_buffer_binding         = 0x8f36,
            copy_write_buffer_binding        = 0x8f37,
            element_array_barrier_bit        = 0x00000002,
            framebuffer_barrier_bit          = 0x00000400,
            image_1d                         = 0x904c,
            image_1d_array                   = 0x9052,
            image_2d                         = 0x904d,
            image_2d_array                   = 0x9053,
            image_2d_multisample             = 0x9055,
            image_2d_multisample_array       = 0x9056,
            image_2d_rect                    = 0x904f,
            image_3d                         = 0x904e,
            image_binding_access             = 0x8f3e,
            image_binding_format             = 0x906e,
            image_binding_layer              = 0x8f3d,
            image_binding_layered            = 0x8f3c,
            image_binding_level              = 0x8f3b,
            image_binding_name               = 0x8f3a,
            image_buffer                     = 0x9051,
            image_cube                       = 0x9050,
            image_cube_map_array             = 0x9054,
            image_format_compatibility_by_class = 0x90c9,
            image_format_compatibility_by_size = 0x90c8,
            image_format_compatibility_type  = 0x90c7,
            int_image_1d                     = 0x9057,
            int_image_1d_array               = 0x905d,
            int_image_2d                     = 0x9058,
            int_image_2d_array               = 0x905e,
            int_image_2d_multisample         = 0x9060,
            int_image_2d_multisample_array   = 0x9061,
            int_image_2d_rect                = 0x905a,
            int_image_3d                     = 0x9059,
            int_image_buffer                 = 0x905c,
            int_image_cube                   = 0x905b,
            int_image_cube_map_array         = 0x905f,
            max_atomic_counter_buffer_bindings = 0x92dc,
            max_atomic_counter_buffer_size   = 0x92d8,
            max_combined_atomic_counters     = 0x92d7,
            max_combined_atomic_counter_buffers = 0x92d1,
            max_combined_image_uniforms      = 0x90cf,
            max_combined_image_units_and_fragment_outputs = 0x8f39,
            max_fragment_atomic_counters     = 0x92d6,
            max_fragment_atomic_counter_buffers = 0x92d0,
            max_fragment_image_uniforms      = 0x90ce,
            max_geometry_atomic_counters     = 0x92d5,
            max_geometry_atomic_counter_buffers = 0x92cf,
            max_geometry_image_uniforms      = 0x90cd,
            max_image_samples                = 0x906d,
            max_image_units                  = 0x8f38,
            max_tess_control_atomic_counters = 0x92d3,
            max_tess_control_atomic_counter_buffers = 0x92cd,
            max_tess_control_image_uniforms  = 0x90cb,
            max_tess_evaluation_atomic_counters = 0x92d4,
            max_tess_evaluation_atomic_counter_buffers = 0x92ce,
            max_tess_evaluation_image_uniforms = 0x90cc,
            max_vertex_atomic_counters       = 0x92d2,
            max_vertex_atomic_counter_buffers = 0x92cc,
            max_vertex_image_uniforms        = 0x90ca,
            min_map_buffer_alignment         = 0x90bc,
            num_sample_counts                = 0x9380,
            pack_compressed_block_depth      = 0x912d,
            pack_compressed_block_height     = 0x912c,
            pack_compressed_block_size       = 0x912e,
            pack_compressed_block_width      = 0x912b,
            pixel_buffer_barrier_bit         = 0x00000080,
            shader_image_access_barrier_bit  = 0x00000020,
            texture_fetch_barrier_bit        = 0x00000008,
            texture_immutable_format         = 0x912f,
            texture_update_barrier_bit       = 0x00000100,
            transform_feedback_active        = 0x8e24,
            transform_feedback_barrier_bit   = 0x00000800,
            transform_feedback_paused        = 0x8e23,
            uniform_atomic_counter_buffer_index = 0x92da,
            uniform_barrier_bit              = 0x00000004,
            unpack_compressed_block_depth    = 0x9129,
            unpack_compressed_block_height   = 0x9128,
            unpack_compressed_block_size     = 0x912a,
            unpack_compressed_block_width    = 0x9127,
            unsigned_int_atomic_counter      = 0x92db,
            unsigned_int_image_1d            = 0x9062,
            unsigned_int_image_1d_array      = 0x9068,
            unsigned_int_image_2d            = 0x9063,
            unsigned_int_image_2d_array      = 0x9069,
            unsigned_int_image_2d_multisample = 0x906b,
            unsigned_int_image_2d_multisample_array = 0x906c,
            unsigned_int_image_2d_rect       = 0x9065,
            unsigned_int_image_3d            = 0x9064,
            unsigned_int_image_buffer        = 0x9067,
            unsigned_int_image_cube          = 0x9066,
            unsigned_int_image_cube_map_array = 0x906a,
            vertex_attrib_array_barrier_bit  = 0x00000001,

            active_resources                 = 0x92f5,
            active_variables                 = 0x9305,
            any_samples_passed_conservative  = 0x8d6a,
            array_size                       = 0x92fb,
            array_stride                     = 0x92fe,
            atomic_counter_buffer_index      = 0x9301,
            atomic_counter_buffer_referenced_by_compute_shader = 0x90ed,
            auto_generate_mipmap             = 0x8295,
            block_index                      = 0x92fd,
            buffer                           = 0x82e0,
            buffer_binding                   = 0x9302,
            buffer_data_size                 = 0x9303,
            buffer_variable                  = 0x92e5,
            caveat_support                   = 0x82b8,
            clear_buffer                     = 0x82b4,
            color_components                 = 0x8283,
            color_encoding                   = 0x8296,
            color_renderable                 = 0x8286,
            compressed_r11_eac               = 0x9270,
            compressed_rg11_eac              = 0x9272,
            compressed_rgb8_etc2             = 0x9274,
            compressed_rgb8_punchthrough_alpha1_etc2 = 0x9276,
            compressed_rgba8_etc2_eac        = 0x9278,
            compressed_signed_r11_eac        = 0x9271,
            compressed_signed_rg11_eac       = 0x9273,
            compressed_srgb8_alpha8_etc2_eac = 0x9279,
            compressed_srgb8_etc2            = 0x9275,
            compressed_srgb8_punchthrough_alpha1_etc2 = 0x9277,
            compute_shader                   = 0x91b9,
            compute_shader_bit               = 0x00000020,
            compute_subroutine               = 0x92ed,
            compute_subroutine_uniform       = 0x92f3,
            compute_texture                  = 0x82a0,
            compute_work_group_size          = 0x8267,
            context_flag_debug_bit           = 0x00000002,
            debug_callback_function          = 0x8244,
            debug_callback_user_param        = 0x8245,
            debug_group_stack_depth          = 0x826d,
            debug_logged_messages            = 0x9145,
            debug_next_logged_message_length = 0x8243,
            debug_output                     = 0x92e0,
            debug_output_synchronous         = 0x8242,
            debug_severity_high              = 0x9146,
            debug_severity_low               = 0x9148,
            debug_severity_medium            = 0x9147,
            debug_severity_notification      = 0x826b,
            debug_source_api                 = 0x8246,
            debug_source_application         = 0x824a,
            debug_source_other               = 0x824b,
            debug_source_shader_compiler     = 0x8248,
            debug_source_third_party         = 0x8249,
            debug_source_window_system       = 0x8247,
            debug_type_deprecated_behavior   = 0x824d,
            debug_type_error                 = 0x824c,
            debug_type_marker                = 0x8268,
            debug_type_other                 = 0x8251,
            debug_type_performance           = 0x8250,
            debug_type_pop_group             = 0x826a,
            debug_type_portability           = 0x824f,
            debug_type_push_group            = 0x8269,
            debug_type_undefined_behavior    = 0x824e,
            depth_components                 = 0x8284,
            depth_renderable                 = 0x8287,
            depth_stencil_texture_mode       = 0x90ea,
            dispatch_indirect_buffer         = 0x90ee,
            dispatch_indirect_buffer_binding = 0x90ef,
            filter                           = 0x829a,
            fragment_subroutine              = 0x92ec,
            fragment_subroutine_uniform      = 0x92f2,
            fragment_texture                 = 0x829f,
            framebuffer_blend                = 0x828b,
            framebuffer_default_fixed_sample_locations = 0x9314,
            framebuffer_default_height       = 0x9311,
            framebuffer_default_layers       = 0x9312,
            framebuffer_default_samples      = 0x9313,
            framebuffer_default_width        = 0x9310,
            framebuffer_renderable           = 0x8289,
            framebuffer_renderable_layered   = 0x828a,
            full_support                     = 0x82b7,
            geometry_subroutine              = 0x92eb,
            geometry_subroutine_uniform      = 0x92f1,
            geometry_texture                 = 0x829e,
            get_texture_image_format         = 0x8291,
            get_texture_image_type           = 0x8292,
            image_class_10_10_10_2           = 0x82c3,
            image_class_11_11_10             = 0x82c2,
            image_class_1_x_16               = 0x82be,
            image_class_1_x_32               = 0x82bb,
            image_class_1_x_8                = 0x82c1,
            image_class_2_x_16               = 0x82bd,
            image_class_2_x_32               = 0x82ba,
            image_class_2_x_8                = 0x82c0,
            image_class_4_x_16               = 0x82bc,
            image_class_4_x_32               = 0x82b9,
            image_class_4_x_8                = 0x82bf,
            image_compatibility_class        = 0x82a8,
            image_pixel_format               = 0x82a9,
            image_pixel_type                 = 0x82aa,
            image_texel_size                 = 0x82a7,
            internalformat_alpha_size        = 0x8274,
            internalformat_alpha_type        = 0x827b,
            internalformat_blue_size         = 0x8273,
            internalformat_blue_type         = 0x827a,
            internalformat_depth_size        = 0x8275,
            internalformat_depth_type        = 0x827c,
            internalformat_green_size        = 0x8272,
            internalformat_green_type        = 0x8279,
            internalformat_preferred         = 0x8270,
            internalformat_red_size          = 0x8271,
            internalformat_red_type          = 0x8278,
            internalformat_shared_size       = 0x8277,
            internalformat_stencil_size      = 0x8276,
            internalformat_stencil_type      = 0x827d,
            internalformat_supported         = 0x826f,
            is_per_patch                     = 0x92e7,
            is_row_major                     = 0x9300,
            location                         = 0x930e,
            location_index                   = 0x930f,
            manual_generate_mipmap           = 0x8294,
            matrix_stride                    = 0x92ff,
            max_combined_compute_uniform_components = 0x8266,
            max_combined_dimensions          = 0x8282,
            max_combined_shader_output_resources = 0x8f39,
            max_combined_shader_storage_blocks = 0x90dc,
            max_compute_atomic_counters      = 0x8265,
            max_compute_atomic_counter_buffers = 0x8264,
            max_compute_image_uniforms       = 0x91bd,
            max_compute_shader_storage_blocks = 0x90db,
            max_compute_shared_memory_size   = 0x8262,
            max_compute_texture_image_units  = 0x91bc,
            max_compute_uniform_blocks       = 0x91bb,
            max_compute_uniform_components   = 0x8263,
            max_compute_work_group_count     = 0x91be,
            max_compute_work_group_invocations = 0x90eb,
            max_compute_work_group_size      = 0x91bf,
            max_debug_group_stack_depth      = 0x826c,
            max_debug_logged_messages        = 0x9144,
            max_debug_message_length         = 0x9143,
            max_depth                        = 0x8280,
            max_element_index                = 0x8d6b,
            max_fragment_shader_storage_blocks = 0x90da,
            max_framebuffer_height           = 0x9316,
            max_framebuffer_layers           = 0x9317,
            max_framebuffer_samples          = 0x9318,
            max_framebuffer_width            = 0x9315,
            max_geometry_shader_storage_blocks = 0x90d7,
            max_height                       = 0x827f,
            max_label_length                 = 0x82e8,
            max_layers                       = 0x8281,
            max_name_length                  = 0x92f6,
            max_num_active_variables         = 0x92f7,
            max_num_compatible_subroutines   = 0x92f8,
            max_shader_storage_block_size    = 0x90de,
            max_shader_storage_buffer_bindings = 0x90dd,
            max_tess_control_shader_storage_blocks = 0x90d8,
            max_tess_evaluation_shader_storage_blocks = 0x90d9,
            max_uniform_locations            = 0x826e,
            max_vertex_attrib_bindings       = 0x82da,
            max_vertex_attrib_relative_offset = 0x82d9,
            max_vertex_shader_storage_blocks = 0x90d6,
            max_width                        = 0x827e,
            mipmap                           = 0x8293,
            name_length                      = 0x92f9,
            num_active_variables             = 0x9304,
            num_shading_language_versions    = 0x82e9,
            offset                           = 0x92fc,
            primitive_restart_fixed_index    = 0x8d69,
            program                          = 0x82e2,
            program_input                    = 0x92e3,
            program_output                   = 0x92e4,
            program_pipeline                 = 0x82e4,
            query                            = 0x82e3,
            read_pixels                      = 0x828c,
            read_pixels_format               = 0x828d,
            read_pixels_type                 = 0x828e,
            referenced_by_compute_shader     = 0x930b,
            referenced_by_fragment_shader    = 0x930a,
            referenced_by_geometry_shader    = 0x9309,
            referenced_by_tess_control_shader = 0x9307,
            referenced_by_tess_evaluation_shader = 0x9308,
            referenced_by_vertex_shader      = 0x9306,
            sampler                          = 0x82e6,
            shader                           = 0x82e1,
            shader_image_atomic              = 0x82a6,
            shader_image_load                = 0x82a4,
            shader_image_store               = 0x82a5,
            shader_storage_barrier_bit       = 0x00002000,
            shader_storage_block             = 0x92e6,
            shader_storage_buffer            = 0x90d2,
            shader_storage_buffer_binding    = 0x90d3,
            shader_storage_buffer_offset_alignment = 0x90df,
            shader_storage_buffer_size       = 0x90d5,
            shader_storage_buffer_start      = 0x90d4,
            simultaneous_texture_and_depth_test = 0x82ac,
            simultaneous_texture_and_depth_write = 0x82ae,
            simultaneous_texture_and_stencil_test = 0x82ad,
            simultaneous_texture_and_stencil_write = 0x82af,
            srgb_read                        = 0x8297,
            srgb_write                       = 0x8298,
            stencil_components               = 0x8285,
            stencil_renderable               = 0x8288,
            tess_control_subroutine          = 0x92e9,
            tess_control_subroutine_uniform  = 0x92ef,
            tess_control_texture             = 0x829c,
            tess_evaluation_subroutine       = 0x92ea,
            tess_evaluation_subroutine_uniform = 0x92f0,
            tess_evaluation_texture          = 0x829d,
            texture_buffer_offset            = 0x919d,
            texture_buffer_offset_alignment  = 0x919f,
            texture_buffer_size              = 0x919e,
            texture_compressed_block_height  = 0x82b2,
            texture_compressed_block_size    = 0x82b3,
            texture_compressed_block_width   = 0x82b1,
            texture_gather                   = 0x82a2,
            texture_gather_shadow            = 0x82a3,
            texture_image_format             = 0x828f,
            texture_image_type               = 0x8290,
            texture_immutable_levels         = 0x82df,
            texture_shadow                   = 0x82a1,
            texture_view                     = 0x82b5,
            texture_view_min_layer           = 0x82dd,
            texture_view_min_level           = 0x82db,
            texture_view_num_layers          = 0x82de,
            texture_view_num_levels          = 0x82dc,
            top_level_array_size             = 0x930c,
            top_level_array_stride           = 0x930d,
            transform_feedback_varying       = 0x92f4,
            type                             = 0x92fa,
            uniform                          = 0x92e1,
            uniform_block                    = 0x92e2,
            uniform_block_referenced_by_compute_shader = 0x90ec,
            vertex_attrib_array_long         = 0x874e,
            vertex_attrib_binding            = 0x82d4,
            vertex_attrib_relative_offset    = 0x82d5,
            vertex_binding_buffer            = 0x8f4f,
            vertex_binding_divisor           = 0x82d6,
            vertex_binding_offset            = 0x82d7,
            vertex_binding_stride            = 0x82d8,
            vertex_subroutine                = 0x92e8,
            vertex_subroutine_uniform        = 0x92ee,
            vertex_texture                   = 0x829b,
            view_class_128_bits              = 0x82c4,
            view_class_16_bits               = 0x82ca,
            view_class_24_bits               = 0x82c9,
            view_class_32_bits               = 0x82c8,
            view_class_48_bits               = 0x82c7,
            view_class_64_bits               = 0x82c6,
            view_class_8_bits                = 0x82cb,
            view_class_96_bits               = 0x82c5,
            view_class_bptc_float            = 0x82d3,
            view_class_bptc_unorm            = 0x82d2,
            view_class_rgtc1_red             = 0x82d0,
            view_class_rgtc2_rg              = 0x82d1,
            view_class_s3tc_dxt1_rgb         = 0x82cc,
            view_class_s3tc_dxt1_rgba        = 0x82cd,
            view_class_s3tc_dxt3_rgba        = 0x82ce,
            view_class_s3tc_dxt5_rgba        = 0x82cf,
            view_compatibility_class         = 0x82b6,

            buffer_immutable_storage         = 0x821f,
            buffer_storage_flags             = 0x8220,
            clear_texture                    = 0x9365,
            client_mapped_buffer_barrier_bit = 0x00004000,
            client_storage_bit               = 0x0200,
            dynamic_storage_bit              = 0x0100,
            location_component               = 0x934a,
            map_coherent_bit                 = 0x0080,
            map_persistent_bit               = 0x0040,
            max_vertex_attrib_stride         = 0x82e5,
            mirror_clamp_to_edge             = 0x8743,
            primitive_restart_for_patches_supported = 0x8221,
            query_buffer                     = 0x9192,
            query_buffer_barrier_bit         = 0x00008000,
            query_buffer_binding             = 0x9193,
            query_result_no_wait             = 0x9194,
            texture_buffer_binding           = 0x8c2a,
            transform_feedback_buffer_index  = 0x934b,
            transform_feedback_buffer_stride = 0x934c,

            clip_depth_mode                  = 0x935d,
            clip_origin                      = 0x935c,
            context_flag_robust_access_bit   = 0x00000004,
            context_lost                     = 0x0507,
            context_release_behavior         = 0x82fb,
            context_release_behavior_flush   = 0x82fc,
            guilty_context_reset             = 0x8253,
            innocent_context_reset           = 0x8254,
            lose_context_on_reset            = 0x8252,
            max_combined_clip_and_cull_distances = 0x82fa,
            max_cull_distances               = 0x82f9,
            negative_one_to_one              = 0x935e,
            no_reset_notification            = 0x8261,
            query_by_region_no_wait_inverted = 0x8e1a,
            query_by_region_wait_inverted    = 0x8e19,
            query_no_wait_inverted           = 0x8e18,
            query_target                     = 0x82ea,
            query_wait_inverted              = 0x8e17,
            reset_notification_strategy      = 0x8256,
            texture_target                   = 0x1006,
            unknown_context_reset            = 0x8255,
            zero_to_one                      = 0x935f,
        };
    }
    using namespace enums;

    namespace constants
    {
        inline constexpr auto timeout_ignored = 0xffffffffffffffff;
    }
    using namespace constants;

    namespace types
    {
        extern "C"
        {
            using boolean_type = unsigned char;
            using bitfield_type = unsigned int;
            using void_type = void;
            using byte_type = signed char;
            using short_type = short;
            using int_type = int;
            using ubyte_type = unsigned char;
            using ushort_type = unsigned short;
            using uint_type = unsigned int;
            using sizei_type = int;
            using float_type = float;
            using clampf_type = float;
            using double_type = double;
            using clampd_type = double;
            using char_type = char;
            using half_type = unsigned short;
            using fixed_type = int_type;
            using intptr_type = ptrdiff_t;
            using sizeiptr_type = ptrdiff_t;
            using int64_type = int64_t;
            using uint64_type = uint64_t;
            using sync_type = struct __sync_type*;
            struct _cl_context;
            struct _cl_event;
            using debug_proc_type = void (enum_type source, enum_type type, uint_type id, enum_type severity, sizei_type length, char_type const* message, void const* userParam);
        }
    }
    using namespace types;

    namespace funcs
    {
        extern auto (*blend_func) (enum_type sfactor, enum_type dfactor) -> void;
        extern auto (*clear) (bitfield_type mask) -> void;
        extern auto (*clear_color) (float_type red, float_type green, float_type blue, float_type alpha) -> void;
        extern auto (*clear_depth) (double_type depth) -> void;
        extern auto (*clear_stencil) (int_type s) -> void;
        extern auto (*color_mask) (boolean_type red, boolean_type green, boolean_type blue, boolean_type alpha) -> void;
        extern auto (*cull_face) (enum_type mode) -> void;
        extern auto (*depth_func) (enum_type func) -> void;
        extern auto (*depth_mask) (boolean_type flag) -> void;
        extern auto (*depth_range) (double_type ren_near, double_type ren_far) -> void;
        extern auto (*disable) (enum_type cap) -> void;
        extern auto (*draw_buffer) (enum_type buf) -> void;
        extern auto (*enable) (enum_type cap) -> void;
        extern auto (*finish) () -> void;
        extern auto (*flush) () -> void;
        extern auto (*front_face) (enum_type mode) -> void;
        extern auto (*get_booleanv) (enum_type pname, boolean_type* data) -> void;
        extern auto (*get_doublev) (enum_type pname, double_type* data) -> void;
        extern auto (*get_error) () -> enum_type;
        extern auto (*get_floatv) (enum_type pname, float_type* data) -> void;
        extern auto (*get_integerv) (enum_type pname, int_type* data) -> void;
        extern auto (*get_string) (enum_type name) -> ubyte_type const*;
        extern auto (*get_tex_image) (enum_type target, int_type level, enum_type format, enum_type type, void* pixels) -> void;
        extern auto (*get_tex_level_parameterfv) (enum_type target, int_type level, enum_type pname, float_type* params) -> void;
        extern auto (*get_tex_level_parameteriv) (enum_type target, int_type level, enum_type pname, int_type* params) -> void;
        extern auto (*get_tex_parameterfv) (enum_type target, enum_type pname, float_type* params) -> void;
        extern auto (*get_tex_parameteriv) (enum_type target, enum_type pname, int_type* params) -> void;
        extern auto (*hint) (enum_type target, enum_type mode) -> void;
        extern auto (*is_enabled) (enum_type cap) -> boolean_type;
        extern auto (*line_width) (float_type width) -> void;
        extern auto (*logic_op) (enum_type opcode) -> void;
        extern auto (*pixel_storef) (enum_type pname, float_type param) -> void;
        extern auto (*pixel_storei) (enum_type pname, int_type param) -> void;
        extern auto (*point_size) (float_type size) -> void;
        extern auto (*polygon_mode) (enum_type face, enum_type mode) -> void;
        extern auto (*read_buffer) (enum_type src) -> void;
        extern auto (*read_pixels) (int_type x, int_type y, sizei_type width, sizei_type height, enum_type format, enum_type type, void* pixels) -> void;
        extern auto (*scissor) (int_type x, int_type y, sizei_type width, sizei_type height) -> void;
        extern auto (*stencil_func) (enum_type func, int_type ref, uint_type mask) -> void;
        extern auto (*stencil_mask) (uint_type mask) -> void;
        extern auto (*stencil_op) (enum_type fail, enum_type zfail, enum_type zpass) -> void;
        extern auto (*tex_image1d) (enum_type target, int_type level, int_type internalformat, sizei_type width, int_type border, enum_type format, enum_type type, void const* pixels) -> void;
        extern auto (*tex_image2d) (enum_type target, int_type level, int_type internalformat, sizei_type width, sizei_type height, int_type border, enum_type format, enum_type type, void const* pixels) -> void;
        extern auto (*tex_parameterf) (enum_type target, enum_type pname, float_type param) -> void;
        extern auto (*tex_parameterfv) (enum_type target, enum_type pname, float_type const* params) -> void;
        extern auto (*tex_parameteri) (enum_type target, enum_type pname, int_type param) -> void;
        extern auto (*tex_parameteriv) (enum_type target, enum_type pname, int_type const* params) -> void;
        extern auto (*viewport) (int_type x, int_type y, sizei_type width, sizei_type height) -> void;

        extern auto (*bind_texture) (enum_type target, uint_type texture) -> void;
        extern auto (*copy_tex_image1d) (enum_type target, int_type level, enum_type internalformat, int_type x, int_type y, sizei_type width, int_type border) -> void;
        extern auto (*copy_tex_image2d) (enum_type target, int_type level, enum_type internalformat, int_type x, int_type y, sizei_type width, sizei_type height, int_type border) -> void;
        extern auto (*copy_tex_sub_image1d) (enum_type target, int_type level, int_type xoffset, int_type x, int_type y, sizei_type width) -> void;
        extern auto (*copy_tex_sub_image2d) (enum_type target, int_type level, int_type xoffset, int_type yoffset, int_type x, int_type y, sizei_type width, sizei_type height) -> void;
        extern auto (*delete_textures) (sizei_type n, uint_type const* textures) -> void;
        extern auto (*draw_arrays) (enum_type mode, int_type first, sizei_type count) -> void;
        extern auto (*draw_elements) (enum_type mode, sizei_type count, enum_type type, void const* indices) -> void;
        extern auto (*gen_textures) (sizei_type n, uint_type* textures) -> void;
        extern auto (*get_pointerv) (enum_type pname, void* * params) -> void;
        extern auto (*is_texture) (uint_type texture) -> boolean_type;
        extern auto (*polygon_offset) (float_type factor, float_type units) -> void;
        extern auto (*tex_sub_image1d) (enum_type target, int_type level, int_type xoffset, sizei_type width, enum_type format, enum_type type, void const* pixels) -> void;
        extern auto (*tex_sub_image2d) (enum_type target, int_type level, int_type xoffset, int_type yoffset, sizei_type width, sizei_type height, enum_type format, enum_type type, void const* pixels) -> void;

        extern auto (*copy_tex_sub_image3d) (enum_type target, int_type level, int_type xoffset, int_type yoffset, int_type zoffset, int_type x, int_type y, sizei_type width, sizei_type height) -> void;
        extern auto (*draw_range_elements) (enum_type mode, uint_type start, uint_type end, sizei_type count, enum_type type, void const* indices) -> void;
        extern auto (*tex_image3d) (enum_type target, int_type level, int_type internalformat, sizei_type width, sizei_type height, sizei_type depth, int_type border, enum_type format, enum_type type, void const* pixels) -> void;
        extern auto (*tex_sub_image3d) (enum_type target, int_type level, int_type xoffset, int_type yoffset, int_type zoffset, sizei_type width, sizei_type height, sizei_type depth, enum_type format, enum_type type, void const* pixels) -> void;

        extern auto (*active_texture) (enum_type texture) -> void;
        extern auto (*compressed_tex_image1d) (enum_type target, int_type level, enum_type internalformat, sizei_type width, int_type border, sizei_type imageSize, void const* data) -> void;
        extern auto (*compressed_tex_image2d) (enum_type target, int_type level, enum_type internalformat, sizei_type width, sizei_type height, int_type border, sizei_type imageSize, void const* data) -> void;
        extern auto (*compressed_tex_image3d) (enum_type target, int_type level, enum_type internalformat, sizei_type width, sizei_type height, sizei_type depth, int_type border, sizei_type imageSize, void const* data) -> void;
        extern auto (*compressed_tex_sub_image1d) (enum_type target, int_type level, int_type xoffset, sizei_type width, enum_type format, sizei_type imageSize, void const* data) -> void;
        extern auto (*compressed_tex_sub_image2d) (enum_type target, int_type level, int_type xoffset, int_type yoffset, sizei_type width, sizei_type height, enum_type format, sizei_type imageSize, void const* data) -> void;
        extern auto (*compressed_tex_sub_image3d) (enum_type target, int_type level, int_type xoffset, int_type yoffset, int_type zoffset, sizei_type width, sizei_type height, sizei_type depth, enum_type format, sizei_type imageSize, void const* data) -> void;
        extern auto (*get_compressed_tex_image) (enum_type target, int_type level, void* img) -> void;
        extern auto (*sample_coverage) (float_type value, boolean_type invert) -> void;

        extern auto (*blend_color) (float_type red, float_type green, float_type blue, float_type alpha) -> void;
        extern auto (*blend_equation) (enum_type mode) -> void;
        extern auto (*blend_func_separate) (enum_type sfactorRGB, enum_type dfactorRGB, enum_type sfactorAlpha, enum_type dfactorAlpha) -> void;
        extern auto (*multi_draw_arrays) (enum_type mode, int_type const* first, sizei_type const* count, sizei_type drawcount) -> void;
        extern auto (*multi_draw_elements) (enum_type mode, sizei_type const* count, enum_type type, void const* const* indices, sizei_type drawcount) -> void;
        extern auto (*point_parameterf) (enum_type pname, float_type param) -> void;
        extern auto (*point_parameterfv) (enum_type pname, float_type const* params) -> void;
        extern auto (*point_parameteri) (enum_type pname, int_type param) -> void;
        extern auto (*point_parameteriv) (enum_type pname, int_type const* params) -> void;

        extern auto (*begin_query) (enum_type target, uint_type id) -> void;
        extern auto (*bind_buffer) (enum_type target, uint_type buffer) -> void;
        extern auto (*buffer_data) (enum_type target, sizeiptr_type size, void const* data, enum_type usage) -> void;
        extern auto (*buffer_sub_data) (enum_type target, intptr_type offset, sizeiptr_type size, void const* data) -> void;
        extern auto (*delete_buffers) (sizei_type n, uint_type const* buffers) -> void;
        extern auto (*delete_queries) (sizei_type n, uint_type const* ids) -> void;
        extern auto (*end_query) (enum_type target) -> void;
        extern auto (*gen_buffers) (sizei_type n, uint_type* buffers) -> void;
        extern auto (*gen_queries) (sizei_type n, uint_type* ids) -> void;
        extern auto (*get_buffer_parameteriv) (enum_type target, enum_type pname, int_type* params) -> void;
        extern auto (*get_buffer_pointerv) (enum_type target, enum_type pname, void* * params) -> void;
        extern auto (*get_buffer_sub_data) (enum_type target, intptr_type offset, sizeiptr_type size, void* data) -> void;
        extern auto (*get_query_objectiv) (uint_type id, enum_type pname, int_type* params) -> void;
        extern auto (*get_query_objectuiv) (uint_type id, enum_type pname, uint_type* params) -> void;
        extern auto (*get_queryiv) (enum_type target, enum_type pname, int_type* params) -> void;
        extern auto (*is_buffer) (uint_type buffer) -> boolean_type;
        extern auto (*is_query) (uint_type id) -> boolean_type;
        extern auto (*map_buffer) (enum_type target, enum_type access) -> void*;
        extern auto (*unmap_buffer) (enum_type target) -> boolean_type;

        extern auto (*attach_shader) (uint_type program, uint_type shader) -> void;
        extern auto (*bind_attrib_location) (uint_type program, uint_type index, char_type const* name) -> void;
        extern auto (*blend_equation_separate) (enum_type modeRGB, enum_type modeAlpha) -> void;
        extern auto (*compile_shader) (uint_type shader) -> void;
        extern auto (*create_program) () -> uint_type;
        extern auto (*create_shader) (enum_type type) -> uint_type;
        extern auto (*delete_program) (uint_type program) -> void;
        extern auto (*delete_shader) (uint_type shader) -> void;
        extern auto (*detach_shader) (uint_type program, uint_type shader) -> void;
        extern auto (*disable_vertex_attrib_array) (uint_type index) -> void;
        extern auto (*draw_buffers) (sizei_type n, enum_type const* bufs) -> void;
        extern auto (*enable_vertex_attrib_array) (uint_type index) -> void;
        extern auto (*get_active_attrib) (uint_type program, uint_type index, sizei_type bufSize, sizei_type* length, int_type* size, enum_type* type, char_type* name) -> void;
        extern auto (*get_active_uniform) (uint_type program, uint_type index, sizei_type bufSize, sizei_type* length, int_type* size, enum_type* type, char_type* name) -> void;
        extern auto (*get_attached_shaders) (uint_type program, sizei_type maxCount, sizei_type* count, uint_type* shaders) -> void;
        extern auto (*get_attrib_location) (uint_type program, char_type const* name) -> int_type;
        extern auto (*get_program_info_log) (uint_type program, sizei_type bufSize, sizei_type* length, char_type* infoLog) -> void;
        extern auto (*get_programiv) (uint_type program, enum_type pname, int_type* params) -> void;
        extern auto (*get_shader_info_log) (uint_type shader, sizei_type bufSize, sizei_type* length, char_type* infoLog) -> void;
        extern auto (*get_shader_source) (uint_type shader, sizei_type bufSize, sizei_type* length, char_type* source) -> void;
        extern auto (*get_shaderiv) (uint_type shader, enum_type pname, int_type* params) -> void;
        extern auto (*get_uniform_location) (uint_type program, char_type const* name) -> int_type;
        extern auto (*get_uniformfv) (uint_type program, int_type location, float_type* params) -> void;
        extern auto (*get_uniformiv) (uint_type program, int_type location, int_type* params) -> void;
        extern auto (*get_vertex_attrib_pointerv) (uint_type index, enum_type pname, void* * pointer) -> void;
        extern auto (*get_vertex_attribdv) (uint_type index, enum_type pname, double_type* params) -> void;
        extern auto (*get_vertex_attribfv) (uint_type index, enum_type pname, float_type* params) -> void;
        extern auto (*get_vertex_attribiv) (uint_type index, enum_type pname, int_type* params) -> void;
        extern auto (*is_program) (uint_type program) -> boolean_type;
        extern auto (*is_shader) (uint_type shader) -> boolean_type;
        extern auto (*link_program) (uint_type program) -> void;
        extern auto (*shader_source) (uint_type shader, sizei_type count, char_type const* const* string, int_type const* length) -> void;
        extern auto (*stencil_func_separate) (enum_type face, enum_type func, int_type ref, uint_type mask) -> void;
        extern auto (*stencil_mask_separate) (enum_type face, uint_type mask) -> void;
        extern auto (*stencil_op_separate) (enum_type face, enum_type sfail, enum_type dpfail, enum_type dppass) -> void;
        extern auto (*uniform1f) (int_type location, float_type v0) -> void;
        extern auto (*uniform1fv) (int_type location, sizei_type count, float_type const* value) -> void;
        extern auto (*uniform1i) (int_type location, int_type v0) -> void;
        extern auto (*uniform1iv) (int_type location, sizei_type count, int_type const* value) -> void;
        extern auto (*uniform2f) (int_type location, float_type v0, float_type v1) -> void;
        extern auto (*uniform2fv) (int_type location, sizei_type count, float_type const* value) -> void;
        extern auto (*uniform2i) (int_type location, int_type v0, int_type v1) -> void;
        extern auto (*uniform2iv) (int_type location, sizei_type count, int_type const* value) -> void;
        extern auto (*uniform3f) (int_type location, float_type v0, float_type v1, float_type v2) -> void;
        extern auto (*uniform3fv) (int_type location, sizei_type count, float_type const* value) -> void;
        extern auto (*uniform3i) (int_type location, int_type v0, int_type v1, int_type v2) -> void;
        extern auto (*uniform3iv) (int_type location, sizei_type count, int_type const* value) -> void;
        extern auto (*uniform4f) (int_type location, float_type v0, float_type v1, float_type v2, float_type v3) -> void;
        extern auto (*uniform4fv) (int_type location, sizei_type count, float_type const* value) -> void;
        extern auto (*uniform4i) (int_type location, int_type v0, int_type v1, int_type v2, int_type v3) -> void;
        extern auto (*uniform4iv) (int_type location, sizei_type count, int_type const* value) -> void;
        extern auto (*uniform_matrix2fv) (int_type location, sizei_type count, boolean_type transpose, float_type const* value) -> void;
        extern auto (*uniform_matrix3fv) (int_type location, sizei_type count, boolean_type transpose, float_type const* value) -> void;
        extern auto (*uniform_matrix4fv) (int_type location, sizei_type count, boolean_type transpose, float_type const* value) -> void;
        extern auto (*use_program) (uint_type program) -> void;
        extern auto (*validate_program) (uint_type program) -> void;
        extern auto (*vertex_attrib1d) (uint_type index, double_type x) -> void;
        extern auto (*vertex_attrib1dv) (uint_type index, double_type const* v) -> void;
        extern auto (*vertex_attrib1f) (uint_type index, float_type x) -> void;
        extern auto (*vertex_attrib1fv) (uint_type index, float_type const* v) -> void;
        extern auto (*vertex_attrib1s) (uint_type index, short_type x) -> void;
        extern auto (*vertex_attrib1sv) (uint_type index, short_type const* v) -> void;
        extern auto (*vertex_attrib2d) (uint_type index, double_type x, double_type y) -> void;
        extern auto (*vertex_attrib2dv) (uint_type index, double_type const* v) -> void;
        extern auto (*vertex_attrib2f) (uint_type index, float_type x, float_type y) -> void;
        extern auto (*vertex_attrib2fv) (uint_type index, float_type const* v) -> void;
        extern auto (*vertex_attrib2s) (uint_type index, short_type x, short_type y) -> void;
        extern auto (*vertex_attrib2sv) (uint_type index, short_type const* v) -> void;
        extern auto (*vertex_attrib3d) (uint_type index, double_type x, double_type y, double_type z) -> void;
        extern auto (*vertex_attrib3dv) (uint_type index, double_type const* v) -> void;
        extern auto (*vertex_attrib3f) (uint_type index, float_type x, float_type y, float_type z) -> void;
        extern auto (*vertex_attrib3fv) (uint_type index, float_type const* v) -> void;
        extern auto (*vertex_attrib3s) (uint_type index, short_type x, short_type y, short_type z) -> void;
        extern auto (*vertex_attrib3sv) (uint_type index, short_type const* v) -> void;
        extern auto (*vertex_attrib4_nbv) (uint_type index, byte_type const* v) -> void;
        extern auto (*vertex_attrib4_niv) (uint_type index, int_type const* v) -> void;
        extern auto (*vertex_attrib4_nsv) (uint_type index, short_type const* v) -> void;
        extern auto (*vertex_attrib4_nub) (uint_type index, ubyte_type x, ubyte_type y, ubyte_type z, ubyte_type w) -> void;
        extern auto (*vertex_attrib4_nubv) (uint_type index, ubyte_type const* v) -> void;
        extern auto (*vertex_attrib4_nuiv) (uint_type index, uint_type const* v) -> void;
        extern auto (*vertex_attrib4_nusv) (uint_type index, ushort_type const* v) -> void;
        extern auto (*vertex_attrib4bv) (uint_type index, byte_type const* v) -> void;
        extern auto (*vertex_attrib4d) (uint_type index, double_type x, double_type y, double_type z, double_type w) -> void;
        extern auto (*vertex_attrib4dv) (uint_type index, double_type const* v) -> void;
        extern auto (*vertex_attrib4f) (uint_type index, float_type x, float_type y, float_type z, float_type w) -> void;
        extern auto (*vertex_attrib4fv) (uint_type index, float_type const* v) -> void;
        extern auto (*vertex_attrib4iv) (uint_type index, int_type const* v) -> void;
        extern auto (*vertex_attrib4s) (uint_type index, short_type x, short_type y, short_type z, short_type w) -> void;
        extern auto (*vertex_attrib4sv) (uint_type index, short_type const* v) -> void;
        extern auto (*vertex_attrib4ubv) (uint_type index, ubyte_type const* v) -> void;
        extern auto (*vertex_attrib4uiv) (uint_type index, uint_type const* v) -> void;
        extern auto (*vertex_attrib4usv) (uint_type index, ushort_type const* v) -> void;
        extern auto (*vertex_attrib_pointer) (uint_type index, int_type size, enum_type type, boolean_type normalized, sizei_type stride, void const* pointer) -> void;

        extern auto (*uniform_matrix2x3fv) (int_type location, sizei_type count, boolean_type transpose, float_type const* value) -> void;
        extern auto (*uniform_matrix2x4fv) (int_type location, sizei_type count, boolean_type transpose, float_type const* value) -> void;
        extern auto (*uniform_matrix3x2fv) (int_type location, sizei_type count, boolean_type transpose, float_type const* value) -> void;
        extern auto (*uniform_matrix3x4fv) (int_type location, sizei_type count, boolean_type transpose, float_type const* value) -> void;
        extern auto (*uniform_matrix4x2fv) (int_type location, sizei_type count, boolean_type transpose, float_type const* value) -> void;
        extern auto (*uniform_matrix4x3fv) (int_type location, sizei_type count, boolean_type transpose, float_type const* value) -> void;

        extern auto (*begin_conditional_render) (uint_type id, enum_type mode) -> void;
        extern auto (*begin_transform_feedback) (enum_type primitiveMode) -> void;
        extern auto (*bind_buffer_base) (enum_type target, uint_type index, uint_type buffer) -> void;
        extern auto (*bind_buffer_range) (enum_type target, uint_type index, uint_type buffer, intptr_type offset, sizeiptr_type size) -> void;
        extern auto (*bind_frag_data_location) (uint_type program, uint_type color, char_type const* name) -> void;
        extern auto (*bind_framebuffer) (enum_type target, uint_type framebuffer) -> void;
        extern auto (*bind_renderbuffer) (enum_type target, uint_type renderbuffer) -> void;
        extern auto (*bind_vertex_array) (uint_type ren_array) -> void;
        extern auto (*blit_framebuffer) (int_type srcX0, int_type srcY0, int_type srcX1, int_type srcY1, int_type dstX0, int_type dstY0, int_type dstX1, int_type dstY1, bitfield_type mask, enum_type filter) -> void;
        extern auto (*check_framebuffer_status) (enum_type target) -> enum_type;
        extern auto (*clamp_color) (enum_type target, enum_type clamp) -> void;
        extern auto (*clear_bufferfi) (enum_type buffer, int_type drawbuffer, float_type depth, int_type stencil) -> void;
        extern auto (*clear_bufferfv) (enum_type buffer, int_type drawbuffer, float_type const* value) -> void;
        extern auto (*clear_bufferiv) (enum_type buffer, int_type drawbuffer, int_type const* value) -> void;
        extern auto (*clear_bufferuiv) (enum_type buffer, int_type drawbuffer, uint_type const* value) -> void;
        extern auto (*color_maski) (uint_type index, boolean_type r, boolean_type g, boolean_type b, boolean_type a) -> void;
        extern auto (*delete_framebuffers) (sizei_type n, uint_type const* framebuffers) -> void;
        extern auto (*delete_renderbuffers) (sizei_type n, uint_type const* renderbuffers) -> void;
        extern auto (*delete_vertex_arrays) (sizei_type n, uint_type const* arrays) -> void;
        extern auto (*disablei) (enum_type target, uint_type index) -> void;
        extern auto (*enablei) (enum_type target, uint_type index) -> void;
        extern auto (*end_conditional_render) () -> void;
        extern auto (*end_transform_feedback) () -> void;
        extern auto (*flush_mapped_buffer_range) (enum_type target, intptr_type offset, sizeiptr_type length) -> void;
        extern auto (*framebuffer_renderbuffer) (enum_type target, enum_type attachment, enum_type renderbuffertarget, uint_type renderbuffer) -> void;
        extern auto (*framebuffer_texture1d) (enum_type target, enum_type attachment, enum_type textarget, uint_type texture, int_type level) -> void;
        extern auto (*framebuffer_texture2d) (enum_type target, enum_type attachment, enum_type textarget, uint_type texture, int_type level) -> void;
        extern auto (*framebuffer_texture3d) (enum_type target, enum_type attachment, enum_type textarget, uint_type texture, int_type level, int_type zoffset) -> void;
        extern auto (*framebuffer_texture_layer) (enum_type target, enum_type attachment, uint_type texture, int_type level, int_type layer) -> void;
        extern auto (*gen_framebuffers) (sizei_type n, uint_type* framebuffers) -> void;
        extern auto (*gen_renderbuffers) (sizei_type n, uint_type* renderbuffers) -> void;
        extern auto (*gen_vertex_arrays) (sizei_type n, uint_type* arrays) -> void;
        extern auto (*generate_mipmap) (enum_type target) -> void;
        extern auto (*get_booleani_v) (enum_type target, uint_type index, boolean_type* data) -> void;
        extern auto (*get_frag_data_location) (uint_type program, char_type const* name) -> int_type;
        extern auto (*get_framebuffer_attachment_parameteriv) (enum_type target, enum_type attachment, enum_type pname, int_type* params) -> void;
        extern auto (*get_integeri_v) (enum_type target, uint_type index, int_type* data) -> void;
        extern auto (*get_renderbuffer_parameteriv) (enum_type target, enum_type pname, int_type* params) -> void;
        extern auto (*get_stringi) (enum_type name, uint_type index) -> ubyte_type const*;
        extern auto (*get_tex_parameter_iiv) (enum_type target, enum_type pname, int_type* params) -> void;
        extern auto (*get_tex_parameter_iuiv) (enum_type target, enum_type pname, uint_type* params) -> void;
        extern auto (*get_transform_feedback_varying) (uint_type program, uint_type index, sizei_type bufSize, sizei_type* length, sizei_type* size, enum_type* type, char_type* name) -> void;
        extern auto (*get_uniformuiv) (uint_type program, int_type location, uint_type* params) -> void;
        extern auto (*get_vertex_attrib_iiv) (uint_type index, enum_type pname, int_type* params) -> void;
        extern auto (*get_vertex_attrib_iuiv) (uint_type index, enum_type pname, uint_type* params) -> void;
        extern auto (*is_enabledi) (enum_type target, uint_type index) -> boolean_type;
        extern auto (*is_framebuffer) (uint_type framebuffer) -> boolean_type;
        extern auto (*is_renderbuffer) (uint_type renderbuffer) -> boolean_type;
        extern auto (*is_vertex_array) (uint_type ren_array) -> boolean_type;
        extern auto (*map_buffer_range) (enum_type target, intptr_type offset, sizeiptr_type length, bitfield_type access) -> void*;
        extern auto (*renderbuffer_storage) (enum_type target, enum_type internalformat, sizei_type width, sizei_type height) -> void;
        extern auto (*renderbuffer_storage_multisample) (enum_type target, sizei_type samples, enum_type internalformat, sizei_type width, sizei_type height) -> void;
        extern auto (*tex_parameter_iiv) (enum_type target, enum_type pname, int_type const* params) -> void;
        extern auto (*tex_parameter_iuiv) (enum_type target, enum_type pname, uint_type const* params) -> void;
        extern auto (*transform_feedback_varyings) (uint_type program, sizei_type count, char_type const* const* varyings, enum_type bufferMode) -> void;
        extern auto (*uniform1ui) (int_type location, uint_type v0) -> void;
        extern auto (*uniform1uiv) (int_type location, sizei_type count, uint_type const* value) -> void;
        extern auto (*uniform2ui) (int_type location, uint_type v0, uint_type v1) -> void;
        extern auto (*uniform2uiv) (int_type location, sizei_type count, uint_type const* value) -> void;
        extern auto (*uniform3ui) (int_type location, uint_type v0, uint_type v1, uint_type v2) -> void;
        extern auto (*uniform3uiv) (int_type location, sizei_type count, uint_type const* value) -> void;
        extern auto (*uniform4ui) (int_type location, uint_type v0, uint_type v1, uint_type v2, uint_type v3) -> void;
        extern auto (*uniform4uiv) (int_type location, sizei_type count, uint_type const* value) -> void;
        extern auto (*vertex_attrib_i1i) (uint_type index, int_type x) -> void;
        extern auto (*vertex_attrib_i1iv) (uint_type index, int_type const* v) -> void;
        extern auto (*vertex_attrib_i1ui) (uint_type index, uint_type x) -> void;
        extern auto (*vertex_attrib_i1uiv) (uint_type index, uint_type const* v) -> void;
        extern auto (*vertex_attrib_i2i) (uint_type index, int_type x, int_type y) -> void;
        extern auto (*vertex_attrib_i2iv) (uint_type index, int_type const* v) -> void;
        extern auto (*vertex_attrib_i2ui) (uint_type index, uint_type x, uint_type y) -> void;
        extern auto (*vertex_attrib_i2uiv) (uint_type index, uint_type const* v) -> void;
        extern auto (*vertex_attrib_i3i) (uint_type index, int_type x, int_type y, int_type z) -> void;
        extern auto (*vertex_attrib_i3iv) (uint_type index, int_type const* v) -> void;
        extern auto (*vertex_attrib_i3ui) (uint_type index, uint_type x, uint_type y, uint_type z) -> void;
        extern auto (*vertex_attrib_i3uiv) (uint_type index, uint_type const* v) -> void;
        extern auto (*vertex_attrib_i4bv) (uint_type index, byte_type const* v) -> void;
        extern auto (*vertex_attrib_i4i) (uint_type index, int_type x, int_type y, int_type z, int_type w) -> void;
        extern auto (*vertex_attrib_i4iv) (uint_type index, int_type const* v) -> void;
        extern auto (*vertex_attrib_i4sv) (uint_type index, short_type const* v) -> void;
        extern auto (*vertex_attrib_i4ubv) (uint_type index, ubyte_type const* v) -> void;
        extern auto (*vertex_attrib_i4ui) (uint_type index, uint_type x, uint_type y, uint_type z, uint_type w) -> void;
        extern auto (*vertex_attrib_i4uiv) (uint_type index, uint_type const* v) -> void;
        extern auto (*vertex_attrib_i4usv) (uint_type index, ushort_type const* v) -> void;
        extern auto (*vertex_attrib_i_pointer) (uint_type index, int_type size, enum_type type, sizei_type stride, void const* pointer) -> void;

        extern auto (*copy_buffer_sub_data) (enum_type readTarget, enum_type writeTarget, intptr_type readOffset, intptr_type writeOffset, sizeiptr_type size) -> void;
        extern auto (*draw_arrays_instanced) (enum_type mode, int_type first, sizei_type count, sizei_type instancecount) -> void;
        extern auto (*draw_elements_instanced) (enum_type mode, sizei_type count, enum_type type, void const* indices, sizei_type instancecount) -> void;
        extern auto (*get_active_uniform_block_name) (uint_type program, uint_type uniformBlockIndex, sizei_type bufSize, sizei_type* length, char_type* uniformBlockName) -> void;
        extern auto (*get_active_uniform_blockiv) (uint_type program, uint_type uniformBlockIndex, enum_type pname, int_type* params) -> void;
        extern auto (*get_active_uniform_name) (uint_type program, uint_type uniformIndex, sizei_type bufSize, sizei_type* length, char_type* uniformName) -> void;
        extern auto (*get_active_uniformsiv) (uint_type program, sizei_type uniformCount, uint_type const* uniformIndices, enum_type pname, int_type* params) -> void;
        extern auto (*get_uniform_block_index) (uint_type program, char_type const* uniformBlockName) -> uint_type;
        extern auto (*get_uniform_indices) (uint_type program, sizei_type uniformCount, char_type const* const* uniformNames, uint_type* uniformIndices) -> void;
        extern auto (*primitive_restart_index) (uint_type index) -> void;
        extern auto (*tex_buffer) (enum_type target, enum_type internalformat, uint_type buffer) -> void;
        extern auto (*uniform_block_binding) (uint_type program, uint_type uniformBlockIndex, uint_type uniformBlockBinding) -> void;

        extern auto (*client_wait_sync) (sync_type sync, bitfield_type flags, uint64_type timeout) -> enum_type;
        extern auto (*delete_sync) (sync_type sync) -> void;
        extern auto (*draw_elements_base_vertex) (enum_type mode, sizei_type count, enum_type type, void const* indices, int_type basevertex) -> void;
        extern auto (*draw_elements_instanced_base_vertex) (enum_type mode, sizei_type count, enum_type type, void const* indices, sizei_type instancecount, int_type basevertex) -> void;
        extern auto (*draw_range_elements_base_vertex) (enum_type mode, uint_type start, uint_type end, sizei_type count, enum_type type, void const* indices, int_type basevertex) -> void;
        extern auto (*fence_sync) (enum_type condition, bitfield_type flags) -> sync_type;
        extern auto (*framebuffer_texture) (enum_type target, enum_type attachment, uint_type texture, int_type level) -> void;
        extern auto (*get_buffer_parameteri64v) (enum_type target, enum_type pname, int64_type* params) -> void;
        extern auto (*get_integer64i_v) (enum_type target, uint_type index, int64_type* data) -> void;
        extern auto (*get_integer64v) (enum_type pname, int64_type* data) -> void;
        extern auto (*get_multisamplefv) (enum_type pname, uint_type index, float_type* val) -> void;
        extern auto (*get_synciv) (sync_type sync, enum_type pname, sizei_type bufSize, sizei_type* length, int_type* values) -> void;
        extern auto (*is_sync) (sync_type sync) -> boolean_type;
        extern auto (*multi_draw_elements_base_vertex) (enum_type mode, sizei_type const* count, enum_type type, void const* const* indices, sizei_type drawcount, int_type const* basevertex) -> void;
        extern auto (*provoking_vertex) (enum_type mode) -> void;
        extern auto (*sample_maski) (uint_type maskNumber, bitfield_type mask) -> void;
        extern auto (*tex_image2d_multisample) (enum_type target, sizei_type samples, enum_type internalformat, sizei_type width, sizei_type height, boolean_type fixedsamplelocations) -> void;
        extern auto (*tex_image3d_multisample) (enum_type target, sizei_type samples, enum_type internalformat, sizei_type width, sizei_type height, sizei_type depth, boolean_type fixedsamplelocations) -> void;
        extern auto (*wait_sync) (sync_type sync, bitfield_type flags, uint64_type timeout) -> void;

        extern auto (*bind_frag_data_location_indexed) (uint_type program, uint_type colorNumber, uint_type index, char_type const* name) -> void;
        extern auto (*bind_sampler) (uint_type unit, uint_type sampler) -> void;
        extern auto (*delete_samplers) (sizei_type count, uint_type const* samplers) -> void;
        extern auto (*gen_samplers) (sizei_type count, uint_type* samplers) -> void;
        extern auto (*get_frag_data_index) (uint_type program, char_type const* name) -> int_type;
        extern auto (*get_query_objecti64v) (uint_type id, enum_type pname, int64_type* params) -> void;
        extern auto (*get_query_objectui64v) (uint_type id, enum_type pname, uint64_type* params) -> void;
        extern auto (*get_sampler_parameter_iiv) (uint_type sampler, enum_type pname, int_type* params) -> void;
        extern auto (*get_sampler_parameter_iuiv) (uint_type sampler, enum_type pname, uint_type* params) -> void;
        extern auto (*get_sampler_parameterfv) (uint_type sampler, enum_type pname, float_type* params) -> void;
        extern auto (*get_sampler_parameteriv) (uint_type sampler, enum_type pname, int_type* params) -> void;
        extern auto (*is_sampler) (uint_type sampler) -> boolean_type;
        extern auto (*query_counter) (uint_type id, enum_type target) -> void;
        extern auto (*sampler_parameter_iiv) (uint_type sampler, enum_type pname, int_type const* param) -> void;
        extern auto (*sampler_parameter_iuiv) (uint_type sampler, enum_type pname, uint_type const* param) -> void;
        extern auto (*sampler_parameterf) (uint_type sampler, enum_type pname, float_type param) -> void;
        extern auto (*sampler_parameterfv) (uint_type sampler, enum_type pname, float_type const* param) -> void;
        extern auto (*sampler_parameteri) (uint_type sampler, enum_type pname, int_type param) -> void;
        extern auto (*sampler_parameteriv) (uint_type sampler, enum_type pname, int_type const* param) -> void;
        extern auto (*vertex_attrib_divisor) (uint_type index, uint_type divisor) -> void;
        extern auto (*vertex_attrib_p1ui) (uint_type index, enum_type type, boolean_type normalized, uint_type value) -> void;
        extern auto (*vertex_attrib_p1uiv) (uint_type index, enum_type type, boolean_type normalized, uint_type const* value) -> void;
        extern auto (*vertex_attrib_p2ui) (uint_type index, enum_type type, boolean_type normalized, uint_type value) -> void;
        extern auto (*vertex_attrib_p2uiv) (uint_type index, enum_type type, boolean_type normalized, uint_type const* value) -> void;
        extern auto (*vertex_attrib_p3ui) (uint_type index, enum_type type, boolean_type normalized, uint_type value) -> void;
        extern auto (*vertex_attrib_p3uiv) (uint_type index, enum_type type, boolean_type normalized, uint_type const* value) -> void;
        extern auto (*vertex_attrib_p4ui) (uint_type index, enum_type type, boolean_type normalized, uint_type value) -> void;
        extern auto (*vertex_attrib_p4uiv) (uint_type index, enum_type type, boolean_type normalized, uint_type const* value) -> void;

        extern auto (*begin_query_indexed) (enum_type target, uint_type index, uint_type id) -> void;
        extern auto (*bind_transform_feedback) (enum_type target, uint_type id) -> void;
        extern auto (*blend_equation_separatei) (uint_type buf, enum_type modeRGB, enum_type modeAlpha) -> void;
        extern auto (*blend_equationi) (uint_type buf, enum_type mode) -> void;
        extern auto (*blend_func_separatei) (uint_type buf, enum_type srcRGB, enum_type dstRGB, enum_type srcAlpha, enum_type dstAlpha) -> void;
        extern auto (*blend_funci) (uint_type buf, enum_type src, enum_type dst) -> void;
        extern auto (*delete_transform_feedbacks) (sizei_type n, uint_type const* ids) -> void;
        extern auto (*draw_arrays_indirect) (enum_type mode, void const* indirect) -> void;
        extern auto (*draw_elements_indirect) (enum_type mode, enum_type type, void const* indirect) -> void;
        extern auto (*draw_transform_feedback) (enum_type mode, uint_type id) -> void;
        extern auto (*draw_transform_feedback_stream) (enum_type mode, uint_type id, uint_type stream) -> void;
        extern auto (*end_query_indexed) (enum_type target, uint_type index) -> void;
        extern auto (*gen_transform_feedbacks) (sizei_type n, uint_type* ids) -> void;
        extern auto (*get_active_subroutine_name) (uint_type program, enum_type shadertype, uint_type index, sizei_type bufsize, sizei_type* length, char_type* name) -> void;
        extern auto (*get_active_subroutine_uniform_name) (uint_type program, enum_type shadertype, uint_type index, sizei_type bufsize, sizei_type* length, char_type* name) -> void;
        extern auto (*get_active_subroutine_uniformiv) (uint_type program, enum_type shadertype, uint_type index, enum_type pname, int_type* values) -> void;
        extern auto (*get_program_stageiv) (uint_type program, enum_type shadertype, enum_type pname, int_type* values) -> void;
        extern auto (*get_query_indexediv) (enum_type target, uint_type index, enum_type pname, int_type* params) -> void;
        extern auto (*get_subroutine_index) (uint_type program, enum_type shadertype, char_type const* name) -> uint_type;
        extern auto (*get_subroutine_uniform_location) (uint_type program, enum_type shadertype, char_type const* name) -> int_type;
        extern auto (*get_uniform_subroutineuiv) (enum_type shadertype, int_type location, uint_type* params) -> void;
        extern auto (*get_uniformdv) (uint_type program, int_type location, double_type* params) -> void;
        extern auto (*is_transform_feedback) (uint_type id) -> boolean_type;
        extern auto (*min_sample_shading) (float_type value) -> void;
        extern auto (*patch_parameterfv) (enum_type pname, float_type const* values) -> void;
        extern auto (*patch_parameteri) (enum_type pname, int_type value) -> void;
        extern auto (*pause_transform_feedback) () -> void;
        extern auto (*resume_transform_feedback) () -> void;
        extern auto (*uniform1d) (int_type location, double_type x) -> void;
        extern auto (*uniform1dv) (int_type location, sizei_type count, double_type const* value) -> void;
        extern auto (*uniform2d) (int_type location, double_type x, double_type y) -> void;
        extern auto (*uniform2dv) (int_type location, sizei_type count, double_type const* value) -> void;
        extern auto (*uniform3d) (int_type location, double_type x, double_type y, double_type z) -> void;
        extern auto (*uniform3dv) (int_type location, sizei_type count, double_type const* value) -> void;
        extern auto (*uniform4d) (int_type location, double_type x, double_type y, double_type z, double_type w) -> void;
        extern auto (*uniform4dv) (int_type location, sizei_type count, double_type const* value) -> void;
        extern auto (*uniform_matrix2dv) (int_type location, sizei_type count, boolean_type transpose, double_type const* value) -> void;
        extern auto (*uniform_matrix2x3dv) (int_type location, sizei_type count, boolean_type transpose, double_type const* value) -> void;
        extern auto (*uniform_matrix2x4dv) (int_type location, sizei_type count, boolean_type transpose, double_type const* value) -> void;
        extern auto (*uniform_matrix3dv) (int_type location, sizei_type count, boolean_type transpose, double_type const* value) -> void;
        extern auto (*uniform_matrix3x2dv) (int_type location, sizei_type count, boolean_type transpose, double_type const* value) -> void;
        extern auto (*uniform_matrix3x4dv) (int_type location, sizei_type count, boolean_type transpose, double_type const* value) -> void;
        extern auto (*uniform_matrix4dv) (int_type location, sizei_type count, boolean_type transpose, double_type const* value) -> void;
        extern auto (*uniform_matrix4x2dv) (int_type location, sizei_type count, boolean_type transpose, double_type const* value) -> void;
        extern auto (*uniform_matrix4x3dv) (int_type location, sizei_type count, boolean_type transpose, double_type const* value) -> void;
        extern auto (*uniform_subroutinesuiv) (enum_type shadertype, sizei_type count, uint_type const* indices) -> void;

        extern auto (*active_shader_program) (uint_type pipeline, uint_type program) -> void;
        extern auto (*bind_program_pipeline) (uint_type pipeline) -> void;
        extern auto (*clear_depthf) (float_type d) -> void;
        extern auto (*create_shader_programv) (enum_type type, sizei_type count, char_type const* const* strings) -> uint_type;
        extern auto (*delete_program_pipelines) (sizei_type n, uint_type const* pipelines) -> void;
        extern auto (*depth_range_arrayv) (uint_type first, sizei_type count, double_type const* v) -> void;
        extern auto (*depth_range_indexed) (uint_type index, double_type n, double_type f) -> void;
        extern auto (*depth_rangef) (float_type n, float_type f) -> void;
        extern auto (*gen_program_pipelines) (sizei_type n, uint_type* pipelines) -> void;
        extern auto (*get_doublei_v) (enum_type target, uint_type index, double_type* data) -> void;
        extern auto (*get_floati_v) (enum_type target, uint_type index, float_type* data) -> void;
        extern auto (*get_program_binary) (uint_type program, sizei_type bufSize, sizei_type* length, enum_type* binaryFormat, void* binary) -> void;
        extern auto (*get_program_pipeline_info_log) (uint_type pipeline, sizei_type bufSize, sizei_type* length, char_type* infoLog) -> void;
        extern auto (*get_program_pipelineiv) (uint_type pipeline, enum_type pname, int_type* params) -> void;
        extern auto (*get_shader_precision_format) (enum_type shadertype, enum_type precisiontype, int_type* range, int_type* precision) -> void;
        extern auto (*get_vertex_attrib_ldv) (uint_type index, enum_type pname, double_type* params) -> void;
        extern auto (*is_program_pipeline) (uint_type pipeline) -> boolean_type;
        extern auto (*program_binary) (uint_type program, enum_type binaryFormat, void const* binary, sizei_type length) -> void;
        extern auto (*program_parameteri) (uint_type program, enum_type pname, int_type value) -> void;
        extern auto (*program_uniform1d) (uint_type program, int_type location, double_type v0) -> void;
        extern auto (*program_uniform1dv) (uint_type program, int_type location, sizei_type count, double_type const* value) -> void;
        extern auto (*program_uniform1f) (uint_type program, int_type location, float_type v0) -> void;
        extern auto (*program_uniform1fv) (uint_type program, int_type location, sizei_type count, float_type const* value) -> void;
        extern auto (*program_uniform1i) (uint_type program, int_type location, int_type v0) -> void;
        extern auto (*program_uniform1iv) (uint_type program, int_type location, sizei_type count, int_type const* value) -> void;
        extern auto (*program_uniform1ui) (uint_type program, int_type location, uint_type v0) -> void;
        extern auto (*program_uniform1uiv) (uint_type program, int_type location, sizei_type count, uint_type const* value) -> void;
        extern auto (*program_uniform2d) (uint_type program, int_type location, double_type v0, double_type v1) -> void;
        extern auto (*program_uniform2dv) (uint_type program, int_type location, sizei_type count, double_type const* value) -> void;
        extern auto (*program_uniform2f) (uint_type program, int_type location, float_type v0, float_type v1) -> void;
        extern auto (*program_uniform2fv) (uint_type program, int_type location, sizei_type count, float_type const* value) -> void;
        extern auto (*program_uniform2i) (uint_type program, int_type location, int_type v0, int_type v1) -> void;
        extern auto (*program_uniform2iv) (uint_type program, int_type location, sizei_type count, int_type const* value) -> void;
        extern auto (*program_uniform2ui) (uint_type program, int_type location, uint_type v0, uint_type v1) -> void;
        extern auto (*program_uniform2uiv) (uint_type program, int_type location, sizei_type count, uint_type const* value) -> void;
        extern auto (*program_uniform3d) (uint_type program, int_type location, double_type v0, double_type v1, double_type v2) -> void;
        extern auto (*program_uniform3dv) (uint_type program, int_type location, sizei_type count, double_type const* value) -> void;
        extern auto (*program_uniform3f) (uint_type program, int_type location, float_type v0, float_type v1, float_type v2) -> void;
        extern auto (*program_uniform3fv) (uint_type program, int_type location, sizei_type count, float_type const* value) -> void;
        extern auto (*program_uniform3i) (uint_type program, int_type location, int_type v0, int_type v1, int_type v2) -> void;
        extern auto (*program_uniform3iv) (uint_type program, int_type location, sizei_type count, int_type const* value) -> void;
        extern auto (*program_uniform3ui) (uint_type program, int_type location, uint_type v0, uint_type v1, uint_type v2) -> void;
        extern auto (*program_uniform3uiv) (uint_type program, int_type location, sizei_type count, uint_type const* value) -> void;
        extern auto (*program_uniform4d) (uint_type program, int_type location, double_type v0, double_type v1, double_type v2, double_type v3) -> void;
        extern auto (*program_uniform4dv) (uint_type program, int_type location, sizei_type count, double_type const* value) -> void;
        extern auto (*program_uniform4f) (uint_type program, int_type location, float_type v0, float_type v1, float_type v2, float_type v3) -> void;
        extern auto (*program_uniform4fv) (uint_type program, int_type location, sizei_type count, float_type const* value) -> void;
        extern auto (*program_uniform4i) (uint_type program, int_type location, int_type v0, int_type v1, int_type v2, int_type v3) -> void;
        extern auto (*program_uniform4iv) (uint_type program, int_type location, sizei_type count, int_type const* value) -> void;
        extern auto (*program_uniform4ui) (uint_type program, int_type location, uint_type v0, uint_type v1, uint_type v2, uint_type v3) -> void;
        extern auto (*program_uniform4uiv) (uint_type program, int_type location, sizei_type count, uint_type const* value) -> void;
        extern auto (*program_uniform_matrix2dv) (uint_type program, int_type location, sizei_type count, boolean_type transpose, double_type const* value) -> void;
        extern auto (*program_uniform_matrix2fv) (uint_type program, int_type location, sizei_type count, boolean_type transpose, float_type const* value) -> void;
        extern auto (*program_uniform_matrix2x3dv) (uint_type program, int_type location, sizei_type count, boolean_type transpose, double_type const* value) -> void;
        extern auto (*program_uniform_matrix2x3fv) (uint_type program, int_type location, sizei_type count, boolean_type transpose, float_type const* value) -> void;
        extern auto (*program_uniform_matrix2x4dv) (uint_type program, int_type location, sizei_type count, boolean_type transpose, double_type const* value) -> void;
        extern auto (*program_uniform_matrix2x4fv) (uint_type program, int_type location, sizei_type count, boolean_type transpose, float_type const* value) -> void;
        extern auto (*program_uniform_matrix3dv) (uint_type program, int_type location, sizei_type count, boolean_type transpose, double_type const* value) -> void;
        extern auto (*program_uniform_matrix3fv) (uint_type program, int_type location, sizei_type count, boolean_type transpose, float_type const* value) -> void;
        extern auto (*program_uniform_matrix3x2dv) (uint_type program, int_type location, sizei_type count, boolean_type transpose, double_type const* value) -> void;
        extern auto (*program_uniform_matrix3x2fv) (uint_type program, int_type location, sizei_type count, boolean_type transpose, float_type const* value) -> void;
        extern auto (*program_uniform_matrix3x4dv) (uint_type program, int_type location, sizei_type count, boolean_type transpose, double_type const* value) -> void;
        extern auto (*program_uniform_matrix3x4fv) (uint_type program, int_type location, sizei_type count, boolean_type transpose, float_type const* value) -> void;
        extern auto (*program_uniform_matrix4dv) (uint_type program, int_type location, sizei_type count, boolean_type transpose, double_type const* value) -> void;
        extern auto (*program_uniform_matrix4fv) (uint_type program, int_type location, sizei_type count, boolean_type transpose, float_type const* value) -> void;
        extern auto (*program_uniform_matrix4x2dv) (uint_type program, int_type location, sizei_type count, boolean_type transpose, double_type const* value) -> void;
        extern auto (*program_uniform_matrix4x2fv) (uint_type program, int_type location, sizei_type count, boolean_type transpose, float_type const* value) -> void;
        extern auto (*program_uniform_matrix4x3dv) (uint_type program, int_type location, sizei_type count, boolean_type transpose, double_type const* value) -> void;
        extern auto (*program_uniform_matrix4x3fv) (uint_type program, int_type location, sizei_type count, boolean_type transpose, float_type const* value) -> void;
        extern auto (*release_shader_compiler) () -> void;
        extern auto (*scissor_arrayv) (uint_type first, sizei_type count, int_type const* v) -> void;
        extern auto (*scissor_indexed) (uint_type index, int_type left, int_type bottom, sizei_type width, sizei_type height) -> void;
        extern auto (*scissor_indexedv) (uint_type index, int_type const* v) -> void;
        extern auto (*shader_binary) (sizei_type count, uint_type const* shaders, enum_type binaryformat, void const* binary, sizei_type length) -> void;
        extern auto (*use_program_stages) (uint_type pipeline, bitfield_type stages, uint_type program) -> void;
        extern auto (*validate_program_pipeline) (uint_type pipeline) -> void;
        extern auto (*vertex_attrib_l1d) (uint_type index, double_type x) -> void;
        extern auto (*vertex_attrib_l1dv) (uint_type index, double_type const* v) -> void;
        extern auto (*vertex_attrib_l2d) (uint_type index, double_type x, double_type y) -> void;
        extern auto (*vertex_attrib_l2dv) (uint_type index, double_type const* v) -> void;
        extern auto (*vertex_attrib_l3d) (uint_type index, double_type x, double_type y, double_type z) -> void;
        extern auto (*vertex_attrib_l3dv) (uint_type index, double_type const* v) -> void;
        extern auto (*vertex_attrib_l4d) (uint_type index, double_type x, double_type y, double_type z, double_type w) -> void;
        extern auto (*vertex_attrib_l4dv) (uint_type index, double_type const* v) -> void;
        extern auto (*vertex_attrib_l_pointer) (uint_type index, int_type size, enum_type type, sizei_type stride, void const* pointer) -> void;
        extern auto (*viewport_arrayv) (uint_type first, sizei_type count, float_type const* v) -> void;
        extern auto (*viewport_indexedf) (uint_type index, float_type x, float_type y, float_type w, float_type h) -> void;
        extern auto (*viewport_indexedfv) (uint_type index, float_type const* v) -> void;

        extern auto (*bind_image_texture) (uint_type unit, uint_type texture, int_type level, boolean_type layered, int_type layer, enum_type access, enum_type format) -> void;
        extern auto (*draw_arrays_instanced_base_instance) (enum_type mode, int_type first, sizei_type count, sizei_type instancecount, uint_type baseinstance) -> void;
        extern auto (*draw_elements_instanced_base_instance) (enum_type mode, sizei_type count, enum_type type, void const* indices, sizei_type instancecount, uint_type baseinstance) -> void;
        extern auto (*draw_elements_instanced_base_vertex_base_instance) (enum_type mode, sizei_type count, enum_type type, void const* indices, sizei_type instancecount, int_type basevertex, uint_type baseinstance) -> void;
        extern auto (*draw_transform_feedback_instanced) (enum_type mode, uint_type id, sizei_type instancecount) -> void;
        extern auto (*draw_transform_feedback_stream_instanced) (enum_type mode, uint_type id, uint_type stream, sizei_type instancecount) -> void;
        extern auto (*get_active_atomic_counter_bufferiv) (uint_type program, uint_type bufferIndex, enum_type pname, int_type* params) -> void;
        extern auto (*get_internalformativ) (enum_type target, enum_type internalformat, enum_type pname, sizei_type bufSize, int_type* params) -> void;
        extern auto (*memory_barrier) (bitfield_type barriers) -> void;
        extern auto (*tex_storage1d) (enum_type target, sizei_type levels, enum_type internalformat, sizei_type width) -> void;
        extern auto (*tex_storage2d) (enum_type target, sizei_type levels, enum_type internalformat, sizei_type width, sizei_type height) -> void;
        extern auto (*tex_storage3d) (enum_type target, sizei_type levels, enum_type internalformat, sizei_type width, sizei_type height, sizei_type depth) -> void;

        extern auto (*bind_vertex_buffer) (uint_type bindingindex, uint_type buffer, intptr_type offset, sizei_type stride) -> void;
        extern auto (*clear_buffer_data) (enum_type target, enum_type internalformat, enum_type format, enum_type type, void const* data) -> void;
        extern auto (*clear_buffer_sub_data) (enum_type target, enum_type internalformat, intptr_type offset, sizeiptr_type size, enum_type format, enum_type type, void const* data) -> void;
        extern auto (*copy_image_sub_data) (uint_type srcName, enum_type srcTarget, int_type srcLevel, int_type srcX, int_type srcY, int_type srcZ, uint_type dstName, enum_type dstTarget, int_type dstLevel, int_type dstX, int_type dstY, int_type dstZ, sizei_type srcWidth, sizei_type srcHeight, sizei_type srcDepth) -> void;
        extern auto (*debug_message_callback) (debug_proc_type* callback, void const* userParam) -> void;
        extern auto (*debug_message_control) (enum_type source, enum_type type, enum_type severity, sizei_type count, uint_type const* ids, boolean_type enabled) -> void;
        extern auto (*debug_message_insert) (enum_type source, enum_type type, uint_type id, enum_type severity, sizei_type length, char_type const* buf) -> void;
        extern auto (*dispatch_compute) (uint_type num_groups_x, uint_type num_groups_y, uint_type num_groups_z) -> void;
        extern auto (*dispatch_compute_indirect) (intptr_type indirect) -> void;
        extern auto (*framebuffer_parameteri) (enum_type target, enum_type pname, int_type param) -> void;
        extern auto (*get_debug_message_log) (uint_type count, sizei_type bufSize, enum_type* sources, enum_type* types, uint_type* ids, enum_type* severities, sizei_type* lengths, char_type* messageLog) -> uint_type;
        extern auto (*get_framebuffer_parameteriv) (enum_type target, enum_type pname, int_type* params) -> void;
        extern auto (*get_internalformati64v) (enum_type target, enum_type internalformat, enum_type pname, sizei_type bufSize, int64_type* params) -> void;
        extern auto (*get_object_label) (enum_type identifier, uint_type name, sizei_type bufSize, sizei_type* length, char_type* label) -> void;
        extern auto (*get_object_ptr_label) (void const* ptr, sizei_type bufSize, sizei_type* length, char_type* label) -> void;
        extern auto (*get_program_interfaceiv) (uint_type program, enum_type programInterface, enum_type pname, int_type* params) -> void;
        extern auto (*get_program_resource_index) (uint_type program, enum_type programInterface, char_type const* name) -> uint_type;
        extern auto (*get_program_resource_location) (uint_type program, enum_type programInterface, char_type const* name) -> int_type;
        extern auto (*get_program_resource_location_index) (uint_type program, enum_type programInterface, char_type const* name) -> int_type;
        extern auto (*get_program_resource_name) (uint_type program, enum_type programInterface, uint_type index, sizei_type bufSize, sizei_type* length, char_type* name) -> void;
        extern auto (*get_program_resourceiv) (uint_type program, enum_type programInterface, uint_type index, sizei_type propCount, enum_type const* props, sizei_type bufSize, sizei_type* length, int_type* params) -> void;
        extern auto (*invalidate_buffer_data) (uint_type buffer) -> void;
        extern auto (*invalidate_buffer_sub_data) (uint_type buffer, intptr_type offset, sizeiptr_type length) -> void;
        extern auto (*invalidate_framebuffer) (enum_type target, sizei_type numAttachments, enum_type const* attachments) -> void;
        extern auto (*invalidate_sub_framebuffer) (enum_type target, sizei_type numAttachments, enum_type const* attachments, int_type x, int_type y, sizei_type width, sizei_type height) -> void;
        extern auto (*invalidate_tex_image) (uint_type texture, int_type level) -> void;
        extern auto (*invalidate_tex_sub_image) (uint_type texture, int_type level, int_type xoffset, int_type yoffset, int_type zoffset, sizei_type width, sizei_type height, sizei_type depth) -> void;
        extern auto (*multi_draw_arrays_indirect) (enum_type mode, void const* indirect, sizei_type drawcount, sizei_type stride) -> void;
        extern auto (*multi_draw_elements_indirect) (enum_type mode, enum_type type, void const* indirect, sizei_type drawcount, sizei_type stride) -> void;
        extern auto (*object_label) (enum_type identifier, uint_type name, sizei_type length, char_type const* label) -> void;
        extern auto (*object_ptr_label) (void const* ptr, sizei_type length, char_type const* label) -> void;
        extern auto (*pop_debug_group) () -> void;
        extern auto (*push_debug_group) (enum_type source, uint_type id, sizei_type length, char_type const* message) -> void;
        extern auto (*shader_storage_block_binding) (uint_type program, uint_type storageBlockIndex, uint_type storageBlockBinding) -> void;
        extern auto (*tex_buffer_range) (enum_type target, enum_type internalformat, uint_type buffer, intptr_type offset, sizeiptr_type size) -> void;
        extern auto (*tex_storage2d_multisample) (enum_type target, sizei_type samples, enum_type internalformat, sizei_type width, sizei_type height, boolean_type fixedsamplelocations) -> void;
        extern auto (*tex_storage3d_multisample) (enum_type target, sizei_type samples, enum_type internalformat, sizei_type width, sizei_type height, sizei_type depth, boolean_type fixedsamplelocations) -> void;
        extern auto (*texture_view) (uint_type texture, enum_type target, uint_type origtexture, enum_type internalformat, uint_type minlevel, uint_type numlevels, uint_type minlayer, uint_type numlayers) -> void;
        extern auto (*vertex_attrib_binding) (uint_type attribindex, uint_type bindingindex) -> void;
        extern auto (*vertex_attrib_format) (uint_type attribindex, int_type size, enum_type type, boolean_type normalized, uint_type relativeoffset) -> void;
        extern auto (*vertex_attrib_i_format) (uint_type attribindex, int_type size, enum_type type, uint_type relativeoffset) -> void;
        extern auto (*vertex_attrib_l_format) (uint_type attribindex, int_type size, enum_type type, uint_type relativeoffset) -> void;
        extern auto (*vertex_binding_divisor) (uint_type bindingindex, uint_type divisor) -> void;

        extern auto (*bind_buffers_base) (enum_type target, uint_type first, sizei_type count, uint_type const* buffers) -> void;
        extern auto (*bind_buffers_range) (enum_type target, uint_type first, sizei_type count, uint_type const* buffers, intptr_type const* offsets, sizeiptr_type const* sizes) -> void;
        extern auto (*bind_image_textures) (uint_type first, sizei_type count, uint_type const* textures) -> void;
        extern auto (*bind_samplers) (uint_type first, sizei_type count, uint_type const* samplers) -> void;
        extern auto (*bind_textures) (uint_type first, sizei_type count, uint_type const* textures) -> void;
        extern auto (*bind_vertex_buffers) (uint_type first, sizei_type count, uint_type const* buffers, intptr_type const* offsets, sizei_type const* strides) -> void;
        extern auto (*buffer_storage) (enum_type target, sizeiptr_type size, void const* data, bitfield_type flags) -> void;
        extern auto (*clear_tex_image) (uint_type texture, int_type level, enum_type format, enum_type type, void const* data) -> void;
        extern auto (*clear_tex_sub_image) (uint_type texture, int_type level, int_type xoffset, int_type yoffset, int_type zoffset, sizei_type width, sizei_type height, sizei_type depth, enum_type format, enum_type type, void const* data) -> void;

        extern auto (*bind_texture_unit) (uint_type unit, uint_type texture) -> void;
        extern auto (*blit_named_framebuffer) (uint_type readFramebuffer, uint_type drawFramebuffer, int_type srcX0, int_type srcY0, int_type srcX1, int_type srcY1, int_type dstX0, int_type dstY0, int_type dstX1, int_type dstY1, bitfield_type mask, enum_type filter) -> void;
        extern auto (*check_named_framebuffer_status) (uint_type framebuffer, enum_type target) -> enum_type;
        extern auto (*clear_named_buffer_data) (uint_type buffer, enum_type internalformat, enum_type format, enum_type type, void const* data) -> void;
        extern auto (*clear_named_buffer_sub_data) (uint_type buffer, enum_type internalformat, intptr_type offset, sizeiptr_type size, enum_type format, enum_type type, void const* data) -> void;
        extern auto (*clear_named_framebufferfi) (uint_type framebuffer, enum_type buffer, int_type drawbuffer, const float_type depth, int_type stencil) -> void;
        extern auto (*clear_named_framebufferfv) (uint_type framebuffer, enum_type buffer, int_type drawbuffer, float_type const* value) -> void;
        extern auto (*clear_named_framebufferiv) (uint_type framebuffer, enum_type buffer, int_type drawbuffer, int_type const* value) -> void;
        extern auto (*clear_named_framebufferuiv) (uint_type framebuffer, enum_type buffer, int_type drawbuffer, uint_type const* value) -> void;
        extern auto (*clip_control) (enum_type origin, enum_type depth) -> void;
        extern auto (*compressed_texture_sub_image1d) (uint_type texture, int_type level, int_type xoffset, sizei_type width, enum_type format, sizei_type imageSize, void const* data) -> void;
        extern auto (*compressed_texture_sub_image2d) (uint_type texture, int_type level, int_type xoffset, int_type yoffset, sizei_type width, sizei_type height, enum_type format, sizei_type imageSize, void const* data) -> void;
        extern auto (*compressed_texture_sub_image3d) (uint_type texture, int_type level, int_type xoffset, int_type yoffset, int_type zoffset, sizei_type width, sizei_type height, sizei_type depth, enum_type format, sizei_type imageSize, void const* data) -> void;
        extern auto (*copy_named_buffer_sub_data) (uint_type readBuffer, uint_type writeBuffer, intptr_type readOffset, intptr_type writeOffset, sizeiptr_type size) -> void;
        extern auto (*copy_texture_sub_image1d) (uint_type texture, int_type level, int_type xoffset, int_type x, int_type y, sizei_type width) -> void;
        extern auto (*copy_texture_sub_image2d) (uint_type texture, int_type level, int_type xoffset, int_type yoffset, int_type x, int_type y, sizei_type width, sizei_type height) -> void;
        extern auto (*copy_texture_sub_image3d) (uint_type texture, int_type level, int_type xoffset, int_type yoffset, int_type zoffset, int_type x, int_type y, sizei_type width, sizei_type height) -> void;
        extern auto (*create_buffers) (sizei_type n, uint_type* buffers) -> void;
        extern auto (*create_framebuffers) (sizei_type n, uint_type* framebuffers) -> void;
        extern auto (*create_program_pipelines) (sizei_type n, uint_type* pipelines) -> void;
        extern auto (*create_queries) (enum_type target, sizei_type n, uint_type* ids) -> void;
        extern auto (*create_renderbuffers) (sizei_type n, uint_type* renderbuffers) -> void;
        extern auto (*create_samplers) (sizei_type n, uint_type* samplers) -> void;
        extern auto (*create_textures) (enum_type target, sizei_type n, uint_type* textures) -> void;
        extern auto (*create_transform_feedbacks) (sizei_type n, uint_type* ids) -> void;
        extern auto (*create_vertex_arrays) (sizei_type n, uint_type* arrays) -> void;
        extern auto (*disable_vertex_array_attrib) (uint_type vaobj, uint_type index) -> void;
        extern auto (*enable_vertex_array_attrib) (uint_type vaobj, uint_type index) -> void;
        extern auto (*flush_mapped_named_buffer_range) (uint_type buffer, intptr_type offset, sizeiptr_type length) -> void;
        extern auto (*generate_texture_mipmap) (uint_type texture) -> void;
        extern auto (*get_compressed_texture_image) (uint_type texture, int_type level, sizei_type bufSize, void* pixels) -> void;
        extern auto (*get_compressed_texture_sub_image) (uint_type texture, int_type level, int_type xoffset, int_type yoffset, int_type zoffset, sizei_type width, sizei_type height, sizei_type depth, sizei_type bufSize, void* pixels) -> void;
        extern auto (*get_graphics_reset_status) () -> enum_type;
        extern auto (*get_named_buffer_parameteri64v) (uint_type buffer, enum_type pname, int64_type* params) -> void;
        extern auto (*get_named_buffer_parameteriv) (uint_type buffer, enum_type pname, int_type* params) -> void;
        extern auto (*get_named_buffer_pointerv) (uint_type buffer, enum_type pname, void* * params) -> void;
        extern auto (*get_named_buffer_sub_data) (uint_type buffer, intptr_type offset, sizeiptr_type size, void* data) -> void;
        extern auto (*get_named_framebuffer_attachment_parameteriv) (uint_type framebuffer, enum_type attachment, enum_type pname, int_type* params) -> void;
        extern auto (*get_named_framebuffer_parameteriv) (uint_type framebuffer, enum_type pname, int_type* param) -> void;
        extern auto (*get_named_renderbuffer_parameteriv) (uint_type renderbuffer, enum_type pname, int_type* params) -> void;
        extern auto (*get_query_buffer_objecti64v) (uint_type id, uint_type buffer, enum_type pname, intptr_type offset) -> void;
        extern auto (*get_query_buffer_objectiv) (uint_type id, uint_type buffer, enum_type pname, intptr_type offset) -> void;
        extern auto (*get_query_buffer_objectui64v) (uint_type id, uint_type buffer, enum_type pname, intptr_type offset) -> void;
        extern auto (*get_query_buffer_objectuiv) (uint_type id, uint_type buffer, enum_type pname, intptr_type offset) -> void;
        extern auto (*get_texture_image) (uint_type texture, int_type level, enum_type format, enum_type type, sizei_type bufSize, void* pixels) -> void;
        extern auto (*get_texture_level_parameterfv) (uint_type texture, int_type level, enum_type pname, float_type* params) -> void;
        extern auto (*get_texture_level_parameteriv) (uint_type texture, int_type level, enum_type pname, int_type* params) -> void;
        extern auto (*get_texture_parameter_iiv) (uint_type texture, enum_type pname, int_type* params) -> void;
        extern auto (*get_texture_parameter_iuiv) (uint_type texture, enum_type pname, uint_type* params) -> void;
        extern auto (*get_texture_parameterfv) (uint_type texture, enum_type pname, float_type* params) -> void;
        extern auto (*get_texture_parameteriv) (uint_type texture, enum_type pname, int_type* params) -> void;
        extern auto (*get_texture_sub_image) (uint_type texture, int_type level, int_type xoffset, int_type yoffset, int_type zoffset, sizei_type width, sizei_type height, sizei_type depth, enum_type format, enum_type type, sizei_type bufSize, void* pixels) -> void;
        extern auto (*get_transform_feedbacki64_v) (uint_type xfb, enum_type pname, uint_type index, int64_type* param) -> void;
        extern auto (*get_transform_feedbacki_v) (uint_type xfb, enum_type pname, uint_type index, int_type* param) -> void;
        extern auto (*get_transform_feedbackiv) (uint_type xfb, enum_type pname, int_type* param) -> void;
        extern auto (*get_vertex_array_indexed64iv) (uint_type vaobj, uint_type index, enum_type pname, int64_type* param) -> void;
        extern auto (*get_vertex_array_indexediv) (uint_type vaobj, uint_type index, enum_type pname, int_type* param) -> void;
        extern auto (*get_vertex_arrayiv) (uint_type vaobj, enum_type pname, int_type* param) -> void;
        extern auto (*get_n_compressed_tex_image) (enum_type target, int_type lod, sizei_type bufSize, void* pixels) -> void;
        extern auto (*get_n_tex_image) (enum_type target, int_type level, enum_type format, enum_type type, sizei_type bufSize, void* pixels) -> void;
        extern auto (*get_n_uniformdv) (uint_type program, int_type location, sizei_type bufSize, double_type* params) -> void;
        extern auto (*get_n_uniformfv) (uint_type program, int_type location, sizei_type bufSize, float_type* params) -> void;
        extern auto (*get_n_uniformiv) (uint_type program, int_type location, sizei_type bufSize, int_type* params) -> void;
        extern auto (*get_n_uniformuiv) (uint_type program, int_type location, sizei_type bufSize, uint_type* params) -> void;
        extern auto (*invalidate_named_framebuffer_data) (uint_type framebuffer, sizei_type numAttachments, enum_type const* attachments) -> void;
        extern auto (*invalidate_named_framebuffer_sub_data) (uint_type framebuffer, sizei_type numAttachments, enum_type const* attachments, int_type x, int_type y, sizei_type width, sizei_type height) -> void;
        extern auto (*map_named_buffer) (uint_type buffer, enum_type access) -> void*;
        extern auto (*map_named_buffer_range) (uint_type buffer, intptr_type offset, sizeiptr_type length, bitfield_type access) -> void*;
        extern auto (*memory_barrier_by_region) (bitfield_type barriers) -> void;
        extern auto (*named_buffer_data) (uint_type buffer, sizeiptr_type size, void const* data, enum_type usage) -> void;
        extern auto (*named_buffer_storage) (uint_type buffer, sizeiptr_type size, void const* data, bitfield_type flags) -> void;
        extern auto (*named_buffer_sub_data) (uint_type buffer, intptr_type offset, sizeiptr_type size, void const* data) -> void;
        extern auto (*named_framebuffer_draw_buffer) (uint_type framebuffer, enum_type buf) -> void;
        extern auto (*named_framebuffer_draw_buffers) (uint_type framebuffer, sizei_type n, enum_type const* bufs) -> void;
        extern auto (*named_framebuffer_parameteri) (uint_type framebuffer, enum_type pname, int_type param) -> void;
        extern auto (*named_framebuffer_read_buffer) (uint_type framebuffer, enum_type src) -> void;
        extern auto (*named_framebuffer_renderbuffer) (uint_type framebuffer, enum_type attachment, enum_type renderbuffertarget, uint_type renderbuffer) -> void;
        extern auto (*named_framebuffer_texture) (uint_type framebuffer, enum_type attachment, uint_type texture, int_type level) -> void;
        extern auto (*named_framebuffer_texture_layer) (uint_type framebuffer, enum_type attachment, uint_type texture, int_type level, int_type layer) -> void;
        extern auto (*named_renderbuffer_storage) (uint_type renderbuffer, enum_type internalformat, sizei_type width, sizei_type height) -> void;
        extern auto (*named_renderbuffer_storage_multisample) (uint_type renderbuffer, sizei_type samples, enum_type internalformat, sizei_type width, sizei_type height) -> void;
        extern auto (*read_n_pixels) (int_type x, int_type y, sizei_type width, sizei_type height, enum_type format, enum_type type, sizei_type bufSize, void* data) -> void;
        extern auto (*texture_barrier) () -> void;
        extern auto (*texture_buffer) (uint_type texture, enum_type internalformat, uint_type buffer) -> void;
        extern auto (*texture_buffer_range) (uint_type texture, enum_type internalformat, uint_type buffer, intptr_type offset, sizeiptr_type size) -> void;
        extern auto (*texture_parameter_iiv) (uint_type texture, enum_type pname, int_type const* params) -> void;
        extern auto (*texture_parameter_iuiv) (uint_type texture, enum_type pname, uint_type const* params) -> void;
        extern auto (*texture_parameterf) (uint_type texture, enum_type pname, float_type param) -> void;
        extern auto (*texture_parameterfv) (uint_type texture, enum_type pname, float_type const* param) -> void;
        extern auto (*texture_parameteri) (uint_type texture, enum_type pname, int_type param) -> void;
        extern auto (*texture_parameteriv) (uint_type texture, enum_type pname, int_type const* param) -> void;
        extern auto (*texture_storage1d) (uint_type texture, sizei_type levels, enum_type internalformat, sizei_type width) -> void;
        extern auto (*texture_storage2d) (uint_type texture, sizei_type levels, enum_type internalformat, sizei_type width, sizei_type height) -> void;
        extern auto (*texture_storage2d_multisample) (uint_type texture, sizei_type samples, enum_type internalformat, sizei_type width, sizei_type height, boolean_type fixedsamplelocations) -> void;
        extern auto (*texture_storage3d) (uint_type texture, sizei_type levels, enum_type internalformat, sizei_type width, sizei_type height, sizei_type depth) -> void;
        extern auto (*texture_storage3d_multisample) (uint_type texture, sizei_type samples, enum_type internalformat, sizei_type width, sizei_type height, sizei_type depth, boolean_type fixedsamplelocations) -> void;
        extern auto (*texture_sub_image1d) (uint_type texture, int_type level, int_type xoffset, sizei_type width, enum_type format, enum_type type, void const* pixels) -> void;
        extern auto (*texture_sub_image2d) (uint_type texture, int_type level, int_type xoffset, int_type yoffset, sizei_type width, sizei_type height, enum_type format, enum_type type, void const* pixels) -> void;
        extern auto (*texture_sub_image3d) (uint_type texture, int_type level, int_type xoffset, int_type yoffset, int_type zoffset, sizei_type width, sizei_type height, sizei_type depth, enum_type format, enum_type type, void const* pixels) -> void;
        extern auto (*transform_feedback_buffer_base) (uint_type xfb, uint_type index, uint_type buffer) -> void;
        extern auto (*transform_feedback_buffer_range) (uint_type xfb, uint_type index, uint_type buffer, intptr_type offset, sizeiptr_type size) -> void;
        extern auto (*unmap_named_buffer) (uint_type buffer) -> boolean_type;
        extern auto (*vertex_array_attrib_binding) (uint_type vaobj, uint_type attribindex, uint_type bindingindex) -> void;
        extern auto (*vertex_array_attrib_format) (uint_type vaobj, uint_type attribindex, int_type size, enum_type type, boolean_type normalized, uint_type relativeoffset) -> void;
        extern auto (*vertex_array_attrib_i_format) (uint_type vaobj, uint_type attribindex, int_type size, enum_type type, uint_type relativeoffset) -> void;
        extern auto (*vertex_array_attrib_l_format) (uint_type vaobj, uint_type attribindex, int_type size, enum_type type, uint_type relativeoffset) -> void;
        extern auto (*vertex_array_binding_divisor) (uint_type vaobj, uint_type bindingindex, uint_type divisor) -> void;
        extern auto (*vertex_array_element_buffer) (uint_type vaobj, uint_type buffer) -> void;
        extern auto (*vertex_array_vertex_buffer) (uint_type vaobj, uint_type bindingindex, uint_type buffer, intptr_type offset, sizei_type stride) -> void;
        extern auto (*vertex_array_vertex_buffers) (uint_type vaobj, uint_type first, sizei_type count, uint_type const* buffers, intptr_type const* offsets, sizei_type const* strides) -> void;
    }
    using namespace funcs;

    namespace loader
    {
        bool load();
    }
    using namespace loader;
}

