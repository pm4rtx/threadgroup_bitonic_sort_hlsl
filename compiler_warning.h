/**
 * Copyright (C) 2025-2026, by Pavel Martishevsky
 *
 * This header is distributed under the MIT License. See notice at the end of this file.
 */

#pragma once

#ifndef COMPILER_WARNING_H
#define COMPILER_WARNING_H

#ifndef COMPILER_PRAGMA_MSVC
#   if defined(_MSC_VER)
#       define COMPILER_PRAGMA_MSVC(x) __pragma(x)
#   else
#       define COMPILER_PRAGMA_MSVC(x)
#   endif
#endif /* COMPILER_PRAGMA_MSVC */

#ifndef COMPILER_PRAGMA_GNUC
#   if defined(__clang__) || defined(__GNUC__)
#       define COMPILER_PRAGMA_GNUC(x) _Pragma(#x)
#   else
#       define COMPILER_PRAGMA_GNUC(x)
#   endif
#endif /* COMPILER_PRAGMA_GNUC */

#ifndef COMPILER_WARNING_DISABLE_GNUC
#   if defined(__clang__) || defined(__GNUC__)
#       if defined(__clang__)
#           define COMPILER_WARNING_GNUC_PUSH()     COMPILER_PRAGMA_GNUC(clang diagnostic push)
#           define COMPILER_WARNING_GNUC_POP()      COMPILER_PRAGMA_GNUC(clang diagnostic pop)
#           define COMPILER_WARNING_DISABLE_GNUC(w) COMPILER_PRAGMA_GNUC(clang diagnostic ignored #w)
#       elif defined(__GNUC__)
#           define COMPILER_WARNING_GNUC_PUSH()     COMPILER_PRAGMA_GNUC(GCC diagnostic push)
#           define COMPILER_WARNING_GNUC_POP()      COMPILER_PRAGMA_GNUC(GCC diagnostic pop)
#           define COMPILER_WARNING_DISABLE_GNUC(w) COMPILER_PRAGMA_GNUC(GCC diagnostic ignored #w)
#       endif
#   elif defined (_MSC_VER)
        /* Do nothing, just passthrough for known compiler. Defines are going to be defined later */
#   else
#       error Unknown Compiler
#   endif
#endif /** COMPILER_WARNING_DISABLE_GNUC */

#ifndef COMPILER_WARNING_DISABLE_MSVC
#   if defined(_MSC_VER)
#       define COMPILER_WARNING_MSVC_PUSH()     COMPILER_PRAGMA_MSVC(warning(push))
#       define COMPILER_WARNING_MSVC_POP()      COMPILER_PRAGMA_MSVC(warning(pop))
#       define COMPILER_WARNING_DISABLE_MSVC(w) COMPILER_PRAGMA_MSVC(warning(disable : w))
#       if _MSC_VER >= 1912
#           define COMPILER_WARNING_DISABLE_MSVC1912(w) COMPILER_WARNING_DISABLE_MSVC(w)
#       endif
#       if _MSC_VER >= 1914
#           define COMPILER_WARNING_DISABLE_MSVC1914(w) COMPILER_WARNING_DISABLE_MSVC(w)
#       endif
#       if _MSC_VER >= 1920
#           define COMPILER_WARNING_DISABLE_MSVC1920(w) COMPILER_WARNING_DISABLE_MSVC(w)
#       endif
#   elif defined(__clang__) || defined(__GNUC__)
    /* Do nothing, just passthrough for known compilers. Defines are going to be defined later */
#   else
#       error Unknown Compiler
#   endif
#endif /** COMPILER_WARNING_DISABLE_MSVC */

#ifndef COMPILER_WARNING_GNUC_PUSH
#   define COMPILER_WARNING_GNUC_PUSH()
#endif

#ifndef COMPILER_WARNING_GNUC_POP
#   define COMPILER_WARNING_GNUC_POP()
#endif

#ifndef COMPILER_WARNING_DISABLE_GNUC
#   define COMPILER_WARNING_DISABLE_GNUC(w)
#endif

#ifndef COMPILER_WARNING_MSVC_PUSH
#   define COMPILER_WARNING_MSVC_PUSH
#endif

#ifndef COMPILER_WARNING_MSVC_POP
#   define COMPILER_WARNING_MSVC_POP()
#endif

#ifndef COMPILER_WARNING_DISABLE_MSVC
#   define COMPILER_WARNING_DISABLE_MSVC(w)
#endif

#ifndef COMPILER_WARNING_DISABLE_MSVC1912
#   define COMPILER_WARNING_DISABLE_MSVC1912(w)
#endif

#ifndef COMPILER_WARNING_DISABLE_MSVC1914
#   define COMPILER_WARNING_DISABLE_MSVC1914(w)
#endif

#ifndef COMPILER_WARNING_DISABLE_MSVC1920
#   define COMPILER_WARNING_DISABLE_MSVC1920(w)
#endif

#if !defined(COMPILER_WARNING_PUSH) && !defined(COMPILER_WARNING_POP)
#   define COMPILER_WARNING_PUSH() COMPILER_WARNING_MSVC_PUSH() COMPILER_WARNING_GNUC_PUSH()
#   define COMPILER_WARNING_POP()  COMPILER_WARNING_MSVC_POP()  COMPILER_WARNING_GNUC_POP()
#endif /** COMPILER_WARNING_PUSH/COMPILER_WARNING_POP */

#endif /* COMPILER_WARNING_H */

/**
 * Copyright (c) 2025-2026 Pavel Martishevsky
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
