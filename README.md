# Image-Based Lighting Generation
This is tool takes an equirectangular hdr image and generates irradiance and specular cubemap textures for image-based lighting used in Physically Based Shading.

For the moment the tool outputs .png and ktx files for the textures. The Ktx format is RGB16F and is not compressed.

# Usage

## Requirements
The tool requires [Bazel build](https://bazel.build/) process.

Mac/Linux: CMAKE is also required.

## Prepare source image

Replace the image in data/source.hdr with a different equirectangular image that you want to use for generating the maps.

## Build
Once you're set up, run in command line:

```
$ bazel run :tool
```

And the images will be generated and outputted to the bazel-bin/tool.runfiles/__MAIN__ folder.

# Future Plans
Hopefully in the near future:

- Take input image from cmd args.
- Control resolution and outputs from cmd args.
- Remove cmake dependancy on Mac/Linux.

# Note
I made the tool because I needed something to integrate with my build process. I hope it'll be useful for you.

The tool is based on these tutorials:

https://learnopengl.com/PBR/IBL/Diffuse-irradiance  
https://learnopengl.com/PBR/IBL/Specular-IBL

The credit should really go to Joey de Vries for making an amazing website and fantastic tutorials :)
