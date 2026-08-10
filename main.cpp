/*
    =============================================================
     GUDAU - Ride Sharing & On-Demand Services (Console App)
     College OOP Project - C++
    =============================================================
    OOP concepts demonstrated in this single file:
        - Encapsulation   (private data + public getters/setters)
        - Inheritance     (Service is the base class; RideBooking,
                            FoodDelivery, ParcelDelivery and
                            RepairService derive from it)
        - Polymorphism    (virtual functions requestService() and
                            displayDetails() are overridden by
                            each derived class and called through
                            a Service* base pointer)
        - Abstraction     (Service is an abstract class - it
                            cannot be instantiated directly)
        - Constructors/Destructors, composition (Service HAS-A
          pointer to the User who owns the request)

    Menu, questions and fare formulas follow the student's own
    handwritten design:
        1. Book a Ride     -> Bike/Scooter, destination + KM,
                               Fare = KM x 16 NPR
        2. Food Delivery   -> Cycle/Scooter, destination + KM,
                               Fare = KM x 9 NPR
        3. Parcel Delivery -> weight(kg), destination + KM
                               weight < 10kg  -> 12 NPR/KM
                               weight < 20kg  -> 14 NPR/KM
                               weight > 20kg  -> 18 NPR/KM, and a
                               4-Wheeler is sent instead of the
                               default Scooter, with a warning
                               message printed.
        4. Repair Service  -> location, what needs repair
    =============================================================
*/
