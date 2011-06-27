/* linbox/blas/level1-generic.h
 * Copyright 2011 Bradford Hovinen <hovinen@gmail.com>
 *
 * Generic implementations of level 1 BLAS interface
 * ------------------------------------
 *
 * See COPYING for license information.
 */

#ifndef __BLAS_LEVEL1_GENERIC_H
#define __BLAS_LEVEL1_GENERIC_H

#include <algorithm>
#include <iostream>

#include "linbox/blas/context.h"
#include "linbox/vector/traits.h"
#include "linbox/blas/level1-ll.h"

namespace LinBox
{

namespace BLAS1 
{

template <class Ring>
class _dot<Ring, GenericModule::Tag>
{
	template <class Modules, class Vector1, class Vector2>
	static typename Ring::Element &dot_impl (const Ring &F, Modules &M, typename Ring::Element &res, const Vector1 &x, const Vector2 &y,
						 size_t start_idx, size_t end_idx,
						 VectorCategories::DenseVectorTag, VectorCategories::DenseVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static typename Ring::Element &dot_impl (const Ring &F, Modules &M, typename Ring::Element &res, const Vector1 &x, const Vector2 &y,
						 size_t start_idx, size_t end_idx,
						 VectorCategories::DenseVectorTag, VectorCategories::SparseVectorTag)
		{ return _dot (F, M, res, y, x, start_idx, end_idx); }

	template <class Modules, class Vector1, class Vector2>
	static typename Ring::Element &dot_impl (const Ring &F, Modules &M, typename Ring::Element &res, const Vector1 &x, const Vector2 &y,
						 size_t start_idx, size_t end_idx,
						 VectorCategories::SparseVectorTag, VectorCategories::DenseVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static typename Ring::Element &dot_impl (const Ring &F, Modules &M, typename Ring::Element &res, const Vector1 &x, const Vector2 &y,
						 size_t start_idx, size_t end_idx,
						 VectorCategories::SparseVectorTag, VectorCategories::SparseVectorTag);

public:
	template <class Modules, class reference, class Vector1, class Vector2>
	static reference &op (const Ring &F, Modules &M, reference &res, const Vector1 &x, const Vector2 &y,
			      size_t start_idx = 0, size_t end_idx = (size_t) -1)
		{ return dot_impl (F, M, res, x, y, start_idx, end_idx,
				   typename VectorTraits<Ring, Vector1>::VectorCategory (),
				   typename VectorTraits<Ring, Vector2>::VectorCategory ()); }
};

template <class Ring>
class _swap<Ring, GenericModule::Tag>
{
public:
	template <class Modules, class Vector>
	static void op (const Ring &F, Modules &M, Vector &x, Vector &y)
		{ std::swap (x, y); }
};

template <class Ring>
class _copy<Ring, GenericModule::Tag>
{
	template <class Modules, class Vector1, class Vector2>
	static Vector2 &copy_impl (const Ring &F, Modules &M, const Vector1 &x, Vector2 &y,
				   VectorCategories::DenseVectorTag, VectorCategories::DenseVectorTag)
	{
		linbox_check (x.size () == y.size ());

		std::copy (x.begin (), x.end (), y.begin ());
		return y;
	}

	template <class Modules, class Vector1, class Vector2>
	static Vector2 &copy_impl (const Ring &F, Modules &M, const Vector1 &x, Vector2 &y,
				   VectorCategories::DenseVectorTag, VectorCategories::SparseVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static Vector2 &copy_impl (const Ring &F, Modules &M, const Vector1 &x, Vector2 &y,
				   VectorCategories::SparseVectorTag, VectorCategories::DenseVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static Vector2 &copy_impl (const Ring &F, Modules &M, const Vector1 &x, Vector2 &y,
				   VectorCategories::SparseVectorTag, VectorCategories::SparseVectorTag)
		{ y.assign (x.begin (), x.end ()); return y; }

public:
	template <class Modules, class Vector1, class Vector2>
	static Vector2 &op (const Ring &F, Modules &M, const Vector1 &x, Vector2 &y)
		{ return copy_impl (F, M, x, y,
				    typename VectorTraits<Ring, Vector1>::VectorCategory (),
				    typename VectorTraits<Ring, Vector2>::VectorCategory ()); }
};

template <class Ring>
class _axpy<Ring, GenericModule::Tag>
{
	template <class Modules, class Vector1, class Vector2>
	static Vector2 &axpy_impl (const Ring &F, Modules &M, const typename Ring::Element &a, const Vector1 &x, Vector2 &y,
				   VectorCategories::DenseVectorTag, VectorCategories::DenseVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static Vector2 &axpy_impl (const Ring &F, Modules &M, const typename Ring::Element &a, const Vector1 &x, Vector2 &y,
				   VectorCategories::DenseVectorTag, VectorCategories::SparseVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static Vector2 &axpy_impl (const Ring &F, Modules &M, const typename Ring::Element &a, const Vector1 &x, Vector2 &y,
				   VectorCategories::SparseVectorTag, VectorCategories::DenseVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static Vector2 &axpy_impl (const Ring &F, Modules &M, const typename Ring::Element &a, const Vector1 &x, Vector2 &y,
				   VectorCategories::SparseVectorTag, VectorCategories::SparseVectorTag);

public:
	template <class Modules, class Vector1, class Vector2>
	static Vector2 &op (const Ring &F, Modules &M, const typename Ring::Element &a, const Vector1 &x, Vector2 &y)
		{ return axpy_impl (F, M, a, x, y,
				    typename VectorTraits<Ring, Vector1>::VectorCategory (),
				    typename VectorTraits<Ring, Vector2>::VectorCategory ()); }
};

template <class Ring>
class _scal<Ring, GenericModule::Tag>
{
	template <class Modules, class Vector>
	static Vector &scal_impl (const Ring &F, Modules &M, const typename Ring::Element &a, Vector &x, VectorCategories::DenseVectorTag);

	template <class Modules, class Vector>
	static Vector &scal_impl (const Ring &F, Modules &M, const typename Ring::Element &a, Vector &x, VectorCategories::SparseVectorTag);

public:
	template <class Modules, class Vector>
	static Vector &op (const Ring &F, Modules &M, const typename Ring::Element &a, Vector &x)
		{ return scal_impl (F, M, a, x,
				    typename VectorTraits<Ring, Vector>::VectorCategory ()); }
};

template <class Ring>
class _permute<Ring, GenericModule::Tag>
{
	template <class Modules, class Iterator, class Vector>
	static Vector &permute_impl (const Ring &F, Modules &M, Iterator P_begin, Iterator P_end, Vector &v, VectorCategories::DenseVectorTag);

	template <class Modules, class Iterator, class Vector>
	static Vector &permute_impl (const Ring &F, Modules &M, Iterator P_begin, Iterator P_end, Vector &v, VectorCategories::SparseVectorTag);

public:
	template <class Modules, class Iterator, class Vector>
	static Vector &op (const Ring &F, Modules &M, Iterator P_begin, Iterator P_end, Vector &v)
		{ return permute_impl (F, M, P_begin, P_end, v,
				       typename VectorTraits<Ring, Vector>::VectorCategory ()); }
};

template <class Ring>
class _equal<Ring, GenericModule::Tag>
{
	template <class Modules, class Vector1, class Vector2>
	static bool equal_impl (const Ring &F, Modules &M, const Vector1 &x, const Vector2 &y,
				VectorCategories::DenseVectorTag, VectorCategories::DenseVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static bool equal_impl (const Ring &F, Modules &M, const Vector1 &x, const Vector2 &y,
				VectorCategories::SparseVectorTag, VectorCategories::DenseVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static bool equal_impl (const Ring &F, Modules &M, const Vector1 &x, const Vector2 &y,
				VectorCategories::DenseVectorTag, VectorCategories::SparseVectorTag)
		{ return _equal<Ring, typename Modules::Tag>::op (F, M, y, x); }

	template <class Modules, class Vector1, class Vector2>
	static bool equal_impl (const Ring &F, Modules &M, const Vector1 &x, const Vector2 &y,
				VectorCategories::SparseVectorTag, VectorCategories::SparseVectorTag);

public:
	template <class Modules, class Vector1, class Vector2>
	static bool op (const Ring &F, Modules &M, const Vector1 &x, const Vector2 &y)
		{ return equal_impl (F, M, x, y,
				     typename VectorTraits<Ring, Vector1>::VectorCategory (),
				     typename VectorTraits<Ring, Vector2>::VectorCategory ()); }
};

template <class Ring>
class _is_zero<Ring, GenericModule::Tag>
{
	template <class Modules, class Vector>
	static bool is_zero_impl (const Ring &F, Modules &M, const Vector &x, VectorCategories::DenseVectorTag);

	template <class Modules, class Vector>
	static bool is_zero_impl (const Ring &F, Modules &M, const Vector &x, VectorCategories::SparseVectorTag);

public:
	template <class Modules, class Vector>
	static bool op (const Ring &F, Modules &M, const Vector &x)
		{ return is_zero_impl (F, M, x,
				       typename VectorTraits<Ring, Vector>::VectorCategory ()); }
};

template <class Ring>
class _head<Ring, GenericModule::Tag>
{
	template <class Modules, class Vector>
	static int head_impl (const Ring &F, Modules &M, typename Ring::Element &a, const Vector &x, VectorCategories::DenseVectorTag);

	template <class Modules, class Vector>
	static int head_impl (const Ring &F, Modules &M, typename Ring::Element &a, const Vector &x, VectorCategories::SparseVectorTag);

public:
	template <class Modules, class Vector>
	static int op (const Ring &F, Modules &M, typename Ring::Element &a, const Vector &x)
		{ return head_impl (F, M, a, x,
				    typename VectorTraits<Ring, Vector>::VectorCategory ()); }
};

template <class Ring>
class _read<Ring, GenericModule::Tag>
{
	template <class Modules, class Vector>
	static std::istream &read_impl (const Ring &F, Modules &M, std::istream &is, Vector &v, VectorCategories::DenseVectorTag);

	template <class Modules, class Vector>
	static std::istream &read_impl (const Ring &F, Modules &M, std::istream &is, Vector &v, VectorCategories::SparseVectorTag);

public:
	template <class Modules, class Vector>
	static std::istream &op (const Ring &F, Modules &M, std::istream &is, Vector &v)
		{ return read_impl (F, M, is, v,
				    typename VectorTraits<Ring, Vector>::VectorCategory ()); }
};

template <class Ring>
class _write<Ring, GenericModule::Tag>
{
	template <class Modules, class Vector>
	static std::ostream &write_impl (const Ring &F, Modules &M, std::ostream &os, const Vector &v, VectorCategories::DenseVectorTag);

	template <class Modules, class Vector>
	static std::ostream &write_impl (const Ring &F, Modules &M, std::ostream &os, const Vector &v, VectorCategories::SparseVectorTag);

public:
	template <class Modules, class Vector>
	static std::ostream &op (const Ring &F, Modules &M, std::ostream &os, const Vector &v)
		{ return write_impl (F, M, os, v,
				     typename VectorTraits<Ring, Vector>::VectorCategory ()); }
};

} // namespace BLAS1

} // namespace LinBox

#include "linbox/blas/level1-generic.tcc"

#endif // __BLAS_LEVEL1_GENERIC_H

// Local Variables:
// mode: C++
// tab-width: 8
// indent-tabs-mode: t
// c-basic-offset: 8
// End:

// vim:sts=8:sw=8:ts=8:noet:sr:cino=>s,f0,{0,g0,(0,\:0,t0,+0,=s:syntax=cpp.doxygen:foldmethod=syntax
