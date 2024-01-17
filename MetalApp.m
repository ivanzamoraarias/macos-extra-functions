#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>
#include <simd/simd.h>
#import <Cocoa/Cocoa.h>

// Vertex data for the triangle
typedef struct {
    vector_float2 position;
} Vertex;

// Uniform data for the triangle
typedef struct {
    matrix_float4x4 modelViewProjectionMatrix;
} Uniforms;

// Function to create a translation matrix
matrix_float4x4 matrix4x4_translation(float x, float y, float z) {
    matrix_float4x4 matrix = matrix_identity_float4x4;
    matrix.columns[3][0] = x;
    matrix.columns[3][1] = y;
    matrix.columns[3][2] = z;
    return matrix;
}

// Function for perspective projection
matrix_float4x4 matrix_perspective_left_hand(float fovy, float aspect, float near, float far) {
    float yScale = 1.0f / tan(fovy * 0.5f);
    float xScale = yScale / aspect;
    float zRange = far - near;
    float zScale = -(far + near) / zRange;
    float wzScale = -2.0f * far * near / zRange;

    matrix_float4x4 matrix;
    matrix.columns[0] = simd_make_float4(xScale, 0.0f, 0.0f, 0.0f);
    matrix.columns[1] = simd_make_float4(0.0f, yScale, 0.0f, 0.0f);
    matrix.columns[2] = simd_make_float4(0.0f, 0.0f, zScale, -1.0f);
    matrix.columns[3] = simd_make_float4(0.0f, 0.0f, wzScale, 0.0f);

    return matrix;
}

int main(int argc, const char *argv[]) {
    @autoreleasepool {
        // Create Metal device
        id<MTLDevice> device = MTLCreateSystemDefaultDevice();
        if (!device) {
            NSLog(@"Metal is not supported on this device.");
            return -1;
        }

        // Create Metal layer
        CAMetalLayer *metalLayer = [CAMetalLayer layer];
        metalLayer.device = device;
        
        // Set up Metal buffers
        Vertex vertices[] = {
            {{-0.5, -0.5}},
            {{0.5, -0.5}},
            {{0.0, 0.5}}
        };

        id<MTLBuffer> vertexBuffer = [device newBufferWithBytes:vertices length:sizeof(vertices) options:MTLResourceStorageModeShared];

        Uniforms uniforms;
        uniforms.modelViewProjectionMatrix =
            matrix_multiply(
                matrix_perspective_left_hand(M_PI/3, 1.0, 0.1, 100.0) ,
                matrix4x4_translation(0.0, 0.0, -5.0)
            );
        id<MTLBuffer> uniformBuffer = [device newBufferWithBytes:&uniforms length:sizeof(Uniforms) options:MTLResourceStorageModeShared];

        // Create Metal drawable
        NSView *view = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 800, 600)];
        [view setLayer:metalLayer];
        [view setWantsLayer:YES];

        metalLayer.contentsScale = view.window.backingScaleFactor;  // Set contentsScale
        metalLayer.drawableSize = view.bounds.size;  // Set drawableSize

        // Render loop
        while (![NSThread isMainThread]) {
            @autoreleasepool {
                // Create Metal command buffer
                id<MTLCommandBuffer> commandBuffer = [device newCommandQueue].commandBuffer;

                // Create Metal render pass descriptor
                MTLRenderPassDescriptor *passDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
id<CAMetalDrawable> drawable = [passDescriptor.colorAttachments[0] drawable];
passDescriptor.colorAttachments[0].texture = drawable.texture;
passDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
passDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 1.0);
passDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;


                // Create Metal render command encoder
                id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:passDescriptor];
                [renderEncoder setRenderPipelineState:[device newRenderPipelineStateWithDescriptor:[[MTLRenderPipelineDescriptor alloc] init] error:nil]];
                [renderEncoder setVertexBuffer:vertexBuffer offset:0 atIndex:0];
                [renderEncoder setVertexBuffer:uniformBuffer offset:0 atIndex:1];

                // Draw the triangle
                [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];

                [renderEncoder endEncoding];

                // Present drawable
                //id<CAMetalDrawable> drawable = [passDescriptor.colorAttachments[0] drawable];
                [commandBuffer presentDrawable:drawable];


                // Commit command buffer
                [commandBuffer commit];

                // Wait for the completion of the command buffer to check for errors
                [commandBuffer waitUntilCompleted];
                
                // Check for errors during the rendering process
                if (commandBuffer.error) {
                    NSLog(@"Error during command buffer execution: %@", commandBuffer.error);
                }
            }
        }
    }
    return 0;
}
