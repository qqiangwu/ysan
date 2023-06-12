#ifndef YRUI_DETAIL_CONTRACT_H_
#define YRUI_DETAIL_CONTRACT_H_

#include <boost/assert.hpp>

//! \name   contract support
//! @{

/*!
 * pre condition
 */
#ifdef NDEBUG
#define YPRE_CONDITION(expr)
#else
#define YPRE_CONDITION(expr) BOOST_ASSERT_MSG(expr, "Precondition: " #expr)
#endif // !NDEBUG

/*!
 * post condition
 */
#ifdef NDEBUG
#define YPOST_CONDITION(expr)
#else
#define YPOST_CONDITION(expr) BOOST_ASSERT_MSG(expr, "Postcondition: " #expr)
#endif // !NDEBUG

/*!
 *
 * class invariants
 *
 */
#ifdef NDEBUG
#define YINVARIANT(expr)
#else
#define YINVARIANT(expr) BOOST_ASSERT_MSG(expr, "Invariant: " #expr)
#endif // !NDEBUG

/*!
 * If the constraint is voilated, the behavior is undefined.
 */
#ifdef NDEBUG
#define YCONSTRAINT(expr)
#else
#define YCONSTRAINT(expr) BOOST_ASSERT_MSG(expr, "Constraint: " #expr)
#endif // !NDEBUG
//! @}

#endif // YRUI_DETAIL_CONTRACT_H_
