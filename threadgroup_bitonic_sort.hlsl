
#include "threadgroup_bitonic_sort.hlsli"

#ifndef TGROUP_SIZE_LOG2
#   error `TGROUP_SIZE_LOG2` defines the number of threads in a threadgroup, so it must be defined.
#endif

#ifndef KERNEL_SIZE_LOG2
#   error `KERNEL_SIZE_LOG2` defines the number of work items processed by a threadgroup, so it must be defined.
#endif

#if (KERNEL_SIZE_LOG2 < TGROUP_SIZE_LOG2 || KERNEL_SIZE_LOG2 > TGROUP_SIZE_LOG2 + 2)
#   error `KERNEL_SIZE_LOG2` must be defined as `KERNEL_SIZE_LOG2=TGROUP_SIZE_LOG2`, `KERNEL_SIZE_LOG2=TGROUP_SIZE_LOG2 + 1` or `KERNEL_SIZE_LOG2=TGROUP_SIZE_LOG2 + 2`
#endif

groupshared uint LdsSwizzleStore[1u << TGROUP_SIZE_LOG2];

namespace tgbs {
/**
 *  The callbacks we need to define
 */
template<typename T> static void lds_store(uint index, T x)
{
    LdsSwizzleStore[index] = x;
}

template<typename T> static T lds_load(uint index)
{
    return LdsSwizzleStore[index];
}

}

StructuredBuffer<uint> PerTgSortKeysIn : register(t0);
RWStructuredBuffer<uint> PerTgSortKeysOut : register(u0);

struct Consts
{
    uint PerDispatchTgCountX;
};

ConstantBuffer<Consts> Constants : register(b0);

[RootSignature("SRV(t0), UAV(u0), RootConstants(b0, num32BitConstants=1)")]
[numthreads(1u << TGROUP_SIZE_LOG2, 1, 1)]
void main(uint2 groupCoord : SV_GroupId, uint threadId : SV_GroupThreadId)
{
    uint groupId = Constants.PerDispatchTgCountX * groupCoord.y + groupCoord.x;
#define OFFSET (1u << TGROUP_SIZE_LOG2)

#if (KERNEL_SIZE_LOG2 == TGROUP_SIZE_LOG2)
    uint key0 = PerTgSortKeysIn[(groupId << KERNEL_SIZE_LOG2) + threadId];

    key0 = tgbs::bitonic_sort<true, KERNEL_SIZE_LOG2>(key0, threadId);

    PerTgSortKeysOut[(groupId << KERNEL_SIZE_LOG2) + threadId] = key0;
#elif (KERNEL_SIZE_LOG2 == TGROUP_SIZE_LOG2 + 1)
    uint key0 = PerTgSortKeysIn[(groupId << KERNEL_SIZE_LOG2) + threadId + OFFSET * 0];
    uint key1 = PerTgSortKeysIn[(groupId << KERNEL_SIZE_LOG2) + threadId + OFFSET * 1];

    tgbs::bitonic_sort_unrolled_2x<true, KERNEL_SIZE_LOG2>(key0, key1, threadId);

    PerTgSortKeysOut[(groupId << KERNEL_SIZE_LOG2) + threadId + OFFSET * 0] = key0;
    PerTgSortKeysOut[(groupId << KERNEL_SIZE_LOG2) + threadId + OFFSET * 1] = key1;
#elif (KERNEL_SIZE_LOG2 == TGROUP_SIZE_LOG2 + 2)
    uint key0 = PerTgSortKeysIn[(groupId << KERNEL_SIZE_LOG2) + threadId + OFFSET * 0];
    uint key1 = PerTgSortKeysIn[(groupId << KERNEL_SIZE_LOG2) + threadId + OFFSET * 1];
    uint key2 = PerTgSortKeysIn[(groupId << KERNEL_SIZE_LOG2) + threadId + OFFSET * 2];
    uint key3 = PerTgSortKeysIn[(groupId << KERNEL_SIZE_LOG2) + threadId + OFFSET * 3];

    tgbs::bitonic_sort_unrolled_4x<true, KERNEL_SIZE_LOG2>(key0, key1, key2, key3, threadId);

    PerTgSortKeysOut[(groupId << KERNEL_SIZE_LOG2) + threadId + OFFSET * 0] = key0;
    PerTgSortKeysOut[(groupId << KERNEL_SIZE_LOG2) + threadId + OFFSET * 1] = key1;
    PerTgSortKeysOut[(groupId << KERNEL_SIZE_LOG2) + threadId + OFFSET * 2] = key2;
    PerTgSortKeysOut[(groupId << KERNEL_SIZE_LOG2) + threadId + OFFSET * 3] = key3;
#endif
}
