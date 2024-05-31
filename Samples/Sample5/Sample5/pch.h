// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#define LINE_NUMBER_VAR_NAME  m_line_num
#define ACQUIRE_LN(a) (a)->LINE_NUMBER_VAR_NAME
#define STORE_LN(a) (a)->LINE_NUMBER_VAR_NAME = __LINE__;
#define DEFINE_LN()  int LINE_NUMBER_VAR_NAME = __LINE__;

#endif //PCH_H
