#include "DCSRMatrix.hpp"

#include <exception>
#include <algorithm>
#include <cmath>

#include <iostream>

namespace Osi2 {

DCSRMatrix::DCSRMatrix(){
    matrix_size_row = 0;
    matrix_size_col = 0;
}

DCSRMatrix::DCSRMatrix(size_t nb_rows, size_t nb_cols){
    matrix_size_row = nb_rows;
    matrix_size_col = nb_cols;

    row_indices.reserve(nb_rows);
 }


bool DCSRMatrix::addRow(const PackedVector& v){

    bool ret_val = true;

    if (v.dimension() > matrix_size_col){ // If the vector size is larger than the matrix width
        matrix_size_col = v.dimension(); // We update the maximum width of the matrix
    }
    if (v.dimension() > col_count){ // If the vector size is larger than the matrix actual column count
        col_count = v.dimension(); // We update the actual column count of the matrix
    }

    Indices pair = std::make_pair(end_of_rows, v.size() );

    std::vector<Indices> index_seg;
    index_seg.reserve(index_segment_size);
    index_seg.push_back(pair); 

    row_indices.push_back(index_seg);

    if (row_count == matrix_size_row){ // If the matrix row capacity is reached, we increase the row capacity of the matrix
        matrix_size_row++;
    }

    uint32_t bloc_allocated = std::floor((v.size() / segment_size)) + 1; // Number of "blocs" of size segment_size to allocate. Each bloc can hold segment_size values

    end_of_rows = end_of_rows + bloc_allocated * segment_size; // Compute the end index of the row

    for ( const auto& e : v ){ // Push all the column indices and values in the corresponding vectors
        col_indices.push_back(e.first);
        values.push_back(e.second);
    }
    for( uint i = v.size(); i < bloc_allocated * segment_size ; i++){ // Create the empty space at the end of column indices and values arrays
        col_indices.push_back(0);
        values.push_back(0);
    }

    row_count++;

    return ret_val;
}

bool DCSRMatrix::addColumn(const PackedVector& v){
    bool ret_val = true;

    PackedVector empty;
    while (v.dimension() > matrix_size_row) addRow(empty); // If the size of the column we add is larger than the matrix row capacity, add empty rows until it is the same amount

    for ( const auto& e : v){ // For each element in the vector
        uint32_t index = e.first;
        double value = e.second;

        Indices& ind = row_indices[index][row_indices[index].size() - 1]; // Get right the row start index
        if ( ind.second == 0 || ind.second % segment_size != 0 ){ // If we have space left in the "bloc" allocated, we insert the value 
            uint32_t i = ind.first + ind.second;
            col_indices [ i ] = col_count;
            values[ i ] = value;

            ++ind.second;
        }
        else{ // If we have no space left in the "bloc", we need to allocate a new one
            row_indices[index].push_back( std::make_pair(end_of_rows, 1) ); // Add a new pair of row start index and element count

            col_indices.push_back(col_count); // Push the column index and valuein the corresponding arrays
            values.push_back(value);
            for( uint i = 1; i < segment_size ; i++){ // Fill the remaining space of the bloc with 0's
                col_indices.push_back(0);
                values.push_back(0);
            }

            end_of_rows = end_of_rows + segment_size; // Update the new end_of_row index, as we added a new bloc of size segment_size

            consistant = false;
        }

    }

    col_count++;

    if (col_count > matrix_size_col){ // If the column count is greater than the column capacity of the matrix, increase the column capacity
        matrix_size_col++;
    }

    return ret_val;
}

double DCSRMatrix::getValue(uint32_t i, uint32_t j) const {
    double ret_val = 0;

    //Not needed, but do we want to run it ?
    //The current algorithm looks throught all the segments of a row
    //If we defragment, only need to look throught the first
    /*if ( !isConsistant() ){
        defragment();
    }*/

    if ( i >= matrix_size_row)
        throw std::out_of_range("Wrong row index");

    if ( j >= matrix_size_col)
        throw std::out_of_range("Wrong column index");

    bool found = false;
    uint32_t it = 0;
    while (!found && it < row_indices[i].size()){ // For each row start index segment
        if ( row_indices[i][it].second != 0){ // If the segment is not of size 0
            const auto& beg = std::begin(col_indices) + row_indices[i][it].first; // Get the index of the beginning of the row
            
            std::vector<uint32_t> col(beg, beg + row_indices[i][it].second); // REpresent the entire column

            const auto& e = std::find(std::begin(col), std::end(col), j); // Look for the column index
            if ( e != std::end(col) ){ // If we found it
                found = true;
                ret_val = values[ row_indices[i][it].first + std::distance(std::begin(col), e) ]; 
            }
        }
        it++;
    }

    return ret_val;
}

PackedVector DCSRMatrix::getRow(uint32_t index) const {
    if ( index > row_count){
        throw std::out_of_range("Wrong row index");
    }

    PackedVector ret_val;

    for ( const auto& vec_seg : row_indices[index] ){ // For each row segment
        uint32_t beg = vec_seg.first;
        uint32_t size = vec_seg.second;

        for (uint32_t i = beg; i < beg + size; i++){ // Insert each column index/value pair in the ret_val vector
            ret_val.insert(col_indices[i], values[i]);
        }
    }

    return ret_val;
}

PackedVector DCSRMatrix::getColumn(uint32_t index) const {
    if ( index > col_count){
        throw std::out_of_range("Wrong row index");
    }

    PackedVector ret_val;

    uint32_t i = 0;
    for ( const auto& row : row_indices ){ // For each row
        for ( uint32_t k = 0; k < row.size(); k++ ){// For each row start index
            uint32_t beg = row[k].first;
            uint32_t size = row[k].second;

            for (uint32_t j = beg; j < beg + size; j++){ // For each column index
                if (col_indices[j] == index){ // If we find the index we are looking for
                    ret_val.insert(i, values[j]); // Add the column index/value pair in the ret_val vector
                }
            }
        }
    
        i++;
    }

    return ret_val;
}

void DCSRMatrix::setSegmentSize(size_t new_size){
    segment_size = new_size;
    defragment();
}

void DCSRMatrix::defragment(){

    std::vector<std::vector<Indices>> new_row_indices;
    std::vector<uint32_t> new_col_indices;
    std::vector<double> new_values;

    uint32_t index_beg = 0;

    for (uint32_t i = 0; i < row_indices.size(); i++){ // For each row
        uint32_t size = 0;

        std::map<uint32_t, double> col_val_temp; // Temporary map to hold the column indexes and the vlaues associated
        for (const auto& vec_seg : row_indices[i]){ // For each row start segment
            uint32_t s_index = vec_seg.first;
            uint32_t seg_size = vec_seg.second;

            size += seg_size;

            for (uint32_t j = 0; j < seg_size ; j++){ // For each column
                uint32_t col_value = col_indices[s_index + j];
                double value = values[s_index+ j];
                col_val_temp.insert(std::make_pair(col_value, value) ); // Add the column index and value associated to the col_val_temp map
            }
        }

        uint32_t total_bloc_size = size % segment_size == 0 ? size : std::floor(size / segment_size)*segment_size + segment_size; // Total number of values in the column + some to fill the last "bloc"

        for (const auto& e : col_val_temp){ // Add the columns indexes and value to the new columns and values vector
            new_col_indices.push_back(e.first);
            new_values.push_back(e.second);
        }
        for (uint32_t j = size; j < total_bloc_size; j++){ // Fill the last "bloc"
            new_col_indices.push_back(0);
            new_values.push_back(0);
        }
        
        std::vector<Indices> vec_seg; 
        vec_seg.reserve(index_segment_size);
        vec_seg.push_back( std::make_pair(index_beg, size) );

        new_row_indices.push_back( vec_seg ); // Add the defragmented row indices to the the row indices vector

        index_beg += total_bloc_size;
    }

    std::swap(new_row_indices, row_indices); // Swap the old vectors with the new, defragmented ones
    std::swap(new_col_indices, col_indices);
    std::swap(new_values, values);

    consistant = true;
}

void DCSRMatrix::display(bool do_defrag){
    if (do_defrag)
        defragment();

    std::cout << "Row indices : \n";
    auto max_v = *std::max_element( std::begin(row_indices), std::end(row_indices), 
        [](const std::vector<Indices>& v1, const std::vector<Indices>& v2){
            return v1.size() < v2.size();        
        } 
    );
    uint32_t max = max_v.size();
    for (uint32_t i = 0; i < max; i++){
        for (const auto& e : row_indices){
            if (i < e.size()){
                std::cout << e[i].first << "->" << e[i].second << " ";
            }
            else
            {
                std::cout << " " << "  " << " " << " ";
            }
        }
        std::cout << "\n";
    }
    
    std::cout << "\nColumn indices : \n";
    for (const auto& e : col_indices){
        std::cout << e << " ";
    }

    std::cout << "\nValues : \n";
    for (const auto& e : values){
        std::cout << e << " ";
    }
    std::cout << "\n\n";

    for ( uint32_t i = 0; i < row_count; i++){

        for(uint j = 0; j < matrix_size_col; j++){
            std::cout << getValue(i, j) << " ";
        }
        std::cout << "\n";
        
    }

    for (uint32_t i = row_count; i < matrix_size_row; i++){
        for (uint32_t j = 0; j < matrix_size_col; j++){
            std::cout << "0 ";
        }
        std::cout << "\n";
    }

}

}