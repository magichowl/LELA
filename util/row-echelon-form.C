/* util/row-echelon-form.C
 * Copyright 2010 Bradford Hovinen
 * Written by Bradford Hovinen <hovinen@gmail.com>
 *
 * ---------------------------------------------------------
 *
 * See COPYING for license information.
 *
 * Utility to convert a matrix to row-echelon form
 */

#include "linbox/util/commentator.h"
#include "linbox/blas/context.h"
#include "linbox/ring/gf2.h"
#include "linbox/ring/modular.h"
#include "linbox/blas/level3.h"
#include "linbox/solutions/echelon-form.h"
#include "linbox/solutions/echelon-form-gf2.h"

#include "support.h"

using namespace LinBox;

template <class Ring, class Matrix>
int row_echelon_form (const Ring &R, const char *input, FileFormatTag input_format, const char *output, FileFormatTag output_format,
		      typename EchelonForm<Ring>::Method method, bool reduced)
{
	Context<Ring> ctx (R);

	commentator.start ("Converting matrix to row-echelon-form", __FUNCTION__);

	Matrix A;

	commentator.start ("Reading input-matrix");

	std::ifstream ifile (input);

	if (!ifile.good ()) {
		commentator.report (Commentator::LEVEL_IMPORTANT, INTERNAL_ERROR)
			<< "Could not open input-file" << std::endl;
		return -1;
	}

	BLAS3::read (ctx, ifile, A, input_format);

	commentator.stop (MSG_DONE);

	EchelonForm<Ring> EF (ctx);

	EF.RowEchelonForm (A, reduced, method);

	commentator.start ("Writing output-matrix");

	std::ofstream ofile (output);

	if (!ofile.good ()) {
		commentator.report (Commentator::LEVEL_IMPORTANT, INTERNAL_ERROR)
			<< "Could not open output-file" << std::endl;
		return -1;
	}

	BLAS3::write (ctx, ofile, A, output_format);

	commentator.stop (MSG_DONE);

	commentator.stop (MSG_DONE);

	return 0;
}

template <class Ring>
typename EchelonForm<Ring>::Method get_method (const char *str)
{
	if (!strcmp (str, "standard"))
		return EchelonForm<Ring>::METHOD_STANDARD_GJ;
	if (!strcmp (str, "afast"))
		return EchelonForm<Ring>::METHOD_ASYMPTOTICALLY_FAST_GJ;
	if (!strcmp (str, "f4"))
		return EchelonForm<Ring>::METHOD_FAUGERE_LACHARTRE;

	return EchelonForm<Ring>::METHOD_UNKNOWN;
}

template <>
EchelonForm<GF2>::Method get_method<GF2> (const char *str)
{
	if (!strcmp (str, "standard"))
		return EchelonForm<GF2>::METHOD_STANDARD_GJ;
	if (!strcmp (str, "afast"))
		return EchelonForm<GF2>::METHOD_ASYMPTOTICALLY_FAST_GJ;
	if (!strcmp (str, "m4ri"))
		return EchelonForm<GF2>::METHOD_M4RI;
	if (!strcmp (str, "f4"))
		return EchelonForm<GF2>::METHOD_FAUGERE_LACHARTRE;

	return EchelonForm<GF2>::METHOD_UNKNOWN;
}

template <class Ring>
struct ErrorText
{
	static const char *method;
	static const char *type;
};

template <class Ring>
const char *ErrorText<Ring>::method = "Invalid method (use 'standard', 'afast', or 'f4')";

template <class Ring>
const char *ErrorText<Ring>::type = "Invalid matrix-type (use 'dense' or 'sparse')";

template <>
struct ErrorText<GF2>
{
	static const char *method;
	static const char *type;
};

const char *ErrorText<GF2>::method = "Invalid method (use 'standard', 'afast', 'm4ri', or 'f4')";
const char *ErrorText<GF2>::type = "Invalid matrix-type (use 'dense', 'sparse', or 'hybrid')";

template <class Ring>
int inner_run_row_echelon_form (const Ring &R, typename EchelonForm<Ring>::Method method, MatrixType type, bool reduced,
				const char *input, FileFormatTag input_format, const char *output, FileFormatTag output_format)
{
	if (type == MATRIX_DENSE)
		return row_echelon_form<Ring, DenseMatrix<typename Ring::Element> > (R, input, input_format, output, output_format, method, reduced);
	else if (type == MATRIX_SPARSE)
		return row_echelon_form<Ring, SparseMatrix<typename Ring::Element> > (R, input, input_format, output, output_format, method, reduced);
	else {
		std::cerr << ErrorText<Ring>::type << std::endl;
		return -1;
	}
}

int inner_run_row_echelon_form (const GF2 &R, EchelonForm<GF2>::Method method, MatrixType type, bool reduced,
				const char *input, FileFormatTag input_format, const char *output, FileFormatTag output_format)
{
	if (type == MATRIX_DENSE)
		return row_echelon_form<GF2, DenseMatrix<GF2::Element> > (R, input, input_format, output, output_format, method, reduced);
	else if (type == MATRIX_SPARSE)
		return row_echelon_form<GF2, SparseMatrix<GF2::Element, Vector<GF2>::Sparse> > (R, input, input_format, output, output_format, method, reduced);
	else if (type == MATRIX_HYBRID)
		return row_echelon_form<GF2, SparseMatrix<GF2::Element, Vector<GF2>::Hybrid> > (R, input, input_format, output, output_format, method, reduced);
	else {
		std::cerr << ErrorText<GF2>::type << std::endl;
		return -1;
	}
}

template <class Ring>
int run_row_echelon_form (const Ring &R, const char *method_string, const char *matrix_type_string, bool reduced,
			  const char *input, FileFormatTag input_format, const char *output, FileFormatTag output_format)
{
	typename EchelonForm<Ring>::Method method = get_method<Ring> (method_string);

	if (method == EchelonForm<Ring>::METHOD_UNKNOWN) {
		std::cerr << ErrorText<Ring>::method << std::endl;
		return -1;
	}

	MatrixType type = get_matrix_type<Ring> (matrix_type_string);

	return inner_run_row_echelon_form (R, method, type, reduced, input, input_format, output, output_format);
}

int main (int argc, char **argv)
{
	static bool reduced = false;
	static const char *ringString = "modular";
	static integer p = 101;
	static bool floatingPoint = false;
	static const char *methodString = "f4";
	static const char *inputFileFormat = "guess";
	static const char *outputFileFormat = "dumas";
	static const char *matrixType = "dense";
	static char *input = NULL;
	static char *output = NULL;

	static Argument args[] = {
		{ 'r', "-r", "Compute the reduced row-echelon form (default: no)", TYPE_NONE, &reduced },
		{ 'k', "-k", "Ring over which to compute ('gf2', 'modular'; default 'modular')", TYPE_STRING, &ringString },
		{ 'p', "-p", "Modulus of ring, when ring is 'modular' (default 101)", TYPE_INT, &p },
		{ 'f', "-f", "Compute using floating point, when ring is 'modular' (default: no)", TYPE_NONE, &floatingPoint },
		{ 'm', "-m", "Method to be used ('standard', 'afast', 'm4ri', or 'f4'; default f4)", TYPE_STRING, &methodString },
		{ 'i', "-i", "Input file format ('guess', 'dumas', 'turner', 'maple', 'matlab', 'sage', 'png'; default: guess)", TYPE_STRING, &inputFileFormat },
		{ 'o', "-o", "Output file format ('dumas', 'turner', 'maple', 'matlab', 'sage', 'png', 'pretty'; default: dumas)", TYPE_STRING, &outputFileFormat },
		{ 't', "-t", "Type to use for matrix ('dense', 'sparse', 'hybrid'; default: dense)", TYPE_NONE, &reduced },
		{ '\0' }
	};

	parseArguments (argc, argv, args, 2, &input, &output);

	commentator.setBriefReportParameters (Commentator::OUTPUT_CONSOLE, false, false, false);
	commentator.getMessageClass (INTERNAL_DESCRIPTION).setMaxDepth (4);
	commentator.getMessageClass (INTERNAL_DESCRIPTION).setMaxDetailLevel (Commentator::LEVEL_NORMAL);
	commentator.getMessageClass (TIMING_MEASURE).setMaxDepth (3);
	commentator.getMessageClass (TIMING_MEASURE).setMaxDetailLevel (Commentator::LEVEL_NORMAL);
	commentator.getMessageClass (PROGRESS_REPORT).setMaxDepth (3);

	FileFormatTag input_format = get_format_tag (inputFileFormat);
	FileFormatTag output_format = get_format_tag (outputFileFormat);

	if (input_format == FORMAT_UNKNOWN || input_format == FORMAT_PRETTY) {
		std::cerr << "Invalid input-file-format (use 'guess', 'dumas', 'turner', 'maple', 'matlab', 'sage', or 'png')" << std::endl;
		return -1;
	}

	if (output_format == FORMAT_UNKNOWN || output_format == FORMAT_DETECT) {
		std::cerr << "Invalid output-file-format (use 'dumas', 'turner', 'maple', 'matlab', 'sage', 'png', or 'pretty')" << std::endl;
		return -1;
	}

	RingType ring_type = get_ring_type (ringString);

	if (ring_type == RING_GF2)
		return run_row_echelon_form (GF2 (), methodString, matrixType, reduced, input, input_format, output, output_format);
	else if (ring_type == RING_MODULAR) {
		if (floatingPoint) {
			if (p <= Modular<float>::getMaxModulus ())
				return run_row_echelon_form (Modular<float> (p), methodString, matrixType, reduced, input, input_format, output, output_format);
			else if (p <= Modular<double>::getMaxModulus ())
				return run_row_echelon_form (Modular<double> (p), methodString, matrixType, reduced, input, input_format, output, output_format);
			else
				return run_row_echelon_form (Modular<integer> (p), methodString, matrixType, reduced, input, input_format, output, output_format);
		} else {
			if (p <= Modular<uint8>::getMaxModulus ())
				return run_row_echelon_form (Modular<uint8> (p), methodString, matrixType, reduced, input, input_format, output, output_format);
			else if (p <= Modular<uint16>::getMaxModulus ())
				return run_row_echelon_form (Modular<uint16> (p), methodString, matrixType, reduced, input, input_format, output, output_format);
			else if (p <= Modular<uint32>::getMaxModulus ())
				return run_row_echelon_form (Modular<uint32> (p), methodString, matrixType, reduced, input, input_format, output, output_format);
			else
				return run_row_echelon_form (Modular<integer> (p), methodString, matrixType, reduced, input, input_format, output, output_format);
		}
	}
	else if (ring_type == RING_UNKNOWN) {
		std::cerr << "Invalid ring (use 'gf2' or 'modular')" << std::endl;
		return -1;
	}
}

// Local Variables:
// mode: C++
// tab-width: 8
// indent-tabs-mode: t
// c-basic-offset: 8
// End:

// vim:sts=8:sw=8:ts=8:noet:sr:cino=>s,f0,{0,g0,(0,\:0,t0,+0,=s:syntax=cpp.doxygen:foldmethod=syntax