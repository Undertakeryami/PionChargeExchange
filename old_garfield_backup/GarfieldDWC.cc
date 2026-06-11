#include "GarfieldDWC.hh"

using namespace Garfield;

GarfieldDWC::GarfieldDWC() {

    gas.SetComposition("ar",70.,"co2",30.);
    gas.Initialise(true);

    field.SetMedium(&gas);

    // central sense wire
    field.AddWire(
        0.0,
        0.0,
        0.001,      // cm
        1500.0,     // volts
        "s");

    sensor.AddComponent(&field);

    avalanche.SetSensor(&sensor);
}

void GarfieldDWC::DriftElectron(
    double x,
    double y,
    double z) {

    avalanche.DriftElectron(
        x,
        y,
        z,
        0.0);
}
