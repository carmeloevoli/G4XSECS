//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: MyHadronPhysicsINCLXX.hh 66892 2013-01-17 10:57:59Z gunter $
//
//---------------------------------------------------------------------------
//
// ClassName:   MyHadronPhysicsINCLXX
//
// Author: 2011 P. Kaitaniemi
//
// Modified:
// 22.05.2014 D. Mancusi: Extend INCL++ to 20 GeV
// 19.03.2013 A.Ribon: Replace LEP with FTFP and BERT
// 01.03.2013 D. Mancusi: Rename to MyHadronPhysicsINCLXX and introduce
//                        parameters for FTFP and NeutronHP
// 31.10.2012 A.Ribon: Use G4MiscBuilder
// 27.11.2011 P.Kaitaniemi: Created using QGSP_INCL_ABLA as a template
//
//----------------------------------------------------------------------------
//
#ifndef MyHadronPhysicsINCLXX_h
#define MyHadronPhysicsINCLXX_h 1

#include "G4AntiBarionBuilder.hh"
#include "G4BertiniKaonBuilder.hh"
#include "G4FTFPAntiBarionBuilder.hh"
#include "G4FTFPKaonBuilder.hh"
#include "G4FTFPNeutronBuilder.hh"
#include "G4FTFPPionBuilder.hh"
#include "G4FTFPProtonBuilder.hh"
#include "G4HyperonFTFPBuilder.hh"
#include "G4INCLXXNeutronBuilder.hh"
#include "G4INCLXXPionBuilder.hh"
#include "G4INCLXXProtonBuilder.hh"
#include "G4KaonBuilder.hh"
#include "G4NeutronBuilder.hh"
#include "G4NeutronPHPBuilder.hh"
#include "G4PionBuilder.hh"
#include "G4ProtonBuilder.hh"
#include "G4QGSPKaonBuilder.hh"
#include "G4QGSPNeutronBuilder.hh"
#include "G4QGSPPionBuilder.hh"
#include "G4QGSPProtonBuilder.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4ios.hh"
#include "globals.hh"

/**
 * Build hadronic physics using INCL++, high-energy models (QGSP or FTFP) and
 * possibly NeutronHP.
 *
 * @see G4INCLXXProtonBuilder
 * @see G4INCLXXNeutronBuilder
 * @see G4INCLXXPionBuilder
 * @see G4IonINCLXXBuilder
 */

class G4ComponentGGHadronNucleusXsc;

class MyHadronPhysicsINCLXX : public G4VPhysicsConstructor {
   public:
    MyHadronPhysicsINCLXX(G4int verbose = 1);
    MyHadronPhysicsINCLXX(const G4String& name,
                          const G4bool quasiElastic = true,
                          const G4bool neutronHP = false,
                          const G4bool ftfp = false);
    virtual ~MyHadronPhysicsINCLXX();

   public:
    virtual void ConstructParticle();
    virtual void ConstructProcess();

    void SetQuasiElastic(G4bool value) { QuasiElastic = value; };

   private:
    void CreateModels();

    struct ThreadPrivate {
        G4NeutronBuilder* theNeutrons;
        G4QGSPNeutronBuilder* theQGSPNeutron;
        G4FTFPNeutronBuilder* theFTFPNeutron;
        G4INCLXXNeutronBuilder* theINCLXXNeutron;
        G4NeutronPHPBuilder* theNeutronHP;

        G4PionBuilder* thePion;
        G4QGSPPionBuilder* theQGSPPion;
        G4FTFPPionBuilder* theFTFPPion;
        G4INCLXXPionBuilder* theINCLXXPion;

        G4KaonBuilder* theKaon;
        G4QGSPKaonBuilder* theQGSPKaon;
        G4FTFPKaonBuilder* theFTFPKaon;
        G4BertiniKaonBuilder* theBertiniKaon;

        G4ProtonBuilder* thePro;
        G4QGSPProtonBuilder* theQGSPPro;
        G4FTFPProtonBuilder* theFTFPPro;
        G4INCLXXProtonBuilder* theINCLXXPro;

        G4HyperonFTFPBuilder* theHyperon;

        G4AntiBarionBuilder* theAntiBaryon;
        G4FTFPAntiBarionBuilder* theFTFPAntiBaryon;

        G4ComponentGGHadronNucleusXsc* xsKaon;
        G4VCrossSectionDataSet* xsNeutronCaptureXS;
    };
    static G4ThreadLocal ThreadPrivate* tpdata;

    G4bool QuasiElastic;
    G4bool withNeutronHP;
    G4bool withFTFP;
};

#endif
