#pragma once

#if defined(__glew_h__) || defined(__GLEW_H__)
	#error Attempt to include auto-generated header after including glew.h
#endif
#if defined(__gl_h_) || defined(__GL_H__)
	#error Attempt to include auto-generated header after including gl.h
#endif
#if defined(__glext_h_) || defined(__GLEXT_H_)
	#error Attempt to include auto-generated header after including glext.h
#endif
#if defined(__gltypes_h_)
	#error Attempt to include auto-generated header after gltypes.h
#endif
#if defined(__gl_ATI_h_)
	#error Attempt to include auto-generated header after including glATI.h
#endif

#define __glew_h__
#define __GLEW_H__
#define __gl_h_
#define __GL_H__
#define __glext_h_
#define __GLEXT_H_
#define __gltypes_h_
#define __gl_ATI_h_

#ifndef APIENTRY
	#if defined(__MINGW32__)
		#ifndef WIN32_LEAN_AND_MEAN
			#define WIN32_LEAN_AND_MEAN 1
		#endif
		#ifndef NOMINMAX
			#define NOMINMAX
		#endif
		#include <windows.h>
	#elif (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__)
		#ifndef WIN32_LEAN_AND_MEAN
			#define WIN32_LEAN_AND_MEAN 1
		#endif
		#ifndef NOMINMAX
			#define NOMINMAX
		#endif
		#include <windows.h>
	#else
		#define APIENTRY
	#endif
#endif /*APIENTRY*/

#ifndef CODEGEN_FUNCPTR
	#define CODEGEN_REMOVE_FUNCPTR
	#if defined(_WIN32)
		#define CODEGEN_FUNCPTR APIENTRY
	#else
		#define CODEGEN_FUNCPTR
	#endif
#endif /*CODEGEN_FUNCPTR*/

#ifndef GLAPI
	#define GLAPI extern
#endif




#include <stddef.h>
#ifndef GLEXT_64_TYPES_DEFINED
	/* This code block is duplicated in glxext.h, so must be protected */
	#define GLEXT_64_TYPES_DEFINED
	/* Define int32_t, int64_t, and uint64_t types for UST/MSC */
	/* (as used in the __typeEXT_timer_query extension). */
	#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
		#include <inttypes.h>
	#elif defined(__sun__) || defined(__digital__)
		#include <inttypes.h>
		#if defined(__STDC__)
			#if defined(__arch64__) || defined(_LP64)
				using int64_t = long int;
				using uint64_t = unsigned long int;
			#else
				using int64_t = long long int;
				using uint64_t = unsigned long long int;
			#endif /* __arch64__ */
		#endif /* __STDC__ */
	#elif defined( __VMS ) || defined(__sgi)
		#include <inttypes.h>
	#elif defined(__SCO__) || defined(__USLC__)
		#include <stdint.h>
	#elif defined(__UNIXOS2__) || defined(__SOL64__)
		using int32_t = long int;
		using int64_t = long long int;
		using uint64_t = unsigned long long int;
	#elif defined(_WIN32) && defined(__GNUC__)
		#include <stdint.h>
	#elif defined(_WIN32)
		using int32_t = __int32;
		using int64_t = __int64;
		using uint64_t = unsigned __int64;
	#else
		/* Fallback if nothing above works */
		#include <inttypes.h>
	#endif
#endif
#ifdef __APPLE__
#else
#endif

namespace gl
{
	namespace enums
	{
		enum enum_type : unsigned int
		{
			alpha                            = 0x1906,
			always                           = 0x0207,
			and_                             = 0x1501,
			and_inverted                     = 0x1504,
			and_reverse                      = 0x1502,
			back                             = 0x0405,
			back_left                        = 0x0402,
			back_right                       = 0x0403,
			blend                            = 0x0BE2,
			blend_dst                        = 0x0BE0,
			blend_src                        = 0x0BE1,
			blue                             = 0x1905,
			byte                             = 0x1400,
			ccw                              = 0x0901,
			clear                            = 0x1500,
			color                            = 0x1800,
			color_buffer_bit                 = 0x00004000,
			color_clear_value                = 0x0C22,
			color_logic_op                   = 0x0BF2,
			color_writemask                  = 0x0C23,
			copy                             = 0x1503,
			copy_inverted                    = 0x150C,
			cull_face                        = 0x0B44,
			cull_face_mode                   = 0x0B45,
			cw                               = 0x0900,
			decr                             = 0x1E03,
			depth                            = 0x1801,
			depth_buffer_bit                 = 0x00000100,
			depth_clear_value                = 0x0B73,
			depth_component                  = 0x1902,
			depth_func                       = 0x0B74,
			depth_range                      = 0x0B70,
			depth_test                       = 0x0B71,
			depth_writemask                  = 0x0B72,
			dither                           = 0x0BD0,
			dont_care                        = 0x1100,
			double_                          = 0x140A,
			doublebuffer                     = 0x0C32,
			draw_buffer                      = 0x0C01,
			dst_alpha                        = 0x0304,
			dst_color                        = 0x0306,
			equal                            = 0x0202,
			equiv                            = 0x1509,
			extensions                       = 0x1F03,
			false_                           = 0,
			fastest                          = 0x1101,
			fill                             = 0x1B02,
			float_                           = 0x1406,
			front                            = 0x0404,
			front_and_back                   = 0x0408,
			front_face                       = 0x0B46,
			front_left                       = 0x0400,
			front_right                      = 0x0401,
			gequal                           = 0x0206,
			greater                          = 0x0204,
			green                            = 0x1904,
			incr                             = 0x1E02,
			int_                             = 0x1404,
			invalid_enum                     = 0x0500,
			invalid_operation                = 0x0502,
			invalid_value                    = 0x0501,
			invert                           = 0x150A,
			keep                             = 0x1E00,
			left                             = 0x0406,
			lequal                           = 0x0203,
			less                             = 0x0201,
			line                             = 0x1B01,
			linear                           = 0x2601,
			linear_mipmap_linear             = 0x2703,
			linear_mipmap_nearest            = 0x2701,
			lines                            = 0x0001,
			line_loop                        = 0x0002,
			line_smooth                      = 0x0B20,
			line_smooth_hint                 = 0x0C52,
			line_strip                       = 0x0003,
			line_width                       = 0x0B21,
			line_width_granularity           = 0x0B23,
			line_width_range                 = 0x0B22,
			logic_op_mode                    = 0x0BF0,
			max_texture_size                 = 0x0D33,
			max_viewport_dims                = 0x0D3A,
			nand                             = 0x150E,
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
			or_inverted                      = 0x150D,
			or_reverse                       = 0x150B,
			out_of_memory                    = 0x0505,
			pack_alignment                   = 0x0D05,
			pack_lsb_first                   = 0x0D01,
			pack_row_length                  = 0x0D02,
			pack_skip_pixels                 = 0x0D04,
			pack_skip_rows                   = 0x0D03,
			pack_swap_bytes                  = 0x0D00,
			point                            = 0x1B00,
			points                           = 0x0000,
			point_size                       = 0x0B11,
			point_size_granularity           = 0x0B13,
			point_size_range                 = 0x0B12,
			polygon_mode                     = 0x0B40,
			polygon_offset_factor            = 0x8038,
			polygon_offset_fill              = 0x8037,
			polygon_offset_line              = 0x2A02,
			polygon_offset_point             = 0x2A01,
			polygon_offset_units             = 0x2A00,
			polygon_smooth                   = 0x0B41,
			polygon_smooth_hint              = 0x0C53,
			proxy_texture_1d                 = 0x8063,
			proxy_texture_2d                 = 0x8064,
			quads                            = 0x0007,
			r3_g3_b2                         = 0x2A10,
			read_buffer                      = 0x0C02,
			red                              = 0x1903,
			renderer                         = 0x1F01,
			repeat                           = 0x2901,
			replace                          = 0x1E01,
			rgb                              = 0x1907,
			rgb10                            = 0x8052,
			rgb10_a2                         = 0x8059,
			rgb12                            = 0x8053,
			rgb16                            = 0x8054,
			rgb4                             = 0x804F,
			rgb5                             = 0x8050,
			rgb5_a1                          = 0x8057,
			rgb8                             = 0x8051,
			rgba                             = 0x1908,
			rgba12                           = 0x805A,
			rgba16                           = 0x805B,
			rgba2                            = 0x8055,
			rgba4                            = 0x8056,
			rgba8                            = 0x8058,
			right                            = 0x0407,
			scissor_box                      = 0x0C10,
			scissor_test                     = 0x0C11,
			set                              = 0x150F,
			short_                           = 0x1402,
			src_alpha                        = 0x0302,
			src_alpha_saturate               = 0x0308,
			src_color                        = 0x0300,
			stack_overflow                   = 0x0503,
			stack_underflow                  = 0x0504,
			stencil                          = 0x1802,
			stencil_buffer_bit               = 0x00000400,
			stencil_clear_value              = 0x0B91,
			stencil_fail                     = 0x0B94,
			stencil_func                     = 0x0B92,
			stencil_index                    = 0x1901,
			stencil_pass_depth_fail          = 0x0B95,
			stencil_pass_depth_pass          = 0x0B96,
			stencil_ref                      = 0x0B97,
			stencil_test                     = 0x0B90,
			stencil_value_mask               = 0x0B93,
			stencil_writemask                = 0x0B98,
			stereo                           = 0x0C33,
			subpixel_bits                    = 0x0D50,
			texture                          = 0x1702,
			texture_1d                       = 0x0DE0,
			texture_2d                       = 0x0DE1,
			texture_alpha_size               = 0x805F,
			texture_binding_1d               = 0x8068,
			texture_binding_2d               = 0x8069,
			texture_blue_size                = 0x805E,
			texture_border_color             = 0x1004,
			texture_green_size               = 0x805D,
			texture_height                   = 0x1001,
			texture_internal_format          = 0x1003,
			texture_mag_filter               = 0x2800,
			texture_min_filter               = 0x2801,
			texture_red_size                 = 0x805C,
			texture_width                    = 0x1000,
			texture_wrap_s                   = 0x2802,
			texture_wrap_t                   = 0x2803,
			triangles                        = 0x0004,
			triangle_fan                     = 0x0006,
			triangle_strip                   = 0x0005,
			true_                            = 1,
			unpack_alignment                 = 0x0CF5,
			unpack_lsb_first                 = 0x0CF1,
			unpack_row_length                = 0x0CF2,
			unpack_skip_pixels               = 0x0CF4,
			unpack_skip_rows                 = 0x0CF3,
			unpack_swap_bytes                = 0x0CF0,
			unsigned_byte                    = 0x1401,
			unsigned_int                     = 0x1405,
			unsigned_short                   = 0x1403,
			vendor                           = 0x1F00,
			version                          = 0x1F02,
			vertex_array                     = 0x8074,
			viewport                         = 0x0BA2,
			xor_                             = 0x1506,
			zero                             = 0,

			aliased_line_width_range         = 0x846E,
			bgr                              = 0x80E0,
			bgra                             = 0x80E1,
			clamp_to_edge                    = 0x812F,
			max_3d_texture_size              = 0x8073,
			max_elements_indices             = 0x80E9,
			max_elements_vertices            = 0x80E8,
			pack_image_height                = 0x806C,
			pack_skip_images                 = 0x806B,
			proxy_texture_3d                 = 0x8070,
			smooth_line_width_granularity    = 0x0B23,
			smooth_line_width_range          = 0x0B22,
			smooth_point_size_granularity    = 0x0B13,
			smooth_point_size_range          = 0x0B12,
			texture_3d                       = 0x806F,
			texture_base_level               = 0x813C,
			texture_binding_3d               = 0x806A,
			texture_depth                    = 0x8071,
			texture_max_level                = 0x813D,
			texture_max_lod                  = 0x813B,
			texture_min_lod                  = 0x813A,
			texture_wrap_r                   = 0x8072,
			unpack_image_height              = 0x806E,
			unpack_skip_images               = 0x806D,
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

			active_texture                   = 0x84E0,
			clamp_to_border                  = 0x812D,
			compressed_rgb                   = 0x84ED,
			compressed_rgba                  = 0x84EE,
			compressed_texture_formats       = 0x86A3,
			max_cube_map_texture_size        = 0x851C,
			multisample                      = 0x809D,
			num_compressed_texture_formats   = 0x86A2,
			proxy_texture_cube_map           = 0x851B,
			samples                          = 0x80A9,
			sample_alpha_to_coverage         = 0x809E,
			sample_alpha_to_one              = 0x809F,
			sample_buffers                   = 0x80A8,
			sample_coverage                  = 0x80A0,
			sample_coverage_invert           = 0x80AB,
			sample_coverage_value            = 0x80AA,
			texture0                         = 0x84C0,
			texture1                         = 0x84C1,
			texture10                        = 0x84CA,
			texture11                        = 0x84CB,
			texture12                        = 0x84CC,
			texture13                        = 0x84CD,
			texture14                        = 0x84CE,
			texture15                        = 0x84CF,
			texture16                        = 0x84D0,
			texture17                        = 0x84D1,
			texture18                        = 0x84D2,
			texture19                        = 0x84D3,
			texture2                         = 0x84C2,
			texture20                        = 0x84D4,
			texture21                        = 0x84D5,
			texture22                        = 0x84D6,
			texture23                        = 0x84D7,
			texture24                        = 0x84D8,
			texture25                        = 0x84D9,
			texture26                        = 0x84DA,
			texture27                        = 0x84DB,
			texture28                        = 0x84DC,
			texture29                        = 0x84DD,
			texture3                         = 0x84C3,
			texture30                        = 0x84DE,
			texture31                        = 0x84DF,
			texture4                         = 0x84C4,
			texture5                         = 0x84C5,
			texture6                         = 0x84C6,
			texture7                         = 0x84C7,
			texture8                         = 0x84C8,
			texture9                         = 0x84C9,
			texture_binding_cube_map         = 0x8514,
			texture_compressed               = 0x86A1,
			texture_compressed_image_size    = 0x86A0,
			texture_compression_hint         = 0x84EF,
			texture_cube_map                 = 0x8513,
			texture_cube_map_negative_x      = 0x8516,
			texture_cube_map_negative_y      = 0x8518,
			texture_cube_map_negative_z      = 0x851A,
			texture_cube_map_positive_x      = 0x8515,
			texture_cube_map_positive_y      = 0x8517,
			texture_cube_map_positive_z      = 0x8519,

			blend_color                      = 0x8005,
			blend_dst_alpha                  = 0x80CA,
			blend_dst_rgb                    = 0x80C8,
			blend_src_alpha                  = 0x80CB,
			blend_src_rgb                    = 0x80C9,
			constant_alpha                   = 0x8003,
			constant_color                   = 0x8001,
			decr_wrap                        = 0x8508,
			depth_component16                = 0x81A5,
			depth_component24                = 0x81A6,
			depth_component32                = 0x81A7,
			func_add                         = 0x8006,
			func_reverse_subtract            = 0x800B,
			func_subtract                    = 0x800A,
			incr_wrap                        = 0x8507,
			max                              = 0x8008,
			max_texture_lod_bias             = 0x84FD,
			min                              = 0x8007,
			mirrored_repeat                  = 0x8370,
			one_minus_constant_alpha         = 0x8004,
			one_minus_constant_color         = 0x8002,
			point_fade_threshold_size        = 0x8128,
			texture_compare_func             = 0x884D,
			texture_compare_mode             = 0x884C,
			texture_depth_size               = 0x884A,
			texture_lod_bias                 = 0x8501,

			array_buffer                     = 0x8892,
			array_buffer_binding             = 0x8894,
			buffer_access                    = 0x88BB,
			buffer_mapped                    = 0x88BC,
			buffer_map_pointer               = 0x88BD,
			buffer_size                      = 0x8764,
			buffer_usage                     = 0x8765,
			current_query                    = 0x8865,
			dynamic_copy                     = 0x88EA,
			dynamic_draw                     = 0x88E8,
			dynamic_read                     = 0x88E9,
			element_array_buffer             = 0x8893,
			element_array_buffer_binding     = 0x8895,
			query_counter_bits               = 0x8864,
			query_result                     = 0x8866,
			query_result_available           = 0x8867,
			read_only                        = 0x88B8,
			read_write                       = 0x88BA,
			samples_passed                   = 0x8914,
			src1_alpha                       = 0x8589,
			static_copy                      = 0x88E6,
			static_draw                      = 0x88E4,
			static_read                      = 0x88E5,
			stream_copy                      = 0x88E2,
			stream_draw                      = 0x88E0,
			stream_read                      = 0x88E1,
			vertex_attrib_array_buffer_binding = 0x889F,
			write_only                       = 0x88B9,

			active_attributes                = 0x8B89,
			active_attribute_max_length      = 0x8B8A,
			active_uniforms                  = 0x8B86,
			active_uniform_max_length        = 0x8B87,
			attached_shaders                 = 0x8B85,
			blend_equation_alpha             = 0x883D,
			blend_equation_rgb               = 0x8009,
			bool_                            = 0x8B56,
			bool_vec2                        = 0x8B57,
			bool_vec3                        = 0x8B58,
			bool_vec4                        = 0x8B59,
			compile_status                   = 0x8B81,
			current_program                  = 0x8B8D,
			current_vertex_attrib            = 0x8626,
			delete_status                    = 0x8B80,
			draw_buffer0                     = 0x8825,
			draw_buffer1                     = 0x8826,
			draw_buffer10                    = 0x882F,
			draw_buffer11                    = 0x8830,
			draw_buffer12                    = 0x8831,
			draw_buffer13                    = 0x8832,
			draw_buffer14                    = 0x8833,
			draw_buffer15                    = 0x8834,
			draw_buffer2                     = 0x8827,
			draw_buffer3                     = 0x8828,
			draw_buffer4                     = 0x8829,
			draw_buffer5                     = 0x882A,
			draw_buffer6                     = 0x882B,
			draw_buffer7                     = 0x882C,
			draw_buffer8                     = 0x882D,
			draw_buffer9                     = 0x882E,
			float_mat2                       = 0x8B5A,
			float_mat3                       = 0x8B5B,
			float_mat4                       = 0x8B5C,
			float_vec2                       = 0x8B50,
			float_vec3                       = 0x8B51,
			float_vec4                       = 0x8B52,
			fragment_shader                  = 0x8B30,
			fragment_shader_derivative_hint  = 0x8B8B,
			info_log_length                  = 0x8B84,
			int_vec2                         = 0x8B53,
			int_vec3                         = 0x8B54,
			int_vec4                         = 0x8B55,
			link_status                      = 0x8B82,
			lower_left                       = 0x8CA1,
			max_combined_texture_image_units = 0x8B4D,
			max_draw_buffers                 = 0x8824,
			max_fragment_uniform_components  = 0x8B49,
			max_texture_image_units          = 0x8872,
			max_varying_floats               = 0x8B4B,
			max_vertex_attribs               = 0x8869,
			max_vertex_texture_image_units   = 0x8B4C,
			max_vertex_uniform_components    = 0x8B4A,
			point_sprite_coord_origin        = 0x8CA0,
			sampler_1d                       = 0x8B5D,
			sampler_1d_shadow                = 0x8B61,
			sampler_2d                       = 0x8B5E,
			sampler_2d_shadow                = 0x8B62,
			sampler_3d                       = 0x8B5F,
			sampler_cube                     = 0x8B60,
			shader_source_length             = 0x8B88,
			shader_type                      = 0x8B4F,
			shading_language_version         = 0x8B8C,
			stencil_back_fail                = 0x8801,
			stencil_back_func                = 0x8800,
			stencil_back_pass_depth_fail     = 0x8802,
			stencil_back_pass_depth_pass     = 0x8803,
			stencil_back_ref                 = 0x8CA3,
			stencil_back_value_mask          = 0x8CA4,
			stencil_back_writemask           = 0x8CA5,
			upper_left                       = 0x8CA2,
			validate_status                  = 0x8B83,
			vertex_attrib_array_enabled      = 0x8622,
			vertex_attrib_array_normalized   = 0x886A,
			vertex_attrib_array_pointer      = 0x8645,
			vertex_attrib_array_size         = 0x8623,
			vertex_attrib_array_stride       = 0x8624,
			vertex_attrib_array_type         = 0x8625,
			vertex_program_point_size        = 0x8642,
			vertex_shader                    = 0x8B31,

			compressed_srgb                  = 0x8C48,
			compressed_srgb_alpha            = 0x8C49,
			float_mat2x3                     = 0x8B65,
			float_mat2x4                     = 0x8B66,
			float_mat3x2                     = 0x8B67,
			float_mat3x4                     = 0x8B68,
			float_mat4x2                     = 0x8B69,
			float_mat4x3                     = 0x8B6A,
			pixel_pack_buffer                = 0x88EB,
			pixel_pack_buffer_binding        = 0x88ED,
			pixel_unpack_buffer              = 0x88EC,
			pixel_unpack_buffer_binding      = 0x88EF,
			srgb                             = 0x8C40,
			srgb8                            = 0x8C41,
			srgb8_alpha8                     = 0x8C43,
			srgb_alpha                       = 0x8C42,

			bgra_integer                     = 0x8D9B,
			bgr_integer                      = 0x8D9A,
			blue_integer                     = 0x8D96,
			buffer_access_flags              = 0x911F,
			buffer_map_length                = 0x9120,
			buffer_map_offset                = 0x9121,
			clamp_read_color                 = 0x891C,
			clip_distance0                   = 0x3000,
			clip_distance1                   = 0x3001,
			clip_distance2                   = 0x3002,
			clip_distance3                   = 0x3003,
			clip_distance4                   = 0x3004,
			clip_distance5                   = 0x3005,
			clip_distance6                   = 0x3006,
			clip_distance7                   = 0x3007,
			color_attachment0                = 0x8CE0,
			color_attachment1                = 0x8CE1,
			color_attachment10               = 0x8CEA,
			color_attachment11               = 0x8CEB,
			color_attachment12               = 0x8CEC,
			color_attachment13               = 0x8CED,
			color_attachment14               = 0x8CEE,
			color_attachment15               = 0x8CEF,
			color_attachment16               = 0x8CF0,
			color_attachment17               = 0x8CF1,
			color_attachment18               = 0x8CF2,
			color_attachment19               = 0x8CF3,
			color_attachment2                = 0x8CE2,
			color_attachment20               = 0x8CF4,
			color_attachment21               = 0x8CF5,
			color_attachment22               = 0x8CF6,
			color_attachment23               = 0x8CF7,
			color_attachment24               = 0x8CF8,
			color_attachment25               = 0x8CF9,
			color_attachment26               = 0x8CFA,
			color_attachment27               = 0x8CFB,
			color_attachment28               = 0x8CFC,
			color_attachment29               = 0x8CFD,
			color_attachment3                = 0x8CE3,
			color_attachment30               = 0x8CFE,
			color_attachment31               = 0x8CFF,
			color_attachment4                = 0x8CE4,
			color_attachment5                = 0x8CE5,
			color_attachment6                = 0x8CE6,
			color_attachment7                = 0x8CE7,
			color_attachment8                = 0x8CE8,
			color_attachment9                = 0x8CE9,
			compare_ref_to_texture           = 0x884E,
			compressed_red                   = 0x8225,
			compressed_red_rgtc1             = 0x8DBB,
			compressed_rg                    = 0x8226,
			compressed_rg_rgtc2              = 0x8DBD,
			compressed_signed_red_rgtc1      = 0x8DBC,
			compressed_signed_rg_rgtc2       = 0x8DBE,
			context_flags                    = 0x821E,
			context_flag_forward_compatible_bit = 0x00000001,
			depth24_stencil8                 = 0x88F0,
			depth32f_stencil8                = 0x8CAD,
			depth_attachment                 = 0x8D00,
			depth_component32f               = 0x8CAC,
			depth_stencil                    = 0x84F9,
			depth_stencil_attachment         = 0x821A,
			draw_framebuffer                 = 0x8CA9,
			draw_framebuffer_binding         = 0x8CA6,
			fixed_only                       = 0x891D,
			float_32_unsigned_int_24_8_rev   = 0x8DAD,
			framebuffer                      = 0x8D40,
			framebuffer_attachment_alpha_size = 0x8215,
			framebuffer_attachment_blue_size = 0x8214,
			framebuffer_attachment_color_encoding = 0x8210,
			framebuffer_attachment_component_type = 0x8211,
			framebuffer_attachment_depth_size = 0x8216,
			framebuffer_attachment_green_size = 0x8213,
			framebuffer_attachment_object_name = 0x8CD1,
			framebuffer_attachment_object_type = 0x8CD0,
			framebuffer_attachment_red_size  = 0x8212,
			framebuffer_attachment_stencil_size = 0x8217,
			framebuffer_attachment_texture_cube_map_face = 0x8CD3,
			framebuffer_attachment_texture_layer = 0x8CD4,
			framebuffer_attachment_texture_level = 0x8CD2,
			framebuffer_binding              = 0x8CA6,
			framebuffer_complete             = 0x8CD5,
			framebuffer_default              = 0x8218,
			framebuffer_incomplete_attachment = 0x8CD6,
			framebuffer_incomplete_draw_buffer = 0x8CDB,
			framebuffer_incomplete_missing_attachment = 0x8CD7,
			framebuffer_incomplete_multisample = 0x8D56,
			framebuffer_incomplete_read_buffer = 0x8CDC,
			framebuffer_srgb                 = 0x8DB9,
			framebuffer_undefined            = 0x8219,
			framebuffer_unsupported          = 0x8CDD,
			green_integer                    = 0x8D95,
			half_float                       = 0x140B,
			interleaved_attribs              = 0x8C8C,
			int_sampler_1d                   = 0x8DC9,
			int_sampler_1d_array             = 0x8DCE,
			int_sampler_2d                   = 0x8DCA,
			int_sampler_2d_array             = 0x8DCF,
			int_sampler_3d                   = 0x8DCB,
			int_sampler_cube                 = 0x8DCC,
			invalid_framebuffer_operation    = 0x0506,
			major_version                    = 0x821B,
			map_flush_explicit_bit           = 0x0010,
			map_invalidate_buffer_bit        = 0x0008,
			map_invalidate_range_bit         = 0x0004,
			map_read_bit                     = 0x0001,
			map_unsynchronized_bit           = 0x0020,
			map_write_bit                    = 0x0002,
			max_array_texture_layers         = 0x88FF,
			max_clip_distances               = 0x0D32,
			max_color_attachments            = 0x8CDF,
			max_program_texel_offset         = 0x8905,
			max_renderbuffer_size            = 0x84E8,
			max_samples                      = 0x8D57,
			max_transform_feedback_interleaved_components = 0x8C8A,
			max_transform_feedback_separate_attribs = 0x8C8B,
			max_transform_feedback_separate_components = 0x8C80,
			max_varying_components           = 0x8B4B,
			minor_version                    = 0x821C,
			min_program_texel_offset         = 0x8904,
			num_extensions                   = 0x821D,
			primitives_generated             = 0x8C87,
			proxy_texture_1d_array           = 0x8C19,
			proxy_texture_2d_array           = 0x8C1B,
			query_by_region_no_wait          = 0x8E16,
			query_by_region_wait             = 0x8E15,
			query_no_wait                    = 0x8E14,
			query_wait                       = 0x8E13,
			r11f_g11f_b10f                   = 0x8C3A,
			r16                              = 0x822A,
			r16f                             = 0x822D,
			r16i                             = 0x8233,
			r16ui                            = 0x8234,
			r32f                             = 0x822E,
			r32i                             = 0x8235,
			r32ui                            = 0x8236,
			r8                               = 0x8229,
			r8i                              = 0x8231,
			r8ui                             = 0x8232,
			rasterizer_discard               = 0x8C89,
			read_framebuffer                 = 0x8CA8,
			read_framebuffer_binding         = 0x8CAA,
			red_integer                      = 0x8D94,
			renderbuffer                     = 0x8D41,
			renderbuffer_alpha_size          = 0x8D53,
			renderbuffer_binding             = 0x8CA7,
			renderbuffer_blue_size           = 0x8D52,
			renderbuffer_depth_size          = 0x8D54,
			renderbuffer_green_size          = 0x8D51,
			renderbuffer_height              = 0x8D43,
			renderbuffer_internal_format     = 0x8D44,
			renderbuffer_red_size            = 0x8D50,
			renderbuffer_samples             = 0x8CAB,
			renderbuffer_stencil_size        = 0x8D55,
			renderbuffer_width               = 0x8D42,
			rg                               = 0x8227,
			rg16                             = 0x822C,
			rg16f                            = 0x822F,
			rg16i                            = 0x8239,
			rg16ui                           = 0x823A,
			rg32f                            = 0x8230,
			rg32i                            = 0x823B,
			rg32ui                           = 0x823C,
			rg8                              = 0x822B,
			rg8i                             = 0x8237,
			rg8ui                            = 0x8238,
			rgb16f                           = 0x881B,
			rgb16i                           = 0x8D89,
			rgb16ui                          = 0x8D77,
			rgb32f                           = 0x8815,
			rgb32i                           = 0x8D83,
			rgb32ui                          = 0x8D71,
			rgb8i                            = 0x8D8F,
			rgb8ui                           = 0x8D7D,
			rgb9_e5                          = 0x8C3D,
			rgba16f                          = 0x881A,
			rgba16i                          = 0x8D88,
			rgba16ui                         = 0x8D76,
			rgba32f                          = 0x8814,
			rgba32i                          = 0x8D82,
			rgba32ui                         = 0x8D70,
			rgba8i                           = 0x8D8E,
			rgba8ui                          = 0x8D7C,
			rgba_integer                     = 0x8D99,
			rgb_integer                      = 0x8D98,
			rg_integer                       = 0x8228,
			sampler_1d_array                 = 0x8DC0,
			sampler_1d_array_shadow          = 0x8DC3,
			sampler_2d_array                 = 0x8DC1,
			sampler_2d_array_shadow          = 0x8DC4,
			sampler_cube_shadow              = 0x8DC5,
			separate_attribs                 = 0x8C8D,
			stencil_attachment               = 0x8D20,
			stencil_index1                   = 0x8D46,
			stencil_index16                  = 0x8D49,
			stencil_index4                   = 0x8D47,
			stencil_index8                   = 0x8D48,
			texture_1d_array                 = 0x8C18,
			texture_2d_array                 = 0x8C1A,
			texture_alpha_type               = 0x8C13,
			texture_binding_1d_array         = 0x8C1C,
			texture_binding_2d_array         = 0x8C1D,
			texture_blue_type                = 0x8C12,
			texture_depth_type               = 0x8C16,
			texture_green_type               = 0x8C11,
			texture_red_type                 = 0x8C10,
			texture_shared_size              = 0x8C3F,
			texture_stencil_size             = 0x88F1,
			transform_feedback_buffer        = 0x8C8E,
			transform_feedback_buffer_binding = 0x8C8F,
			transform_feedback_buffer_mode   = 0x8C7F,
			transform_feedback_buffer_size   = 0x8C85,
			transform_feedback_buffer_start  = 0x8C84,
			transform_feedback_primitives_written = 0x8C88,
			transform_feedback_varyings      = 0x8C83,
			transform_feedback_varying_max_length = 0x8C76,
			unsigned_int_10f_11f_11f_rev     = 0x8C3B,
			unsigned_int_24_8                = 0x84FA,
			unsigned_int_5_9_9_9_rev         = 0x8C3E,
			unsigned_int_sampler_1d          = 0x8DD1,
			unsigned_int_sampler_1d_array    = 0x8DD6,
			unsigned_int_sampler_2d          = 0x8DD2,
			unsigned_int_sampler_2d_array    = 0x8DD7,
			unsigned_int_sampler_3d          = 0x8DD3,
			unsigned_int_sampler_cube        = 0x8DD4,
			unsigned_int_vec2                = 0x8DC6,
			unsigned_int_vec3                = 0x8DC7,
			unsigned_int_vec4                = 0x8DC8,
			unsigned_normalized              = 0x8C17,
			vertex_array_binding             = 0x85B5,
			vertex_attrib_array_integer      = 0x88FD,

			active_uniform_blocks            = 0x8A36,
			active_uniform_block_max_name_length = 0x8A35,
			copy_read_buffer                 = 0x8F36,
			copy_write_buffer                = 0x8F37,
			int_sampler_2d_rect              = 0x8DCD,
			int_sampler_buffer               = 0x8DD0,
			invalid_index                    = 0xFFFFFFFF,
			max_combined_fragment_uniform_components = 0x8A33,
			max_combined_geometry_uniform_components = 0x8A32,
			max_combined_uniform_blocks      = 0x8A2E,
			max_combined_vertex_uniform_components = 0x8A31,
			max_fragment_uniform_blocks      = 0x8A2D,
			max_geometry_uniform_blocks      = 0x8A2C,
			max_rectangle_texture_size       = 0x84F8,
			max_texture_buffer_size          = 0x8C2B,
			max_uniform_block_size           = 0x8A30,
			max_uniform_buffer_bindings      = 0x8A2F,
			max_vertex_uniform_blocks        = 0x8A2B,
			primitive_restart                = 0x8F9D,
			primitive_restart_index          = 0x8F9E,
			proxy_texture_rectangle          = 0x84F7,
			r16_snorm                        = 0x8F98,
			r8_snorm                         = 0x8F94,
			rg16_snorm                       = 0x8F99,
			rg8_snorm                        = 0x8F95,
			rgb16_snorm                      = 0x8F9A,
			rgb8_snorm                       = 0x8F96,
			rgba16_snorm                     = 0x8F9B,
			rgba8_snorm                      = 0x8F97,
			sampler_2d_rect                  = 0x8B63,
			sampler_2d_rect_shadow           = 0x8B64,
			sampler_buffer                   = 0x8DC2,
			signed_normalized                = 0x8F9C,
			texture_binding_buffer           = 0x8C2C,
			texture_binding_rectangle        = 0x84F6,
			texture_buffer                   = 0x8C2A,
			texture_buffer_data_store_binding = 0x8C2D,
			texture_rectangle                = 0x84F5,
			uniform_array_stride             = 0x8A3C,
			uniform_block_active_uniforms    = 0x8A42,
			uniform_block_active_uniform_indices = 0x8A43,
			uniform_block_binding            = 0x8A3F,
			uniform_block_data_size          = 0x8A40,
			uniform_block_index              = 0x8A3A,
			uniform_block_name_length        = 0x8A41,
			uniform_block_referenced_by_fragment_shader = 0x8A46,
			uniform_block_referenced_by_geometry_shader = 0x8A45,
			uniform_block_referenced_by_vertex_shader = 0x8A44,
			uniform_buffer                   = 0x8A11,
			uniform_buffer_binding           = 0x8A28,
			uniform_buffer_offset_alignment  = 0x8A34,
			uniform_buffer_size              = 0x8A2A,
			uniform_buffer_start             = 0x8A29,
			uniform_is_row_major             = 0x8A3E,
			uniform_matrix_stride            = 0x8A3D,
			uniform_name_length              = 0x8A39,
			uniform_offset                   = 0x8A3B,
			uniform_size                     = 0x8A38,
			uniform_type                     = 0x8A37,
			unsigned_int_sampler_2d_rect     = 0x8DD5,
			unsigned_int_sampler_buffer      = 0x8DD8,

			already_signaled                 = 0x911A,
			condition_satisfied              = 0x911C,
			context_compatibility_profile_bit = 0x00000002,
			context_core_profile_bit         = 0x00000001,
			context_profile_mask             = 0x9126,
			depth_clamp                      = 0x864F,
			first_vertex_convention          = 0x8E4D,
			framebuffer_attachment_layered   = 0x8DA7,
			framebuffer_incomplete_layer_targets = 0x8DA8,
			geometry_input_type              = 0x8917,
			geometry_output_type             = 0x8918,
			geometry_shader                  = 0x8DD9,
			geometry_vertices_out            = 0x8916,
			int_sampler_2d_multisample       = 0x9109,
			int_sampler_2d_multisample_array = 0x910C,
			last_vertex_convention           = 0x8E4E,
			lines_adjacency                  = 0x000A,
			line_strip_adjacency             = 0x000B,
			max_color_texture_samples        = 0x910E,
			max_depth_texture_samples        = 0x910F,
			max_fragment_input_components    = 0x9125,
			max_geometry_input_components    = 0x9123,
			max_geometry_output_components   = 0x9124,
			max_geometry_output_vertices     = 0x8DE0,
			max_geometry_texture_image_units = 0x8C29,
			max_geometry_total_output_components = 0x8DE1,
			max_geometry_uniform_components  = 0x8DDF,
			max_integer_samples              = 0x9110,
			max_sample_mask_words            = 0x8E59,
			max_server_wait_timeout          = 0x9111,
			max_vertex_output_components     = 0x9122,
			object_type                      = 0x9112,
			program_point_size               = 0x8642,
			provoking_vertex                 = 0x8E4F,
			proxy_texture_2d_multisample     = 0x9101,
			proxy_texture_2d_multisample_array = 0x9103,
			quads_follow_provoking_vertex_convention = 0x8E4C,
			sampler_2d_multisample           = 0x9108,
			sampler_2d_multisample_array     = 0x910B,
			sample_mask                      = 0x8E51,
			sample_mask_value                = 0x8E52,
			sample_position                  = 0x8E50,
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
			texture_cube_map_seamless        = 0x884F,
			texture_fixed_sample_locations   = 0x9107,
			texture_samples                  = 0x9106,
			timeout_expired                  = 0x911B,
			triangles_adjacency              = 0x000C,
			triangle_strip_adjacency         = 0x000D,
			unsignaled                       = 0x9118,
			unsigned_int_sampler_2d_multisample = 0x910A,
			unsigned_int_sampler_2d_multisample_array = 0x910D,
			wait_failed                      = 0x911D,

			any_samples_passed               = 0x8C2F,
			int_2_10_10_10_rev               = 0x8D9F,
			max_dual_source_draw_buffers     = 0x88FC,
			one_minus_src1_alpha             = 0x88FB,
			one_minus_src1_color             = 0x88FA,
			rgb10_a2ui                       = 0x906F,
			sampler_binding                  = 0x8919,
			src1_color                       = 0x88F9,
			texture_swizzle_a                = 0x8E45,
			texture_swizzle_b                = 0x8E44,
			texture_swizzle_g                = 0x8E43,
			texture_swizzle_r                = 0x8E42,
			texture_swizzle_rgba             = 0x8E46,
			timestamp                        = 0x8E28,
			time_elapsed                     = 0x88BF,
			vertex_attrib_array_divisor      = 0x88FE,

			active_subroutines               = 0x8DE5,
			active_subroutine_max_length     = 0x8E48,
			active_subroutine_uniforms       = 0x8DE6,
			active_subroutine_uniform_locations = 0x8E47,
			active_subroutine_uniform_max_length = 0x8E49,
			compatible_subroutines           = 0x8E4B,
			double_mat2                      = 0x8F46,
			double_mat2x3                    = 0x8F49,
			double_mat2x4                    = 0x8F4A,
			double_mat3                      = 0x8F47,
			double_mat3x2                    = 0x8F4B,
			double_mat3x4                    = 0x8F4C,
			double_mat4                      = 0x8F48,
			double_mat4x2                    = 0x8F4D,
			double_mat4x3                    = 0x8F4E,
			double_vec2                      = 0x8FFC,
			double_vec3                      = 0x8FFD,
			double_vec4                      = 0x8FFE,
			draw_indirect_buffer             = 0x8F3F,
			draw_indirect_buffer_binding     = 0x8F43,
			fractional_even                  = 0x8E7C,
			fractional_odd                   = 0x8E7B,
			fragment_interpolation_offset_bits = 0x8E5D,
			geometry_shader_invocations      = 0x887F,
			int_sampler_cube_map_array       = 0x900E,
			isolines                         = 0x8E7A,
			max_combined_tess_control_uniform_components = 0x8E1E,
			max_combined_tess_evaluation_uniform_components = 0x8E1F,
			max_fragment_interpolation_offset = 0x8E5C,
			max_geometry_shader_invocations  = 0x8E5A,
			max_patch_vertices               = 0x8E7D,
			max_program_texture_gather_offset = 0x8E5F,
			max_subroutines                  = 0x8DE7,
			max_subroutine_uniform_locations = 0x8DE8,
			max_tess_control_input_components = 0x886C,
			max_tess_control_output_components = 0x8E83,
			max_tess_control_texture_image_units = 0x8E81,
			max_tess_control_total_output_components = 0x8E85,
			max_tess_control_uniform_blocks  = 0x8E89,
			max_tess_control_uniform_components = 0x8E7F,
			max_tess_evaluation_input_components = 0x886D,
			max_tess_evaluation_output_components = 0x8E86,
			max_tess_evaluation_texture_image_units = 0x8E82,
			max_tess_evaluation_uniform_blocks = 0x8E8A,
			max_tess_evaluation_uniform_components = 0x8E80,
			max_tess_gen_level               = 0x8E7E,
			max_tess_patch_components        = 0x8E84,
			max_transform_feedback_buffers   = 0x8E70,
			max_vertex_streams               = 0x8E71,
			min_fragment_interpolation_offset = 0x8E5B,
			min_program_texture_gather_offset = 0x8E5E,
			min_sample_shading_value         = 0x8C37,
			num_compatible_subroutines       = 0x8E4A,
			patches                          = 0x000E,
			patch_default_inner_level        = 0x8E73,
			patch_default_outer_level        = 0x8E74,
			patch_vertices                   = 0x8E72,
			proxy_texture_cube_map_array     = 0x900B,
			sampler_cube_map_array           = 0x900C,
			sampler_cube_map_array_shadow    = 0x900D,
			sample_shading                   = 0x8C36,
			tess_control_output_vertices     = 0x8E75,
			tess_control_shader              = 0x8E88,
			tess_evaluation_shader           = 0x8E87,
			tess_gen_mode                    = 0x8E76,
			tess_gen_point_mode              = 0x8E79,
			tess_gen_spacing                 = 0x8E77,
			tess_gen_vertex_order            = 0x8E78,
			texture_binding_cube_map_array   = 0x900A,
			texture_cube_map_array           = 0x9009,
			transform_feedback               = 0x8E22,
			transform_feedback_binding       = 0x8E25,
			transform_feedback_buffer_active = 0x8E24,
			transform_feedback_buffer_paused = 0x8E23,
			uniform_block_referenced_by_tess_control_shader = 0x84F0,
			uniform_block_referenced_by_tess_evaluation_shader = 0x84F1,
			unsigned_int_sampler_cube_map_array = 0x900F,

			active_program                   = 0x8259,
			all_shader_bits                  = 0xFFFFFFFF,
			fixed                            = 0x140C,
			fragment_shader_bit              = 0x00000002,
			geometry_shader_bit              = 0x00000004,
			high_float                       = 0x8DF2,
			high_int                         = 0x8DF5,
			implementation_color_read_format = 0x8B9B,
			implementation_color_read_type   = 0x8B9A,
			layer_provoking_vertex           = 0x825E,
			low_float                        = 0x8DF0,
			low_int                          = 0x8DF3,
			max_fragment_uniform_vectors     = 0x8DFD,
			max_varying_vectors              = 0x8DFC,
			max_vertex_uniform_vectors       = 0x8DFB,
			max_viewports                    = 0x825B,
			medium_float                     = 0x8DF1,
			medium_int                       = 0x8DF4,
			num_program_binary_formats       = 0x87FE,
			num_shader_binary_formats        = 0x8DF9,
			program_binary_formats           = 0x87FF,
			program_binary_length            = 0x8741,
			program_binary_retrievable_hint  = 0x8257,
			program_pipeline_binding         = 0x825A,
			program_separable                = 0x8258,
			rgb565                           = 0x8D62,
			shader_binary_formats            = 0x8DF8,
			shader_compiler                  = 0x8DFA,
			tess_control_shader_bit          = 0x00000008,
			tess_evaluation_shader_bit       = 0x00000010,
			undefined_vertex                 = 0x8260,
			vertex_shader_bit                = 0x00000001,
			viewport_bounds_range            = 0x825D,
			viewport_index_provoking_vertex  = 0x825F,
			viewport_subpixel_bits           = 0x825C,

			active_atomic_counter_buffers    = 0x92D9,
			all_barrier_bits                 = 0xFFFFFFFF,
			atomic_counter_barrier_bit       = 0x00001000,
			atomic_counter_buffer            = 0x92C0,
			atomic_counter_buffer_active_atomic_counters = 0x92C5,
			atomic_counter_buffer_active_atomic_counter_indices = 0x92C6,
			atomic_counter_buffer_binding    = 0x92C1,
			atomic_counter_buffer_data_size  = 0x92C4,
			atomic_counter_buffer_referenced_by_fragment_shader = 0x92CB,
			atomic_counter_buffer_referenced_by_geometry_shader = 0x92CA,
			atomic_counter_buffer_referenced_by_tess_control_shader = 0x92C8,
			atomic_counter_buffer_referenced_by_tess_evaluation_shader = 0x92C9,
			atomic_counter_buffer_referenced_by_vertex_shader = 0x92C7,
			atomic_counter_buffer_size       = 0x92C3,
			atomic_counter_buffer_start      = 0x92C2,
			buffer_update_barrier_bit        = 0x00000200,
			command_barrier_bit              = 0x00000040,
			compressed_rgba_bptc_unorm       = 0x8E8C,
			compressed_rgb_bptc_signed_float = 0x8E8E,
			compressed_rgb_bptc_unsigned_float = 0x8E8F,
			compressed_srgb_alpha_bptc_unorm = 0x8E8D,
			copy_read_buffer_binding         = 0x8F36,
			copy_write_buffer_binding        = 0x8F37,
			element_array_barrier_bit        = 0x00000002,
			framebuffer_barrier_bit          = 0x00000400,
			image_1d                         = 0x904C,
			image_1d_array                   = 0x9052,
			image_2d                         = 0x904D,
			image_2d_array                   = 0x9053,
			image_2d_multisample             = 0x9055,
			image_2d_multisample_array       = 0x9056,
			image_2d_rect                    = 0x904F,
			image_3d                         = 0x904E,
			image_binding_access             = 0x8F3E,
			image_binding_format             = 0x906E,
			image_binding_layer              = 0x8F3D,
			image_binding_layered            = 0x8F3C,
			image_binding_level              = 0x8F3B,
			image_binding_name               = 0x8F3A,
			image_buffer                     = 0x9051,
			image_cube                       = 0x9050,
			image_cube_map_array             = 0x9054,
			image_format_compatibility_by_class = 0x90C9,
			image_format_compatibility_by_size = 0x90C8,
			image_format_compatibility_type  = 0x90C7,
			int_image_1d                     = 0x9057,
			int_image_1d_array               = 0x905D,
			int_image_2d                     = 0x9058,
			int_image_2d_array               = 0x905E,
			int_image_2d_multisample         = 0x9060,
			int_image_2d_multisample_array   = 0x9061,
			int_image_2d_rect                = 0x905A,
			int_image_3d                     = 0x9059,
			int_image_buffer                 = 0x905C,
			int_image_cube                   = 0x905B,
			int_image_cube_map_array         = 0x905F,
			max_atomic_counter_buffer_bindings = 0x92DC,
			max_atomic_counter_buffer_size   = 0x92D8,
			max_combined_atomic_counters     = 0x92D7,
			max_combined_atomic_counter_buffers = 0x92D1,
			max_combined_image_uniforms      = 0x90CF,
			max_combined_image_units_and_fragment_outputs = 0x8F39,
			max_fragment_atomic_counters     = 0x92D6,
			max_fragment_atomic_counter_buffers = 0x92D0,
			max_fragment_image_uniforms      = 0x90CE,
			max_geometry_atomic_counters     = 0x92D5,
			max_geometry_atomic_counter_buffers = 0x92CF,
			max_geometry_image_uniforms      = 0x90CD,
			max_image_samples                = 0x906D,
			max_image_units                  = 0x8F38,
			max_tess_control_atomic_counters = 0x92D3,
			max_tess_control_atomic_counter_buffers = 0x92CD,
			max_tess_control_image_uniforms  = 0x90CB,
			max_tess_evaluation_atomic_counters = 0x92D4,
			max_tess_evaluation_atomic_counter_buffers = 0x92CE,
			max_tess_evaluation_image_uniforms = 0x90CC,
			max_vertex_atomic_counters       = 0x92D2,
			max_vertex_atomic_counter_buffers = 0x92CC,
			max_vertex_image_uniforms        = 0x90CA,
			min_map_buffer_alignment         = 0x90BC,
			num_sample_counts                = 0x9380,
			pack_compressed_block_depth      = 0x912D,
			pack_compressed_block_height     = 0x912C,
			pack_compressed_block_size       = 0x912E,
			pack_compressed_block_width      = 0x912B,
			pixel_buffer_barrier_bit         = 0x00000080,
			shader_image_access_barrier_bit  = 0x00000020,
			texture_fetch_barrier_bit        = 0x00000008,
			texture_immutable_format         = 0x912F,
			texture_update_barrier_bit       = 0x00000100,
			transform_feedback_active        = 0x8E24,
			transform_feedback_barrier_bit   = 0x00000800,
			transform_feedback_paused        = 0x8E23,
			uniform_atomic_counter_buffer_index = 0x92DA,
			uniform_barrier_bit              = 0x00000004,
			unpack_compressed_block_depth    = 0x9129,
			unpack_compressed_block_height   = 0x9128,
			unpack_compressed_block_size     = 0x912A,
			unpack_compressed_block_width    = 0x9127,
			unsigned_int_atomic_counter      = 0x92DB,
			unsigned_int_image_1d            = 0x9062,
			unsigned_int_image_1d_array      = 0x9068,
			unsigned_int_image_2d            = 0x9063,
			unsigned_int_image_2d_array      = 0x9069,
			unsigned_int_image_2d_multisample = 0x906B,
			unsigned_int_image_2d_multisample_array = 0x906C,
			unsigned_int_image_2d_rect       = 0x9065,
			unsigned_int_image_3d            = 0x9064,
			unsigned_int_image_buffer        = 0x9067,
			unsigned_int_image_cube          = 0x9066,
			unsigned_int_image_cube_map_array = 0x906A,
			vertex_attrib_array_barrier_bit  = 0x00000001,

			active_resources                 = 0x92F5,
			active_variables                 = 0x9305,
			any_samples_passed_conservative  = 0x8D6A,
			array_size                       = 0x92FB,
			array_stride                     = 0x92FE,
			atomic_counter_buffer_index      = 0x9301,
			atomic_counter_buffer_referenced_by_compute_shader = 0x90ED,
			auto_generate_mipmap             = 0x8295,
			block_index                      = 0x92FD,
			buffer                           = 0x82E0,
			buffer_binding                   = 0x9302,
			buffer_data_size                 = 0x9303,
			buffer_variable                  = 0x92E5,
			caveat_support                   = 0x82B8,
			clear_buffer                     = 0x82B4,
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
			compute_shader                   = 0x91B9,
			compute_shader_bit               = 0x00000020,
			compute_subroutine               = 0x92ED,
			compute_subroutine_uniform       = 0x92F3,
			compute_texture                  = 0x82A0,
			compute_work_group_size          = 0x8267,
			context_flag_debug_bit           = 0x00000002,
			debug_callback_function          = 0x8244,
			debug_callback_user_param        = 0x8245,
			debug_group_stack_depth          = 0x826D,
			debug_logged_messages            = 0x9145,
			debug_next_logged_message_length = 0x8243,
			debug_output                     = 0x92E0,
			debug_output_synchronous         = 0x8242,
			debug_severity_high              = 0x9146,
			debug_severity_low               = 0x9148,
			debug_severity_medium            = 0x9147,
			debug_severity_notification      = 0x826B,
			debug_source_api                 = 0x8246,
			debug_source_application         = 0x824A,
			debug_source_other               = 0x824B,
			debug_source_shader_compiler     = 0x8248,
			debug_source_third_party         = 0x8249,
			debug_source_window_system       = 0x8247,
			debug_type_deprecated_behavior   = 0x824D,
			debug_type_error                 = 0x824C,
			debug_type_marker                = 0x8268,
			debug_type_other                 = 0x8251,
			debug_type_performance           = 0x8250,
			debug_type_pop_group             = 0x826A,
			debug_type_portability           = 0x824F,
			debug_type_push_group            = 0x8269,
			debug_type_undefined_behavior    = 0x824E,
			depth_components                 = 0x8284,
			depth_renderable                 = 0x8287,
			depth_stencil_texture_mode       = 0x90EA,
			dispatch_indirect_buffer         = 0x90EE,
			dispatch_indirect_buffer_binding = 0x90EF,
			filter                           = 0x829A,
			fragment_subroutine              = 0x92EC,
			fragment_subroutine_uniform      = 0x92F2,
			fragment_texture                 = 0x829F,
			framebuffer_blend                = 0x828B,
			framebuffer_default_fixed_sample_locations = 0x9314,
			framebuffer_default_height       = 0x9311,
			framebuffer_default_layers       = 0x9312,
			framebuffer_default_samples      = 0x9313,
			framebuffer_default_width        = 0x9310,
			framebuffer_renderable           = 0x8289,
			framebuffer_renderable_layered   = 0x828A,
			full_support                     = 0x82B7,
			geometry_subroutine              = 0x92EB,
			geometry_subroutine_uniform      = 0x92F1,
			geometry_texture                 = 0x829E,
			get_texture_image_format         = 0x8291,
			get_texture_image_type           = 0x8292,
			image_class_10_10_10_2           = 0x82C3,
			image_class_11_11_10             = 0x82C2,
			image_class_1_x_16               = 0x82BE,
			image_class_1_x_32               = 0x82BB,
			image_class_1_x_8                = 0x82C1,
			image_class_2_x_16               = 0x82BD,
			image_class_2_x_32               = 0x82BA,
			image_class_2_x_8                = 0x82C0,
			image_class_4_x_16               = 0x82BC,
			image_class_4_x_32               = 0x82B9,
			image_class_4_x_8                = 0x82BF,
			image_compatibility_class        = 0x82A8,
			image_pixel_format               = 0x82A9,
			image_pixel_type                 = 0x82AA,
			image_texel_size                 = 0x82A7,
			internalformat_alpha_size        = 0x8274,
			internalformat_alpha_type        = 0x827B,
			internalformat_blue_size         = 0x8273,
			internalformat_blue_type         = 0x827A,
			internalformat_depth_size        = 0x8275,
			internalformat_depth_type        = 0x827C,
			internalformat_green_size        = 0x8272,
			internalformat_green_type        = 0x8279,
			internalformat_preferred         = 0x8270,
			internalformat_red_size          = 0x8271,
			internalformat_red_type          = 0x8278,
			internalformat_shared_size       = 0x8277,
			internalformat_stencil_size      = 0x8276,
			internalformat_stencil_type      = 0x827D,
			internalformat_supported         = 0x826F,
			is_per_patch                     = 0x92E7,
			is_row_major                     = 0x9300,
			location                         = 0x930E,
			location_index                   = 0x930F,
			manual_generate_mipmap           = 0x8294,
			matrix_stride                    = 0x92FF,
			max_combined_compute_uniform_components = 0x8266,
			max_combined_dimensions          = 0x8282,
			max_combined_shader_output_resources = 0x8F39,
			max_combined_shader_storage_blocks = 0x90DC,
			max_compute_atomic_counters      = 0x8265,
			max_compute_atomic_counter_buffers = 0x8264,
			max_compute_image_uniforms       = 0x91BD,
			max_compute_shader_storage_blocks = 0x90DB,
			max_compute_shared_memory_size   = 0x8262,
			max_compute_texture_image_units  = 0x91BC,
			max_compute_uniform_blocks       = 0x91BB,
			max_compute_uniform_components   = 0x8263,
			max_compute_work_group_count     = 0x91BE,
			max_compute_work_group_invocations = 0x90EB,
			max_compute_work_group_size      = 0x91BF,
			max_debug_group_stack_depth      = 0x826C,
			max_debug_logged_messages        = 0x9144,
			max_debug_message_length         = 0x9143,
			max_depth                        = 0x8280,
			max_element_index                = 0x8D6B,
			max_fragment_shader_storage_blocks = 0x90DA,
			max_framebuffer_height           = 0x9316,
			max_framebuffer_layers           = 0x9317,
			max_framebuffer_samples          = 0x9318,
			max_framebuffer_width            = 0x9315,
			max_geometry_shader_storage_blocks = 0x90D7,
			max_height                       = 0x827F,
			max_label_length                 = 0x82E8,
			max_layers                       = 0x8281,
			max_name_length                  = 0x92F6,
			max_num_active_variables         = 0x92F7,
			max_num_compatible_subroutines   = 0x92F8,
			max_shader_storage_block_size    = 0x90DE,
			max_shader_storage_buffer_bindings = 0x90DD,
			max_tess_control_shader_storage_blocks = 0x90D8,
			max_tess_evaluation_shader_storage_blocks = 0x90D9,
			max_uniform_locations            = 0x826E,
			max_vertex_attrib_bindings       = 0x82DA,
			max_vertex_attrib_relative_offset = 0x82D9,
			max_vertex_shader_storage_blocks = 0x90D6,
			max_width                        = 0x827E,
			mipmap                           = 0x8293,
			name_length                      = 0x92F9,
			num_active_variables             = 0x9304,
			num_shading_language_versions    = 0x82E9,
			offset                           = 0x92FC,
			primitive_restart_fixed_index    = 0x8D69,
			program                          = 0x82E2,
			program_input                    = 0x92E3,
			program_output                   = 0x92E4,
			program_pipeline                 = 0x82E4,
			query                            = 0x82E3,
			read_pixels                      = 0x828C,
			read_pixels_format               = 0x828D,
			read_pixels_type                 = 0x828E,
			referenced_by_compute_shader     = 0x930B,
			referenced_by_fragment_shader    = 0x930A,
			referenced_by_geometry_shader    = 0x9309,
			referenced_by_tess_control_shader = 0x9307,
			referenced_by_tess_evaluation_shader = 0x9308,
			referenced_by_vertex_shader      = 0x9306,
			sampler                          = 0x82E6,
			shader                           = 0x82E1,
			shader_image_atomic              = 0x82A6,
			shader_image_load                = 0x82A4,
			shader_image_store               = 0x82A5,
			shader_storage_barrier_bit       = 0x00002000,
			shader_storage_block             = 0x92E6,
			shader_storage_buffer            = 0x90D2,
			shader_storage_buffer_binding    = 0x90D3,
			shader_storage_buffer_offset_alignment = 0x90DF,
			shader_storage_buffer_size       = 0x90D5,
			shader_storage_buffer_start      = 0x90D4,
			simultaneous_texture_and_depth_test = 0x82AC,
			simultaneous_texture_and_depth_write = 0x82AE,
			simultaneous_texture_and_stencil_test = 0x82AD,
			simultaneous_texture_and_stencil_write = 0x82AF,
			srgb_read                        = 0x8297,
			srgb_write                       = 0x8298,
			stencil_components               = 0x8285,
			stencil_renderable               = 0x8288,
			tess_control_subroutine          = 0x92E9,
			tess_control_subroutine_uniform  = 0x92EF,
			tess_control_texture             = 0x829C,
			tess_evaluation_subroutine       = 0x92EA,
			tess_evaluation_subroutine_uniform = 0x92F0,
			tess_evaluation_texture          = 0x829D,
			texture_buffer_offset            = 0x919D,
			texture_buffer_offset_alignment  = 0x919F,
			texture_buffer_size              = 0x919E,
			texture_compressed_block_height  = 0x82B2,
			texture_compressed_block_size    = 0x82B3,
			texture_compressed_block_width   = 0x82B1,
			texture_gather                   = 0x82A2,
			texture_gather_shadow            = 0x82A3,
			texture_image_format             = 0x828F,
			texture_image_type               = 0x8290,
			texture_immutable_levels         = 0x82DF,
			texture_shadow                   = 0x82A1,
			texture_view                     = 0x82B5,
			texture_view_min_layer           = 0x82DD,
			texture_view_min_level           = 0x82DB,
			texture_view_num_layers          = 0x82DE,
			texture_view_num_levels          = 0x82DC,
			top_level_array_size             = 0x930C,
			top_level_array_stride           = 0x930D,
			transform_feedback_varying       = 0x92F4,
			type                             = 0x92FA,
			uniform                          = 0x92E1,
			uniform_block                    = 0x92E2,
			uniform_block_referenced_by_compute_shader = 0x90EC,
			vertex_attrib_array_long         = 0x874E,
			vertex_attrib_binding            = 0x82D4,
			vertex_attrib_relative_offset    = 0x82D5,
			vertex_binding_buffer            = 0x8F4F,
			vertex_binding_divisor           = 0x82D6,
			vertex_binding_offset            = 0x82D7,
			vertex_binding_stride            = 0x82D8,
			vertex_subroutine                = 0x92E8,
			vertex_subroutine_uniform        = 0x92EE,
			vertex_texture                   = 0x829B,
			view_class_128_bits              = 0x82C4,
			view_class_16_bits               = 0x82CA,
			view_class_24_bits               = 0x82C9,
			view_class_32_bits               = 0x82C8,
			view_class_48_bits               = 0x82C7,
			view_class_64_bits               = 0x82C6,
			view_class_8_bits                = 0x82CB,
			view_class_96_bits               = 0x82C5,
			view_class_bptc_float            = 0x82D3,
			view_class_bptc_unorm            = 0x82D2,
			view_class_rgtc1_red             = 0x82D0,
			view_class_rgtc2_rg              = 0x82D1,
			view_class_s3tc_dxt1_rgb         = 0x82CC,
			view_class_s3tc_dxt1_rgba        = 0x82CD,
			view_class_s3tc_dxt3_rgba        = 0x82CE,
			view_class_s3tc_dxt5_rgba        = 0x82CF,
			view_compatibility_class         = 0x82B6,

			buffer_immutable_storage         = 0x821F,
			buffer_storage_flags             = 0x8220,
			clear_texture                    = 0x9365,
			client_mapped_buffer_barrier_bit = 0x00004000,
			client_storage_bit               = 0x0200,
			dynamic_storage_bit              = 0x0100,
			location_component               = 0x934A,
			map_coherent_bit                 = 0x0080,
			map_persistent_bit               = 0x0040,
			max_vertex_attrib_stride         = 0x82E5,
			mirror_clamp_to_edge             = 0x8743,
			primitive_restart_for_patches_supported = 0x8221,
			query_buffer                     = 0x9192,
			query_buffer_barrier_bit         = 0x00008000,
			query_buffer_binding             = 0x9193,
			query_result_no_wait             = 0x9194,
			texture_buffer_binding           = 0x8C2A,
			transform_feedback_buffer_index  = 0x934B,
			transform_feedback_buffer_stride = 0x934C,

			clip_depth_mode                  = 0x935D,
			clip_origin                      = 0x935C,
			context_flag_robust_access_bit   = 0x00000004,
			context_lost                     = 0x0507,
			context_release_behavior         = 0x82FB,
			context_release_behavior_flush   = 0x82FC,
			guilty_context_reset             = 0x8253,
			innocent_context_reset           = 0x8254,
			lose_context_on_reset            = 0x8252,
			max_combined_clip_and_cull_distances = 0x82FA,
			max_cull_distances               = 0x82F9,
			negative_one_to_one              = 0x935E,
			no_reset_notification            = 0x8261,
			query_by_region_no_wait_inverted = 0x8E1A,
			query_by_region_wait_inverted    = 0x8E19,
			query_no_wait_inverted           = 0x8E18,
			query_target                     = 0x82EA,
			query_wait_inverted              = 0x8E17,
			reset_notification_strategy      = 0x8256,
			texture_target                   = 0x1006,
			unknown_context_reset            = 0x8255,
			zero_to_one                      = 0x935F,

		};
	}
	using namespace enums;
	namespace constants
	{
		static constexpr auto timeout_ignored = 0xFFFFFFFFFFFFFFFF;
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
			#ifdef GL_LOADER_WANT_LEGACY_TYPES
			namespace legacy
			{
				typedef enum_type GLenum;
				typedef unsigned char GLboolean;
				typedef unsigned int GLbitfield;
				typedef void GLvoid;
				typedef signed char GLbyte;
				typedef short GLshort;
				typedef int GLint;
				typedef unsigned char GLubyte;
				typedef unsigned short GLushort;
				typedef unsigned int GLuint;
				typedef int GLsizei;
				typedef float GLfloat;
				typedef float GLclampf;
				typedef double GLdouble;
				typedef double GLclampd;
				typedef char GLchar;
				typedef unsigned short GLhalf;
				typedef GLint GLfixed;
				typedef ptrdiff_t GLintptr;
				typedef ptrdiff_t GLsizeiptr;
				typedef int64_t GLint64;
				typedef uint64_t GLuint64;
				typedef struct __GLsync *GLsync;
				struct _cl_context;
				struct _cl_event;
				typedef void (APIENTRY *GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
			}
			using namespace legacy;
			#endif // GL_LOADER_WANT_LEGACY_TYPES
		}
	}
	using namespace types;
	namespace funcs
	{
		extern void (*blend_func)(enum_type sfactor, enum_type dfactor);
		extern void (*clear)(bitfield_type mask);
		extern void (*clear_color)(float_type red, float_type green, float_type blue, float_type alpha);
		extern void (*clear_depth)(double_type depth);
		extern void (*clear_stencil)(int_type s);
		extern void (*color_mask)(boolean_type red, boolean_type green, boolean_type blue, boolean_type alpha);
		extern void (*cull_face)(enum_type mode);
		extern void (*depth_func)(enum_type func);
		extern void (*depth_mask)(boolean_type flag);
		extern void (*depth_range)(double_type ren_near, double_type ren_far);
		extern void (*disable)(enum_type cap);
		extern void (*draw_buffer)(enum_type buf);
		extern void (*enable)(enum_type cap);
		extern void (*finish)(void);
		extern void (*flush)(void);
		extern void (*front_face)(enum_type mode);
		extern void (*get_booleanv)(enum_type pname, boolean_type * data);
		extern void (*get_doublev)(enum_type pname, double_type * data);
		extern enum_type (*get_error)(void);
		extern void (*get_floatv)(enum_type pname, float_type * data);
		extern void (*get_integerv)(enum_type pname, int_type * data);
		extern ubyte_type const* (*get_string)(enum_type name);
		extern void (*get_tex_image)(enum_type target, int_type level, enum_type format, enum_type type, void * pixels);
		extern void (*get_tex_level_parameterfv)(enum_type target, int_type level, enum_type pname, float_type * params);
		extern void (*get_tex_level_parameteriv)(enum_type target, int_type level, enum_type pname, int_type * params);
		extern void (*get_tex_parameterfv)(enum_type target, enum_type pname, float_type * params);
		extern void (*get_tex_parameteriv)(enum_type target, enum_type pname, int_type * params);
		extern void (*hint)(enum_type target, enum_type mode);
		extern boolean_type (*is_enabled)(enum_type cap);
		extern void (*line_width)(float_type width);
		extern void (*logic_op)(enum_type opcode);
		extern void (*pixel_storef)(enum_type pname, float_type param);
		extern void (*pixel_storei)(enum_type pname, int_type param);
		extern void (*point_size)(float_type size);
		extern void (*polygon_mode)(enum_type face, enum_type mode);
		extern void (*read_buffer)(enum_type src);
		extern void (*read_pixels)(int_type x, int_type y, sizei_type width, sizei_type height, enum_type format, enum_type type, void * pixels);
		extern void (*scissor)(int_type x, int_type y, sizei_type width, sizei_type height);
		extern void (*stencil_func)(enum_type func, int_type ref, uint_type mask);
		extern void (*stencil_mask)(uint_type mask);
		extern void (*stencil_op)(enum_type fail, enum_type zfail, enum_type zpass);
		extern void (*tex_image1d)(enum_type target, int_type level, int_type internalformat, sizei_type width, int_type border, enum_type format, enum_type type, void const* pixels);
		extern void (*tex_image2d)(enum_type target, int_type level, int_type internalformat, sizei_type width, sizei_type height, int_type border, enum_type format, enum_type type, void const* pixels);
		extern void (*tex_parameterf)(enum_type target, enum_type pname, float_type param);
		extern void (*tex_parameterfv)(enum_type target, enum_type pname, float_type const* params);
		extern void (*tex_parameteri)(enum_type target, enum_type pname, int_type param);
		extern void (*tex_parameteriv)(enum_type target, enum_type pname, int_type const* params);
		extern void (*viewport)(int_type x, int_type y, sizei_type width, sizei_type height);

		extern void (*bind_texture)(enum_type target, uint_type texture);
		extern void (*copy_tex_image1d)(enum_type target, int_type level, enum_type internalformat, int_type x, int_type y, sizei_type width, int_type border);
		extern void (*copy_tex_image2d)(enum_type target, int_type level, enum_type internalformat, int_type x, int_type y, sizei_type width, sizei_type height, int_type border);
		extern void (*copy_tex_sub_image1d)(enum_type target, int_type level, int_type xoffset, int_type x, int_type y, sizei_type width);
		extern void (*copy_tex_sub_image2d)(enum_type target, int_type level, int_type xoffset, int_type yoffset, int_type x, int_type y, sizei_type width, sizei_type height);
		extern void (*delete_textures)(sizei_type n, uint_type const* textures);
		extern void (*draw_arrays)(enum_type mode, int_type first, sizei_type count);
		extern void (*draw_elements)(enum_type mode, sizei_type count, enum_type type, void const* indices);
		extern void (*gen_textures)(sizei_type n, uint_type * textures);
		extern void (*get_pointerv)(enum_type pname, void ** params);
		extern boolean_type (*is_texture)(uint_type texture);
		extern void (*polygon_offset)(float_type factor, float_type units);
		extern void (*tex_sub_image1d)(enum_type target, int_type level, int_type xoffset, sizei_type width, enum_type format, enum_type type, void const* pixels);
		extern void (*tex_sub_image2d)(enum_type target, int_type level, int_type xoffset, int_type yoffset, sizei_type width, sizei_type height, enum_type format, enum_type type, void const* pixels);

		extern void (*copy_tex_sub_image3d)(enum_type target, int_type level, int_type xoffset, int_type yoffset, int_type zoffset, int_type x, int_type y, sizei_type width, sizei_type height);
		extern void (*draw_range_elements)(enum_type mode, uint_type start, uint_type end, sizei_type count, enum_type type, void const* indices);
		extern void (*tex_image3d)(enum_type target, int_type level, int_type internalformat, sizei_type width, sizei_type height, sizei_type depth, int_type border, enum_type format, enum_type type, void const* pixels);
		extern void (*tex_sub_image3d)(enum_type target, int_type level, int_type xoffset, int_type yoffset, int_type zoffset, sizei_type width, sizei_type height, sizei_type depth, enum_type format, enum_type type, void const* pixels);

		extern void (*active_texture)(enum_type texture);
		extern void (*compressed_tex_image1d)(enum_type target, int_type level, enum_type internalformat, sizei_type width, int_type border, sizei_type imageSize, void const* data);
		extern void (*compressed_tex_image2d)(enum_type target, int_type level, enum_type internalformat, sizei_type width, sizei_type height, int_type border, sizei_type imageSize, void const* data);
		extern void (*compressed_tex_image3d)(enum_type target, int_type level, enum_type internalformat, sizei_type width, sizei_type height, sizei_type depth, int_type border, sizei_type imageSize, void const* data);
		extern void (*compressed_tex_sub_image1d)(enum_type target, int_type level, int_type xoffset, sizei_type width, enum_type format, sizei_type imageSize, void const* data);
		extern void (*compressed_tex_sub_image2d)(enum_type target, int_type level, int_type xoffset, int_type yoffset, sizei_type width, sizei_type height, enum_type format, sizei_type imageSize, void const* data);
		extern void (*compressed_tex_sub_image3d)(enum_type target, int_type level, int_type xoffset, int_type yoffset, int_type zoffset, sizei_type width, sizei_type height, sizei_type depth, enum_type format, sizei_type imageSize, void const* data);
		extern void (*get_compressed_tex_image)(enum_type target, int_type level, void * img);
		extern void (*sample_coverage)(float_type value, boolean_type invert);

		extern void (*blend_color)(float_type red, float_type green, float_type blue, float_type alpha);
		extern void (*blend_equation)(enum_type mode);
		extern void (*blend_func_separate)(enum_type sfactorRGB, enum_type dfactorRGB, enum_type sfactorAlpha, enum_type dfactorAlpha);
		extern void (*multi_draw_arrays)(enum_type mode, int_type const* first, sizei_type const* count, sizei_type drawcount);
		extern void (*multi_draw_elements)(enum_type mode, sizei_type const* count, enum_type type, void const* const* indices, sizei_type drawcount);
		extern void (*point_parameterf)(enum_type pname, float_type param);
		extern void (*point_parameterfv)(enum_type pname, float_type const* params);
		extern void (*point_parameteri)(enum_type pname, int_type param);
		extern void (*point_parameteriv)(enum_type pname, int_type const* params);

		extern void (*begin_query)(enum_type target, uint_type id);
		extern void (*bind_buffer)(enum_type target, uint_type buffer);
		extern void (*buffer_data)(enum_type target, sizeiptr_type size, void const* data, enum_type usage);
		extern void (*buffer_sub_data)(enum_type target, intptr_type offset, sizeiptr_type size, void const* data);
		extern void (*delete_buffers)(sizei_type n, uint_type const* buffers);
		extern void (*delete_queries)(sizei_type n, uint_type const* ids);
		extern void (*end_query)(enum_type target);
		extern void (*gen_buffers)(sizei_type n, uint_type * buffers);
		extern void (*gen_queries)(sizei_type n, uint_type * ids);
		extern void (*get_buffer_parameteriv)(enum_type target, enum_type pname, int_type * params);
		extern void (*get_buffer_pointerv)(enum_type target, enum_type pname, void ** params);
		extern void (*get_buffer_sub_data)(enum_type target, intptr_type offset, sizeiptr_type size, void * data);
		extern void (*get_query_objectiv)(uint_type id, enum_type pname, int_type * params);
		extern void (*get_query_objectuiv)(uint_type id, enum_type pname, uint_type * params);
		extern void (*get_queryiv)(enum_type target, enum_type pname, int_type * params);
		extern boolean_type (*is_buffer)(uint_type buffer);
		extern boolean_type (*is_query)(uint_type id);
		extern void * (*map_buffer)(enum_type target, enum_type access);
		extern boolean_type (*unmap_buffer)(enum_type target);

		extern void (*attach_shader)(uint_type program, uint_type shader);
		extern void (*bind_attrib_location)(uint_type program, uint_type index, char_type const* name);
		extern void (*blend_equation_separate)(enum_type modeRGB, enum_type modeAlpha);
		extern void (*compile_shader)(uint_type shader);
		extern uint_type (*create_program)(void);
		extern uint_type (*create_shader)(enum_type type);
		extern void (*delete_program)(uint_type program);
		extern void (*delete_shader)(uint_type shader);
		extern void (*detach_shader)(uint_type program, uint_type shader);
		extern void (*disable_vertex_attrib_array)(uint_type index);
		extern void (*draw_buffers)(sizei_type n, enum_type const* bufs);
		extern void (*enable_vertex_attrib_array)(uint_type index);
		extern void (*get_active_attrib)(uint_type program, uint_type index, sizei_type bufSize, sizei_type * length, int_type * size, enum_type * type, char_type * name);
		extern void (*get_active_uniform)(uint_type program, uint_type index, sizei_type bufSize, sizei_type * length, int_type * size, enum_type * type, char_type * name);
		extern void (*get_attached_shaders)(uint_type program, sizei_type maxCount, sizei_type * count, uint_type * shaders);
		extern int_type (*get_attrib_location)(uint_type program, char_type const* name);
		extern void (*get_program_info_log)(uint_type program, sizei_type bufSize, sizei_type * length, char_type * infoLog);
		extern void (*get_programiv)(uint_type program, enum_type pname, int_type * params);
		extern void (*get_shader_info_log)(uint_type shader, sizei_type bufSize, sizei_type * length, char_type * infoLog);
		extern void (*get_shader_source)(uint_type shader, sizei_type bufSize, sizei_type * length, char_type * source);
		extern void (*get_shaderiv)(uint_type shader, enum_type pname, int_type * params);
		extern int_type (*get_uniform_location)(uint_type program, char_type const* name);
		extern void (*get_uniformfv)(uint_type program, int_type location, float_type * params);
		extern void (*get_uniformiv)(uint_type program, int_type location, int_type * params);
		extern void (*get_vertex_attrib_pointerv)(uint_type index, enum_type pname, void ** pointer);
		extern void (*get_vertex_attribdv)(uint_type index, enum_type pname, double_type * params);
		extern void (*get_vertex_attribfv)(uint_type index, enum_type pname, float_type * params);
		extern void (*get_vertex_attribiv)(uint_type index, enum_type pname, int_type * params);
		extern boolean_type (*is_program)(uint_type program);
		extern boolean_type (*is_shader)(uint_type shader);
		extern void (*link_program)(uint_type program);
		extern void (*shader_source)(uint_type shader, sizei_type count, char_type const* const* string, int_type const* length);
		extern void (*stencil_func_separate)(enum_type face, enum_type func, int_type ref, uint_type mask);
		extern void (*stencil_mask_separate)(enum_type face, uint_type mask);
		extern void (*stencil_op_separate)(enum_type face, enum_type sfail, enum_type dpfail, enum_type dppass);
		extern void (*uniform1f)(int_type location, float_type v0);
		extern void (*uniform1fv)(int_type location, sizei_type count, float_type const* value);
		extern void (*uniform1i)(int_type location, int_type v0);
		extern void (*uniform1iv)(int_type location, sizei_type count, int_type const* value);
		extern void (*uniform2f)(int_type location, float_type v0, float_type v1);
		extern void (*uniform2fv)(int_type location, sizei_type count, float_type const* value);
		extern void (*uniform2i)(int_type location, int_type v0, int_type v1);
		extern void (*uniform2iv)(int_type location, sizei_type count, int_type const* value);
		extern void (*uniform3f)(int_type location, float_type v0, float_type v1, float_type v2);
		extern void (*uniform3fv)(int_type location, sizei_type count, float_type const* value);
		extern void (*uniform3i)(int_type location, int_type v0, int_type v1, int_type v2);
		extern void (*uniform3iv)(int_type location, sizei_type count, int_type const* value);
		extern void (*uniform4f)(int_type location, float_type v0, float_type v1, float_type v2, float_type v3);
		extern void (*uniform4fv)(int_type location, sizei_type count, float_type const* value);
		extern void (*uniform4i)(int_type location, int_type v0, int_type v1, int_type v2, int_type v3);
		extern void (*uniform4iv)(int_type location, sizei_type count, int_type const* value);
		extern void (*uniform_matrix2fv)(int_type location, sizei_type count, boolean_type transpose, float_type const* value);
		extern void (*uniform_matrix3fv)(int_type location, sizei_type count, boolean_type transpose, float_type const* value);
		extern void (*uniform_matrix4fv)(int_type location, sizei_type count, boolean_type transpose, float_type const* value);
		extern void (*use_program)(uint_type program);
		extern void (*validate_program)(uint_type program);
		extern void (*vertex_attrib1d)(uint_type index, double_type x);
		extern void (*vertex_attrib1dv)(uint_type index, double_type const* v);
		extern void (*vertex_attrib1f)(uint_type index, float_type x);
		extern void (*vertex_attrib1fv)(uint_type index, float_type const* v);
		extern void (*vertex_attrib1s)(uint_type index, short_type x);
		extern void (*vertex_attrib1sv)(uint_type index, short_type const* v);
		extern void (*vertex_attrib2d)(uint_type index, double_type x, double_type y);
		extern void (*vertex_attrib2dv)(uint_type index, double_type const* v);
		extern void (*vertex_attrib2f)(uint_type index, float_type x, float_type y);
		extern void (*vertex_attrib2fv)(uint_type index, float_type const* v);
		extern void (*vertex_attrib2s)(uint_type index, short_type x, short_type y);
		extern void (*vertex_attrib2sv)(uint_type index, short_type const* v);
		extern void (*vertex_attrib3d)(uint_type index, double_type x, double_type y, double_type z);
		extern void (*vertex_attrib3dv)(uint_type index, double_type const* v);
		extern void (*vertex_attrib3f)(uint_type index, float_type x, float_type y, float_type z);
		extern void (*vertex_attrib3fv)(uint_type index, float_type const* v);
		extern void (*vertex_attrib3s)(uint_type index, short_type x, short_type y, short_type z);
		extern void (*vertex_attrib3sv)(uint_type index, short_type const* v);
		extern void (*vertex_attrib4_nbv)(uint_type index, byte_type const* v);
		extern void (*vertex_attrib4_niv)(uint_type index, int_type const* v);
		extern void (*vertex_attrib4_nsv)(uint_type index, short_type const* v);
		extern void (*vertex_attrib4_nub)(uint_type index, ubyte_type x, ubyte_type y, ubyte_type z, ubyte_type w);
		extern void (*vertex_attrib4_nubv)(uint_type index, ubyte_type const* v);
		extern void (*vertex_attrib4_nuiv)(uint_type index, uint_type const* v);
		extern void (*vertex_attrib4_nusv)(uint_type index, ushort_type const* v);
		extern void (*vertex_attrib4bv)(uint_type index, byte_type const* v);
		extern void (*vertex_attrib4d)(uint_type index, double_type x, double_type y, double_type z, double_type w);
		extern void (*vertex_attrib4dv)(uint_type index, double_type const* v);
		extern void (*vertex_attrib4f)(uint_type index, float_type x, float_type y, float_type z, float_type w);
		extern void (*vertex_attrib4fv)(uint_type index, float_type const* v);
		extern void (*vertex_attrib4iv)(uint_type index, int_type const* v);
		extern void (*vertex_attrib4s)(uint_type index, short_type x, short_type y, short_type z, short_type w);
		extern void (*vertex_attrib4sv)(uint_type index, short_type const* v);
		extern void (*vertex_attrib4ubv)(uint_type index, ubyte_type const* v);
		extern void (*vertex_attrib4uiv)(uint_type index, uint_type const* v);
		extern void (*vertex_attrib4usv)(uint_type index, ushort_type const* v);
		extern void (*vertex_attrib_pointer)(uint_type index, int_type size, enum_type type, boolean_type normalized, sizei_type stride, void const* pointer);

		extern void (*uniform_matrix2x3fv)(int_type location, sizei_type count, boolean_type transpose, float_type const* value);
		extern void (*uniform_matrix2x4fv)(int_type location, sizei_type count, boolean_type transpose, float_type const* value);
		extern void (*uniform_matrix3x2fv)(int_type location, sizei_type count, boolean_type transpose, float_type const* value);
		extern void (*uniform_matrix3x4fv)(int_type location, sizei_type count, boolean_type transpose, float_type const* value);
		extern void (*uniform_matrix4x2fv)(int_type location, sizei_type count, boolean_type transpose, float_type const* value);
		extern void (*uniform_matrix4x3fv)(int_type location, sizei_type count, boolean_type transpose, float_type const* value);

		extern void (*begin_conditional_render)(uint_type id, enum_type mode);
		extern void (*begin_transform_feedback)(enum_type primitiveMode);
		extern void (*bind_buffer_base)(enum_type target, uint_type index, uint_type buffer);
		extern void (*bind_buffer_range)(enum_type target, uint_type index, uint_type buffer, intptr_type offset, sizeiptr_type size);
		extern void (*bind_frag_data_location)(uint_type program, uint_type color, char_type const* name);
		extern void (*bind_framebuffer)(enum_type target, uint_type framebuffer);
		extern void (*bind_renderbuffer)(enum_type target, uint_type renderbuffer);
		extern void (*bind_vertex_array)(uint_type ren_array);
		extern void (*blit_framebuffer)(int_type srcX0, int_type srcY0, int_type srcX1, int_type srcY1, int_type dstX0, int_type dstY0, int_type dstX1, int_type dstY1, bitfield_type mask, enum_type filter);
		extern enum_type (*check_framebuffer_status)(enum_type target);
		extern void (*clamp_color)(enum_type target, enum_type clamp);
		extern void (*clear_bufferfi)(enum_type buffer, int_type drawbuffer, float_type depth, int_type stencil);
		extern void (*clear_bufferfv)(enum_type buffer, int_type drawbuffer, float_type const* value);
		extern void (*clear_bufferiv)(enum_type buffer, int_type drawbuffer, int_type const* value);
		extern void (*clear_bufferuiv)(enum_type buffer, int_type drawbuffer, uint_type const* value);
		extern void (*color_maski)(uint_type index, boolean_type r, boolean_type g, boolean_type b, boolean_type a);
		extern void (*delete_framebuffers)(sizei_type n, uint_type const* framebuffers);
		extern void (*delete_renderbuffers)(sizei_type n, uint_type const* renderbuffers);
		extern void (*delete_vertex_arrays)(sizei_type n, uint_type const* arrays);
		extern void (*disablei)(enum_type target, uint_type index);
		extern void (*enablei)(enum_type target, uint_type index);
		extern void (*end_conditional_render)(void);
		extern void (*end_transform_feedback)(void);
		extern void (*flush_mapped_buffer_range)(enum_type target, intptr_type offset, sizeiptr_type length);
		extern void (*framebuffer_renderbuffer)(enum_type target, enum_type attachment, enum_type renderbuffertarget, uint_type renderbuffer);
		extern void (*framebuffer_texture1d)(enum_type target, enum_type attachment, enum_type textarget, uint_type texture, int_type level);
		extern void (*framebuffer_texture2d)(enum_type target, enum_type attachment, enum_type textarget, uint_type texture, int_type level);
		extern void (*framebuffer_texture3d)(enum_type target, enum_type attachment, enum_type textarget, uint_type texture, int_type level, int_type zoffset);
		extern void (*framebuffer_texture_layer)(enum_type target, enum_type attachment, uint_type texture, int_type level, int_type layer);
		extern void (*gen_framebuffers)(sizei_type n, uint_type * framebuffers);
		extern void (*gen_renderbuffers)(sizei_type n, uint_type * renderbuffers);
		extern void (*gen_vertex_arrays)(sizei_type n, uint_type * arrays);
		extern void (*generate_mipmap)(enum_type target);
		extern void (*get_booleani_v)(enum_type target, uint_type index, boolean_type * data);
		extern int_type (*get_frag_data_location)(uint_type program, char_type const* name);
		extern void (*get_framebuffer_attachment_parameteriv)(enum_type target, enum_type attachment, enum_type pname, int_type * params);
		extern void (*get_integeri_v)(enum_type target, uint_type index, int_type * data);
		extern void (*get_renderbuffer_parameteriv)(enum_type target, enum_type pname, int_type * params);
		extern ubyte_type const* (*get_stringi)(enum_type name, uint_type index);
		extern void (*get_tex_parameter_iiv)(enum_type target, enum_type pname, int_type * params);
		extern void (*get_tex_parameter_iuiv)(enum_type target, enum_type pname, uint_type * params);
		extern void (*get_transform_feedback_varying)(uint_type program, uint_type index, sizei_type bufSize, sizei_type * length, sizei_type * size, enum_type * type, char_type * name);
		extern void (*get_uniformuiv)(uint_type program, int_type location, uint_type * params);
		extern void (*get_vertex_attrib_iiv)(uint_type index, enum_type pname, int_type * params);
		extern void (*get_vertex_attrib_iuiv)(uint_type index, enum_type pname, uint_type * params);
		extern boolean_type (*is_enabledi)(enum_type target, uint_type index);
		extern boolean_type (*is_framebuffer)(uint_type framebuffer);
		extern boolean_type (*is_renderbuffer)(uint_type renderbuffer);
		extern boolean_type (*is_vertex_array)(uint_type ren_array);
		extern void * (*map_buffer_range)(enum_type target, intptr_type offset, sizeiptr_type length, bitfield_type access);
		extern void (*renderbuffer_storage)(enum_type target, enum_type internalformat, sizei_type width, sizei_type height);
		extern void (*renderbuffer_storage_multisample)(enum_type target, sizei_type samples, enum_type internalformat, sizei_type width, sizei_type height);
		extern void (*tex_parameter_iiv)(enum_type target, enum_type pname, int_type const* params);
		extern void (*tex_parameter_iuiv)(enum_type target, enum_type pname, uint_type const* params);
		extern void (*transform_feedback_varyings)(uint_type program, sizei_type count, char_type const* const* varyings, enum_type bufferMode);
		extern void (*uniform1ui)(int_type location, uint_type v0);
		extern void (*uniform1uiv)(int_type location, sizei_type count, uint_type const* value);
		extern void (*uniform2ui)(int_type location, uint_type v0, uint_type v1);
		extern void (*uniform2uiv)(int_type location, sizei_type count, uint_type const* value);
		extern void (*uniform3ui)(int_type location, uint_type v0, uint_type v1, uint_type v2);
		extern void (*uniform3uiv)(int_type location, sizei_type count, uint_type const* value);
		extern void (*uniform4ui)(int_type location, uint_type v0, uint_type v1, uint_type v2, uint_type v3);
		extern void (*uniform4uiv)(int_type location, sizei_type count, uint_type const* value);
		extern void (*vertex_attrib_i1i)(uint_type index, int_type x);
		extern void (*vertex_attrib_i1iv)(uint_type index, int_type const* v);
		extern void (*vertex_attrib_i1ui)(uint_type index, uint_type x);
		extern void (*vertex_attrib_i1uiv)(uint_type index, uint_type const* v);
		extern void (*vertex_attrib_i2i)(uint_type index, int_type x, int_type y);
		extern void (*vertex_attrib_i2iv)(uint_type index, int_type const* v);
		extern void (*vertex_attrib_i2ui)(uint_type index, uint_type x, uint_type y);
		extern void (*vertex_attrib_i2uiv)(uint_type index, uint_type const* v);
		extern void (*vertex_attrib_i3i)(uint_type index, int_type x, int_type y, int_type z);
		extern void (*vertex_attrib_i3iv)(uint_type index, int_type const* v);
		extern void (*vertex_attrib_i3ui)(uint_type index, uint_type x, uint_type y, uint_type z);
		extern void (*vertex_attrib_i3uiv)(uint_type index, uint_type const* v);
		extern void (*vertex_attrib_i4bv)(uint_type index, byte_type const* v);
		extern void (*vertex_attrib_i4i)(uint_type index, int_type x, int_type y, int_type z, int_type w);
		extern void (*vertex_attrib_i4iv)(uint_type index, int_type const* v);
		extern void (*vertex_attrib_i4sv)(uint_type index, short_type const* v);
		extern void (*vertex_attrib_i4ubv)(uint_type index, ubyte_type const* v);
		extern void (*vertex_attrib_i4ui)(uint_type index, uint_type x, uint_type y, uint_type z, uint_type w);
		extern void (*vertex_attrib_i4uiv)(uint_type index, uint_type const* v);
		extern void (*vertex_attrib_i4usv)(uint_type index, ushort_type const* v);
		extern void (*vertex_attrib_i_pointer)(uint_type index, int_type size, enum_type type, sizei_type stride, void const* pointer);

		extern void (*copy_buffer_sub_data)(enum_type readTarget, enum_type writeTarget, intptr_type readOffset, intptr_type writeOffset, sizeiptr_type size);
		extern void (*draw_arrays_instanced)(enum_type mode, int_type first, sizei_type count, sizei_type instancecount);
		extern void (*draw_elements_instanced)(enum_type mode, sizei_type count, enum_type type, void const* indices, sizei_type instancecount);
		extern void (*get_active_uniform_block_name)(uint_type program, uint_type uniformBlockIndex, sizei_type bufSize, sizei_type * length, char_type * uniformBlockName);
		extern void (*get_active_uniform_blockiv)(uint_type program, uint_type uniformBlockIndex, enum_type pname, int_type * params);
		extern void (*get_active_uniform_name)(uint_type program, uint_type uniformIndex, sizei_type bufSize, sizei_type * length, char_type * uniformName);
		extern void (*get_active_uniformsiv)(uint_type program, sizei_type uniformCount, uint_type const* uniformIndices, enum_type pname, int_type * params);
		extern uint_type (*get_uniform_block_index)(uint_type program, char_type const* uniformBlockName);
		extern void (*get_uniform_indices)(uint_type program, sizei_type uniformCount, char_type const* const* uniformNames, uint_type * uniformIndices);
		extern void (*primitive_restart_index)(uint_type index);
		extern void (*tex_buffer)(enum_type target, enum_type internalformat, uint_type buffer);
		extern void (*uniform_block_binding)(uint_type program, uint_type uniformBlockIndex, uint_type uniformBlockBinding);

		extern enum_type (*client_wait_sync)(sync_type sync, bitfield_type flags, uint64_type timeout);
		extern void (*delete_sync)(sync_type sync);
		extern void (*draw_elements_base_vertex)(enum_type mode, sizei_type count, enum_type type, void const* indices, int_type basevertex);
		extern void (*draw_elements_instanced_base_vertex)(enum_type mode, sizei_type count, enum_type type, void const* indices, sizei_type instancecount, int_type basevertex);
		extern void (*draw_range_elements_base_vertex)(enum_type mode, uint_type start, uint_type end, sizei_type count, enum_type type, void const* indices, int_type basevertex);
		extern sync_type (*fence_sync)(enum_type condition, bitfield_type flags);
		extern void (*framebuffer_texture)(enum_type target, enum_type attachment, uint_type texture, int_type level);
		extern void (*get_buffer_parameteri64v)(enum_type target, enum_type pname, int64_type * params);
		extern void (*get_integer64i_v)(enum_type target, uint_type index, int64_type * data);
		extern void (*get_integer64v)(enum_type pname, int64_type * data);
		extern void (*get_multisamplefv)(enum_type pname, uint_type index, float_type * val);
		extern void (*get_synciv)(sync_type sync, enum_type pname, sizei_type bufSize, sizei_type * length, int_type * values);
		extern boolean_type (*is_sync)(sync_type sync);
		extern void (*multi_draw_elements_base_vertex)(enum_type mode, sizei_type const* count, enum_type type, void const* const* indices, sizei_type drawcount, int_type const* basevertex);
		extern void (*provoking_vertex)(enum_type mode);
		extern void (*sample_maski)(uint_type maskNumber, bitfield_type mask);
		extern void (*tex_image2d_multisample)(enum_type target, sizei_type samples, enum_type internalformat, sizei_type width, sizei_type height, boolean_type fixedsamplelocations);
		extern void (*tex_image3d_multisample)(enum_type target, sizei_type samples, enum_type internalformat, sizei_type width, sizei_type height, sizei_type depth, boolean_type fixedsamplelocations);
		extern void (*wait_sync)(sync_type sync, bitfield_type flags, uint64_type timeout);

		extern void (*bind_frag_data_location_indexed)(uint_type program, uint_type colorNumber, uint_type index, char_type const* name);
		extern void (*bind_sampler)(uint_type unit, uint_type sampler);
		extern void (*delete_samplers)(sizei_type count, uint_type const* samplers);
		extern void (*gen_samplers)(sizei_type count, uint_type * samplers);
		extern int_type (*get_frag_data_index)(uint_type program, char_type const* name);
		extern void (*get_query_objecti64v)(uint_type id, enum_type pname, int64_type * params);
		extern void (*get_query_objectui64v)(uint_type id, enum_type pname, uint64_type * params);
		extern void (*get_sampler_parameter_iiv)(uint_type sampler, enum_type pname, int_type * params);
		extern void (*get_sampler_parameter_iuiv)(uint_type sampler, enum_type pname, uint_type * params);
		extern void (*get_sampler_parameterfv)(uint_type sampler, enum_type pname, float_type * params);
		extern void (*get_sampler_parameteriv)(uint_type sampler, enum_type pname, int_type * params);
		extern boolean_type (*is_sampler)(uint_type sampler);
		extern void (*query_counter)(uint_type id, enum_type target);
		extern void (*sampler_parameter_iiv)(uint_type sampler, enum_type pname, int_type const* param);
		extern void (*sampler_parameter_iuiv)(uint_type sampler, enum_type pname, uint_type const* param);
		extern void (*sampler_parameterf)(uint_type sampler, enum_type pname, float_type param);
		extern void (*sampler_parameterfv)(uint_type sampler, enum_type pname, float_type const* param);
		extern void (*sampler_parameteri)(uint_type sampler, enum_type pname, int_type param);
		extern void (*sampler_parameteriv)(uint_type sampler, enum_type pname, int_type const* param);
		extern void (*vertex_attrib_divisor)(uint_type index, uint_type divisor);
		extern void (*vertex_attrib_p1ui)(uint_type index, enum_type type, boolean_type normalized, uint_type value);
		extern void (*vertex_attrib_p1uiv)(uint_type index, enum_type type, boolean_type normalized, uint_type const* value);
		extern void (*vertex_attrib_p2ui)(uint_type index, enum_type type, boolean_type normalized, uint_type value);
		extern void (*vertex_attrib_p2uiv)(uint_type index, enum_type type, boolean_type normalized, uint_type const* value);
		extern void (*vertex_attrib_p3ui)(uint_type index, enum_type type, boolean_type normalized, uint_type value);
		extern void (*vertex_attrib_p3uiv)(uint_type index, enum_type type, boolean_type normalized, uint_type const* value);
		extern void (*vertex_attrib_p4ui)(uint_type index, enum_type type, boolean_type normalized, uint_type value);
		extern void (*vertex_attrib_p4uiv)(uint_type index, enum_type type, boolean_type normalized, uint_type const* value);

		extern void (*begin_query_indexed)(enum_type target, uint_type index, uint_type id);
		extern void (*bind_transform_feedback)(enum_type target, uint_type id);
		extern void (*blend_equation_separatei)(uint_type buf, enum_type modeRGB, enum_type modeAlpha);
		extern void (*blend_equationi)(uint_type buf, enum_type mode);
		extern void (*blend_func_separatei)(uint_type buf, enum_type srcRGB, enum_type dstRGB, enum_type srcAlpha, enum_type dstAlpha);
		extern void (*blend_funci)(uint_type buf, enum_type src, enum_type dst);
		extern void (*delete_transform_feedbacks)(sizei_type n, uint_type const* ids);
		extern void (*draw_arrays_indirect)(enum_type mode, void const* indirect);
		extern void (*draw_elements_indirect)(enum_type mode, enum_type type, void const* indirect);
		extern void (*draw_transform_feedback)(enum_type mode, uint_type id);
		extern void (*draw_transform_feedback_stream)(enum_type mode, uint_type id, uint_type stream);
		extern void (*end_query_indexed)(enum_type target, uint_type index);
		extern void (*gen_transform_feedbacks)(sizei_type n, uint_type * ids);
		extern void (*get_active_subroutine_name)(uint_type program, enum_type shadertype, uint_type index, sizei_type bufsize, sizei_type * length, char_type * name);
		extern void (*get_active_subroutine_uniform_name)(uint_type program, enum_type shadertype, uint_type index, sizei_type bufsize, sizei_type * length, char_type * name);
		extern void (*get_active_subroutine_uniformiv)(uint_type program, enum_type shadertype, uint_type index, enum_type pname, int_type * values);
		extern void (*get_program_stageiv)(uint_type program, enum_type shadertype, enum_type pname, int_type * values);
		extern void (*get_query_indexediv)(enum_type target, uint_type index, enum_type pname, int_type * params);
		extern uint_type (*get_subroutine_index)(uint_type program, enum_type shadertype, char_type const* name);
		extern int_type (*get_subroutine_uniform_location)(uint_type program, enum_type shadertype, char_type const* name);
		extern void (*get_uniform_subroutineuiv)(enum_type shadertype, int_type location, uint_type * params);
		extern void (*get_uniformdv)(uint_type program, int_type location, double_type * params);
		extern boolean_type (*is_transform_feedback)(uint_type id);
		extern void (*min_sample_shading)(float_type value);
		extern void (*patch_parameterfv)(enum_type pname, float_type const* values);
		extern void (*patch_parameteri)(enum_type pname, int_type value);
		extern void (*pause_transform_feedback)(void);
		extern void (*resume_transform_feedback)(void);
		extern void (*uniform1d)(int_type location, double_type x);
		extern void (*uniform1dv)(int_type location, sizei_type count, double_type const* value);
		extern void (*uniform2d)(int_type location, double_type x, double_type y);
		extern void (*uniform2dv)(int_type location, sizei_type count, double_type const* value);
		extern void (*uniform3d)(int_type location, double_type x, double_type y, double_type z);
		extern void (*uniform3dv)(int_type location, sizei_type count, double_type const* value);
		extern void (*uniform4d)(int_type location, double_type x, double_type y, double_type z, double_type w);
		extern void (*uniform4dv)(int_type location, sizei_type count, double_type const* value);
		extern void (*uniform_matrix2dv)(int_type location, sizei_type count, boolean_type transpose, double_type const* value);
		extern void (*uniform_matrix2x3dv)(int_type location, sizei_type count, boolean_type transpose, double_type const* value);
		extern void (*uniform_matrix2x4dv)(int_type location, sizei_type count, boolean_type transpose, double_type const* value);
		extern void (*uniform_matrix3dv)(int_type location, sizei_type count, boolean_type transpose, double_type const* value);
		extern void (*uniform_matrix3x2dv)(int_type location, sizei_type count, boolean_type transpose, double_type const* value);
		extern void (*uniform_matrix3x4dv)(int_type location, sizei_type count, boolean_type transpose, double_type const* value);
		extern void (*uniform_matrix4dv)(int_type location, sizei_type count, boolean_type transpose, double_type const* value);
		extern void (*uniform_matrix4x2dv)(int_type location, sizei_type count, boolean_type transpose, double_type const* value);
		extern void (*uniform_matrix4x3dv)(int_type location, sizei_type count, boolean_type transpose, double_type const* value);
		extern void (*uniform_subroutinesuiv)(enum_type shadertype, sizei_type count, uint_type const* indices);

		extern void (*active_shader_program)(uint_type pipeline, uint_type program);
		extern void (*bind_program_pipeline)(uint_type pipeline);
		extern void (*clear_depthf)(float_type d);
		extern uint_type (*create_shader_programv)(enum_type type, sizei_type count, char_type const* const* strings);
		extern void (*delete_program_pipelines)(sizei_type n, uint_type const* pipelines);
		extern void (*depth_range_arrayv)(uint_type first, sizei_type count, double_type const* v);
		extern void (*depth_range_indexed)(uint_type index, double_type n, double_type f);
		extern void (*depth_rangef)(float_type n, float_type f);
		extern void (*gen_program_pipelines)(sizei_type n, uint_type * pipelines);
		extern void (*get_doublei_v)(enum_type target, uint_type index, double_type * data);
		extern void (*get_floati_v)(enum_type target, uint_type index, float_type * data);
		extern void (*get_program_binary)(uint_type program, sizei_type bufSize, sizei_type * length, enum_type * binaryFormat, void * binary);
		extern void (*get_program_pipeline_info_log)(uint_type pipeline, sizei_type bufSize, sizei_type * length, char_type * infoLog);
		extern void (*get_program_pipelineiv)(uint_type pipeline, enum_type pname, int_type * params);
		extern void (*get_shader_precision_format)(enum_type shadertype, enum_type precisiontype, int_type * range, int_type * precision);
		extern void (*get_vertex_attrib_ldv)(uint_type index, enum_type pname, double_type * params);
		extern boolean_type (*is_program_pipeline)(uint_type pipeline);
		extern void (*program_binary)(uint_type program, enum_type binaryFormat, void const* binary, sizei_type length);
		extern void (*program_parameteri)(uint_type program, enum_type pname, int_type value);
		extern void (*program_uniform1d)(uint_type program, int_type location, double_type v0);
		extern void (*program_uniform1dv)(uint_type program, int_type location, sizei_type count, double_type const* value);
		extern void (*program_uniform1f)(uint_type program, int_type location, float_type v0);
		extern void (*program_uniform1fv)(uint_type program, int_type location, sizei_type count, float_type const* value);
		extern void (*program_uniform1i)(uint_type program, int_type location, int_type v0);
		extern void (*program_uniform1iv)(uint_type program, int_type location, sizei_type count, int_type const* value);
		extern void (*program_uniform1ui)(uint_type program, int_type location, uint_type v0);
		extern void (*program_uniform1uiv)(uint_type program, int_type location, sizei_type count, uint_type const* value);
		extern void (*program_uniform2d)(uint_type program, int_type location, double_type v0, double_type v1);
		extern void (*program_uniform2dv)(uint_type program, int_type location, sizei_type count, double_type const* value);
		extern void (*program_uniform2f)(uint_type program, int_type location, float_type v0, float_type v1);
		extern void (*program_uniform2fv)(uint_type program, int_type location, sizei_type count, float_type const* value);
		extern void (*program_uniform2i)(uint_type program, int_type location, int_type v0, int_type v1);
		extern void (*program_uniform2iv)(uint_type program, int_type location, sizei_type count, int_type const* value);
		extern void (*program_uniform2ui)(uint_type program, int_type location, uint_type v0, uint_type v1);
		extern void (*program_uniform2uiv)(uint_type program, int_type location, sizei_type count, uint_type const* value);
		extern void (*program_uniform3d)(uint_type program, int_type location, double_type v0, double_type v1, double_type v2);
		extern void (*program_uniform3dv)(uint_type program, int_type location, sizei_type count, double_type const* value);
		extern void (*program_uniform3f)(uint_type program, int_type location, float_type v0, float_type v1, float_type v2);
		extern void (*program_uniform3fv)(uint_type program, int_type location, sizei_type count, float_type const* value);
		extern void (*program_uniform3i)(uint_type program, int_type location, int_type v0, int_type v1, int_type v2);
		extern void (*program_uniform3iv)(uint_type program, int_type location, sizei_type count, int_type const* value);
		extern void (*program_uniform3ui)(uint_type program, int_type location, uint_type v0, uint_type v1, uint_type v2);
		extern void (*program_uniform3uiv)(uint_type program, int_type location, sizei_type count, uint_type const* value);
		extern void (*program_uniform4d)(uint_type program, int_type location, double_type v0, double_type v1, double_type v2, double_type v3);
		extern void (*program_uniform4dv)(uint_type program, int_type location, sizei_type count, double_type const* value);
		extern void (*program_uniform4f)(uint_type program, int_type location, float_type v0, float_type v1, float_type v2, float_type v3);
		extern void (*program_uniform4fv)(uint_type program, int_type location, sizei_type count, float_type const* value);
		extern void (*program_uniform4i)(uint_type program, int_type location, int_type v0, int_type v1, int_type v2, int_type v3);
		extern void (*program_uniform4iv)(uint_type program, int_type location, sizei_type count, int_type const* value);
		extern void (*program_uniform4ui)(uint_type program, int_type location, uint_type v0, uint_type v1, uint_type v2, uint_type v3);
		extern void (*program_uniform4uiv)(uint_type program, int_type location, sizei_type count, uint_type const* value);
		extern void (*program_uniform_matrix2dv)(uint_type program, int_type location, sizei_type count, boolean_type transpose, double_type const* value);
		extern void (*program_uniform_matrix2fv)(uint_type program, int_type location, sizei_type count, boolean_type transpose, float_type const* value);
		extern void (*program_uniform_matrix2x3dv)(uint_type program, int_type location, sizei_type count, boolean_type transpose, double_type const* value);
		extern void (*program_uniform_matrix2x3fv)(uint_type program, int_type location, sizei_type count, boolean_type transpose, float_type const* value);
		extern void (*program_uniform_matrix2x4dv)(uint_type program, int_type location, sizei_type count, boolean_type transpose, double_type const* value);
		extern void (*program_uniform_matrix2x4fv)(uint_type program, int_type location, sizei_type count, boolean_type transpose, float_type const* value);
		extern void (*program_uniform_matrix3dv)(uint_type program, int_type location, sizei_type count, boolean_type transpose, double_type const* value);
		extern void (*program_uniform_matrix3fv)(uint_type program, int_type location, sizei_type count, boolean_type transpose, float_type const* value);
		extern void (*program_uniform_matrix3x2dv)(uint_type program, int_type location, sizei_type count, boolean_type transpose, double_type const* value);
		extern void (*program_uniform_matrix3x2fv)(uint_type program, int_type location, sizei_type count, boolean_type transpose, float_type const* value);
		extern void (*program_uniform_matrix3x4dv)(uint_type program, int_type location, sizei_type count, boolean_type transpose, double_type const* value);
		extern void (*program_uniform_matrix3x4fv)(uint_type program, int_type location, sizei_type count, boolean_type transpose, float_type const* value);
		extern void (*program_uniform_matrix4dv)(uint_type program, int_type location, sizei_type count, boolean_type transpose, double_type const* value);
		extern void (*program_uniform_matrix4fv)(uint_type program, int_type location, sizei_type count, boolean_type transpose, float_type const* value);
		extern void (*program_uniform_matrix4x2dv)(uint_type program, int_type location, sizei_type count, boolean_type transpose, double_type const* value);
		extern void (*program_uniform_matrix4x2fv)(uint_type program, int_type location, sizei_type count, boolean_type transpose, float_type const* value);
		extern void (*program_uniform_matrix4x3dv)(uint_type program, int_type location, sizei_type count, boolean_type transpose, double_type const* value);
		extern void (*program_uniform_matrix4x3fv)(uint_type program, int_type location, sizei_type count, boolean_type transpose, float_type const* value);
		extern void (*release_shader_compiler)(void);
		extern void (*scissor_arrayv)(uint_type first, sizei_type count, int_type const* v);
		extern void (*scissor_indexed)(uint_type index, int_type left, int_type bottom, sizei_type width, sizei_type height);
		extern void (*scissor_indexedv)(uint_type index, int_type const* v);
		extern void (*shader_binary)(sizei_type count, uint_type const* shaders, enum_type binaryformat, void const* binary, sizei_type length);
		extern void (*use_program_stages)(uint_type pipeline, bitfield_type stages, uint_type program);
		extern void (*validate_program_pipeline)(uint_type pipeline);
		extern void (*vertex_attrib_l1d)(uint_type index, double_type x);
		extern void (*vertex_attrib_l1dv)(uint_type index, double_type const* v);
		extern void (*vertex_attrib_l2d)(uint_type index, double_type x, double_type y);
		extern void (*vertex_attrib_l2dv)(uint_type index, double_type const* v);
		extern void (*vertex_attrib_l3d)(uint_type index, double_type x, double_type y, double_type z);
		extern void (*vertex_attrib_l3dv)(uint_type index, double_type const* v);
		extern void (*vertex_attrib_l4d)(uint_type index, double_type x, double_type y, double_type z, double_type w);
		extern void (*vertex_attrib_l4dv)(uint_type index, double_type const* v);
		extern void (*vertex_attrib_l_pointer)(uint_type index, int_type size, enum_type type, sizei_type stride, void const* pointer);
		extern void (*viewport_arrayv)(uint_type first, sizei_type count, float_type const* v);
		extern void (*viewport_indexedf)(uint_type index, float_type x, float_type y, float_type w, float_type h);
		extern void (*viewport_indexedfv)(uint_type index, float_type const* v);

		extern void (*bind_image_texture)(uint_type unit, uint_type texture, int_type level, boolean_type layered, int_type layer, enum_type access, enum_type format);
		extern void (*draw_arrays_instanced_base_instance)(enum_type mode, int_type first, sizei_type count, sizei_type instancecount, uint_type baseinstance);
		extern void (*draw_elements_instanced_base_instance)(enum_type mode, sizei_type count, enum_type type, void const* indices, sizei_type instancecount, uint_type baseinstance);
		extern void (*draw_elements_instanced_base_vertex_base_instance)(enum_type mode, sizei_type count, enum_type type, void const* indices, sizei_type instancecount, int_type basevertex, uint_type baseinstance);
		extern void (*draw_transform_feedback_instanced)(enum_type mode, uint_type id, sizei_type instancecount);
		extern void (*draw_transform_feedback_stream_instanced)(enum_type mode, uint_type id, uint_type stream, sizei_type instancecount);
		extern void (*get_active_atomic_counter_bufferiv)(uint_type program, uint_type bufferIndex, enum_type pname, int_type * params);
		extern void (*get_internalformativ)(enum_type target, enum_type internalformat, enum_type pname, sizei_type bufSize, int_type * params);
		extern void (*memory_barrier)(bitfield_type barriers);
		extern void (*tex_storage1d)(enum_type target, sizei_type levels, enum_type internalformat, sizei_type width);
		extern void (*tex_storage2d)(enum_type target, sizei_type levels, enum_type internalformat, sizei_type width, sizei_type height);
		extern void (*tex_storage3d)(enum_type target, sizei_type levels, enum_type internalformat, sizei_type width, sizei_type height, sizei_type depth);

		extern void (*bind_vertex_buffer)(uint_type bindingindex, uint_type buffer, intptr_type offset, sizei_type stride);
		extern void (*clear_buffer_data)(enum_type target, enum_type internalformat, enum_type format, enum_type type, void const* data);
		extern void (*clear_buffer_sub_data)(enum_type target, enum_type internalformat, intptr_type offset, sizeiptr_type size, enum_type format, enum_type type, void const* data);
		extern void (*copy_image_sub_data)(uint_type srcName, enum_type srcTarget, int_type srcLevel, int_type srcX, int_type srcY, int_type srcZ, uint_type dstName, enum_type dstTarget, int_type dstLevel, int_type dstX, int_type dstY, int_type dstZ, sizei_type srcWidth, sizei_type srcHeight, sizei_type srcDepth);
		extern void (*debug_message_callback)(debug_proc_type* callback, void const* userParam);
		extern void (*debug_message_control)(enum_type source, enum_type type, enum_type severity, sizei_type count, uint_type const* ids, boolean_type enabled);
		extern void (*debug_message_insert)(enum_type source, enum_type type, uint_type id, enum_type severity, sizei_type length, char_type const* buf);
		extern void (*dispatch_compute)(uint_type num_groups_x, uint_type num_groups_y, uint_type num_groups_z);
		extern void (*dispatch_compute_indirect)(intptr_type indirect);
		extern void (*framebuffer_parameteri)(enum_type target, enum_type pname, int_type param);
		extern uint_type (*get_debug_message_log)(uint_type count, sizei_type bufSize, enum_type * sources, enum_type * types, uint_type * ids, enum_type * severities, sizei_type * lengths, char_type * messageLog);
		extern void (*get_framebuffer_parameteriv)(enum_type target, enum_type pname, int_type * params);
		extern void (*get_internalformati64v)(enum_type target, enum_type internalformat, enum_type pname, sizei_type bufSize, int64_type * params);
		extern void (*get_object_label)(enum_type identifier, uint_type name, sizei_type bufSize, sizei_type * length, char_type * label);
		extern void (*get_object_ptr_label)(void const* ptr, sizei_type bufSize, sizei_type * length, char_type * label);
		extern void (*get_program_interfaceiv)(uint_type program, enum_type programInterface, enum_type pname, int_type * params);
		extern uint_type (*get_program_resource_index)(uint_type program, enum_type programInterface, char_type const* name);
		extern int_type (*get_program_resource_location)(uint_type program, enum_type programInterface, char_type const* name);
		extern int_type (*get_program_resource_location_index)(uint_type program, enum_type programInterface, char_type const* name);
		extern void (*get_program_resource_name)(uint_type program, enum_type programInterface, uint_type index, sizei_type bufSize, sizei_type * length, char_type * name);
		extern void (*get_program_resourceiv)(uint_type program, enum_type programInterface, uint_type index, sizei_type propCount, enum_type const* props, sizei_type bufSize, sizei_type * length, int_type * params);
		extern void (*invalidate_buffer_data)(uint_type buffer);
		extern void (*invalidate_buffer_sub_data)(uint_type buffer, intptr_type offset, sizeiptr_type length);
		extern void (*invalidate_framebuffer)(enum_type target, sizei_type numAttachments, enum_type const* attachments);
		extern void (*invalidate_sub_framebuffer)(enum_type target, sizei_type numAttachments, enum_type const* attachments, int_type x, int_type y, sizei_type width, sizei_type height);
		extern void (*invalidate_tex_image)(uint_type texture, int_type level);
		extern void (*invalidate_tex_sub_image)(uint_type texture, int_type level, int_type xoffset, int_type yoffset, int_type zoffset, sizei_type width, sizei_type height, sizei_type depth);
		extern void (*multi_draw_arrays_indirect)(enum_type mode, void const* indirect, sizei_type drawcount, sizei_type stride);
		extern void (*multi_draw_elements_indirect)(enum_type mode, enum_type type, void const* indirect, sizei_type drawcount, sizei_type stride);
		extern void (*object_label)(enum_type identifier, uint_type name, sizei_type length, char_type const* label);
		extern void (*object_ptr_label)(void const* ptr, sizei_type length, char_type const* label);
		extern void (*pop_debug_group)(void);
		extern void (*push_debug_group)(enum_type source, uint_type id, sizei_type length, char_type const* message);
		extern void (*shader_storage_block_binding)(uint_type program, uint_type storageBlockIndex, uint_type storageBlockBinding);
		extern void (*tex_buffer_range)(enum_type target, enum_type internalformat, uint_type buffer, intptr_type offset, sizeiptr_type size);
		extern void (*tex_storage2d_multisample)(enum_type target, sizei_type samples, enum_type internalformat, sizei_type width, sizei_type height, boolean_type fixedsamplelocations);
		extern void (*tex_storage3d_multisample)(enum_type target, sizei_type samples, enum_type internalformat, sizei_type width, sizei_type height, sizei_type depth, boolean_type fixedsamplelocations);
		extern void (*texture_view)(uint_type texture, enum_type target, uint_type origtexture, enum_type internalformat, uint_type minlevel, uint_type numlevels, uint_type minlayer, uint_type numlayers);
		extern void (*vertex_attrib_binding)(uint_type attribindex, uint_type bindingindex);
		extern void (*vertex_attrib_format)(uint_type attribindex, int_type size, enum_type type, boolean_type normalized, uint_type relativeoffset);
		extern void (*vertex_attrib_i_format)(uint_type attribindex, int_type size, enum_type type, uint_type relativeoffset);
		extern void (*vertex_attrib_l_format)(uint_type attribindex, int_type size, enum_type type, uint_type relativeoffset);
		extern void (*vertex_binding_divisor)(uint_type bindingindex, uint_type divisor);

		extern void (*bind_buffers_base)(enum_type target, uint_type first, sizei_type count, uint_type const* buffers);
		extern void (*bind_buffers_range)(enum_type target, uint_type first, sizei_type count, uint_type const* buffers, intptr_type const* offsets, sizeiptr_type const* sizes);
		extern void (*bind_image_textures)(uint_type first, sizei_type count, uint_type const* textures);
		extern void (*bind_samplers)(uint_type first, sizei_type count, uint_type const* samplers);
		extern void (*bind_textures)(uint_type first, sizei_type count, uint_type const* textures);
		extern void (*bind_vertex_buffers)(uint_type first, sizei_type count, uint_type const* buffers, intptr_type const* offsets, sizei_type const* strides);
		extern void (*buffer_storage)(enum_type target, sizeiptr_type size, void const* data, bitfield_type flags);
		extern void (*clear_tex_image)(uint_type texture, int_type level, enum_type format, enum_type type, void const* data);
		extern void (*clear_tex_sub_image)(uint_type texture, int_type level, int_type xoffset, int_type yoffset, int_type zoffset, sizei_type width, sizei_type height, sizei_type depth, enum_type format, enum_type type, void const* data);

		extern void (*bind_texture_unit)(uint_type unit, uint_type texture);
		extern void (*blit_named_framebuffer)(uint_type readFramebuffer, uint_type drawFramebuffer, int_type srcX0, int_type srcY0, int_type srcX1, int_type srcY1, int_type dstX0, int_type dstY0, int_type dstX1, int_type dstY1, bitfield_type mask, enum_type filter);
		extern enum_type (*check_named_framebuffer_status)(uint_type framebuffer, enum_type target);
		extern void (*clear_named_buffer_data)(uint_type buffer, enum_type internalformat, enum_type format, enum_type type, void const* data);
		extern void (*clear_named_buffer_sub_data)(uint_type buffer, enum_type internalformat, intptr_type offset, sizeiptr_type size, enum_type format, enum_type type, void const* data);
		extern void (*clear_named_framebufferfi)(uint_type framebuffer, enum_type buffer, int_type drawbuffer, const float_type depth, int_type stencil);
		extern void (*clear_named_framebufferfv)(uint_type framebuffer, enum_type buffer, int_type drawbuffer, float_type const* value);
		extern void (*clear_named_framebufferiv)(uint_type framebuffer, enum_type buffer, int_type drawbuffer, int_type const* value);
		extern void (*clear_named_framebufferuiv)(uint_type framebuffer, enum_type buffer, int_type drawbuffer, uint_type const* value);
		extern void (*clip_control)(enum_type origin, enum_type depth);
		extern void (*compressed_texture_sub_image1d)(uint_type texture, int_type level, int_type xoffset, sizei_type width, enum_type format, sizei_type imageSize, void const* data);
		extern void (*compressed_texture_sub_image2d)(uint_type texture, int_type level, int_type xoffset, int_type yoffset, sizei_type width, sizei_type height, enum_type format, sizei_type imageSize, void const* data);
		extern void (*compressed_texture_sub_image3d)(uint_type texture, int_type level, int_type xoffset, int_type yoffset, int_type zoffset, sizei_type width, sizei_type height, sizei_type depth, enum_type format, sizei_type imageSize, void const* data);
		extern void (*copy_named_buffer_sub_data)(uint_type readBuffer, uint_type writeBuffer, intptr_type readOffset, intptr_type writeOffset, sizeiptr_type size);
		extern void (*copy_texture_sub_image1d)(uint_type texture, int_type level, int_type xoffset, int_type x, int_type y, sizei_type width);
		extern void (*copy_texture_sub_image2d)(uint_type texture, int_type level, int_type xoffset, int_type yoffset, int_type x, int_type y, sizei_type width, sizei_type height);
		extern void (*copy_texture_sub_image3d)(uint_type texture, int_type level, int_type xoffset, int_type yoffset, int_type zoffset, int_type x, int_type y, sizei_type width, sizei_type height);
		extern void (*create_buffers)(sizei_type n, uint_type * buffers);
		extern void (*create_framebuffers)(sizei_type n, uint_type * framebuffers);
		extern void (*create_program_pipelines)(sizei_type n, uint_type * pipelines);
		extern void (*create_queries)(enum_type target, sizei_type n, uint_type * ids);
		extern void (*create_renderbuffers)(sizei_type n, uint_type * renderbuffers);
		extern void (*create_samplers)(sizei_type n, uint_type * samplers);
		extern void (*create_textures)(enum_type target, sizei_type n, uint_type * textures);
		extern void (*create_transform_feedbacks)(sizei_type n, uint_type * ids);
		extern void (*create_vertex_arrays)(sizei_type n, uint_type * arrays);
		extern void (*disable_vertex_array_attrib)(uint_type vaobj, uint_type index);
		extern void (*enable_vertex_array_attrib)(uint_type vaobj, uint_type index);
		extern void (*flush_mapped_named_buffer_range)(uint_type buffer, intptr_type offset, sizeiptr_type length);
		extern void (*generate_texture_mipmap)(uint_type texture);
		extern void (*get_compressed_texture_image)(uint_type texture, int_type level, sizei_type bufSize, void * pixels);
		extern void (*get_compressed_texture_sub_image)(uint_type texture, int_type level, int_type xoffset, int_type yoffset, int_type zoffset, sizei_type width, sizei_type height, sizei_type depth, sizei_type bufSize, void * pixels);
		extern enum_type (*get_graphics_reset_status)(void);
		extern void (*get_named_buffer_parameteri64v)(uint_type buffer, enum_type pname, int64_type * params);
		extern void (*get_named_buffer_parameteriv)(uint_type buffer, enum_type pname, int_type * params);
		extern void (*get_named_buffer_pointerv)(uint_type buffer, enum_type pname, void ** params);
		extern void (*get_named_buffer_sub_data)(uint_type buffer, intptr_type offset, sizeiptr_type size, void * data);
		extern void (*get_named_framebuffer_attachment_parameteriv)(uint_type framebuffer, enum_type attachment, enum_type pname, int_type * params);
		extern void (*get_named_framebuffer_parameteriv)(uint_type framebuffer, enum_type pname, int_type * param);
		extern void (*get_named_renderbuffer_parameteriv)(uint_type renderbuffer, enum_type pname, int_type * params);
		extern void (*get_query_buffer_objecti64v)(uint_type id, uint_type buffer, enum_type pname, intptr_type offset);
		extern void (*get_query_buffer_objectiv)(uint_type id, uint_type buffer, enum_type pname, intptr_type offset);
		extern void (*get_query_buffer_objectui64v)(uint_type id, uint_type buffer, enum_type pname, intptr_type offset);
		extern void (*get_query_buffer_objectuiv)(uint_type id, uint_type buffer, enum_type pname, intptr_type offset);
		extern void (*get_texture_image)(uint_type texture, int_type level, enum_type format, enum_type type, sizei_type bufSize, void * pixels);
		extern void (*get_texture_level_parameterfv)(uint_type texture, int_type level, enum_type pname, float_type * params);
		extern void (*get_texture_level_parameteriv)(uint_type texture, int_type level, enum_type pname, int_type * params);
		extern void (*get_texture_parameter_iiv)(uint_type texture, enum_type pname, int_type * params);
		extern void (*get_texture_parameter_iuiv)(uint_type texture, enum_type pname, uint_type * params);
		extern void (*get_texture_parameterfv)(uint_type texture, enum_type pname, float_type * params);
		extern void (*get_texture_parameteriv)(uint_type texture, enum_type pname, int_type * params);
		extern void (*get_texture_sub_image)(uint_type texture, int_type level, int_type xoffset, int_type yoffset, int_type zoffset, sizei_type width, sizei_type height, sizei_type depth, enum_type format, enum_type type, sizei_type bufSize, void * pixels);
		extern void (*get_transform_feedbacki64_v)(uint_type xfb, enum_type pname, uint_type index, int64_type * param);
		extern void (*get_transform_feedbacki_v)(uint_type xfb, enum_type pname, uint_type index, int_type * param);
		extern void (*get_transform_feedbackiv)(uint_type xfb, enum_type pname, int_type * param);
		extern void (*get_vertex_array_indexed64iv)(uint_type vaobj, uint_type index, enum_type pname, int64_type * param);
		extern void (*get_vertex_array_indexediv)(uint_type vaobj, uint_type index, enum_type pname, int_type * param);
		extern void (*get_vertex_arrayiv)(uint_type vaobj, enum_type pname, int_type * param);
		extern void (*get_n_compressed_tex_image)(enum_type target, int_type lod, sizei_type bufSize, void * pixels);
		extern void (*get_n_tex_image)(enum_type target, int_type level, enum_type format, enum_type type, sizei_type bufSize, void * pixels);
		extern void (*get_n_uniformdv)(uint_type program, int_type location, sizei_type bufSize, double_type * params);
		extern void (*get_n_uniformfv)(uint_type program, int_type location, sizei_type bufSize, float_type * params);
		extern void (*get_n_uniformiv)(uint_type program, int_type location, sizei_type bufSize, int_type * params);
		extern void (*get_n_uniformuiv)(uint_type program, int_type location, sizei_type bufSize, uint_type * params);
		extern void (*invalidate_named_framebuffer_data)(uint_type framebuffer, sizei_type numAttachments, enum_type const* attachments);
		extern void (*invalidate_named_framebuffer_sub_data)(uint_type framebuffer, sizei_type numAttachments, enum_type const* attachments, int_type x, int_type y, sizei_type width, sizei_type height);
		extern void * (*map_named_buffer)(uint_type buffer, enum_type access);
		extern void * (*map_named_buffer_range)(uint_type buffer, intptr_type offset, sizeiptr_type length, bitfield_type access);
		extern void (*memory_barrier_by_region)(bitfield_type barriers);
		extern void (*named_buffer_data)(uint_type buffer, sizeiptr_type size, void const* data, enum_type usage);
		extern void (*named_buffer_storage)(uint_type buffer, sizeiptr_type size, void const* data, bitfield_type flags);
		extern void (*named_buffer_sub_data)(uint_type buffer, intptr_type offset, sizeiptr_type size, void const* data);
		extern void (*named_framebuffer_draw_buffer)(uint_type framebuffer, enum_type buf);
		extern void (*named_framebuffer_draw_buffers)(uint_type framebuffer, sizei_type n, enum_type const* bufs);
		extern void (*named_framebuffer_parameteri)(uint_type framebuffer, enum_type pname, int_type param);
		extern void (*named_framebuffer_read_buffer)(uint_type framebuffer, enum_type src);
		extern void (*named_framebuffer_renderbuffer)(uint_type framebuffer, enum_type attachment, enum_type renderbuffertarget, uint_type renderbuffer);
		extern void (*named_framebuffer_texture)(uint_type framebuffer, enum_type attachment, uint_type texture, int_type level);
		extern void (*named_framebuffer_texture_layer)(uint_type framebuffer, enum_type attachment, uint_type texture, int_type level, int_type layer);
		extern void (*named_renderbuffer_storage)(uint_type renderbuffer, enum_type internalformat, sizei_type width, sizei_type height);
		extern void (*named_renderbuffer_storage_multisample)(uint_type renderbuffer, sizei_type samples, enum_type internalformat, sizei_type width, sizei_type height);
		extern void (*read_n_pixels)(int_type x, int_type y, sizei_type width, sizei_type height, enum_type format, enum_type type, sizei_type bufSize, void * data);
		extern void (*texture_barrier)(void);
		extern void (*texture_buffer)(uint_type texture, enum_type internalformat, uint_type buffer);
		extern void (*texture_buffer_range)(uint_type texture, enum_type internalformat, uint_type buffer, intptr_type offset, sizeiptr_type size);
		extern void (*texture_parameter_iiv)(uint_type texture, enum_type pname, int_type const* params);
		extern void (*texture_parameter_iuiv)(uint_type texture, enum_type pname, uint_type const* params);
		extern void (*texture_parameterf)(uint_type texture, enum_type pname, float_type param);
		extern void (*texture_parameterfv)(uint_type texture, enum_type pname, float_type const* param);
		extern void (*texture_parameteri)(uint_type texture, enum_type pname, int_type param);
		extern void (*texture_parameteriv)(uint_type texture, enum_type pname, int_type const* param);
		extern void (*texture_storage1d)(uint_type texture, sizei_type levels, enum_type internalformat, sizei_type width);
		extern void (*texture_storage2d)(uint_type texture, sizei_type levels, enum_type internalformat, sizei_type width, sizei_type height);
		extern void (*texture_storage2d_multisample)(uint_type texture, sizei_type samples, enum_type internalformat, sizei_type width, sizei_type height, boolean_type fixedsamplelocations);
		extern void (*texture_storage3d)(uint_type texture, sizei_type levels, enum_type internalformat, sizei_type width, sizei_type height, sizei_type depth);
		extern void (*texture_storage3d_multisample)(uint_type texture, sizei_type samples, enum_type internalformat, sizei_type width, sizei_type height, sizei_type depth, boolean_type fixedsamplelocations);
		extern void (*texture_sub_image1d)(uint_type texture, int_type level, int_type xoffset, sizei_type width, enum_type format, enum_type type, void const* pixels);
		extern void (*texture_sub_image2d)(uint_type texture, int_type level, int_type xoffset, int_type yoffset, sizei_type width, sizei_type height, enum_type format, enum_type type, void const* pixels);
		extern void (*texture_sub_image3d)(uint_type texture, int_type level, int_type xoffset, int_type yoffset, int_type zoffset, sizei_type width, sizei_type height, sizei_type depth, enum_type format, enum_type type, void const* pixels);
		extern void (*transform_feedback_buffer_base)(uint_type xfb, uint_type index, uint_type buffer);
		extern void (*transform_feedback_buffer_range)(uint_type xfb, uint_type index, uint_type buffer, intptr_type offset, sizeiptr_type size);
		extern boolean_type (*unmap_named_buffer)(uint_type buffer);
		extern void (*vertex_array_attrib_binding)(uint_type vaobj, uint_type attribindex, uint_type bindingindex);
		extern void (*vertex_array_attrib_format)(uint_type vaobj, uint_type attribindex, int_type size, enum_type type, boolean_type normalized, uint_type relativeoffset);
		extern void (*vertex_array_attrib_i_format)(uint_type vaobj, uint_type attribindex, int_type size, enum_type type, uint_type relativeoffset);
		extern void (*vertex_array_attrib_l_format)(uint_type vaobj, uint_type attribindex, int_type size, enum_type type, uint_type relativeoffset);
		extern void (*vertex_array_binding_divisor)(uint_type vaobj, uint_type bindingindex, uint_type divisor);
		extern void (*vertex_array_element_buffer)(uint_type vaobj, uint_type buffer);
		extern void (*vertex_array_vertex_buffer)(uint_type vaobj, uint_type bindingindex, uint_type buffer, intptr_type offset, sizei_type stride);
		extern void (*vertex_array_vertex_buffers)(uint_type vaobj, uint_type first, sizei_type count, uint_type const* buffers, intptr_type const* offsets, sizei_type const* strides);

	}
	using namespace funcs;
	namespace loader
	{

		bool load();


	}
	using namespace loader;
}

