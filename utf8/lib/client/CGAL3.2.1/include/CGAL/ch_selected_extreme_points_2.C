// Copyright (c) 1999  Max-Planck-Institute Saarbruecken (Germany).
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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Convex_hull_2/include/CGAL/ch_selected_extreme_points_2.C $
// $Id: ch_selected_extreme_points_2.C 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Stefan Schirra


#ifndef CGAL_CH_SELECTED_EXTREME_POINTS_2_C
#define CGAL_CH_SELECTED_EXTREME_POINTS_2_C

#ifndef CGAL_CH_SELECTED_EXTREME_POINTS_2_H
#include <CGAL/ch_selected_extreme_points_2.h>
#endif // CGAL_CH_SELECTED_EXTREME_POINTS_2_H

#ifndef CGAL_CH_NO_POSTCONDITIONS
#include <CGAL/convexity_check_2.h>
#endif // CGAL_CH_NO_POSTCONDITIONS

CGAL_BEGIN_NAMESPACE
template <class ForwardIterator, class Traits>
void
ch_nswe_point( ForwardIterator first, ForwardIterator last,
                    ForwardIterator& n,
                    ForwardIterator& s,
                    ForwardIterator& w,
                    ForwardIterator& e,
                    const Traits& ch_traits )
{
  typename Traits::Less_xy_2    
      lexicographically_xy_smaller = ch_traits.less_xy_2_object();
  typename Traits::Less_yx_2    
      lexicographically_yx_smaller = ch_traits.less_yx_2_object();
  n = s = w = e = first;
  while ( first != last )
  {
      if ( lexicographically_xy_smaller( *first, *w ))  w = first;
      if ( lexicographically_xy_smaller( *e, *first ))  e = first;
      if ( lexicographically_yx_smaller( *n, *first ))  n = first;
      if ( lexicographically_yx_smaller( *first, *s ))  s = first;
      ++first;
  }
}


template <class ForwardIterator, class Traits>
void
ch_we_point( ForwardIterator first, ForwardIterator last,
                  ForwardIterator& w,
                  ForwardIterator& e,
                  const Traits& ch_traits)
{
 typename Traits::Less_xy_2    
    lexicographically_xy_smaller = ch_traits.less_xy_2_object();
 w = e = first;
 while ( first != last )
 {
    if ( lexicographically_xy_smaller( *first, *w ))  w = first;
    if ( lexicographically_xy_smaller( *e, *first ))  e = first;
    ++first;
 }
}

template <class ForwardIterator, class Traits>
void
ch_ns_point( ForwardIterator first, ForwardIterator last,
                  ForwardIterator& n,
                  ForwardIterator& s,
                  const Traits& ch_traits)
{
 typename Traits::Less_yx_2    
    lexicographically_yx_smaller = ch_traits.less_yx_2_object();
 n = s = first;
 while ( first != last )
 {
    if ( lexicographically_yx_smaller( *first, *s ))  s = first;
    if ( lexicographically_yx_smaller( *n, *first ))  n = first;
    ++first;
 }
}

template <class ForwardIterator, class Traits>
void
ch_n_point( ForwardIterator first, ForwardIterator last,
                 ForwardIterator& n,
                 const Traits& ch_traits)
{
 typename Traits::Less_yx_2    
    lexicographically_yx_smaller = ch_traits.less_yx_2_object();
 n = first;
 while ( first != last )
 {
    if ( lexicographically_yx_smaller ( *n, *first ))  n = first;
    ++first;
 }
}

template <class ForwardIterator, class Traits>
void
ch_s_point( ForwardIterator first, ForwardIterator last,
                 ForwardIterator& s,
                 const Traits& ch_traits)
{
 typename Traits::Less_yx_2    
    lexicographically_yx_smaller = ch_traits.less_yx_2_object();
 s = first;
 while ( first != last )
 {
    if ( lexicographically_yx_smaller( *first, *s ))  s = first;
    ++first;
 }
}

template <class ForwardIterator, class Traits>
void
ch_e_point( ForwardIterator first, ForwardIterator last,
                 ForwardIterator& e,
                 const Traits& ch_traits)
{
 typename Traits::Less_xy_2    
    lexicographically_xy_smaller = ch_traits.less_xy_2_object();
 e = first;
 while ( first != last )
 {
    if ( lexicographically_xy_smaller( *e, *first ))  e = first;
    ++first;
 }
}

template <class ForwardIterator, class Traits>
void
ch_w_point( ForwardIterator first, ForwardIterator last,
                 ForwardIterator& w,
                 const Traits& ch_traits)
{
 typename Traits::Less_xy_2    
    lexicographically_xy_smaller = ch_traits.less_xy_2_object();
 w = first;
 while ( first != last )
 {
    if ( lexicographically_xy_smaller( *first, *w ))  w = first;
    ++first;
 }
}
CGAL_END_NAMESPACE

#endif // CGAL_CH_SELECTED_EXTREME_POINTS_2_C

