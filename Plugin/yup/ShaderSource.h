#pragma once
#ifdef YUP_INCLUDE_GLEW

//============================================================================//
//
// Vertex Shaders
//
//============================================================================//

#define SIMPLE_VERTEX_SHADER R"(

#version 330
uniform mat4 matrix;
layout(location = 0) in vec4 position;
		
void main()
{
	gl_Position = matrix * position;
}
)"

#define UV_VERTEX_SHADER R"(

#version 330
uniform mat4 matrix;
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uv;
		
out vec2 uv_coord;

void main()
{
	gl_Position = matrix * position;
	uv_coord = uv;
}
)"

#define VCOLOR_VERTEX_SHADER R"(

#version 330
uniform mat4 matrix;
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
		
out vec4 vert_color;

void main()
{
	gl_Position = matrix * position;
	vert_color = color;
}
)"

#define POS_VERTEX_SHADER R"(

#version 330
uniform mat4 matrix;
layout(location = 0) in vec4 position;
		
out vec4 vert_pos;

void main()
{
	gl_Position = matrix * position;
	vert_pos = position;
}
)"

#define MONO_RESIZABLE_VERTEX_SHADER R"(

#version 330
uniform mat4 matrix;
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uv;

uniform float u_ratio;
uniform float v_ratio;

out vec2 uv_coord;

void main()
{
	gl_Position = matrix * position;
	uv_coord.x = uv.x * u_ratio;
	uv_coord.y = uv.y * v_ratio;
}
)"

#define STEREO_VERTEX_SHADER R"(

#version 330
uniform mat4 matrix;
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uv;

uniform int uv_mode;

out vec2 uv_coord;
out vec2 lo_bound;
out vec2 hi_bound;

void main()
{
	vec4 pos = position;
	vec2 z_coord = uv.xy;

	// Set UV rectangle

	lo_bound.x = 0.0;
	lo_bound.y = 0.0;
	hi_bound.x = 1.0;
	hi_bound.y = 1.0;

	switch (uv_mode)
	{
	case 1:									// 1: Top half
		uv_coord.x = uv.x;
		uv_coord.y = uv.y * 0.5;
		hi_bound.y = 0.5;
		break;

	case 2:									// 2: Bottom half
		uv_coord.x = uv.x;
		uv_coord.y = uv.y * 0.5 + 0.5;
		lo_bound.y = 0.5;
		break;

	case 3:									// 3: Left half
		uv_coord.x = uv.x * 0.5;
		uv_coord.y = uv.y;
		hi_bound.x = 0.5;
		break;

	case 4:									// 4: Right half
		uv_coord.x = uv.x * 0.5 + 0.5;
		uv_coord.y = uv.y;
		lo_bound.x = 0.5;
		break;

	case 5:									// 5: RGB over depth
		uv_coord.x = uv.x;
		uv_coord.y = uv.y * 0.5;
		hi_bound.y = 0.5;
		z_coord.y = z_coord.y * 0.5 + 0.5;
		break;

	default:
		uv_coord = uv;
	}

	gl_Position = matrix * pos;
}
)"

//============================================================================//
//
// Vertex Shaders
//
//============================================================================//

#define SOLID_FRAGMENT_SHADER R"(

#version 330
uniform vec4 color;
out vec4 outputColor;
		
void main()
{
	outputColor = color;
}

)"

#define VCOLOR_FRAGMENT_SHADER R"(

#version 330
in vec4 vert_color;
out vec4 outputColor;

void main()
{
	outputColor = vert_color;
}

)"

#define POS_FRAGMENT_SHADER R"(

#version 330
in vec4 vert_pos;
out vec4 outputColor;
		
void main()
{
	outputColor.rgb = abs(vert_pos.xyz);
	outputColor.a = 1;
}

)"

#define DEPTH_FRAGMENT_SHADER R"(

#version 330
out vec4 outputColor;
		
void main()
{
	float luma = gl_FragCoord.z * gl_FragCoord.w; 
	outputColor = vec4(0.0, luma, 0.0, 1.0);
}

)"

#define RGB_FRAGMENT_SHADER R"(

#version 330
uniform sampler2D tex0;
in vec2 uv_coord;

out vec4 outputColor;
		
void main()
{
	outputColor = texture(tex0, uv_coord);
}

)"

#define STEREO_RGBHSV_FRAGMENT_SHADER R"(

#version 330
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;

uniform int color_mode;
uniform float opacity;

uniform int clip_out_of_bound;

in vec2 uv_coord;
in vec2 lo_bound;
in vec2 hi_bound;

out vec4 outputColor;

const vec3 R_cf = vec3(1.164383, 0.000000, 1.596027);
const vec3 G_cf = vec3(1.164383, -0.391762, -0.812968);
const vec3 B_cf = vec3(1.164383, 2.017232, 0.000000);
const vec3 offset = vec3(-0.0625, -0.5, -0.5);

void main()
{
	if (clip_out_of_bound != 0 && (
		uv_coord.x < lo_bound.x ||
		uv_coord.x > hi_bound.x ||
		uv_coord.y < lo_bound.y ||
		uv_coord.y > hi_bound.y))
	{
		outputColor = vec4(0.15, 0.15, 0.18, 0.0);
	}
	else if (color_mode == 1)
	{
		float y = texture(tex0, uv_coord).r;
		float u = texture(tex1, uv_coord).r;
		float v = texture(tex2, uv_coord).r;
		vec3 yuv = vec3(y, u, v);
		yuv += offset;
		outputColor = vec4(0.0, 0.0, 0.0, 1.0);
		outputColor.r = dot(yuv, R_cf);
		outputColor.g = dot(yuv, G_cf);
		outputColor.b = dot(yuv, B_cf);
	}
	else
	{
		outputColor = texture(tex0, uv_coord);
	}

	outputColor.rgb = outputColor.rgb * opacity;
}
)"

#endif // YUP_INCLUDE_GLEW
