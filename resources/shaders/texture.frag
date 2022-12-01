#version 330 core

// Create a UV coordinate in variable
in vec2 uv_coord;

// Add a sampler2D uniform
uniform sampler2D texture_samp;

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

//    fragColor = 1 - fragColor;

//    if (perPixel) { // PER PIXEL INVERT FILTER
//        fragColor = 1 - fragColor;
//    }


}
