#ifndef PLOC_MODELING_COMBINER_H
#define PLOC_MODELING_COMBINER_H

#include "Localization/Combiner/MeanCombiner.h"
#include "Localization/Combiner/FilteredMeanCombiner.h"
#include "Localization/Combiner/MedianCombiner.h"
#include "Localization/Combiner/TriangleCombiner.h"
#include "Localization/Combiner/TimeDifferenceCombiner.h"

class MeanCombiner;
class FilteredMeanCombiner;
class MedianCombiner;
class TriangleCombiner;
class TimeDifferenceCombiner;

class Combiner {
public:
    virtual pl::Combiner& get() = 0;

    virtual ~Combiner() {}
};

class MeanCombiner: public Combiner {
public:
    pl::Combiner& get() final { return combiner_; }

private:
    pl::MedianCombiner combiner_;
};

class FilteredMeanCombiner: public Combiner {
public:
    pl::Combiner& get() final { return combiner_; }

private:
    pl::FilteredMeanCombiner combiner_;
};

class MedianCombiner: public Combiner {
public:
    pl::Combiner& get() final { return combiner_; }

private:
    pl::MedianCombiner combiner_;
};

class TriangleCombiner: public Combiner {
public:
    pl::Combiner& get() final { return combiner_; }

private:
    pl::TriangleCombiner combiner_;
};

class TimeDifferenceCombiner: public Combiner {
public:
    pl::Combiner& get() final { return combiner_; }

private:
    pl::TimeDifferenceCombiner combiner_;
};

#endif //PLOC_MODELING_COMBINER_H
