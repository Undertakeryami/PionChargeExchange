# CERN BL4S Pion Charge Exchange Simulation

This repository contains a fully configured Geant4 simulation framework engineered to model and measure the **Exclusive Pion Charge Exchange** reaction ($\pi^+ + n \rightarrow \pi^0 + p$). The simulation architecture and physical layouts are accurately aligned with the official CERN T9 beamline environment templates.

The apparatus incorporates 4 Drift Wire Chambers (DWCs) for beam track profiling and a 4x4 Lead Glass Electromagnetic Calorimeter (ECAL) matrix optimized to measure complete electromagnetic showers from neutral pion decay ($\pi^0 \rightarrow \gamma\gamma$).

---

## 1. Physical Background & Yield Expectations

The application handles track filtering inside your targets (`LithiumTarget` or `GraphiteTarget`) to capture pure, exclusive charge-exchange channels. Due to quantum mechanics and cross-section scaling principles, your yield changes depending on the $GeV$ input:

* **0.5 GeV Regime:** High exclusive interaction yield ($\sim 1.5\% - 2.5\%$). The incoming pion acts as a wide quantum wave maximizing overlap with target nucleons. Choice of channels is constrained because there is not enough kinetic energy to produce extra particle mass.
* **3.0 GeV Regime:** Low exclusive interaction yield ($\sim 0.15\% - 0.35\%$). The pion behaves as a highly localized, point-like bullet. Relativistic velocities unlock highly competing inelastic channels (multi-pion generation, spallation, resonance states) which violate exclusive criteria.

---

## 2. Repository Architecture

Ensure your primary directory is structured as follows before running compilation scripts:

```text
~/DWC_simulation/
├── CMakeLists.txt
├── bl4s_analysis.py          # Master plotting script (Python)
├── run.mac                   # Macro file steering beam parameters
├── include/
│   ├── DetectorConstruction.hh
│   ├── EventAction.hh
│   └── SteppingAction.hh
└── src/
    ├── DetectorConstruction.cc
    ├── EventAction.cc
    └── SteppingAction.cc



```
so yeah then shift the .py and run.mac file to the build folder and then 
```
cd ~/DWC_simulation
mkdir -p build
cd build
cmake ..
make -j$(nproc)
```

```
./DWC_simualtion run.mac
```
```
hadd -f Complete_Pion_Data.root Pion_Data_t*.root
```
```
python bl4s_analysis.py
```

