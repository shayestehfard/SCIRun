/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2015 Scientific Computing and Imaging Institute,
   University of Utah.


   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/
/// @todo Documentation Core/Datatypes/MatrixTypeConversion.h

#ifndef CORE_DATATYPES_MATRIX_TYPE_CONVERSIONS_H
#define CORE_DATATYPES_MATRIX_TYPE_CONVERSIONS_H

#include <Core/Datatypes/Matrix.h>
#include <Core/Datatypes/DenseMatrix.h>
#include <Core/Datatypes/SparseRowMatrix.h>
#include <Core/Datatypes/DenseColumnMatrix.h>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <Core/Datatypes/share.h>

namespace SCIRun {
namespace Core {
namespace Datatypes {

  /// No conversion is done.
  /// NULL is returned if the matrix is not of the appropriate type.
  class SCISHARE castMatrix
  {
  public:
    template <class ToType, typename T, template <typename> class MatrixType>
    static SharedPointer<ToType> to(const SharedPointer<MatrixType<T>>& matrix, typename boost::enable_if<boost::is_same<T, typename ToType::value_type> >::type* = nullptr)
    {
      return boost::dynamic_pointer_cast<ToType>(matrix);
    }

    template <typename T, template <typename> class MatrixType>
    static SharedPointer<DenseMatrixGeneric<T>> toDense(const SharedPointer<MatrixType<T>>& mh)
    {
      return to<DenseMatrixGeneric<T>>(mh);
    }

    template <typename T, template <typename> class MatrixType>
    static SharedPointer<SparseRowMatrixGeneric<T>> toSparse(const SharedPointer<MatrixType<T>>& mh)
    {
      return to<SparseRowMatrixGeneric<T>>(mh);
    }

    template <typename T, template <typename> class MatrixType>
    static SharedPointer<DenseColumnMatrixGeneric<T>> toColumn(const SharedPointer<MatrixType<T>>& mh)
    {
      return to<DenseColumnMatrixGeneric<T>>(mh);
    }

    castMatrix() = delete;
  };

  class SCISHARE matrixIs
  {
  public:
    // Test to see if the matrix is this subtype.
    template <typename T>
    static bool dense(const SharedPointer<MatrixBase<T>>& mh)
    {
      return castMatrix::toDense(mh) != nullptr;
    }

    template <typename T>
    static bool sparse(const SharedPointer<MatrixBase<T>>& mh)
    {
      return castMatrix::toSparse(mh) != nullptr;
    }

    static bool column(const MatrixHandle& mh);
    static std::string whatType(const MatrixHandle& mh);
    static std::string whatType(const ComplexMatrixHandle& mh);
    static MatrixTypeCode typeCode(const MatrixHandle& mh);

    matrixIs() = delete;
  };

  /// @todo: move
  class SCISHARE convertMatrix
  {
  public:
    template <typename T, template <typename> class MatrixType>
    static SharedPointer<DenseColumnMatrixGeneric<typename MatrixType<T>::value_type>> toColumn(const SharedPointer<MatrixType<T>>& mh)
    {
      auto col = castMatrix::toColumn(mh);
      if (col)
        return col;

      auto dense = castMatrix::toDense(mh);
      if (dense)
        return boost::make_shared<DenseColumnMatrixGeneric<T>>(dense->col(0));

      auto sparse = castMatrix::toSparse(mh);
      if (sparse)
      {
        DenseColumnMatrixGeneric<T> dense_col(DenseColumnMatrixGeneric<T>::Zero(sparse->nrows()));
        for (auto i = 0; i < sparse->nrows(); i++)
          dense_col(i, 0) = sparse->coeff(i, 0);

        return boost::make_shared<DenseColumnMatrixGeneric<T>>(dense_col);
      }

      return nullptr;
    }
    static DenseMatrixHandle toDense(const MatrixHandle& mh);
    static SparseRowMatrixHandle toSparse(const MatrixHandle& mh);
    static SparseRowMatrixHandle fromDenseToSparse(const DenseMatrix& mh);

    convertMatrix() = delete;
  private:
    static const double zero_threshold;  /// defines a threshold below that its a zero matrix element (sparsematrix)
  };

}}}


#endif
