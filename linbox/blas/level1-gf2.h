/* linbox/blas/level1-gf2.h
 * Copyright 2011 Bradford Hovinen <hovinen@gmail.com>
 *
 * Implementations of level 1 BLAS interface for GF2
 * ------------------------------------
 *
 * See COPYING for license information.
 */

#ifndef __BLAS_LEVEL1_GF2_H
#define __BLAS_LEVEL1_GF2_H

#include <algorithm>
#include <iostream>

#include "linbox/blas/context.h"
#include "linbox/ring/gf2.h"
#include "linbox/vector/traits.h"
#include "linbox/blas/level1-ll.h"

namespace LinBox
{

namespace BLAS1 
{

template <>
class _dot<GF2, GenericModule::Tag>
{
	template <class Modules, class reference, class Vector1, class Vector2>
	static reference &dot_impl (const GF2 &F, Modules &M, reference &res, const Vector1 &x, const Vector2 &y,
				    size_t start_idx, size_t end_idx,
				    VectorCategories::DenseZeroOneVectorTag, VectorCategories::DenseZeroOneVectorTag);

	template <class Modules, class reference, class Vector1, class Vector2>
	static reference &dot_impl (const GF2 &F, Modules &M, reference &res, const Vector1 &x, const Vector2 &y,
				    size_t start_idx, size_t end_idx,
				    VectorCategories::DenseZeroOneVectorTag, VectorCategories::SparseZeroOneVectorTag);

	template <class Modules, class reference, class Vector1, class Vector2>
	static reference &dot_impl (const GF2 &F, Modules &M, reference &res, const Vector1 &x, const Vector2 &y,
				    size_t start_idx, size_t end_idx,
				    VectorCategories::DenseZeroOneVectorTag, VectorCategories::HybridZeroOneVectorTag);

	template <class Modules, class reference, class Vector1, class Vector2>
	static reference &dot_impl (const GF2 &F, Modules &M, reference &res, const Vector1 &x, const Vector2 &y,
				    size_t start_idx, size_t end_idx,
				    VectorCategories::SparseZeroOneVectorTag, VectorCategories::DenseZeroOneVectorTag)
		{ return op (F, M, res, y, x, start_idx, end_idx); }

	template <class Modules, class reference, class Vector1, class Vector2>
	static reference &dot_impl (const GF2 &F, Modules &M, reference &res, const Vector1 &x, const Vector2 &y,
				    size_t start_idx, size_t end_idx,
				    VectorCategories::SparseZeroOneVectorTag, VectorCategories::SparseZeroOneVectorTag);

	template <class Modules, class reference, class Vector1, class Vector2>
	static reference &dot_impl (const GF2 &F, Modules &M, reference &res, const Vector1 &x, const Vector2 &y,
				    size_t start_idx, size_t end_idx,
				    VectorCategories::HybridZeroOneVectorTag, VectorCategories::DenseZeroOneVectorTag)
		{ return op (F, M, res, y, x, start_idx, end_idx); }

public:
	template <class Modules, class reference, class Vector1, class Vector2>
	static reference &op (const GF2 &F, Modules &M, reference &res, const Vector1 &x, const Vector2 &y,
			      size_t start_idx = 0, size_t end_idx = (size_t) -1)
		{ return dot_impl (F, M, res, x, y, start_idx, end_idx,
				   typename VectorTraits<GF2, Vector1>::VectorCategory (),
				   typename VectorTraits<GF2, Vector2>::VectorCategory ()); }
};

template <>
class _copy<GF2, GenericModule::Tag>
{
	template <class Modules, class Vector1, class Vector2>
	static Vector2 &copy_impl (const GF2 &F, Modules &M, const Vector1 &x, Vector2 &y,
				   VectorCategories::DenseZeroOneVectorTag, VectorCategories::DenseZeroOneVectorTag)
		{ std::copy (x.word_begin (), x.word_end (), y.word_begin ()); y.back_word () = x.back_word (); return y; }

	template <class Modules, class Vector1, class Vector2>
	static Vector2 &copy_impl (const GF2 &F, Modules &M, const Vector1 &x, Vector2 &y,
				   VectorCategories::DenseZeroOneVectorTag, VectorCategories::SparseZeroOneVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static Vector2 &copy_impl (const GF2 &F, Modules &M, const Vector1 &x, Vector2 &y,
				   VectorCategories::DenseZeroOneVectorTag, VectorCategories::HybridZeroOneVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static Vector2 &copy_impl (const GF2 &F, Modules &M, const Vector1 &x, Vector2 &y,
				   VectorCategories::SparseZeroOneVectorTag, VectorCategories::DenseZeroOneVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static Vector2 &copy_impl (const GF2 &F, Modules &M, const Vector1 &x, Vector2 &y,
				   VectorCategories::SparseZeroOneVectorTag, VectorCategories::SparseZeroOneVectorTag)
		{ y.assign (x.begin (), x.end ()); return y; }

	template <class Modules, class Vector1, class Vector2>
	static Vector2 &copy_impl (const GF2 &F, Modules &M, const Vector1 &x, Vector2 &y,
				   VectorCategories::SparseZeroOneVectorTag, VectorCategories::HybridZeroOneVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static Vector2 &copy_impl (const GF2 &F, Modules &M, const Vector1 &x, Vector2 &y,
				   VectorCategories::HybridZeroOneVectorTag, VectorCategories::DenseZeroOneVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static Vector2 &copy_impl (const GF2 &F, Modules &M, const Vector1 &x, Vector2 &y,
				   VectorCategories::HybridZeroOneVectorTag, VectorCategories::SparseZeroOneVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static Vector2 &copy_impl (const GF2 &F, Modules &M, const Vector1 &x, Vector2 &y,
				   VectorCategories::HybridZeroOneVectorTag, VectorCategories::HybridZeroOneVectorTag)
		{ y.assign (x.begin (), x.end ()); return y; }

public:
	template <class Modules, class Vector1, class Vector2>
	static Vector2 &op (const GF2 &F, Modules &M, const Vector1 &x, Vector2 &y)
		{ return copy_impl (F, M, x, y,
				    typename VectorTraits<GF2, Vector1>::VectorCategory (),
				    typename VectorTraits<GF2, Vector2>::VectorCategory ()); }
};

template <>
class _axpy<GF2, GenericModule::Tag>
{
	template <class Modules, class Vector1, class Vector2>
	static Vector2 &axpy_impl (const GF2 &F, Modules &M, bool a, const Vector1 &x, Vector2 &y,
				   VectorCategories::DenseZeroOneVectorTag, VectorCategories::DenseZeroOneVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static Vector2 &axpy_impl (const GF2 &F, Modules &M, bool a, const Vector1 &x, Vector2 &y,
				   VectorCategories::SparseZeroOneVectorTag, VectorCategories::SparseZeroOneVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static Vector2 &axpy_impl (const GF2 &F, Modules &M, bool a, const Vector1 &x, Vector2 &y,
				   VectorCategories::SparseZeroOneVectorTag, VectorCategories::DenseZeroOneVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static Vector2 &axpy_impl (const GF2 &F, Modules &M, bool a, const Vector1 &x, Vector2 &y,
				   VectorCategories::HybridZeroOneVectorTag, VectorCategories::DenseZeroOneVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static Vector2 &axpy_impl (const GF2 &F, Modules &M, bool a, const Vector1 &x, Vector2 &y,
				   VectorCategories::HybridZeroOneVectorTag, VectorCategories::HybridZeroOneVectorTag);

public:
	template <class Modules, class Vector1, class Vector2>
	static Vector2 &op (const GF2 &F, Modules &M, bool a, const Vector1 &x, Vector2 &y)
		{ return axpy_impl (F, M, a, x, y,
				    typename VectorTraits<GF2, Vector1>::VectorCategory (),
				    typename VectorTraits<GF2, Vector2>::VectorCategory ()); }
};

template <>
class _scal<GF2, GenericModule::Tag>
{
	template <class Modules, class Vector>
	static Vector &scal_impl (const GF2 &F, Modules &M, bool a, Vector &x, VectorCategories::DenseZeroOneVectorTag)
		{ if (!a) { std::fill (x.word_begin (), x.word_end (), 0); x.back_word () = 0; } return x; }

	template <class Modules, class Vector>
	static Vector &scal_impl (const GF2 &F, Modules &M, bool a, Vector &x, VectorCategories::SparseZeroOneVectorTag)
		{ if (!a) x.clear (); return x; }

	template <class Modules, class Vector>
	static Vector &scal_impl (const GF2 &F, Modules &M, bool a, Vector &x, VectorCategories::HybridZeroOneVectorTag)
		{ if (!a) x.clear (); return x; }

public:
	template <class Modules, class Vector>
	static Vector &op (const GF2 &F, Modules &M, bool a, Vector &x)
		{ return scal_impl (F, M, a, x,
				    typename VectorTraits<GF2, Vector>::VectorCategory ()); }
};

template <>
class _permute<GF2, GenericModule::Tag>
{
	template <class Modules, class Iterator, class Vector>
	static Vector &permute_impl (const GF2 &F, Modules &M, Iterator P_begin, Iterator P_end, Vector &v, VectorCategories::SparseZeroOneVectorTag);

	template <class Modules, class Iterator, class Vector>
	static Vector &permute_impl (const GF2 &F, Modules &M, Iterator P_begin, Iterator P_end, Vector &v, VectorCategories::HybridZeroOneVectorTag);

public:
	template <class Modules, class Iterator, class Vector>
	static Vector &op (const GF2 &F, Modules &M, Iterator P_begin, Iterator P_end, Vector &v)
		{ return permute_impl (F, M, P_begin, P_end, v,
				       typename VectorTraits<GF2, Vector>::VectorCategory ()); }
};

template <>
class _equal<GF2, GenericModule::Tag>
{
	template <class Modules, class Vector1, class Vector2>
	static bool equal_impl (const GF2 &F, Modules &M, const Vector1 &x, const Vector2 &y,
				VectorCategories::DenseZeroOneVectorTag, VectorCategories::DenseZeroOneVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static bool equal_impl (const GF2 &F, Modules &M, const Vector1 &x, const Vector2 &y,
				VectorCategories::DenseZeroOneVectorTag, VectorCategories::SparseZeroOneVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static bool equal_impl (const GF2 &F, Modules &M, const Vector1 &x, const Vector2 &y,
				VectorCategories::SparseZeroOneVectorTag, VectorCategories::DenseZeroOneVectorTag)
		{ return op (F, M, y, x); }

	template <class Modules, class Vector1, class Vector2>
	static bool equal_impl (const GF2 &F, Modules &M, const Vector1 &x, const Vector2 &y,
				VectorCategories::SparseZeroOneVectorTag, VectorCategories::SparseZeroOneVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static bool equal_impl (const GF2 &F, Modules &M, const Vector1 &x, const Vector2 &y,
				VectorCategories::DenseZeroOneVectorTag, VectorCategories::HybridZeroOneVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static bool equal_impl (const GF2 &F, Modules &M, const Vector1 &x, const Vector2 &y,
				VectorCategories::HybridZeroOneVectorTag, VectorCategories::DenseZeroOneVectorTag)
		{ return op (F, M, y, x); }

	template <class Modules, class Vector1, class Vector2>
	static bool equal_impl (const GF2 &F, Modules &M, const Vector1 &x, const Vector2 &y,
				VectorCategories::HybridZeroOneVectorTag, VectorCategories::SparseZeroOneVectorTag);

	template <class Modules, class Vector1, class Vector2>
	static bool equal_impl (const GF2 &F, Modules &M, const Vector1 &x, const Vector2 &y,
				VectorCategories::SparseZeroOneVectorTag, VectorCategories::HybridZeroOneVectorTag)
		{ return op (F, M, y, x); }

	template <class Modules, class Vector1, class Vector2>
	static bool equal_impl (const GF2 &F, Modules &M, const Vector1 &x, const Vector2 &y,
				VectorCategories::HybridZeroOneVectorTag, VectorCategories::HybridZeroOneVectorTag);

public:
	template <class Modules, class Vector1, class Vector2>
	static bool op (const GF2 &F, Modules &M, const Vector1 &x, const Vector2 &y)
		{ return equal_impl (F, M, x, y,
				     typename VectorTraits<GF2, Vector1>::VectorCategory (),
				     typename VectorTraits<GF2, Vector2>::VectorCategory ()); }
};

template <>
class _is_zero<GF2, GenericModule::Tag>
{
	template <class Modules, class Vector>
	static bool is_zero_impl (const GF2 &F, Modules &M, const Vector &x, VectorCategories::DenseZeroOneVectorTag);

	template <class Modules, class Vector>
	static bool is_zero_impl (const GF2 &F, Modules &M, const Vector &x, VectorCategories::SparseZeroOneVectorTag)
		{ return x.empty (); }

	template <class Modules, class Vector>
	static bool is_zero_impl (const GF2 &F, Modules &M, const Vector &x, VectorCategories::HybridZeroOneVectorTag)
		{ return x.empty (); }

public:
	template <class Modules, class Vector>
	static bool op (const GF2 &F, Modules &M, const Vector &x)
		{ return is_zero_impl (F, M, x,
				       typename VectorTraits<GF2, Vector>::VectorCategory ()); }
};

template <>
class _head<GF2, GenericModule::Tag>
{
	template <class Modules, class reference, class Vector>
	static int head_impl (const GF2 &F, Modules &M, reference &a, const Vector &x, VectorCategories::DenseZeroOneVectorTag);

	template <class Modules, class reference, class Vector>
	static int head_impl (const GF2 &F, Modules &M, reference &a, const Vector &x, VectorCategories::SparseZeroOneVectorTag)
		{ if (x.empty ()) return -1; a = true; return x.front (); }

	template <class Modules, class reference, class Vector>
	static int head_impl (const GF2 &F, Modules &M, reference &a, const Vector &x, VectorCategories::HybridZeroOneVectorTag);

public:
	template <class Modules, class reference, class Vector>
	static int op (const GF2 &F, Modules &M, reference &a, const Vector &x)
		{ return head_impl (F, M, a, x,
				    typename VectorTraits<GF2, Vector>::VectorCategory ()); }
};

template <>
class _read<GF2, GenericModule::Tag>
{
	template <class Modules, class Vector>
	static std::istream &read_impl (const GF2 &F, Modules &M, std::istream &is, const Vector &v, VectorCategories::DenseZeroOneVectorTag);

	template <class Modules, class Vector>
	static std::istream &read_impl (const GF2 &F, Modules &M, std::istream &is, const Vector &v, VectorCategories::SparseZeroOneVectorTag);

public:
	template <class Modules, class Vector>
	static std::istream &op (const GF2 &F, Modules &M, std::istream &is, Vector &v)
		{ return read_impl (F, M, is, v,
				    typename VectorTraits<GF2, Vector>::VectorCategory ()); }
};

template <>
class _write<GF2, GenericModule::Tag>
{
	template <class Modules, class Vector>
	static std::ostream &write_impl (const GF2 &F, Modules &M, std::ostream &os, const Vector &v, VectorCategories::DenseZeroOneVectorTag);

	template <class Modules, class Vector>
	static std::ostream &write_impl (const GF2 &F, Modules &M, std::ostream &os, const Vector &v, VectorCategories::SparseZeroOneVectorTag);

	template <class Modules, class Vector>
	static std::ostream &write_impl (const GF2 &F, Modules &M, std::ostream &os, const Vector &v, VectorCategories::HybridZeroOneVectorTag);

public:
	template <class Modules, class Vector>
	static std::ostream &op (const GF2 &F, Modules &M, std::ostream &os, const Vector &v)
		{ return write_impl (F, M, os, v,
				     typename VectorTraits<GF2, Vector>::VectorCategory ()); }
};

} // namespace BLAS1

} // namespace LinBox

#endif // __BLAS_LEVEL1_GF2_H

// Local Variables:
// mode: C++
// tab-width: 8
// indent-tabs-mode: t
// c-basic-offset: 8
// End:

// vim:sts=8:sw=8:ts=8:noet:sr:cino=>s,f0,{0,g0,(0,\:0,t0,+0,=s:syntax=cpp.doxygen:foldmethod=syntax
