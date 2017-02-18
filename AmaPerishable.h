#ifndef SICT_AMAPERISHABLE_H_
#define SICT_AMAPERISHABLE_H_

#include <fstream>
#include <iostream>
#include "AmaProduct.h"
#include "Date.h"
#include "ErrorMessage.h"

namespace sict
{
    class AmaPerishable : public AmaProduct
    {
    private:
        Date expiry_;
    public:
        AmaPerishable();
        const Date& expiry() const;
        void expiry(const Date& value);
        std::fstream& store(std::fstream& file, bool addNewLine = true) const;
        std::fstream& load(std::fstream& file);
        std::ostream& write(std::ostream& os, bool linear) const;
        std::istream& read(std::istream& istr);
    };
}

#endif
