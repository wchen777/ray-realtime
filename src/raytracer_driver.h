#pragma once

#include <QImage>
#include <QtCore>

#include "raytracer/raytracer.h"
#include "raytracer/raytracescene.h"
#include "utils/sceneparser.h"


void RunRayTracer(int width, int height, QString iScenePath, RGBA* data) {

    // create render data
    RenderData metaData;
    bool success = SceneParser::parse(iScenePath.toStdString(), metaData);

    if (!success) {
        return;
    }

    // Setting up the raytracer
    RayTracer::Config rtConfig{};
    rtConfig.enableShadow        = true;
    rtConfig.enableReflection    = true;
    rtConfig.enableRefraction    = true;
    rtConfig.enableTextureMap    = true;
    rtConfig.enableTextureFilter = true;
    rtConfig.enableParallelism   = true;
    rtConfig.enableSuperSample   = true;
    rtConfig.enableAcceleration  = true;
    rtConfig.enableDepthOfField  = true;

    RayTracer raytracer{ rtConfig };

    RayTraceScene rtScene{ width, height, metaData };

    raytracer.render(data, rtScene);

}
