/*
 * MIT License
 *
 * Copyright (c) 2019-2021 Albin Johansson
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
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef CENTURION_DETAIL_CZSTRING_EQ_HEADER
#define CENTURION_DETAIL_CZSTRING_EQ_HEADER

#include <cstring>  // strcmp

#include "../centurion_cfg.hpp"
#include "../czstring.hpp"

#ifdef CENTURION_USE_PRAGMA_ONCE
#pragma once
#endif  // CENTURION_USE_PRAGMA_ONCE

/// \cond FALSE
namespace cen::detail {

/**
 * \brief Indicates whether or not two C-style strings are equal.
 *
 * \param lhs the left-hand side string, can safely be null.
 * \param rhs the right-hand side string, can safely be null.
 *
 * \return `true` if the strings are equal; `false` otherwise.
 *
 * \since 4.1.0
 */
[[nodiscard]] inline auto czstring_eq(const czstring lhs,
                                      const czstring rhs) noexcept -> bool
{
  if (lhs && rhs) {
    return std::strcmp(lhs, rhs) == 0;
  } else {
    return false;
  }
}

}  // namespace cen::detail
/// \endcond

#endif  // CENTURION_DETAIL_CZSTRING_EQ_HEADER