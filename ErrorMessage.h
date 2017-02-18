#ifndef SICT_ERRORMESSAGE_H__
#define SICT_ERRORMESSAGE_H__
#include <iostream>

namespace sict
{
    class ErrorMessage
    {
        char* message_;
    public:
        ErrorMessage();
        ErrorMessage(const char* errorMessage);
        ErrorMessage(const ErrorMessage& em) = delete;
        ErrorMessage& operator=(const ErrorMessage& em) = delete;
        ErrorMessage& operator=(const char* errorMessage);
        void clear();
        void message(const char* value);
        ~ErrorMessage();
        bool isClear()const;
        const char* message() const;
    };

    std::ostream& operator<<(std::ostream&, const ErrorMessage&);
}

#endif

