#include "Log.h"

#include <iomanip>
#include <iostream>

namespace pl {
std::shared_ptr<std::ostream> Log::out_ = std::shared_ptr<std::ostream>(&std::cout);
std::mutex Log::lock_;

void Log::write(const std::string& s) {
    lock_.lock();
    (*out_) << s << std::endl;
    lock_.unlock();
}

}
