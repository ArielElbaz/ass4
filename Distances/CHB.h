#ifndef ASS3_CHB_H
#define ASS3_CHB_H

#include "General/Vector.h"
#include "Distances/Distances.h"


class CHB : public Distances {
    public:
    void operator()(Vector &a, Vector &b) override;
    CHB();  // Constructor
    ~CHB() ; // Destructor
};
#endif //ASS3_CHB_H
