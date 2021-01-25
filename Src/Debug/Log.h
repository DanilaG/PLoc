#ifndef PLOC_LOG_H
#define PLOC_LOG_H

#include <fstream>
#include <memory>
#include <mutex>
#include <string>

namespace pl {
class Log {
   public:
    static void write(const std::string& s);
    static void setOut(std::shared_ptr<std::ostream> out) { out_ = out; }

   private:
    Log() {}
    ~Log() {}

    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;

    static std::shared_ptr<std::ostream> out_;
    static std::mutex lock_;
};

}

#endif  // PLOC_LOG_H
