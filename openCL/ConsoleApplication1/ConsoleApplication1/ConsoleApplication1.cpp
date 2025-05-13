// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <CL/cl.h>

#define ARRAY_SIZE 10

const char* kernelSource =
"__kernel void arrayAdd(__global int* A, __global int* B, __global int* C) {"
"    int gid = get_global_id(0);"
"    C[gid] = A[gid] + B[gid];"
"}";

int main() {
    // Create input arrays
    int A[ARRAY_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int B[ARRAY_SIZE] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
    int C[ARRAY_SIZE] = { 0 };

    // Get the platform and device information
    cl_platform_id platform;
    clGetPlatformIDs(1, &platform, NULL);

    cl_device_id device;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    // Create an OpenCL context
    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);

    // Create a command queue
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, NULL);


    // Compilation


    // Create the input and output buffers
    cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int) * ARRAY_SIZE, NULL, NULL);
    cl_mem bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int) * ARRAY_SIZE, NULL, NULL);
    cl_mem bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(int) * ARRAY_SIZE, NULL, NULL);

    // Write data to the input buffers
    clEnqueueWriteBuffer(queue, bufferA, CL_TRUE, 0, sizeof(int) * ARRAY_SIZE, A, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, bufferB, CL_TRUE, 0, sizeof(int) * ARRAY_SIZE, B, 0, NULL, NULL);

    // Create and compile the OpenCL program
    cl_program program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    // Create the kernel
    cl_kernel kernel = clCreateKernel(program, "arrayAdd", NULL);

    // Set the arguments of the kernel
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufferA);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferB);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferC);

    // Execute the kernel
    size_t globalWorkSize = ARRAY_SIZE;
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);

    // Read the result from the device
    clEnqueueReadBuffer(queue, bufferC, CL_TRUE, 0, sizeof(int) * ARRAY_SIZE, C, 0, NULL, NULL);

    // Print the result
    printf("Result: ");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", C[i]);
    }
    printf("\n");

    // Release OpenCL resources
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
