#ifndef GARFIELDDWC_H
#define GARFIELDDWC_H

#include <Garfield/MediumMagboltz.hh>
#include <Garfield/ComponentAnalyticField.hh>
#include <Garfield/Sensor.hh>
#include <Garfield/AvalancheMC.hh>

class GarfieldDWC {

public:
    GarfieldDWC();

    void DriftElectron(double x,
                       double y,
                       double z);

private:
    Garfield::MediumMagboltz gas;
    Garfield::ComponentAnalyticField field;
    Garfield::Sensor sensor;
    Garfield::AvalancheMC avalanche;
};

#endif
