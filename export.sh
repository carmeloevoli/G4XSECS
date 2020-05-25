G4DIR=/Users/carmeloevoli/Work/libs/geant4-data

export G4ENSDFSTATEDATA=$G4DIR/G4ENSDFSTATE2.2
export G4LEVELGAMMADATA=$G4DIR/PhotonEvaporation5.5
export G4SAIDXSDATA=$G4DIR/G4SAIDDATA2.0
export G4NEUTRONXSDATA=$G4DIR/G4NEUTRONXS1.4
export G4NEUTRONHPDATA=$G4DIR/G4NDL4.6
export G4PARTICLEXSDATA=$G4DIR/G4PARTICLEXS2.1
export G4INCLDATA=$G4DIR/G4INCL1.0

FILE=$G4ENSDFSTATEDATA/ENSDFSTATE.dat
if [[ ! -f "$FILE" ]]; then
    echo "G4ENSDFSTATEDATA does not exist"
fi

FILE=$G4LEVELGAMMADATA/z1.a1
if [[ ! -f "$FILE" ]]; then
    echo "G4LEVELGAMMADATA does not exist"
fi

FILE=$G4SAIDXSDATA/pp_in.dat
if [[ ! -f "$FILE" ]]; then
    echo "G4SAIDXSDATA does not exist"
fi

FILE=$G4NEUTRONXSDATA/inelast1
if [[ ! -f "$FILE" ]]; then
    echo "G4NEUTRONXSDATA does not exist"
fi

FILE=$G4NEUTRONHPDATA/README
if [[ ! -f "$FILE" ]]; then
    echo "G4NEUTRONHPDATA does not exist"
fi

FILE=$G4PARTICLEXSDATA/proton/inel1
if [[ ! -f "$FILE" ]]; then
    echo "G4PARTICLEXSDATA does not exist"
fi

FILE=$G4INCLDATA/table_radius_hfb.dat
if [[ ! -f "$FILE" ]]; then
    echo "G4INCLDATA does not exist"
fi