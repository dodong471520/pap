// Copyright (c) 1997-2001  ETH Zurich (Switzerland).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/QP_solver/include/CGAL/QP_solver/Bounds.C $
// $Id: Bounds.C 30642 2006-04-18 12:42:52Z lsaboret $
// 
//
// Author(s)     : Sven Schoenherr <sven@inf.fu-berlin.de>
//                 Bernd Gaertner <gaertner@inf.ethz.ch>
//                 Franz Wessendorp <fransw@inf.ethz.ch>
//                 Kaspar Fischer <fischerk@inf.ethz.ch>

CGAL_BEGIN_NAMESPACE

template < class Rep_ >
bool QP_solver<Rep_>::has_finite_lower_bound(int i) const
  // Given an index of an original or slack variable, returns whether
  // or not the variable has a finite lower bound.
{
  CGAL_qpe_assertion(i < qp_n + static_cast<int>(slack_A.size()));
  return i>=qp_n || check_tag(Is_in_standard_form()) || *(qp_fl+i);
}

template < class Rep_ >
bool QP_solver<Rep_>::has_finite_upper_bound(int i) const
  // Given an index of an original or slack variable, returns whether
  // or not the variable has a finite upper bound.
{
  CGAL_qpe_assertion(i < qp_n + static_cast<int>(slack_A.size()));
  return i<qp_n && !check_tag(Is_in_standard_form()) && *(qp_fu+i);
}

template < class Rep_ >
typename QP_solver<Rep_>::ET QP_solver<Rep_>::lower_bound(int i) const
  // Given an index of an original or slack variable, returns its
  // lower bound.
{
  CGAL_qpe_assertion(i < qp_n + static_cast<int>(slack_A.size()));
  if (i < qp_n)                     // original variable?
    if (check_tag(Is_in_standard_form()))
      return et0;
    else {
      CGAL_qpe_assertion(has_finite_lower_bound(i));
      return *(qp_l+i);
    }
  else                              // slack variable?
    return et0;
}

template < class Rep_ >
typename QP_solver<Rep_>::ET QP_solver<Rep_>::upper_bound(int i) const
  // Given an index of an original variable, returns its upper bound.
{
  CGAL_qpe_assertion(i < qp_n); // Note: slack variables cannot have
				// finite upper bounds.
  CGAL_qpe_assertion(has_finite_upper_bound(i));
  return *(qp_u+i);
}

template < class Rep_ >
typename QP_solver<Rep_>::Bnd
QP_solver<Rep_>::lower_bnd(int i) const
  // Given an index of an original, slack, or artificial variable,
  // return its lower bound.
{
  if (i < qp_n) {                                      // original variable?
    const bool is_finite = has_finite_lower_bound(i);
    return Bnd(false, is_finite, is_finite? lower_bound(i) : ET(0));
  } else                                              // slacky or art. var.?
    return Bnd(false, true, ET(0));
}

template < class Rep_ >
typename QP_solver<Rep_>::Bnd
QP_solver<Rep_>::upper_bnd(int i) const
  // Given an index of an original, slack, or artificial variable,
  // return its upper bound.
{
  if (i < qp_n) {                                      // original variable?
    const bool is_finite = has_finite_upper_bound(i);
    return Bnd(true, is_finite, is_finite? upper_bound(i) : ET(0));
  } else                                              // slacky or art. var.?
    return Bnd(true, false, ET(0));
}

CGAL_END_NAMESPACE

// ===== EOF ==================================================================
