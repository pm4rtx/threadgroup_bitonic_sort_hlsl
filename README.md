# threadgroup_bitonic_sort\<T\> HLSL ![MIT](https://img.shields.io/badge/license-MIT-blue.svg) [![GitHub](https://img.shields.io/badge/repo-github-green.svg)](https://github.com/pm4rtx/threadgroup_bitonic_sort_hlsl)

## What is it?

This is an implementaion of threadgroup wide bitonic sort in HLSL.

## Purpose

Sometimes, it is desired to sort elements within a thread group on the GPU. The `threadgroup_bitonic_sort.hlsli` header file provides multiple variants of the bitonic sort to support any power-of-2 threadgroup size and the number of sortable elements of up to 4096.

## Features

- It is agnostic of wave/warp sizes
- It automatically switches to sorting and shuffling within waves/warps by utilising wave intrinsics when the sizes of sorted/shuffled blocks become smaller than the size of waves/warps in a threadgroup ([check out AMD RGA codegen on godbolt.org](https://godbolt.org/z/nT95Ta7qx))
- It supports GPUs without wave intrinsic support
- It supports sorting of up to 4096 elements within a thread group (sorting 4096 elements requires the size of a thread group to be 1024 threads)
- For a thread group with `N` threads, it supports sorting of `N`, `N * 2` or `N * 4` elements

## Building Demo

To build `demo.cpp`, run `build.bat` from Visual Studio Command Prompt. The batch file should automatically download the required packages (D3D12, DXC), build and run all shader variants as benchmarks.

The header file can be compiled with [DX Compiler release for February 2025](https://github.com/microsoft/DirectXShaderCompiler/releases/tag/v1.8.2502) or earlier.

## License

This header file is available to anybody free of charge, under the terms of MIT License (see LICENSE.md).
