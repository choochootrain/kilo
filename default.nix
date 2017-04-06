{ pkgs ? import <nixpkgs> {} }: with pkgs;

stdenv.mkDerivation rec {
    name = "edit";
    version = "0.0.0";
    src = ./.;
    buildInputs = [
        gcc
        gnumake
    ];
}
