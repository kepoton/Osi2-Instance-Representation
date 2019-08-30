#ifndef _CONSTRAINT_HPP
#define _CONSTRAINT_HPP

#include <string>

namespace Osi2 {

/*! \brief Base abstract class for representing a constraint 

*/
class Constraint {

    public:

        /// Type of constraint
        enum class Type {
            LINEAR,
            QUADRATIC,
            GENERAL              
        };

        /// \name Getters
        //{@

        /// Pure virtual fonction to get the type of the constraint
        virtual Constraint::Type getType() const = 0;

        /// String representation of the constraint
        virtual std::string toString() const = 0;

        /// Get the id the the constraint
        uint32_t getID() const { return id;}

        /// Get the name of the constraint
        std::string getName() const { return name; }
        //@}

        /// Set the name of the constraint
        void setName(const std::string& new_name) { name = new_name; }
        //@}

    protected:
        /// \name Constructors
        //{@

        /// Default constructor
        Constraint();

        /// Constructs a constraint with a name
        Constraint(const std::string& name);

        /// Copy constructor
        Constraint(const Constraint& other);

        /// Assignment operator
        Constraint& operator=(const Constraint& other);

        /// Move constructor
        Constraint(Constraint&& other);
        //@}

    private:
        static uint32_t next_id;

        uint32_t id;
        std::string name;

        
};

}

#endif