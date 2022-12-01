#version 330 core

// Create a UV coordinate in variable
in vec2 uv_coord;

// Add a sampler2D uniform
uniform sampler2D texture_samp;

// dimensions
uniform int height;
uniform int width;

// filters
uniform bool perPixel;
uniform bool kernelBased;
uniform bool perPixelExtra;
uniform bool kernelBasedExtra;

out vec4 fragColor;

void main()
{
    fragColor = vec4(1);

    // set fragColor using the sampler2D at the UV coordinate
    fragColor = texture(texture_samp, uv_coord);

//    fragColor = vec4(uv_coord[0], uv_coord[1], 0.f, 0.f);

//    fragColor[0] = 0.75f;
//    fragColor[1] = 0.75f;
//    fragColor[2] = 0.75f;

//    fragColor = 1 - fragColor;

    if (perPixel) { // PER PIXEL INVERT FILTER
        fragColor = 1 - fragColor;
    }

    if (perPixelExtra) { // PER PIXEL EXTRA COLOR SWAP FILTER
        fragColor = vec4(fragColor[1], fragColor[2], fragColor[0], 1.f);
    }

}
