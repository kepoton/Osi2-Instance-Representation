#ifndef _PACKED_VECTOR_HPP
#define _PACKED_VECTOR_HPP

#include <map>
#include <vector>
#include <functional>

namespace Osi2 {

/*! \brief Struct to represent an element of a PackedVector

    Contains two values : an index and a value
 */
struct Element{
    uint32_t index; ///< Index of the element
    double value; ///< Value of the element

    /// Constructor
    Element(uint32_t index, double value) : index(index), value(value) {}
};

/*! \brief Class to represent a Packed Vector (sparse vector)
 */
class PackedVector {

    public:
        /// \name Constructors
        //{@

        /// Default constructor
        PackedVector();

        /// Copy constructor
        PackedVector(const PackedVector& other);

        /// Assignment operator
        PackedVector& operator=(const PackedVector& other);

        /// Move constructor
        PackedVector(PackedVector&& other);
        //@}

        /// \name Editing functions
        //{@

        /// Insert a value at a given index. Does not store zeros.
        bool insert(uint32_t index, double value);

        /// Remove the value at a given index
        bool remove(uint32_t index);

        /// Set the value at an existing index. Does store zeros. Prefer the insert method.
        void set(uint32_t index, double value);

        /// Clear the content of the PackedVector
        void clear();
        //@}

        /// \name Lookup functions
        //{@

        /// Get the value at a given index
        double get(uint32_t index) const;

        /// Get the value at a given index
        double at(uint32_t index) const;

        /// Get the value at a given index
        double operator[](uint32_t index) const;

        /// Check if the vector contains a given index
        bool has(uint32_t index) const;
        //@}
        
        /// \name Getters
        //{@

        /// Get the size of the PackedVector (the number of elements stored)
        size_t size() const;

        /// Get a vector of the indices of the PackedVector
        std::vector<uint32_t> indices() const;

        /// Get the greatest index in the PackedVector
        uint32_t maxIndex() const;

        /// Get the dimension of the PackedVector
        uint32_t dimension() const;

        /// Get the Element pairs as a std::vector
        const std::vector<Element> elements() const;

        /// Get the map containing the Element pairs
        const std::map<uint32_t, double> data() const;

        /// Get a std::string representation of the PackedVector
        std::string toString() const;
        //@}
        
        /// \name Iterator functions
        //{@

        /// Get the begin iterator of the PackedVector
        std::map<uint32_t, double>::const_iterator begin() const;

        /// Get the end iterator of the PackedVector
        std::map<uint32_t, double>::const_iterator end() const;
        //@}

    private:
        std::map<uint32_t, double> values; ///< Map containing the PackedVector's values
};

/// Apply a function to each elements 1 on 1 between two PackedVector, and return the result as a PackedVector
PackedVector binaryOP(const PackedVector& v1, const PackedVector& v2, std::function<PackedVector(const PackedVector&, const PackedVector&)> op);

/// Sum of two PackedVector
PackedVector operator+(const PackedVector& v1, const PackedVector& v2);

/// Difference of two PackedVector
PackedVector operator-(const PackedVector& v1, const PackedVector& v2);

/// Dot product of two PackedVector
double dotProduct(const PackedVector& v1, const PackedVector& v2);

}

#endif // _PACKED_VECTOR_HPP