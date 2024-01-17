#include <metal_stdlib>

using namespace metal;

struct Vertex {
    float2 position [[attribute(0)]];
};

struct Uniforms {
    float4x4 modelViewProjectionMatrix;
};

vertex float4 vertex_main(Vertex in [[stage_in]], constant Uniforms &uniforms [[buffer(1)]]) {
    return uniforms.modelViewProjectionMatrix * float4(in.position, 0.0, 1.0);
}
