#ifndef SICT_AMAPRODUCT_H_
#define SICT_AMAPRODUCT_H_

#include "Product.h"
#include "ErrorMessage.h"

namespace sict
{
    class AmaProduct : public Product
    {
    private:
       char fileTag_;
       char unit_[11];
    protected:
       ErrorMessage err_;
    public:
        AmaProduct(char = 'N');
        const char* unit() const;
        void unit(const char* value);
        std::fstream& store(std::fstream& file, bool addNewLine = true) const;
        std::fstream& load(std::fstream& file);
        std::ostream& write(std::ostream& os, bool linear) const;
        std::istream& read(std::istream& istr);
    };
}

#endif


