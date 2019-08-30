#ifndef _DCSRMATRIX_HPP
#define _DCSRMATRIX_HPP

#include <vector>
#include <utility>
#include <cstdint>

#include "PackedVector.hpp"

namespace Osi2 {

typedef std::pair<uint32_t, uint32_t> Indices;


/*! \brief Implementation of a DCSR matrix

    Works on the principle of lazy updates, and needs to be defragmented before some operations

    From a paper of James King, Thomas Gilray, Robert M. Kirby and Matthew Might (https://thomas.gilray.org/pdf/dynamic-csr-journal.pdf)
 */
class DCSRMatrix {
    public:
        /// \name Constructors
        //{@

        /// Default Constructor
        DCSRMatrix();

        /// Constructs a DCSRMatrix initialized with a given number of rows and columns
        DCSRMatrix(size_t nb_rows, size_t nb_cols);
        //@}

        /// \name Editing functions
        //{@

        /// Append a row, described by a PackedVector
        bool addRow(const PackedVector& v);

        /// Append a Column, described by a PackedVector
        bool addColumn(const PackedVector& v);
        //@}

        /// Defragment the matrix (no need to call it by hand)
        void defragment();

        /// \name Getters
        //{@

        /// Get the value at coordinate (i,j) (starting at (0,0))
        double getValue(uint32_t i, uint32_t j) const;

        /// Get an entire row
        PackedVector getRow(uint32_t index) const;

        /// Get an entire column
        PackedVector getColumn(uint32_t index) const;

        /// Check if the matrix needs to be defragmented before use
        bool isConsistant() const { return consistant; }

        /// Get the size of the "chunks" of numbers we are using
        size_t getSegmentSize() const { return segment_size; }

        /// Get the number of rows
        uint32_t getRowCount() const { return row_count; }

        /// Get the number of columns
        uint32_t getColumnCount() const { return col_count; }
        //@}

        /// \name Setters
        //{@  

        /// Set the size of the "chunks" of numbers we are using
        void setSegmentSize(size_t new_size);
        //@}

        /// For debugging purpose
        void display(bool do_defrag = true);
        
    private:
        std::vector<std::vector<Indices>> row_indices;  ///< Vector of vector representing rows indices and segments of data related to this row
        std::vector<uint32_t> col_indices; ///< Vector of column indices
        std::vector<double> values; ///< Vector of values

        size_t matrix_size_row; ///< Height of the matrix (for initialization)
        size_t matrix_size_col; ///< Width of the matrix (for initialization)

        size_t row_count = 0; ///< Number of actual rows in the matrix
        size_t col_count = 0; ///< Number of actual columns in the matrix
        size_t segment_size = 5; ///< Size of the "chunks" of numbers
        size_t index_segment_size = 3; ///< Initial number of index segments
        bool consistant = true; ///< Contains the current consistancy state

        uint32_t end_of_rows = 0; ///< To take into account empty spaces at the end of rows (TODO: is it usefull of just use values vector length ?)
};

}

#endif // _DCSRMATRIX_HPP