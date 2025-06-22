/**
 * Copyright (C) 2025, by Pavel Martishevsky
 *
 * This header is distributed under the MIT License. See notice at the end of this file.
 */

#pragma once

#ifndef D3D12AID_H
#define D3D12AID_H

#ifdef _MSC_VER
    #define D3D12AID_INLINE __forceinline
#else
    #ifdef __cplusplus
        #define D3D12AID_INLINE inline
    #else
        #define D3D12AID_INLINE
    #endif
#endif

#ifdef __cplusplus
    #define D3D12AID_EXTERN extern "C"
#else
    #define D3D12AID_EXTERN extern
#endif

#ifndef D3D12AID_API
    #ifdef D3D12AID_API_STATIC
        #define D3D12AID_API static D3D12AID_INLINE
    #else
        #define D3D12AID_API D3D12AID_EXTERN
    #endif
#endif

#ifndef D3D12AID_CHECK
#define D3D12AID_CHECK(call)                            \
    do                                                  \
    {                                                   \
        HRESULT hr = call;                              \
        if (S_OK != hr)                                 \
        {                                               \
            printf("S_OK != 0x%08lx " #call "\n", hr);  \
            if (IsDebuggerPresent())                    \
                __debugbreak();                         \
        }                                               \
    }                                                   \
    while(0)
#endif

#ifndef D3D12AID_ASSERT
#define D3D12AID_ASSERT(cond)                               \
    do                                                      \
    {                                                       \
        if (!(cond))                                        \
        {                                                   \
            puts("Assert with condition "#cond" failed.");  \
            if (IsDebuggerPresent())                        \
                __debugbreak();                             \
        }                                                   \
    }                                                       \
    while(0)
#endif

#ifndef D3D12AID_SAFE_RELEASE
#define D3D12AID_SAFE_RELEASE(p)    \
    do                              \
    {                               \
        p->Release();               \
        p = NULL;                   \
    }                               \
    while (0)
#endif


D3D12AID_API ID3D12QueryHeap *d3d12aid_QueryHeap_CreateTimestamps(ID3D12Device *device, uint32_t count)
{
    D3D12_QUERY_HEAP_DESC desc;
    ID3D12QueryHeap *heap = NULL;

    desc.Type      = D3D12_QUERY_HEAP_TYPE_TIMESTAMP;
    desc.Count     = count;
    desc.NodeMask  = 0x1;

    D3D12AID_CHECK(device->CreateQueryHeap(&desc, IID_PPV_ARGS(&heap)));
    return heap;
}

D3D12AID_API void d3d12aid_Resource_InitAsBuffer(D3D12_RESOURCE_DESC *outDesc, uint64_t sizeInBytes)
{
    outDesc->Dimension          = D3D12_RESOURCE_DIMENSION_BUFFER;
    outDesc->Alignment          = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
    outDesc->Width              = sizeInBytes;
    outDesc->Height             = 1;
    outDesc->DepthOrArraySize   = 1;
    outDesc->MipLevels          = 1;
    outDesc->Format             = DXGI_FORMAT_UNKNOWN;
    outDesc->SampleDesc.Count   = 1;
    outDesc->SampleDesc.Quality = 0;
    outDesc->Layout             = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    outDesc->Flags              = D3D12_RESOURCE_FLAG_NONE;
}

D3D12AID_API ID3D12Resource *d3d12aid_Resource_CreateCommittedWithHeapFlags(ID3D12Device *device, const D3D12_RESOURCE_DESC *desc, D3D12_HEAP_TYPE heapType, D3D12_HEAP_FLAGS heapFlags)
{
    D3D12_RESOURCE_STATES initState = D3D12_RESOURCE_STATE_COMMON;
    D3D12_HEAP_PROPERTIES heapProps;
    ID3D12Resource *resource = NULL;

    heapProps.Type                  = heapType;
    heapProps.CPUPageProperty       = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProps.MemoryPoolPreference  = D3D12_MEMORY_POOL_UNKNOWN;
    heapProps.CreationNodeMask      = 0x1u;
    heapProps.VisibleNodeMask       = 0x1u;

    if (heapType == D3D12_HEAP_TYPE_UPLOAD)
    {
        initState = D3D12_RESOURCE_STATE_GENERIC_READ;
    }
    else if (heapType == D3D12_HEAP_TYPE_READBACK)
    {
        initState = D3D12_RESOURCE_STATE_COPY_DEST;
    }
    D3D12AID_CHECK(device->CreateCommittedResource(&heapProps, heapFlags, desc, initState, NULL, IID_PPV_ARGS(&resource)));
    return resource;
}

D3D12AID_API ID3D12Resource *d3d12aid_Resource_CreateCommitted(ID3D12Device *device, const D3D12_RESOURCE_DESC *desc, D3D12_HEAP_TYPE heapType)
{
    return d3d12aid_Resource_CreateCommittedWithHeapFlags(device, desc, heapType, D3D12_HEAP_FLAG_NONE);
}

D3D12AID_API void *d3d12aid_Resource_MapSubresourceReadRange(ID3D12Resource *resource, uint32_t subresourceIdx, size_t offsetInBytes, size_t sizeInBytes)
{
    D3D12_RANGE range;
    void *mappedMemory = NULL;

    range.Begin = offsetInBytes;
    range.End   = offsetInBytes + sizeInBytes;

    D3D12AID_CHECK(resource->Map(subresourceIdx, &range, &mappedMemory));
    return mappedMemory;
}

D3D12AID_API void *d3d12aid_Resource_MapSubresourceRead(ID3D12Resource *resource, uint32_t subresourceIdx)
{
    void *mappedMemory = NULL;

    /**
     *  https://learn.microsoft.com/en-us/windows/win32/api/d3d12/nf-d3d12-id3d12resource-map
     *  "This indicates the region the CPU might read, and the coordinates are subresource-relative.
     *   A null pointer indicates the entire subresource might be read by the CPU."
     */
    D3D12AID_CHECK(resource->Map(subresourceIdx, NULL, &mappedMemory));
    return mappedMemory;
}

D3D12AID_API void *d3d12aid_Resource_MapRead(ID3D12Resource *resource)
{
    return d3d12aid_Resource_MapSubresourceRead(resource, 0);
}

D3D12AID_API void *d3d12aid_Resource_MapSubresourceWrite(ID3D12Resource *resource, uint32_t subresourceIdx)
{
    D3D12_RANGE range;
    void *mappedMemory = NULL;

    /**
     *  https://learn.microsoft.com/en-us/windows/win32/api/d3d12/nf-d3d12-id3d12resource-map
     *  "It is valid to specify the CPU won't read any data by passing a range where End is less than or equal to Begin."
     */
    range.Begin = 0;
    range.End   = 0;

    D3D12AID_CHECK(resource->Map(subresourceIdx, &range, &mappedMemory));
    return mappedMemory;
}

D3D12AID_API void *d3d12aid_Resource_MapWrite(ID3D12Resource *resource)
{
    return d3d12aid_Resource_MapSubresourceWrite(resource, 0);
}

D3D12AID_API void d3d12aid_Resource_TransitionBarrier(D3D12_RESOURCE_BARRIER *outBarrier, ID3D12Resource *resource, D3D12_RESOURCE_STATES prevState, D3D12_RESOURCE_STATES nextState)
{
    outBarrier->Type                    = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    outBarrier->Flags                   = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    outBarrier->Transition.pResource    = resource;
    outBarrier->Transition.Subresource  = 0;
    outBarrier->Transition.StateBefore  = prevState;
    outBarrier->Transition.StateAfter   = nextState;
}

D3D12AID_API void d3d12aid_Resource_AliasingBarrier(D3D12_RESOURCE_BARRIER *outBarrier, ID3D12Resource *prevResource, ID3D12Resource *nextResource)
{
    outBarrier->Type                        = D3D12_RESOURCE_BARRIER_TYPE_ALIASING;
    outBarrier->Flags                       = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    outBarrier->Aliasing.pResourceBefore    = prevResource;
    outBarrier->Aliasing.pResourceAfter     = nextResource;
}

D3D12AID_API void d3d12aid_Resource_UavBarrier(D3D12_RESOURCE_BARRIER *outBarrier, ID3D12Resource *resource)
{
    outBarrier->Type                        = D3D12_RESOURCE_BARRIER_TYPE_UAV;
    outBarrier->Flags                       = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    outBarrier->UAV.pResource               = resource;
}

D3D12AID_API ID3D12RootSignature *d3d12aid_RootSignature_Create(ID3D12Device *device, const void *shaderBytecode, size_t shaderBytecodeSizeInBytes)
{
    ID3D12RootSignature *rs = NULL;
    D3D12AID_CHECK(device->CreateRootSignature(0x1, shaderBytecode, shaderBytecodeSizeInBytes, IID_PPV_ARGS(&rs)));
    return rs;
}

D3D12AID_API ID3D12PipelineState *d3d12aid_PipelineState_CreateCompute(ID3D12Device *device, ID3D12RootSignature *rs, const void *shaderBytecode, size_t shaderBytecodeSizeInBytes)
{
    D3D12_COMPUTE_PIPELINE_STATE_DESC desc;
    ID3D12PipelineState *ps = NULL;
    desc.pRootSignature                   = rs;
    desc.CS                               = { shaderBytecode, shaderBytecodeSizeInBytes };
    desc.NodeMask                         = 0x1;
    desc.CachedPSO.pCachedBlob            = NULL;
    desc.CachedPSO.CachedBlobSizeInBytes  = 0;
    desc.Flags                            = D3D12_PIPELINE_STATE_FLAG_NONE;
    D3D12AID_CHECK(device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&ps)));
    return ps;
}

struct d3d12aid_ComputeRsPs
{
    ID3D12RootSignature *rs;
    ID3D12PipelineState *ps;
};

typedef struct d3d12aid_ComputeRsPs d3d12aid_ComputeRsPs;

D3D12AID_API void d3d12aid_ComputeRsPs_Create(d3d12aid_ComputeRsPs *outComputeRsPs, ID3D12Device *device, const void *shaderBytecode, size_t shaderBytecodeSizeInBytes)
{
    outComputeRsPs->rs = d3d12aid_RootSignature_Create(device, shaderBytecode, shaderBytecodeSizeInBytes);
    outComputeRsPs->ps = d3d12aid_PipelineState_CreateCompute(device, outComputeRsPs->rs, shaderBytecode, shaderBytecodeSizeInBytes);
}

D3D12AID_API void d3d12aid_ComputeRsPs_Release(d3d12aid_ComputeRsPs *computeRsPs)
{
    D3D12AID_SAFE_RELEASE(computeRsPs->ps);
    D3D12AID_SAFE_RELEASE(computeRsPs->rs);
}

D3D12AID_API void d3d12aid_ComputeRsPs_Set(const d3d12aid_ComputeRsPs *computeRsPs, ID3D12GraphicsCommandList *cmdList)
{
    cmdList->SetPipelineState(computeRsPs->ps);
    cmdList->SetComputeRootSignature(computeRsPs->rs);
}

struct d3d12aid_Timestamps
{
    ID3D12QueryHeap *heap;
    ID3D12Resource  *readbackBuf;
    const uint64_t  *readbackMem;

    uint32_t         allocatedSlotCount;
    uint32_t         activatedSlotStart;
    uint32_t         activatedSlotCount;

    uint32_t         latencyFrameIndex;
    uint32_t         latencyFrameCount;

    uint32_t         padding;
};

typedef d3d12aid_Timestamps d3d12aid_Timestamps;

D3D12AID_API void d3d12aid_Timestamps_Create(d3d12aid_Timestamps *outTimestamps, ID3D12Device *device, uint32_t perFrameTimestampCount, uint32_t latencyFrameCount)
{
    D3D12_RESOURCE_DESC bufferDesc;
    uint32_t timestampCount = perFrameTimestampCount * latencyFrameCount;

    outTimestamps->heap = d3d12aid_QueryHeap_CreateTimestamps(device, timestampCount);

    /**
     *  NOTE:   we allocate buffer sufficient to read the entire heap
     *          the reason why we don't allocate smaller buffer that could read only per-frame `count` timestamps is
     *          simplificity: we don't want to introduce any synchronisation
     */
    d3d12aid_Resource_InitAsBuffer(&bufferDesc, sizeof(uint64_t) * timestampCount);

    outTimestamps->readbackBuf = d3d12aid_Resource_CreateCommitted(device, &bufferDesc, D3D12_HEAP_TYPE_READBACK);

    /** NOTE: we map the readback buffer during initialisation and unmap when destroying it. This is because D3D12 allows persistent mapping */
    outTimestamps->readbackMem = (uint64_t *)d3d12aid_Resource_MapRead(outTimestamps->readbackBuf);

    outTimestamps->allocatedSlotCount  = perFrameTimestampCount;
    outTimestamps->activatedSlotStart  = 0;
    outTimestamps->activatedSlotCount  = 0;
    outTimestamps->latencyFrameIndex   = 0;
    outTimestamps->latencyFrameCount   = latencyFrameCount;
}

D3D12AID_API void d3d12aid_Timestamps_Release(d3d12aid_Timestamps *timestamps)
{
    timestamps->readbackBuf->Unmap(0, NULL);

    D3D12AID_SAFE_RELEASE(timestamps->readbackBuf);
    D3D12AID_SAFE_RELEASE(timestamps->heap);
}

D3D12AID_API void d3d12aid_Timestamps_AdvanceFrame(d3d12aid_Timestamps *timestamps, ID3D12GraphicsCommandList *cmdList)
{
    uint32_t queryStart = (timestamps->latencyFrameIndex ++ % timestamps->latencyFrameCount) * timestamps->allocatedSlotCount;

    cmdList->ResolveQueryData(timestamps->heap, D3D12_QUERY_TYPE_TIMESTAMP, queryStart, timestamps->activatedSlotCount, timestamps->readbackBuf, queryStart * sizeof(uint64_t));

    timestamps->activatedSlotStart = (timestamps->latencyFrameIndex % timestamps->latencyFrameCount) * timestamps->allocatedSlotCount;
    timestamps->activatedSlotCount = 0;
}

D3D12AID_API uint32_t d3d12aid_Timestamps_Push(d3d12aid_Timestamps *timestamps, ID3D12GraphicsCommandList *cmdList)
{
    D3D12AID_ASSERT(timestamps->activatedSlotCount < timestamps->allocatedSlotCount);
    cmdList->EndQuery(timestamps->heap, D3D12_QUERY_TYPE_TIMESTAMP, timestamps->activatedSlotStart + timestamps->activatedSlotCount);
    return timestamps->activatedSlotCount ++;
}

D3D12AID_API uint64_t d3d12aid_Timestamps_Get(d3d12aid_Timestamps *timestamps, uint32_t latencyFrameIndex, uint32_t timestampIndex)
{
    D3D12AID_ASSERT(latencyFrameIndex < timestamps->latencyFrameCount);
    D3D12AID_ASSERT(timestampIndex < timestamps->allocatedSlotCount);
    return timestamps->readbackMem[latencyFrameIndex  * timestamps->allocatedSlotCount + timestampIndex];
}

D3D12AID_API uint64_t d3d12aid_Timestamps_GetDelta(d3d12aid_Timestamps *timestamps, uint32_t latencyFrameIndex, uint32_t timestampStartIndex, uint32_t timestampStopIndex)
{
    D3D12AID_ASSERT(latencyFrameIndex < timestamps->latencyFrameCount);
    D3D12AID_ASSERT(timestampStartIndex < timestamps->allocatedSlotCount);
    D3D12AID_ASSERT(timestampStopIndex < timestamps->allocatedSlotCount);
    const uint64_t *data = &timestamps->readbackMem[latencyFrameIndex  * timestamps->allocatedSlotCount];
    return data[timestampStopIndex] - data[timestampStartIndex];
}

#ifndef D3D12AID_MAPPED_BUFFER_LATENCY_FRAME_MAX_COUNT
#define D3D12AID_MAPPED_BUFFER_LATENCY_FRAME_MAX_COUNT 3
#endif

struct d3d12aid_MappedBuffer
{
    void           *bufMem[D3D12AID_MAPPED_BUFFER_LATENCY_FRAME_MAX_COUNT];
    ID3D12Resource *bufCpu[D3D12AID_MAPPED_BUFFER_LATENCY_FRAME_MAX_COUNT];
    ID3D12Resource *bufGpu;

    uint32_t        offsInBytes;
    uint32_t        sizeInBytes;

    uint32_t        frameCount;

    D3D12_HEAP_TYPE heapType;
};

typedef struct d3d12aid_MappedBuffer d3d12aid_MappedBuffer;

D3D12AID_API void d3d12aid_MappedBuffer_Create(d3d12aid_MappedBuffer *outBuffer, ID3D12Device *device, uint32_t frameCount, uint32_t sizeInBytes, D3D12_HEAP_TYPE heapType)
{
    D3D12_RESOURCE_DESC desc;
    d3d12aid_Resource_InitAsBuffer(&desc, sizeInBytes);

    D3D12AID_ASSERT(heapType == D3D12_HEAP_TYPE_UPLOAD || heapType == D3D12_HEAP_TYPE_READBACK);
    D3D12AID_ASSERT(frameCount <= D3D12AID_MAPPED_BUFFER_LATENCY_FRAME_MAX_COUNT);

    for (uint32_t i = 0; i < D3D12AID_MAPPED_BUFFER_LATENCY_FRAME_MAX_COUNT; ++i)
        outBuffer->bufCpu[i] = NULL;

    for (uint32_t i = 0; i < D3D12AID_MAPPED_BUFFER_LATENCY_FRAME_MAX_COUNT; ++i)
        outBuffer->bufMem[i] = NULL;

    for (uint32_t i = 0; i < frameCount; ++i)
    {
        outBuffer->bufCpu[i] = d3d12aid_Resource_CreateCommitted(device, &desc, heapType);
    }

    if (heapType == D3D12_HEAP_TYPE_UPLOAD)
    {
        for (uint32_t i = 0; i < frameCount; ++i)
        {
            outBuffer->bufMem[i] = d3d12aid_Resource_MapWrite(outBuffer->bufCpu[i]);
        }
    }
    else if (heapType == D3D12_HEAP_TYPE_READBACK)
    {
        for (uint32_t i = 0; i < frameCount; ++i)
        {
            outBuffer->bufMem[i] = d3d12aid_Resource_MapRead(outBuffer->bufCpu[i]);
        }
    }

    if (heapType == D3D12_HEAP_TYPE_READBACK)
    {
        desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
    }
    outBuffer->bufGpu = d3d12aid_Resource_CreateCommitted(device, &desc, D3D12_HEAP_TYPE_DEFAULT);

    outBuffer->offsInBytes  = 0;
    outBuffer->sizeInBytes  = sizeInBytes;
    outBuffer->frameCount   = frameCount;
    outBuffer->heapType     = heapType;
}

D3D12AID_API void d3d12aid_MappedBuffer_Release(d3d12aid_MappedBuffer *inoutBuffer)
{
    for (uint32_t i = 0; i < inoutBuffer->frameCount; ++i)
    {
        inoutBuffer->bufCpu[i]->Unmap(0, NULL);
        inoutBuffer->bufMem[i] = NULL;
    }
    for (uint32_t i = 0; i < inoutBuffer->frameCount; ++i)
    {
        D3D12AID_SAFE_RELEASE(inoutBuffer->bufCpu[i]);
    }
    D3D12AID_SAFE_RELEASE(inoutBuffer->bufGpu);
}

D3D12AID_API void d3d12aid_MappedBuffer_Append(d3d12aid_MappedBuffer *inoutBuffer, uint32_t frameIndex, const void *data, uint32_t sizeInBytes)
{
    if (inoutBuffer->heapType == D3D12_HEAP_TYPE_UPLOAD)
    {
        D3D12AID_ASSERT(frameIndex < inoutBuffer->frameCount);
        D3D12AID_ASSERT(frameIndex < inoutBuffer->frameCount);
        D3D12AID_ASSERT(inoutBuffer->sizeInBytes - inoutBuffer->offsInBytes >= sizeInBytes);

        memcpy((char *)inoutBuffer->bufMem[frameIndex] + inoutBuffer->offsInBytes, data, sizeInBytes);
        inoutBuffer->offsInBytes += sizeInBytes;
    }
}

D3D12AID_API void d3d12aid_MappedBuffer_Skip(d3d12aid_MappedBuffer* inoutBuffer, uint32_t frameIndex, uint32_t sizeInBytes)
{
    if (inoutBuffer->heapType == D3D12_HEAP_TYPE_UPLOAD)
    {
        D3D12AID_ASSERT(frameIndex < inoutBuffer->frameCount);
        D3D12AID_ASSERT(frameIndex < inoutBuffer->frameCount);
        D3D12AID_ASSERT(inoutBuffer->sizeInBytes - inoutBuffer->offsInBytes <= sizeInBytes);

        memset((char*)inoutBuffer->bufMem[frameIndex] + inoutBuffer->offsInBytes, 0, sizeInBytes);
        inoutBuffer->offsInBytes += sizeInBytes;
    }
}

D3D12AID_API void d3d12aid_MappedBuffer_Transfer(ID3D12GraphicsCommandList *cmdList, d3d12aid_MappedBuffer *inoutBuffer, uint32_t frameIndex)
{
    if (inoutBuffer->heapType == D3D12_HEAP_TYPE_UPLOAD)
    {
        if (inoutBuffer->offsInBytes == inoutBuffer->sizeInBytes)
        {
            cmdList->CopyResource(inoutBuffer->bufGpu, inoutBuffer->bufCpu[frameIndex]);
        }
        else
        {
            cmdList->CopyBufferRegion(inoutBuffer->bufGpu, 0, inoutBuffer->bufCpu[frameIndex], 0, inoutBuffer->offsInBytes);
            inoutBuffer->offsInBytes = 0;
        }
    }
    else if (inoutBuffer->heapType == D3D12_HEAP_TYPE_READBACK)
    {
        cmdList->CopyResource(inoutBuffer->bufCpu[frameIndex], inoutBuffer->bufGpu);
    }
}

D3D12AID_API void d3d12aid_MappedBuffer_BeginTransfer(D3D12_RESOURCE_BARRIER *outBarrier, d3d12aid_MappedBuffer *inoutBuffer, D3D12_RESOURCE_STATES prevState)
{
    D3D12AID_ASSERT(inoutBuffer->heapType == D3D12_HEAP_TYPE_READBACK);
    d3d12aid_Resource_TransitionBarrier(outBarrier, inoutBuffer->bufGpu, prevState, D3D12_RESOURCE_STATE_COPY_SOURCE);
}

D3D12AID_API void d3d12aid_MappedBuffer_EndTransfer(D3D12_RESOURCE_BARRIER *outBarrier, d3d12aid_MappedBuffer *inoutBuffer, D3D12_RESOURCE_STATES nextState)
{
    D3D12AID_ASSERT(inoutBuffer->heapType == D3D12_HEAP_TYPE_UPLOAD);
    d3d12aid_Resource_TransitionBarrier(outBarrier, inoutBuffer->bufGpu, D3D12_RESOURCE_STATE_COPY_DEST, nextState);
}

#ifndef D3D12AID_CMD_QUEUE_LATENCY_FRAME_MAX_COUNT
#define D3D12AID_CMD_QUEUE_LATENCY_FRAME_MAX_COUNT 3
#endif

#ifndef D3D12AID_CMD_QUEUE_LIST_MAX_COUNT_PER_FRAME
#define D3D12AID_CMD_QUEUE_LIST_MAX_COUNT_PER_FRAME 1
#endif

#ifdef D3D12AID_CMD_QUEUE_ALLOC_MAX_COUNT
#error `D3D12AID_CMD_QUEUE_ALLOC_MAX_COUNT` must not be defined
#endif

#define D3D12AID_CMD_QUEUE_ALLOC_MAX_COUNT (D3D12AID_CMD_QUEUE_LATENCY_FRAME_MAX_COUNT * D3D12AID_CMD_QUEUE_LIST_MAX_COUNT_PER_FRAME)

struct d3d12aid_CmdQueue
{
    uint32_t                    cmdAllocCount;
    uint32_t                    cmdListCount;

    uint64_t                    fenceValue;
    HANDLE                      fenceEvent;
    ID3D12Fence                *fence;
    ID3D12CommandQueue         *queue;

    ID3D12CommandAllocator     *cmdAllocs[D3D12AID_CMD_QUEUE_ALLOC_MAX_COUNT];
    uint64_t                    cmdAllocReuseFenceValues[D3D12AID_CMD_QUEUE_ALLOC_MAX_COUNT];

    ID3D12GraphicsCommandList  *cmdLists[D3D12AID_CMD_QUEUE_LIST_MAX_COUNT_PER_FRAME];
    uint32_t                    cmdAllocIndices[D3D12AID_CMD_QUEUE_LIST_MAX_COUNT_PER_FRAME];

#if (D3D12AID_CMD_QUEUE_LIST_MAX_COUNT_PER_FRAME % 2) != 0
    uint32_t                    padding;
#endif
};

typedef struct d3d12aid_CmdQueue d3d12aid_CmdQueue;

D3D12AID_API void d3d12aid_CmdQueue_Create(d3d12aid_CmdQueue *outQueue, ID3D12Device *device, uint32_t frameCount, uint32_t listCountPerFrame, D3D12_COMMAND_LIST_TYPE cmdQueueType)
{
    D3D12_COMMAND_QUEUE_DESC queueDesc;

    queueDesc.Type      = cmdQueueType;
    queueDesc.Priority  = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    queueDesc.Flags     = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.NodeMask  = 0x1;

    D3D12AID_CHECK(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&outQueue->queue)));

    outQueue->cmdAllocCount = frameCount * listCountPerFrame;

    outQueue->cmdListCount = listCountPerFrame;

    /** create a fence with `outQueue->fenceValue` value set (originally set to zero, but could be set to anything) */
    outQueue->fenceValue = 0;

    D3D12AID_CHECK(device->CreateFence(outQueue->fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&outQueue->fence)));

    /** query back the completed value and advanced at the same time*/
    outQueue->fenceValue = outQueue->fence->GetCompletedValue();

    /** initially all submit fences are set to the last completed value of the fence */
    for (uint32_t i = 0; i < outQueue->cmdAllocCount; ++i)
        outQueue->cmdAllocReuseFenceValues[i] = outQueue->fenceValue;

    outQueue->fenceValue += 1ull;

    /** intially allocator index matches the index of the command list */
    for (uint32_t i = 0; i < outQueue->cmdListCount; ++i)
        outQueue->cmdAllocIndices[i] = i;

    /** initialise the allocators with the `Type` of the queue they are bound to */
    for (uint32_t i = 0; i < outQueue->cmdAllocCount; ++i)
        D3D12AID_CHECK(device->CreateCommandAllocator(queueDesc.Type, IID_PPV_ARGS(&outQueue->cmdAllocs[i])));

    for (uint32_t i = outQueue->cmdAllocCount; i < D3D12AID_CMD_QUEUE_ALLOC_MAX_COUNT; ++i)
        outQueue->cmdAllocs[i] = NULL;

    /** initialise the lists with the `Type` and `NodeMask` of the queue they are bound to */
    for (uint32_t i = 0; i < outQueue->cmdListCount; ++i)
    {
        D3D12AID_CHECK(device->CreateCommandList(queueDesc.NodeMask, queueDesc.Type, outQueue->cmdAllocs[i], NULL, IID_PPV_ARGS(&outQueue->cmdLists[i])));

        outQueue->cmdLists[i]->Close();
    }

    for (uint32_t i = outQueue->cmdListCount; i < D3D12AID_CMD_QUEUE_LIST_MAX_COUNT_PER_FRAME; ++i)
        outQueue->cmdLists[i] = NULL;

    outQueue->fenceEvent = CreateEventW(NULL, 0, 0, L"d3d12aid_CmdQueue_FenceEvent");
}

D3D12AID_API void d3d12aid_CmdQueue_CpuWaitForFence(d3d12aid_CmdQueue *queue, uint64_t fenceId)
{
    uint64_t fenceIdCompleted = queue->fence->GetCompletedValue();
    if (fenceId > fenceIdCompleted)
    {
        D3D12AID_CHECK(queue->fence->SetEventOnCompletion(fenceId, queue->fenceEvent));
        WaitForSingleObjectEx(queue->fenceEvent, INFINITE, FALSE);
    }
}

D3D12AID_API void d3d12aid_CmdQueue_CpuWaitForGpuIdle(d3d12aid_CmdQueue *queue)
{
    d3d12aid_CmdQueue_CpuWaitForFence(queue, queue->fenceValue - 1ull);
}

D3D12AID_API void d3d12aid_CmdQueue_Release(d3d12aid_CmdQueue *queue)
{
    d3d12aid_CmdQueue_CpuWaitForGpuIdle(queue);

    for (uint32_t i = 0; i < queue->cmdListCount; ++i)
        D3D12AID_SAFE_RELEASE(queue->cmdLists[i]);

    for (uint32_t i = 0; i < queue->cmdAllocCount; ++i)
        D3D12AID_SAFE_RELEASE(queue->cmdAllocs[i]);

    CloseHandle(queue->fenceEvent);

    D3D12AID_SAFE_RELEASE(queue->fence);
    D3D12AID_SAFE_RELEASE(queue->queue);
}

D3D12AID_API uint64_t d3d12aid_CmdQueue_SubmitMultiCmdLists(d3d12aid_CmdQueue *queue, uint32_t cmdListIdStart, uint32_t cmdListIdCount)
{
    D3D12AID_ASSERT(cmdListIdStart < queue->cmdListCount);
    D3D12AID_ASSERT(cmdListIdStart + cmdListIdCount <= queue->cmdListCount);

    uint64_t *allocReuseFenceValues = queue->cmdAllocReuseFenceValues;
    uint32_t *allocIndices = queue->cmdAllocIndices;

    uint64_t submitFenceValue = queue->fenceValue ++;

    /** close the command lists we are going to submit */
    for (uint32_t i = cmdListIdStart; i < cmdListIdStart + cmdListIdCount; ++i)
        D3D12AID_CHECK(queue->cmdLists[i]->Close());

    queue->queue->ExecuteCommandLists(cmdListIdCount, (ID3D12CommandList * const *)&queue->cmdLists[cmdListIdStart]);
    D3D12AID_CHECK(queue->queue->Signal(queue->fence, submitFenceValue));

    /** for each allocator associated with the submitted command list .. */
    for (uint32_t i = cmdListIdStart; i < cmdListIdStart + cmdListIdCount; ++i)
    {
        const uint32_t allocatorIdx = allocIndices[i];

        /** 1. update its fence value to track when it was submitted */
        allocReuseFenceValues[allocatorIdx] = submitFenceValue;

        /** 2. choose next available allocator index */
        allocIndices[i] = (allocatorIdx + D3D12AID_CMD_QUEUE_LIST_MAX_COUNT_PER_FRAME) % D3D12AID_CMD_QUEUE_ALLOC_MAX_COUNT;
    }
    return submitFenceValue;
}

D3D12AID_API ID3D12GraphicsCommandList **d3d12aid_CmdQueue_StartMultiCmdLists(d3d12aid_CmdQueue *queue, uint32_t cmdListIdStart, uint32_t cmdListIdCount)
{
    D3D12AID_ASSERT(cmdListIdStart < queue->cmdListCount);
    D3D12AID_ASSERT(cmdListIdStart + cmdListIdCount <= queue->cmdListCount);

    const uint64_t *allocReuseFenceValues = queue->cmdAllocReuseFenceValues;
    const uint32_t *allocIndices = queue->cmdAllocIndices;

    uint64_t lastAllocReuseFenceValueToWait = 0;

    /** iterate over all command buffers to start, fetch their allocator indices [0..maxQueueFrameCount-1] and figure the last submitted fence */
    for (uint32_t i = cmdListIdStart; i < cmdListIdStart + cmdListIdCount; ++i)
    {
        const uint32_t allocatorIdx = allocIndices[i];

        if (lastAllocReuseFenceValueToWait < allocReuseFenceValues[allocatorIdx])
            lastAllocReuseFenceValueToWait = allocReuseFenceValues[allocatorIdx];
    }

    d3d12aid_CmdQueue_CpuWaitForFence(queue, lastAllocReuseFenceValueToWait);

    /** iterate over all command buffers to start, fetch their allocator indices [0..maxQueueFrameCount-1], reset corresponding allocators and reuse them for command lists */
    for (uint32_t i = cmdListIdStart; i < cmdListIdStart + cmdListIdCount; ++i)
    {
        const uint32_t allocatorIdx = allocIndices[i];

        D3D12AID_CHECK(queue->cmdAllocs[allocatorIdx]->Reset());
        D3D12AID_CHECK(queue->cmdLists[i]->Reset(queue->cmdAllocs[allocatorIdx], NULL));
    }
    return &queue->cmdLists[cmdListIdStart];
}

D3D12AID_API uint64_t d3d12aid_CmdQueue_SubmitCmdList(d3d12aid_CmdQueue *queue, uint32_t cmdListId)
{
    return d3d12aid_CmdQueue_SubmitMultiCmdLists(queue, cmdListId, 1u);
}

D3D12AID_API ID3D12GraphicsCommandList *d3d12aid_CmdQueue_StartCmdList(d3d12aid_CmdQueue *queue, uint32_t cmdListId)
{
    return d3d12aid_CmdQueue_StartMultiCmdLists(queue, cmdListId, 1)[0];
}

D3D12AID_API bool d3d12aid_CmdQueue_IsFenceCompleted(d3d12aid_CmdQueue *queue, uint64_t fenceId)
{
    return fenceId <= queue->fence->GetCompletedValue();

}

#endif /** D3D12AID_H */

/**
 * Copyright (c) 2025 Pavel Martishevsky
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
