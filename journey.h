#ifndef JOURNEY_H
#define JOURNEY_H

#include <string>
#include <memory>
#include <iostream>
#include "list.h"
#include "trip.h"
#include "time.h"

class Journey {
private:
    List<std::shared_ptr<Trip>> trips;
    List<std::string> transferPoints;
    Time startTime;
    Time endTime;
    int transferCount;

public:
    Journey(const List<std::shared_ptr<Trip>>& tripList,
            const List<std::string>& transfers,
            Time start, Time end);

    int getTotalDuration() const;
    int getTransferCount() const;
    Time getStartTime() const;
    Time getEndTime() const;
    const List<std::shared_ptr<Trip>>& getTrips() const;
    const List<std::string>& getTransferPoints() const;

    void display() const;
};

#endif // JOURNEY_H