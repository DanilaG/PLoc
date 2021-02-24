#ifndef PLOC_EXPERIMENTRESULT_H
#define PLOC_EXPERIMENTRESULT_H

#include <optional>
#include <vector>

#include "Geometric/Points/TimePoint.h"

/** Grid protocol */
class GridProtocol {
public:
    /** Size the grid */
    struct Size {
        size_t width;
        size_t height;
    };

    GridProtocol(const Size& size) : size_(size),
                                     data_(size.width * size.height, 0) {}

    /** Get value in a node */
    virtual double operator()(unsigned int x, unsigned int y) const { return data_[getDataIndex(x, y)]; }

    /** Get size of the grid */
    Size size() const { return size_; }

protected:
    /** Converts grid's indices to date's indices */
    unsigned int getDataIndex(unsigned int x, unsigned int y) const { return y * size_.width + x; }

    /** Grid size */
    Size size_;
    /** Data */
    std::vector<double> data_;
};

/** Grid class */
class Grid : public GridProtocol {
public:
    Grid(const Size& size) : GridProtocol(size) {}

    /** Get / set value in a node */
    double& operator()(unsigned int x, unsigned int y) { return data_[getDataIndex(x, y)]; }
};

/** Experiment result in grid */
class GridResult : public GridProtocol {
public:
    GridResult(const Size& size) : GridProtocol(size),
                                   counter_(size.width * size.height, 0) {}

    /** Get number answer in a node */
    unsigned int massAnswer(unsigned int x, unsigned int y) const { return counter_[getDataIndex(x, y)]; }

    /** Return true, if the grid has value on a node */
    bool hasValue(unsigned int x, unsigned int y) const { return massAnswer(x, y); }

    /** Added new value to the grid */
    void add(unsigned int x, unsigned int y, double value) {
        unsigned int index = getDataIndex(x, y);
        if (counter_[index] == 0) {
            data_[index] = value;
        } else {
            data_[index] = (data_[index] * counter_[index] + value) / (counter_[index] + 1);
        }
        counter_[index]++;
    }

private:
    /** Count number answers in node */
    std::vector<unsigned int> counter_;
};

/** Result of a experiment in scene grid */
class GridExperimentResult {
public:
    GridExperimentResult(const GridProtocol::Size& size) : distance(size), time(size) {}

    /** Results */
    GridResult distance;
    GridResult time;

    /** Duration of all calculates in milliseconds */
    double duration;
};

/** Result of a signal detection experiment */
struct SignalExperimentResult {
    std::vector<pl::TimePoint<>> detected;
};

/** Full result of experiment */
struct ExperimentResult {
    std::optional<GridExperimentResult> grid;
    std::vector<SignalExperimentResult> signal;
};

#endif //PLOC_EXPERIMENTRESULT_H
