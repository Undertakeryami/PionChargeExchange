import uproot
import numpy as np
import matplotlib.pyplot as plt

# ====================================================================
# 1. LOAD THE MERGED GEANT4 ROOT FILE
# ====================================================================
print("[INFO] Opening simulation data...")
try:
    file = uproot.open("Complete_Pion_Data.root")
    tree = file["SimulationData"]
except Exception as e:
    print(f"\n[ERROR] Could not open Complete_Pion_Data.root.")
    print(f"-> Did you forget to run 'hadd -f Complete_Pion_Data.root Pion_Data_t*.root'?")
    print(f"Details: {e}\n")
    exit(1)

# Read variables into memory
ecal_energy = tree["ECAL_TotalEdep"].array(library="np")
target_has_pi0 = tree["Target_HasPi0"].array(library="np")
total_events = len(ecal_energy)

# Jagged arrays for vector branches (DWC hits)
dwc_ids = tree["DWC_ID"].array()
dwc_xs = tree["DWC_X"].array()
dwc_ys = tree["DWC_Y"].array()

# ====================================================================
# 2. PHYSICS SIGNAL CALCULATION
# ====================================================================
successful_ce = np.sum(target_has_pi0 == 1)
unsuccessful_events = np.sum(target_has_pi0 == 0)
ce_probability = (successful_ce / total_events) * 100

# Print clean text report to terminal
print("\n" + "="*55)
print("     CERN BL4S EXCLUSIVE CHARGE EXCHANGE REPORT      ")
print("="*55)
print(f" Total Incident Pions (pi+)      : {total_events}")
print(f" Background/Unreacted Events     : {unsuccessful_events}")
print(f" Confirmed Charge Exchanges      : {successful_ce}")
print("-"*55)
print(f" Cleaned Interaction Yield       : {ce_probability:.4f}%")
print("="*55 + "\n")

# ====================================================================
# 3. EXTRACT DWC TRACKING HITS FOR PROFILE PLOTTING
# ====================================================================
# Extract all X and Y coordinate positions for DWC 1 (Upstream Beam Monitor)
dwc1_x = []
dwc1_y = []

for i in range(len(dwc_ids)):
    ids = dwc_ids[i]
    xs = dwc_xs[i]
    ys = dwc_ys[i]
    for idx, d_id in enumerate(ids):
        if d_id == 1:  # Filter specifically for DWC 1 hits
            dwc1_x.append(xs[idx])
            dwc1_y.append(ys[idx])

# ====================================================================
# 4. GENERATE MASTER DIAGNOSTIC PLOT
# ====================================================================
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))

# Panel A: Lead Glass Calorimeter Response Spectrum
ax1.hist(ecal_energy[target_has_pi0 == 0], bins=100, range=(0, 3500),
         color='#d9534f', alpha=0.65, label=r'Background (No $\pi^0$ In Target)')
ax1.hist(ecal_energy[target_has_pi0 == 1], bins=100, range=(0, 3500),
         color='#5cb85c', alpha=0.85, label=r'Signal (True Exclusive $\pi^0$ Produced)')

ax1.set_title("Calorimeter Matrix Response (Monte Carlo Truth)", fontsize=12, fontweight='bold')
ax1.set_xlabel("Total Deposited Energy in ECAL Matrix (MeV)", fontsize=11)
ax1.set_ylabel("Number of Events (Log Scale)", fontsize=11)
ax1.set_yscale('log')
ax1.grid(True, which="both", linestyle='--', alpha=0.4)
ax1.legend(fontsize=10, loc='upper right')

# Panel B: DWC 1 Spatial Beam Profile Tracker
if len(dwc1_x) > 0:
    counts, xedges, yedges, im = ax2.hist2d(dwc1_x, dwc1_y, bins=40, range=[[-50, 50], [-50, 50]], cmap='viridis')
    fig.colorbar(im, ax=ax2, label='Number of Hits')   # <-- Fixed: 'ax' tells it to attach to ax2
    ax2.set_title("DWC 1 Upstream Beam Profile Map", fontsize=12, fontweight='bold')
    ax2.set_xlabel("Horizontal Beam Position X (mm)", fontsize=11)
    ax2.set_ylabel("Vertical Beam Position Y (mm)", fontsize=11)
    ax2.grid(True, linestyle='--', alpha=0.3)
else:
    ax2.text(0.5, 0.5, 'No DWC Hits Recorded\nCheck SteppingAction Volume Match', 
             ha='center', va='center', fontsize=12, color='red')
    ax2.set_title("DWC Tracking Profile Map", fontsize=12, fontweight='bold')

plt.tight_layout()
output_plot = "charge_exchange_comprehensive_results.png"
plt.savefig(output_plot, dpi=300)
print(f"[INFO] Analysis visualization saved successfully as '{output_plot}'.\n")
