#include "PrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "G4NuclideTable.hh"
#include "G4SystemOfUnits.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() {
    fParticleGun = new G4GeneralParticleSource();
}
PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fParticleGun;
}
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    fParticleGun->GeneratePrimaryVertex(anEvent);
}