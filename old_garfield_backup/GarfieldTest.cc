#include <Garfield/MediumMagboltz.hh>
#include <iostream>

void TestGarfield() {

    Garfield::MediumMagboltz gas;

    gas.SetComposition(
        "ar", 70.,
        "co2", 30.
    );

    gas.Initialise(true);

    std::cout << "Garfield++ loaded successfully!"
              << std::endl;
}
