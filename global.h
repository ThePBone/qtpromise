// Created via CMake from template global.h.in
// WARNING! Any changes to this file will be overwritten by the next CMake run!

#ifndef QTPROMISE_GLOBAL
#define QTPROMISE_GLOBAL

#include <c++utilities/application/global.h>

#ifdef QTPROMISE_STATIC
#define QTPROMISE_EXPORT
#define QTPROMISE_IMPORT
#else
#define QTPROMISE_EXPORT CPP_UTILITIES_GENERIC_LIB_EXPORT
#define QTPROMISE_IMPORT CPP_UTILITIES_GENERIC_LIB_IMPORT
#endif

/*!
 * \def QTPROMISE_EXPORT
 * \brief Marks the symbol to be exported by the qtpromise library.
 */

/*!
 * \def QTPROMISE_IMPORT
 * \brief Marks the symbol to be imported from the qtpromise library.
 */

#endif // QTPROMISE_GLOBAL
