#ifndef PLOC_EXPERIMENTRESULT_H
#define PLOC_EXPERIMENTRESULT_H

/** Result in grid */
class GridResult {
public:
    /** Size the grid */
    struct Size {
        size_t width;
        size_t height;
    };

    GridResult(const Size& size) : size_(size),
                                   data_(size.width * size.height, 0),
                                   counter_(size.width * size.height, 0) {}

    /** Get value in a node */
    double& operator()(unsigned int x, unsigned int y) { return data_[getDataIndex(x, y)]; }
    double operator()(unsigned int x, unsigned int y) const { return data_[getDataIndex(x, y)]; }

    /** Get number answer in a node */
    unsigned int massAnswer(unsigned int x, unsigned int y) { return counter_[getDataIndex(x, y)]; }

    /** Return true, if the grid has value on a node */
    bool hasValue(unsigned int x, unsigned int y) { return massAnswer(x, y); }

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

    /** Get size of the grid */
    Size size() const { return size_; }

private:
    unsigned int getDataIndex(unsigned int x, unsigned int y) const { return y * size_.width + x; }

    Size size_;
    std::vector<double> data_;
    std::vector<unsigned int> counter_;
};

/** Result of a experiment */
class ExperimentResult {
public:
    ExperimentResult(const GridResult::Size& size) : distance(size), time(size) {}

    GridResult distance;
    GridResult time;
    double duration;
};

#endif //PLOC_EXPERIMENTRESULT_H
