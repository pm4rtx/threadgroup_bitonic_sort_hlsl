/**
 * threadgroup_bitonic_sort HLSL - version 0.01
 *
 * Copyright (C) 2025, by Pavel Martishevsky
 * Report bugs and download new versions at https://github.com/pm4rtx/threadgroup_bitonic_sort_hlsl
 *
 * This header is distributed under the MIT License. See notice at the end of this file.
 */

#ifndef THREADGROUP_BITONIC_SORT_HLSLI
#define THREADGROUP_BITONIC_SORT_HLSLI

namespace tgbs {
    /**
     *  The callbacks the calling code needs to define
     */
    template<typename T> static void lds_store(uint index, T x);
    template<typename T> static T lds_load(uint index);

    namespace internal
    {
        #ifndef NO_WAVE_INTRINSICS

        template<uint and_mask, uint or_mask, uint xor_mask, typename T> static T wave_swizzle(T v)
        {
            return WaveReadLaneAt(v, ((WaveGetLaneIndex() & and_mask) | or_mask) ^ xor_mask);
        }

        template<uint groupSize, typename T> static T wave_swizzle_swap(T x)
        {
            return wave_swizzle<0x3fu, 0x00, groupSize>(x);
        }
        #endif

        template<uint and_mask, uint or_mask, uint xor_mask, typename T> static T lds_swizzle(T x, uint i)
        {
            GroupMemoryBarrierWithGroupSync();
            lds_store(i & and_mask, x);
            GroupMemoryBarrierWithGroupSync();
            return lds_load<T>(((i & and_mask) | or_mask) ^ xor_mask);
        }

        template<uint groupSize, typename T> static T lds_swizzled_swap(T x, uint i)
        {
            return lds_swizzle<0xffffffffu, 0x00, groupSize>(x, i);
        }
        template<uint id> bool bitonic_mask(uint i)
        {
            const uint mask = (1u << (id + 1u)) - 1u;
            const uint bound = (1u << id) - 1u;
            return (i & mask) > bound;
        }

        template<bool decreasingBit, uint passId, uint subpassId, typename T> static T bitonic_shuffle_and_select(T x, uint i)
        {
            const bool passBit = bitonic_mask<passId>(i);
            const bool subpassBit = bitonic_mask<subpassId - 1u>(i);

            #ifdef NO_WAVE_INTRINSICS
            const T t = lds_swizzled_swap<1u << (subpassId - 1u)>(x, i);
            #else
            T t = x;
            [branch]
            if (WaveGetLaneCount() >= (1u << subpassId)) // On pre-RDNA 4 AMD cards, these produces sub-optimal codegen when swapping 32-elem blocks because ds_swizzle_b32 is 32-wide only, so v_readlane_b32 loop is used instead...
            {
                t = wave_swizzle_swap<1u << (subpassId - 1u)>(x);
            }
            else
            {
                t = lds_swizzled_swap<1u << (subpassId - 1u)>(x, i);
            }
            #endif

            return (x < t) ^ decreasingBit ^ passBit ^ subpassBit ? x : t;
        }

        template<bool decreasingBit, uint passId, uint subpassId, typename T> struct bitonic_shuffle_and_select_unroll
        {
            static T execute(T x, uint i)
            {
                x = bitonic_shuffle_and_select<decreasingBit, passId, subpassId, T>(x, i);
                x = bitonic_shuffle_and_select_unroll<decreasingBit, passId, subpassId - 1u, T>::execute(x, i);
                return x;
            }
        };


        template<bool decreasingBit, uint passId, typename T> struct bitonic_shuffle_and_select_unroll<decreasingBit, passId, 0, T>
        {
            static T execute(T x, uint i)
            {
                return x;
            }
        };

        template<bool decreasingBit, uint startPassId, uint startSubpassId, typename T> static T bitonic_shuffle_and_select_unrolled(T x, uint i)
        {
            return bitonic_shuffle_and_select_unroll<decreasingBit, startPassId, startSubpassId, T>::execute(x, i);
        }

        template<bool decreasingBit, uint SortSequenceLenLog2, typename T> struct bitonic_sort_unroll
        {
            static T execute(T x, uint i)
            {
                x = bitonic_sort_unroll<decreasingBit, SortSequenceLenLog2 - 1u, T>::execute(x, i);
                x = bitonic_shuffle_and_select_unrolled<decreasingBit, SortSequenceLenLog2, SortSequenceLenLog2, T>(x, i);
                return x;
            }
        };

        template<bool decreasingBit, typename T> struct bitonic_sort_unroll<decreasingBit, 1u, T>
        {
            static T execute(T x, uint i)
            {
                x = bitonic_shuffle_and_select<decreasingBit, 1u, 1u>(x, i);
                return x;
            }
        };

    }

template<bool decreasingBit, uint SortSequenceLenLog2, typename T> static T bitonic_sort(T x, uint i)
{
    return internal::bitonic_sort_unroll<decreasingBit, SortSequenceLenLog2, T>::execute(x, i);
}

template<bool decreasingBit, typename T> static T bitonic_sort_2(T x, uint i)     { return bitonic_sort<decreasingBit, 1u, T>(x, i); }
template<bool decreasingBit, typename T> static T bitonic_sort_4(T x, uint i)     { return bitonic_sort<decreasingBit, 2u, T>(x, i); }
template<bool decreasingBit, typename T> static T bitonic_sort_8(T x, uint i)     { return bitonic_sort<decreasingBit, 3u, T>(x, i); }
template<bool decreasingBit, typename T> static T bitonic_sort_16(T x, uint i)    { return bitonic_sort<decreasingBit, 4u, T>(x, i); }
template<bool decreasingBit, typename T> static T bitonic_sort_32(T x, uint i)    { return bitonic_sort<decreasingBit, 5u, T>(x, i); }
template<bool decreasingBit, typename T> static T bitonic_sort_64(T x, uint i)    { return bitonic_sort<decreasingBit, 6u, T>(x, i); }
template<bool decreasingBit, typename T> static T bitonic_sort_128(T x, uint i)   { return bitonic_sort<decreasingBit, 7u, T>(x, i); }
template<bool decreasingBit, typename T> static T bitonic_sort_256(T x, uint i)   { return bitonic_sort<decreasingBit, 8u, T>(x, i); }
template<bool decreasingBit, typename T> static T bitonic_sort_512(T x, uint i)   { return bitonic_sort<decreasingBit, 9u, T>(x, i); }
template<bool decreasingBit, typename T> static T bitonic_sort_1024(T x, uint i)  { return bitonic_sort<decreasingBit, 10u, T>(x, i); }

    namespace internal {

        template<bool decreasingBit, uint passId, uint subpassId, typename T> static void bitonic_shuffle_and_select_2x(inout T x, inout T y, uint i, uint offset0, uint offset1)
        {
            const bool passBit0 = internal::bitonic_mask<passId>(i + offset0);
            const bool subpassBit0 = internal::bitonic_mask<subpassId - 1u>(i + offset0);

            const bool passBit1 = internal::bitonic_mask<passId>(i + offset1);
            const bool subpassBit1 = internal::bitonic_mask<subpassId - 1u>(i + offset1);

            const T newx = (x < y) ^ decreasingBit ^ passBit0 ^ subpassBit0 ? x : y;
            const T newy = (y < x) ^ decreasingBit ^ passBit1 ^ subpassBit1 ? y : x;

            x = newx;
            y = newy;
        }

    }

/**
 *  These variant of bitonic sort, as name implies, is unrolled 2 times per each thread
 *
 *  The main advantage of this variant over `bitonic_sort_*` is that it requires
 *  N/2 threads and N/2 elements of LDS storage to sort `N` elements comparing `bitonic_sort_*`
 *
 *  It's expected that input `x`, `y`  elements of each thread are N/2 elements apart
 *      - elements 0...x...  N/2-1
 *      - elements 0...y...  N-1
 */
template<bool decreasingBit, uint SortSequenceLenLog2, typename T> static void bitonic_sort_unrolled_2x(inout T x, inout T y, uint i)
{
    const uint stride = (1u << (SortSequenceLenLog2 - 1u));

    x = bitonic_sort<decreasingBit, SortSequenceLenLog2 - 1u>(x, i);
    y = bitonic_sort<decreasingBit, SortSequenceLenLog2 - 1u>(y, i);

    internal::bitonic_shuffle_and_select_2x<decreasingBit, SortSequenceLenLog2, SortSequenceLenLog2>(x, y, i, 0, stride);

    x = internal::bitonic_shuffle_and_select_unrolled<decreasingBit, SortSequenceLenLog2, SortSequenceLenLog2 - 1u>(x, i);
    y = internal::bitonic_shuffle_and_select_unrolled<decreasingBit, SortSequenceLenLog2, SortSequenceLenLog2 - 1u>(y, i);
}

/**
 *  These are specialized versions `bitonic_sort_unrolled_2x`
 */
template<bool decreasingBit, typename T> static void bitonic_sort_unrolled_2x_4(inout T x, inout T y, uint i)     { return bitonic_sort_unrolled_2x<decreasingBit, 2u, T>(x, y, i); }
template<bool decreasingBit, typename T> static void bitonic_sort_unrolled_2x_8(inout T x, inout T y, uint i)     { return bitonic_sort_unrolled_2x<decreasingBit, 3u, T>(x, y, i); }
template<bool decreasingBit, typename T> static void bitonic_sort_unrolled_2x_16(inout T x, inout T y, uint i)    { return bitonic_sort_unrolled_2x<decreasingBit, 4u, T>(x, y, i); }
template<bool decreasingBit, typename T> static void bitonic_sort_unrolled_2x_32(inout T x, inout T y, uint i)    { return bitonic_sort_unrolled_2x<decreasingBit, 5u, T>(x, y, i); }
template<bool decreasingBit, typename T> static void bitonic_sort_unrolled_2x_64(inout T x, inout T y, uint i)    { return bitonic_sort_unrolled_2x<decreasingBit, 6u, T>(x, y, i); }
template<bool decreasingBit, typename T> static void bitonic_sort_unrolled_2x_128(inout T x, inout T y, uint i)   { return bitonic_sort_unrolled_2x<decreasingBit, 7u, T>(x, y, i); }
template<bool decreasingBit, typename T> static void bitonic_sort_unrolled_2x_256(inout T x, inout T y, uint i)   { return bitonic_sort_unrolled_2x<decreasingBit, 8u, T>(x, y, i); }
template<bool decreasingBit, typename T> static void bitonic_sort_unrolled_2x_512(inout T x, inout T y, uint i)   { return bitonic_sort_unrolled_2x<decreasingBit, 9u, T>(x, y, i); }
template<bool decreasingBit, typename T> static void bitonic_sort_unrolled_2x_1024(inout T x, inout T y, uint i)  { return bitonic_sort_unrolled_2x<decreasingBit, 10u, T>(x, y, i); }
template<bool decreasingBit, typename T> static void bitonic_sort_unrolled_2x_2048(inout T x, inout T y, uint i)  { return bitonic_sort_unrolled_2x<decreasingBit, 11u, T>(x, y, i); }

/**
 *  These variant of bitonic sort, as name implies, is unrolled 4 times per each thread
 *
 *  The main advantage of this variant over `bitonic_sort_*` is that it requires
 *  N/4 threads and N/4 elements of LDS storage to sort `N` elements comparing `bitonic_sort_*`
 *
 *  It's expected that input `x`, `y`, `z`, `y` elements of each thread are N/4 elements apart
 *      - elements 0...x...  N/4-1
 *      - elements 0...y...2*N/4-1
 *      - elements 0...z...3*N/4-1
 *      - elements 0...w...4*N/4-1
 */
template<bool decreasingBit, uint SortSequenceLenLog2, typename T> static void bitonic_sort_unrolled_4x(inout T x, inout T y, inout T z, inout T w, uint i)
{
    const uint stride = (1u << (SortSequenceLenLog2 - 2u));

    x = bitonic_sort<decreasingBit, SortSequenceLenLog2 - 2u>(x, i);
    y = bitonic_sort<decreasingBit, SortSequenceLenLog2 - 2u>(y, i);
    z = bitonic_sort<decreasingBit, SortSequenceLenLog2 - 2u>(z, i);
    w = bitonic_sort<decreasingBit, SortSequenceLenLog2 - 2u>(w, i);

    internal::bitonic_shuffle_and_select_2x<decreasingBit, SortSequenceLenLog2 - 1u, SortSequenceLenLog2 - 1u>(x, y, i, stride * 0, stride * 1);
    internal::bitonic_shuffle_and_select_2x<decreasingBit, SortSequenceLenLog2 - 1u, SortSequenceLenLog2 - 1u>(z, w, i, stride * 2, stride * 3);

    x = internal::bitonic_shuffle_and_select_unrolled<decreasingBit, SortSequenceLenLog2 - 1u, SortSequenceLenLog2 - 2u>(x, i);
    y = internal::bitonic_shuffle_and_select_unrolled<decreasingBit, SortSequenceLenLog2 - 1u, SortSequenceLenLog2 - 2u>(y, i);
    z = internal::bitonic_shuffle_and_select_unrolled<decreasingBit, SortSequenceLenLog2 - 1u, SortSequenceLenLog2 - 2u>(z, i);
    w = internal::bitonic_shuffle_and_select_unrolled<decreasingBit, SortSequenceLenLog2 - 1u, SortSequenceLenLog2 - 2u>(w, i);

    const bool passBit0 = internal::bitonic_mask<SortSequenceLenLog2>(i + stride * 0);
    const bool passBit1 = internal::bitonic_mask<SortSequenceLenLog2>(i + stride * 1);
    const bool passBit2 = internal::bitonic_mask<SortSequenceLenLog2>(i + stride * 2);
    const bool passBit3 = internal::bitonic_mask<SortSequenceLenLog2>(i + stride * 3);

    const bool subpassBit0 = internal::bitonic_mask<SortSequenceLenLog2 - 1u>(i + stride * 0);
    const bool subpassBit1 = internal::bitonic_mask<SortSequenceLenLog2 - 1u>(i + stride * 1);
    const bool subpassBit2 = internal::bitonic_mask<SortSequenceLenLog2 - 1u>(i + stride * 2);
    const bool subpassBit3 = internal::bitonic_mask<SortSequenceLenLog2 - 1u>(i + stride * 3);

    const T newx = (x < z) ^ decreasingBit ^ passBit0 ^ subpassBit0 ? x : z;
    const T newy = (y < w) ^ decreasingBit ^ passBit1 ^ subpassBit1 ? y : w;
    const T newz = (z < x) ^ decreasingBit ^ passBit2 ^ subpassBit2 ? z : x;
    const T neww = (w < y) ^ decreasingBit ^ passBit3 ^ subpassBit3 ? w : y;

    x = newx;
    y = newy;
    z = newz;
    w = neww;

    internal::bitonic_shuffle_and_select_2x<decreasingBit, SortSequenceLenLog2, SortSequenceLenLog2 - 1u>(x, y, i, stride * 0, stride * 1);
    internal::bitonic_shuffle_and_select_2x<decreasingBit, SortSequenceLenLog2, SortSequenceLenLog2 - 1u>(z, w, i, stride * 2, stride * 3);

    x = internal::bitonic_shuffle_and_select_unrolled<decreasingBit, SortSequenceLenLog2, SortSequenceLenLog2 - 2u>(x, i);
    y = internal::bitonic_shuffle_and_select_unrolled<decreasingBit, SortSequenceLenLog2, SortSequenceLenLog2 - 2u>(y, i);
    z = internal::bitonic_shuffle_and_select_unrolled<decreasingBit, SortSequenceLenLog2, SortSequenceLenLog2 - 2u>(z, i);
    w = internal::bitonic_shuffle_and_select_unrolled<decreasingBit, SortSequenceLenLog2, SortSequenceLenLog2 - 2u>(w, i);
}

/**
 *  These are specialized versions `bitonic_sort_unrolled_4x`
 */
template<bool decreasingBit, typename T> static void bitonic_sort_unrolled_4x_8(inout T x, inout T y, inout T z, inout T w, uint i)     { return bitonic_sort_unrolled_4x<decreasingBit, 3u, T>(x, y, z, w, i); }
template<bool decreasingBit, typename T> static void bitonic_sort_unrolled_4x_16(inout T x, inout T y, inout T z, inout T w, uint i)    { return bitonic_sort_unrolled_4x<decreasingBit, 4u, T>(x, y, z, w, i); }
template<bool decreasingBit, typename T> static void bitonic_sort_unrolled_4x_32(inout T x, inout T y, inout T z, inout T w, uint i)    { return bitonic_sort_unrolled_4x<decreasingBit, 5u, T>(x, y, z, w, i); }
template<bool decreasingBit, typename T> static void bitonic_sort_unrolled_4x_64(inout T x, inout T y, inout T z, inout T w, uint i)    { return bitonic_sort_unrolled_4x<decreasingBit, 6u, T>(x, y, z, w, i); }
template<bool decreasingBit, typename T> static void bitonic_sort_unrolled_4x_128(inout T x, inout T y, inout T z, inout T w, uint i)   { return bitonic_sort_unrolled_4x<decreasingBit, 7u, T>(x, y, z, w, i); }
template<bool decreasingBit, typename T> static void bitonic_sort_unrolled_4x_256(inout T x, inout T y, inout T z, inout T w, uint i)   { return bitonic_sort_unrolled_4x<decreasingBit, 8u, T>(x, y, z, w, i); }
template<bool decreasingBit, typename T> static void bitonic_sort_unrolled_4x_512(inout T x, inout T y, inout T z, inout T w, uint i)   { return bitonic_sort_unrolled_4x<decreasingBit, 9u, T>(x, y, z, w, i); }
template<bool decreasingBit, typename T> static void bitonic_sort_unrolled_4x_1024(inout T x, inout T y, inout T z, inout T w, uint i)  { return bitonic_sort_unrolled_4x<decreasingBit, 10u, T>(x, y, z, w, i); }
template<bool decreasingBit, typename T> static void bitonic_sort_unrolled_4x_2048(inout T x, inout T y, inout T z, inout T w, uint i)  { return bitonic_sort_unrolled_4x<decreasingBit, 11u, T>(x, y, z, w, i); }
template<bool decreasingBit, typename T> static void bitonic_sort_unrolled_4x_4096(inout T x, inout T y, inout T z, inout T w, uint i)  { return bitonic_sort_unrolled_4x<decreasingBit, 12u, T>(x, y, z, w, i); }
}

#endif /** THREADGROUP_BITONIC_SORT_HLSLI */

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
